#!/usr/bin/env python3
import os
import re
import sys
from functools import lru_cache
from collections import Counter #подсчет частотности элементов, применяется для анализа повторов шумовых слов
from dataclasses import dataclass #для ProcessResult
from typing import Dict, List, Optional, Sequence, Tuple #модуль аннотации типов

from natasha import (
    Doc, # Контейнер-документ, к которому последовательно применяются все NLP-компоненты
    MorphVocab, # Словарь для лемматизации (приведения слов к начальной форме)
    NewsEmbedding, # Предобученные векторные представления слов (эмбеддинги)
    NewsMorphTagger, # Морфологический теггер: определяет части речи, падежи, числа и тд
    NewsSyntaxParser, # Синтаксический парсер: строит дерево зависимостей (подлежащее, сказуемое...)
    Segmenter, # Сегментатор: разбивает сырой текст на предложения
)

# Инициализация компонент Наташи
segmenter = Segmenter()
morph_vocab = MorphVocab()
embedding = NewsEmbedding()
morph_tagger = NewsMorphTagger(embedding)
syntax_parser = NewsSyntaxParser(embedding)

# Флаг для отладки, включен по умолчанию.Отключить через ANALYZER_DEBUG=0
DEBUG_ENABLED = os.getenv("ANALYZER_DEBUG", "1").lower() not in {"0", "false", "no"}

# Регулярные выражения для поиска кириллицы и слов
CYRILLIC_CHAR_RE = re.compile(r"[а-яА-ЯёЁ]")
CYRILLIC_WORD_RE = re.compile(r"[а-яА-ЯёЁ-]+")
SPACE_RE = re.compile(r"\s+")

# Наборы кириллических символов для проверок
VOWELS = set("аеёиоуыэюяАЕЁИОУЫЭЮЯ")
VOWELS_LOWER = set("аеёиоуыэюя")
KEYBOARD_ROWS = ("йцукенгшщзхъ", "фывапролджэ", "ячсмитьбю")

# Редкие/неестественные сочетания кириллических букв
RARE_CYRILLIC_CLUSTERS = (
    "йц",
    "йщ",
    "йъ",
    "йы",
    "йь",
    "щщ",
    "щц",
    "щй",
    "щф",
    "щх",
    "щъ",
    "щы",
    "щь",
    "цщ",
    "цъ",
    "ць",
    "ъй",
    "ъь",
    "ыь",
    "кщ",
    "гц",
)

# цщ, щц, ыэ
EXTRA_RARE_CYRILLIC_CLUSTERS = ("цщ", "щц", "ыэ")

# аы, еы, ёы, иы, оы, уы, эы, юы, яы + ыэ, ыю, ыя, ыё
IMPOSSIBLE_Y_BIGRAMS = tuple(f"{left}ы" for left in "аеёиоуэюя") + tuple(
    f"ы{right}" for right in "эюяё"
)
# слова, которые встречаются во всех естественных текстах
COMMON_RU_STOPWORDS = {
    "и",
    "в",
    "во",
    "на",
    "к",
    "ко",
    "с",
    "со",
    "у",
    "о",
    "об",
    "по",
    "за",
    "под",
    "над",
    "не",
    "ни",
    "но",
    "а",
    "да",
    "или",
    "ли",
    "же",
    "что",
    "как",
    "это",
    "то",
    "я",
    "ты",
    "он",
    "она",
    "мы",
    "вы",
    "они",
    "его",
    "ее",
    "их",
    "мой",
    "твой",
    "наш",
    "ваш",
    "этот",
    "тот",
    "кто",
    "где",
    "когда",
    "чем",
    "чтобы",
    "для",
    "из",
    "от",
    "до",
    "при",
    "без",
    "через",
    "про",
}
# синтаксические роли
CONJ_ROLE_MAP = {
    "nsubj": ("pod", "Подлежащее"), "root": ("skaz", "Сказуемое"),
    "advcl": ("skaz", "Сказуемое"), "obj": ("dop", "Дополнение"),
    "iobj": ("dop", "Дополнение"), "obl": ("dop", "Дополнение"),
    "xcomp": ("dop", "Дополнение"), "ccomp": ("dop", "Дополнение"),
    "amod": ("opred", "Определение"), "det": ("opred", "Определение"),
    "nmod": ("opred", "Определение"), "acl": ("opred", "Определение"),
    "advmod": ("ob", "Обстоятельство"), "parataxis": ("ob", "Обстоятельство"),
}

BASE_RELATION_DESC_MAP = {
    "nsubj": "Подлежащее", "obj": "Дополнение", "iobj": "Дополнение",
    "obl": "Дополнение", "xcomp": "Дополнение", "ccomp": "Дополнение",
    "amod": "Определение", "det": "Определение", "nmod": "Определение",
    "acl": "Определение", "advmod": "Обстоятельство", "advcl": "Обстоятельство",
    "parataxis": "Обстоятельство", "root": "Сказуемое", "cop": "Связка",
    "case": "Предлог", "cc": "Союз", "mark": "Союз", "nummod": "Числительное",
    "aux": "Вспомогательный", "punct": "Пунктуация"
}

BASE_RELATION_TYPE_MAP = {
    "nsubj": "pod", "obj": "dop", "iobj": "dop", "obl": "dop",
    "xcomp": "dop", "ccomp": "dop", "amod": "opred", "det": "opred",
    "nmod": "opred", "acl": "opred", "advmod": "ob", "parataxis": "ob",
    "root": "skaz", "advcl": "skaz"
}

# коды завершения скрипта
EXIT_OK = 0
EXIT_USAGE_ERROR = 2
EXIT_IO_ERROR = 3
EXIT_ANALYSIS_ERROR = 4
EXIT_FILTERED = 5


@dataclass
class ProcessResult:
    """Контейнер результата обработки текста"""
    status: str
    relations: List[str]
    message: str = ""


def debug_log(message: str) -> None:
    """Вывод отладочных сообщений в stderr если включён DEBUG"""
    if DEBUG_ENABLED:
        print(f"[DEBUG] {message}", file=sys.stderr)


def configure_stdio() -> None:
    """Защита от UnicodeEncodeError при выводе в консоль"""
    for stream in (sys.stdout, sys.stderr):
        if hasattr(stream, "reconfigure"):
            stream.reconfigure(errors="replace")


def normalize_text(text: str) -> str:
    """Удаление BOM, нормализация переносов строк, добавление финальной точки"""
    text = text.replace("\ufeff", "")
    text = text.replace("\r\n", "\n").replace("\r", "\n")
    return ensure_terminal_punctuation(text)


def ensure_terminal_punctuation(text: str) -> str:
    """Добавление точки в конец коротких осмысленных фраз, если ее нет"""
    stripped = text.strip()
    if not stripped or stripped[-1] in ".!?":
        return text

    words = [w.strip("-") for w in CYRILLIC_WORD_RE.findall(stripped)]
    words = [w for w in words if w]
    if not 1 <= len(words) <= 3:
        return text
    if any(get_suspicious_word_reasons(word) for word in words):
        return text
    if not any(any(ch in VOWELS_LOWER for ch in word.lower()) for word in words):
        return text

    return f"{stripped}."


def sanitize_output_field(value: object, fallback: str = "_") -> str:
    """Перед записью в вывод удаление BOM, переносов, замена '|' на '/', схлопывание пробелов"""
    s = "" if value is None else str(value)
    s = s.replace("\ufeff", "")
    s = s.replace("\r", " ").replace("\n", " ")
    s = s.replace("|", "/")
    s = SPACE_RE.sub(" ", s).strip()
    return s if s else fallback

# эвристика, фильтры "белиберды"
def is_suspicious_mixed_alnum_token(token: str) -> bool:
    """кириллица перемешанная с цифрами"""
    cyrillic_count = 0
    digit_count = 0
    for ch in token:
        lower = ch.lower()
        if ("а" <= lower <= "я") or lower == "ё":
            cyrillic_count += 1
        elif ch.isdigit():
            digit_count += 1
    return len(token) >= 8 and cyrillic_count >= 4 and digit_count >= 3


def contains_keyboard_run(word: str, min_run: int = 4) -> bool:
    """наличие подряд идущих клавиш из одного ряда русской раскладки"""
    w = word.lower()
    if len(w) < min_run:
        return False
    for i in range(len(w) - min_run + 1):
        chunk = w[i : i + min_run]
        for row in KEYBOARD_ROWS:
            if chunk in row or chunk in row[::-1]:
                return True
    return False


def max_repeating_run(word: str) -> int:
    """возвращает длину самой длинной последовательности одинаковых символов подряд"""
    if not word:
        return 0
    max_run = 1
    run = 1
    for i in range(1, len(word)):
        if word[i] == word[i - 1]:
            run += 1
            if run > max_run:
                max_run = run
        else:
            run = 1
    return max_run


def max_consonant_run(word: str) -> int:
    """возвращает длину самой длинной цепочки согласных"""
    run = 0
    max_run = 0
    for ch in word.lower():
        if ch in VOWELS_LOWER:
            run = 0
            continue
        if "а" <= ch <= "я" or ch == "ё":
            run += 1
            if run > max_run:
                max_run = run
        else:
            run = 0
    return max_run


def contains_rare_cyrillic_cluster(word: str) -> bool:
    """проверка на редкие/неестественные сочетания кириллицы"""
    w = word.lower()
    return any(cluster in w for cluster in RARE_CYRILLIC_CLUSTERS) or any(
        cluster in w for cluster in EXTRA_RARE_CYRILLIC_CLUSTERS
    )


def count_impossible_y_bigrams(word: str) -> int:
    """биграммы с буквой ы, которые невозможны"""
    w = word.lower()
    count = 0
    for i in range(len(w) - 1):
        if w[i : i + 2] in IMPOSSIBLE_Y_BIGRAMS:
            count += 1
    return count


def has_hard_sign_anomaly(word: str) -> bool:
    """твердый знак только перед е,ё,ю,я"""
    w = word.lower()
    allowed_next = "еёюя"
    hard_sign = "ъ"
    for i, ch in enumerate(w):
        if ch != hard_sign:
            continue
        if i == len(w) - 1 or w[i + 1] not in allowed_next:
            return True
    return False


def is_weird_mixed_case(word: str) -> bool:
    """неестественный регистр"""
    letters = [ch for ch in word if ("а" <= ch.lower() <= "я") or ch.lower() == "ё"]
    if len(letters) < 3:
        return False

    has_upper = any(ch.isupper() for ch in letters)
    has_lower = any(ch.islower() for ch in letters)
    if not (has_upper and has_lower):
        return False

    # Нормальный title-case не считается мусором
    if letters[0].isupper() and all(ch.islower() for ch in letters[1:]):
        return False

    return True

@lru_cache(maxsize=8192)
def get_suspicious_word_reasons(word: str) -> Tuple[str, ...]:
    """все проверки слов. возвращает список причин, по которым оно выглядит как шум."""
    w = word.lower().strip("-") # w в нижнем регистре
    reasons: List[str] = []
    
    if len(w) < 3:
        return ()
        
    if is_weird_mixed_case(word): # word в оригинальном регистре, так и задумано, все ок
        # Проверка на хаотичный регистр буквы верхнего и нижнего регистра перемешаны без логики
        # Встречается в спаме, капчах или сгенерированном мусоре
        reasons.append("неестественный регистр")
        
    # слова длиннее 2 букв почти всегда содержат гласную
    if not any(ch in VOWELS_LOWER for ch in w):
        reasons.append("без гласных")
    # для слов от 5 символов проверка на наличие невозможных сочетаний с буквой ы
    if len(w) >= 5 and count_impossible_y_bigrams(w) >= 1:
        reasons.append("маловероятное сочетание с ы")
    # поиск редких неестественных сочетаний кириллицы
    if len(w) >= 5 and contains_rare_cyrillic_cluster(w):
        reasons.append("редкое сочетание букв")
    # твердый знак только перед еёюя или в конце слова после приставки на согласную
    if len(w) >= 4 and has_hard_sign_anomaly(w):
        reasons.append("подозрительное использование ъ")
    # 4+ клавиши идущие подряд "йцук" "фыва" "ячсм" итд
    if len(w) >= 4 and contains_keyboard_run(w, min_run=4):
        reasons.append("похоже на клавиатурный набор")
    # удвоение гласной в конце слова
    if len(w) >= 4 and w[-1] == w[-2] and w[-1] in VOWELS_LOWER:
        reasons.append("подозрительное удвоение гласной")
    # 3+ одинаковых буквы подряд 
    if len(w) >= 5 and max_repeating_run(w) >= 3:
        reasons.append("длинная повторяющаяся последовательность")
    # 5+ согласных подряд
    if len(w) >= 6 and max_consonant_run(w) >= 5:
        reasons.append("слишком много согласных подряд")
    # проверка разнообразия символа в слове. если слово состоит из малого набора повторяющихся букв - это мусор
    if len(w) >= 6:
        unique_ratio = len(set(w)) / len(w)
        if unique_ratio < 0.35 or (len(w) >= 8 and unique_ratio < 0.46):
            reasons.append("слишком низкое разнообразие символов")
    # проверка доли гласных для длинных букв, если меньше 15% то текст похож на сгенерированную псевдокириллицу
    if len(w) >= 8:
        vowel_ratio = sum(1 for ch in w if ch in VOWELS_LOWER) / len(w)
        if vowel_ratio < 0.15:
            reasons.append("аномально мало гласных")
            
    return tuple(reasons)

def _analyze_suspicious_words(words: List[str]) -> Dict:
    """
    Общая функция анализа подозрительных слов.
    Принимает список слов, возвращает словарь со статистикой:
    - suspicious_words: список подозрительных слов
    - suspicious_ratio: доля подозрительных слов
    - weird_case_count: количество слов с неестественным регистром
    - rare_cluster_word_count: количество слов с редкими сочетаниями букв
    - rare_cluster_ratio: доля слов с редкими сочетаниями
    - repeated_noise: максимальное количество повторов одного шумового слова
    - suspicious_details: список кортежей (слово, причины)
    """
    suspicious_details: List[Tuple[str, List[str]]] = []
    for w in words:
        reasons = get_suspicious_word_reasons(w)
        if reasons:
            suspicious_details.append((w, reasons))

    suspicious_words = [w for w, _ in suspicious_details]
    suspicious_ratio = len(suspicious_words) / len(words) if words else 0.0

    weird_case_count = sum(
        1 for _, reasons in suspicious_details if "неестественный регистр" in reasons
    )

    rare_cluster_word_count = sum(
        1 for w in words if len(w) >= 5 and contains_rare_cyrillic_cluster(w)
    )
    rare_cluster_ratio = rare_cluster_word_count / len(words) if words else 0.0

    repeated_noise = max(
        Counter(w.lower() for w in suspicious_words).values(), default=0
    )

    return {
        "suspicious_words": suspicious_words,
        "suspicious_ratio": suspicious_ratio,
        "weird_case_count": weird_case_count,
        "rare_cluster_word_count": rare_cluster_word_count,
        "rare_cluster_ratio": rare_cluster_ratio,
        "repeated_noise": repeated_noise,
        "suspicious_details": suspicious_details,
    }

# оценка качества текста 
def looks_like_short_sentence(text: str, words: Sequence[str]) -> bool:
    """короткое, но осмысленное предложение"""
    stripped = text.strip()
    if not stripped or stripped[-1] not in ".!?":
        return False
    if not 1 <= len(words) <= 3:
        return False
    if not all(not get_suspicious_word_reasons(word) for word in words):
        return False
    return any(any(ch in VOWELS_LOWER for ch in word.lower()) for word in words)


def evaluate_raw_text_quality(
    text: str,
    vowel_min: float = 0.30,
    vowel_max: float = 0.70,
    avg_word_max: float = 14.0,
) -> Tuple[bool, str]:
    """оценка до запуска наташи, проверяет долю кириллицы, соотношение гласных, среднюю длину слова, долю
    подозрительных слов, редкие кластеры, странный регистр, повторы, долю типичных слов"""
    # подсчет кириллицы, если меньше 5 букв, то текст не русский
    cyrillic_chars = CYRILLIC_CHAR_RE.findall(text)
    debug_log(f"Кириллических символов: {len(cyrillic_chars)}")
    # очистка регулярными выражениями от дефисов, фильтрация пустых строк
    words = [w.strip("-") for w in CYRILLIC_WORD_RE.findall(text)]
    words = [w for w in words if w]
    # если мало кириллицы, проверка на короткое осмысленное предложение
    if len(cyrillic_chars) < 5:
        if looks_like_short_sentence(text, words):
            debug_log("Короткое правдоподобное предложение обошло фильтр")
        else:
            return True, "мало кириллицы"
    # если нет слов текст сразу отклоняется
    if not words:
        return True, "нет слов"
    # поиск смешанных кириллица+цифры слов, признак мусора
    mixed_alnum_count = sum(1 for token in re.findall(r"\S+", text) if is_suspicious_mixed_alnum_token(token))
    debug_log(f"Смешанных кириллица+цифры токенов: {mixed_alnum_count}")
    if mixed_alnum_count >= 1:
        return True, "подозрительное смешение кириллицы и цифр"
    # проверка доли гласных во всем тексте
    vowel_ratio = sum(1 for c in cyrillic_chars if c in VOWELS) / len(cyrillic_chars)
    debug_log(f"Соотношение гласных: {vowel_ratio:.3f} (норма: {vowel_min}-{vowel_max})")
    if vowel_ratio < vowel_min or vowel_ratio > vowel_max:
        return True, "аномальное соотношение гласных"
    # расчет средней длины слова, слишком длинные - шум
    avg_len = sum(len(w) for w in words) / len(words)
    debug_log(f"Средняя длина слова: {avg_len:.2f} (макс: {avg_word_max})")
    if avg_len > avg_word_max:
        return True, "слова слишком длинные"
    susp_stats = _analyze_suspicious_words(words)
    suspicious_words = susp_stats["suspicious_words"]
    suspicious_ratio = susp_stats["suspicious_ratio"]
    weird_case_count = susp_stats["weird_case_count"]
    rare_cluster_word_count = susp_stats["rare_cluster_word_count"]
    rare_cluster_ratio = susp_stats["rare_cluster_ratio"]
    repeated_noise = susp_stats["repeated_noise"]

    debug_log(f"Подозрительных слов: {len(suspicious_words)}/{len(words)} = {suspicious_ratio:.3f}")
    debug_log(f"Слов с редкими сочетаниями: {rare_cluster_word_count}/{len(words)} = {rare_cluster_ratio:.3f}")
    debug_log(f"Неестественный регистр: {weird_case_count}, повтор одного шумового слова: {repeated_noise}")

    # подсчет слов которые есть почти во всех текстах
    stopword_count = sum(1 for w in words if w.lower() in COMMON_RU_STOPWORDS)
    stopword_ratio = stopword_count / len(words)
    debug_log(f"Служебных русских слов: {stopword_count}/{len(words)} = {stopword_ratio:.3f}")
    
    # отсев при высокой доле мусора
    if (
        (len(words) >= 3 and suspicious_ratio >= 0.55)
        or (len(words) >= 4 and suspicious_ratio >= 0.50)
        or (len(words) >= 6 and suspicious_ratio >= 0.40)
    ):
        preview = ", ".join(suspicious_words[:5])
        return True, f"слишком много мусорных слов ({preview})"
    # отсев при высокой доле редких буквосочетаний
    if len(words) >= 8 and rare_cluster_ratio >= 0.25:
        return True, "слишком много маловероятных сочетаний букв"
    # отсев при множестве слов со странным регистром
    if len(words) >= 8 and weird_case_count >= 2 and suspicious_ratio >= 0.15:
        return True, "слишком много слов с неестественным регистром"
    
    # отсев при повторе одного шумового слова в большом количестве
    if len(words) >= 8 and repeated_noise >= 2 and suspicious_ratio >= 0.15:
        return True, "много повторяющегося шумового слова"
    # проверка на псевдорусский текст при малом числе слов, которые встречаются во всех текстах
    if len(words) >= 12 and stopword_ratio < 0.10 and suspicious_ratio >= 0.20:
        return True, "похоже на псевдорусский текст"
    # для длинных текстов аномально низкая доля слов, которые встречаются во всех текстах
    if len(words) >= 30 and stopword_ratio < 0.03:
        return True, "аномально мало служебных слов для русского текста"

    return False, ""

def evaluate_doc_quality(doc: Doc) -> Tuple[bool, str]:
    """оценка качества после работы наташи
    оценка доли нераспознаных токеноы X, кол-во синтаксических основ на предложение,
    распределение частей речи (noun|adj vs verb vs служебные),
    повторяемость мусорных токенов"""
    meaningful = [t for t in doc.tokens if t.pos not in ("PUNCT", "SPACE")]
    if not meaningful:
        return True, "нет осмысленных токенов"

    x_count = sum(1 for t in meaningful if t.pos == "X")
    x_ratio = x_count / len(meaningful)
    debug_log(f"Нераспознанных слов (X): {x_count}/{len(meaningful)} = {x_ratio:.3f} (порог: 0.35)")
    if (len(meaningful) >= 4 and x_ratio > 0.35) or x_ratio > 0.60:
        return True, "слишком много нераспознанных слов"

    sent_count = max(1, len(doc.sents))
    root_count = sum(1 for t in meaningful if t.rel == "root")
    roots_per_sent = root_count / sent_count
    debug_log(f"Синтаксических root: {root_count}/{sent_count} = {roots_per_sent:.3f} на предложение")
    if len(meaningful) >= 10 and roots_per_sent >= 1.8:
        return True, "аномально много синтаксических основ"

    token_words = [t.text for t in meaningful if CYRILLIC_CHAR_RE.search(t.text or "")]
    token_stopword_count = sum(1 for w in token_words if w.lower() in COMMON_RU_STOPWORDS)
    token_stopword_ratio = (token_stopword_count / len(token_words)) if token_words else 0.0
    if token_words:
        debug_log(
            f"Служебных слов по токенам: {token_stopword_count}/{len(token_words)} = {token_stopword_ratio:.3f}"
        )

    pos_counts = Counter(t.pos for t in meaningful)
    noun_adj_ratio = (pos_counts["NOUN"] + pos_counts["ADJ"]) / len(meaningful)
    verb_ratio = pos_counts["VERB"] / len(meaningful)
    function_ratio = (
        pos_counts["ADP"]
        + pos_counts["CCONJ"]
        + pos_counts["SCONJ"]
        + pos_counts["PART"]
        + pos_counts["PRON"]
        + pos_counts["DET"]
        + pos_counts["AUX"]
    ) / len(meaningful)
    debug_log(
        f"POS-профиль: NOUN+ADJ={noun_adj_ratio:.3f}, VERB={verb_ratio:.3f}, "
        f"служебные={function_ratio:.3f}"
    )

    if len(meaningful) >= 40 and sent_count <= 2 and noun_adj_ratio >= 0.70 and function_ratio <= 0.12 and verb_ratio <= 0.12:
        return True, "аномальный POS-профиль (похоже на псевдотекст)"
    if len(token_words) >= 30 and token_stopword_ratio < 0.03:
        return True, "аномально мало служебных токенов"
    if token_words:
        # сбор статистики по подозрительным токенам через общую функцию
        susp_stats = _analyze_suspicious_words(token_words)
        suspicious_tokens = susp_stats["suspicious_words"]
        suspicious_ratio = susp_stats["suspicious_ratio"]
        weird_case_count = susp_stats["weird_case_count"]
        rare_cluster_word_count = susp_stats["rare_cluster_word_count"]
        rare_cluster_ratio = susp_stats["rare_cluster_ratio"]
        repeated_noise = susp_stats["repeated_noise"]

        debug_log(f"Подозрительных токенов: {len(suspicious_tokens)}/{len(token_words)} = {suspicious_ratio:.3f}")
        debug_log(f"Токенов с редкими сочетаниями: {rare_cluster_word_count}/{len(token_words)} = {rare_cluster_ratio:.3f}")
        debug_log(f"Токены с неестественным регистром: {weird_case_count}, повтор одного шумового токена: {repeated_noise}")

        if (
            (len(token_words) >= 4 and suspicious_ratio >= 0.50)
            or (len(token_words) >= 8 and suspicious_ratio >= 0.40)
        ):
            preview = ", ".join(suspicious_tokens[:5])
            return True, f"текст содержит много мусорных токенов ({preview})"
        if len(token_words) >= 8 and weird_case_count >= 2 and suspicious_ratio >= 0.15:
            return True, "текст содержит много токенов с неестественным регистром"
        if len(token_words) >= 8 and repeated_noise >= 2 and suspicious_ratio >= 0.15:
            return True, "текст содержит повторяющийся шумовой токен"

    return False, ""

# синтаксические зависимости
def get_root_member_info(token) -> Tuple[str, str]:
    """сказуемое или нет"""
    pos = getattr(token, "pos", None)
    if pos in {"VERB", "AUX"}:
        return "skaz", "Сказуемое"
    return "none", "Другое"


def get_conj_member_info(token, sent, token_by_id: Dict[str, object]) -> Tuple[str, str]:
    """для сочиненных слов conj поиск синтаксической роли с помощью дерева"""
    if not hasattr(token, "head_id") or token.head_id is None:
        return "none", "Другое"

    current_token = token
    visited = set()
    for _ in range(1000):
        if not current_token:
            break
        if hasattr(current_token, "id") and current_token.id in visited:
            break
        if hasattr(current_token, "id"):
            visited.add(current_token.id)

        head_token = token_by_id.get(current_token.head_id)
        if not head_token:
            break

        rel = head_token.rel if hasattr(head_token, "rel") else None
        
        if rel == "root":
            return get_root_member_info(head_token)
        if rel in CONJ_ROLE_MAP:
            return CONJ_ROLE_MAP[rel]
        if rel == "conj":
            current_token = head_token
            continue
        return "none", "Другое"
    return "none", "Другое"


def get_relation_description(relation, token=None, sent=None, token_by_id=None) -> str:
    """Перевод терминов Наташи на русский"""
    if relation == "root" and token:
        _, desc = get_root_member_info(token)
        return desc
    if relation == "conj" and token and sent and token_by_id:
        _, desc = get_conj_member_info(token, sent, token_by_id)
        return desc
    return BASE_RELATION_DESC_MAP.get(relation, "Другое")

def get_relation_type(relation, token=None, sent=None, token_by_id=None) -> str:
    """Короткий тип связи для обработки"""
    if relation == "root" and token:
        rel_type, _ = get_root_member_info(token)
        return rel_type
    if relation == "conj" and token and sent and token_by_id:
        rel_type, _ = get_conj_member_info(token, sent, token_by_id)
        return rel_type
    return BASE_RELATION_TYPE_MAP.get(relation, "none")

def get_pos_russian(pos: str) -> str:
    """перевод тегов на русский"""
    pos_map = {
        "NOUN": "существительное",
        "VERB": "глагол",
        "ADJ": "прилагательное",
        "X": "неопределенное слово",
        "ADV": "наречие",
        "PRON": "местоимение",
        "NUM": "числительное",
        "ADP": "предлог",
        "CONJ": "союз",
        "CCONJ": "союз",
        "PART": "частица",
        "INTJ": "междометие",
        "PUNCT": "знак препинания",
        "PROPN": "имя собственное",
        "DET": "определитель",
        "SCONJ": "подчинительный союз",
        "AUX": "вспомогательный глагол",
        "SPACE": "пробел",
    }
    return pos_map.get(pos, pos)

# ядро анализа, извлечение данных
def analyze_syntax(text: str) -> Doc:
    """сегментация-морфология-синтаксис-лемматизация"""
    doc = Doc(text)
    doc.segment(segmenter)
    doc.tag_morph(morph_tagger)
    doc.parse_syntax(syntax_parser)
    for token in doc.tokens:
        token.lemmatize(morph_vocab)
    return doc


def extract_syntax_relations(doc: Doc) -> List[str]:
    """строки вида СЛОВО|POS|REL|HEAD_WORD|SENT_NUM|TYPE|DESC|POS_RU"""
    relations: List[str] = []
    for sent_num, sent in enumerate(doc.sents, 1):
        original_text = sanitize_output_field(sent.text, fallback="")
        if not original_text:
            continue
        relations.append(f"SENT|{sent_num}|{original_text}")

        token_by_id: Dict[str, object] = {}
        for item in sent.tokens:
            if hasattr(item, "id"):
                token_by_id[item.id] = item
                
        for token in sent.tokens:
            if token.pos in ("SPACE", "PUNCT"):
                continue
            head_word = "_"
            if token.head_id is not None and token.head_id in token_by_id:
                head_word = sanitize_output_field(token_by_id[token.head_id].text)

            relation_type = get_relation_type(token.rel, token, sent, token_by_id)
            relation_desc = get_relation_description(token.rel, token, sent, token_by_id)
            pos_ru = get_pos_russian(token.pos)
            relations.append("|".join([
                sanitize_output_field(token.text), sanitize_output_field(token.pos),
                sanitize_output_field(token.rel), sanitize_output_field(head_word),
                str(sent_num), sanitize_output_field(relation_type),
                sanitize_output_field(relation_desc), sanitize_output_field(pos_ru)
            ]))
    return relations

# ввод-вывод
def create_formatted_file_from_text(text: str, output_file: str) -> bool:
    """Запись исходного пронумерованного текста в _formatted.txt"""
    try:
        text = normalize_text(text).strip()
        doc = Doc(text)
        doc.segment(segmenter)
        with open(output_file, "w", encoding="utf-8") as f:
            for i, sent in enumerate(doc.sents, 1):
                cleaned = sanitize_output_field(sent.text, fallback="")
                if cleaned:
                    f.write(f"({i}) {cleaned}\n")
        return True
    except Exception as exc:
        print(f"Ошибка создания форматированного файла: {exc}", file=sys.stderr)
        return False

#Предотвращает бесконечную рекурсию: не создаёт файл, если входной уже имеет суффикс _formatted
def should_create_formatted_file(input_file: str) -> bool: 
    base_name = os.path.splitext(input_file)[0].lower()
    return not base_name.endswith("_formatted")


def process_text(text: str) -> ProcessResult:
    """Главная функция анализа. Очищает текст, проверяет на мусор, проводит разбор и возвращает результат."""
    text = normalize_text(text).strip()
    if not text:
        return ProcessResult(status="filtered", relations=[], message="пустой текст")

    debug_log(f"Проверка текста: {sanitize_output_field(text[:80], fallback='')}")

    blocked, reason = evaluate_raw_text_quality(text, vowel_min=0.30, vowel_max=0.70, avg_word_max=14.0)
    if blocked:
        debug_log(f"Отсев на быстрой проверке: {reason}")
        return ProcessResult(status="filtered", relations=[], message=reason)

    try:
        doc = analyze_syntax(text)
    except Exception as exc:
        return ProcessResult(status="error", relations=[], message=f"ошибка анализа: {exc}")

    blocked, reason = evaluate_doc_quality(doc)
    if blocked:
        debug_log(f"Отсев после морфологии: {reason}")
        return ProcessResult(status="filtered", relations=[], message=reason)

    relations = extract_syntax_relations(doc)
    if not relations:
        return ProcessResult(status="filtered", relations=[], message="пустой результат после разбора")

    debug_log("Текст прошел все проверки")
    return ProcessResult(status="ok", relations=relations)


def read_input_file(path: str) -> str:
    """Чтение файла в кодировке utf-8-sig (автоматически убирает BOM)."""
    with open(path, "r", encoding="utf-8-sig") as f:
        return f.read()


def parse_input_path(argv: Sequence[str]) -> Optional[str]:
    """ожидает ровно один путь к файлу"""
    if len(argv) < 2:
        return None
    return argv[1]


def main() -> int:
    configure_stdio()
    input_file = parse_input_path(sys.argv)
    if not input_file:
        print("Укажите путь к файлу для анализа", file=sys.stderr)
        return EXIT_USAGE_ERROR

    if not os.path.exists(input_file):
        print(f"Файл не найден: {input_file}", file=sys.stderr)
        return EXIT_USAGE_ERROR

    try:
        text = read_input_file(input_file)
    except Exception as exc:
        print(f"Ошибка чтения файла: {exc}", file=sys.stderr)
        return EXIT_IO_ERROR

    result = process_text(text)
    if result.status == "error":
        print(result.message, file=sys.stderr)
        return EXIT_ANALYSIS_ERROR

    if result.status != "ok":
        print(f"ВНИМАНИЕ: анализ отклонен ({result.message})", file=sys.stderr)
        print("Анализ не дал результатов", file=sys.stderr)
        return EXIT_FILTERED

    if should_create_formatted_file(input_file):
        base_name = os.path.splitext(input_file)[0]
        formatted_file = f"{base_name}_formatted.txt"
        if create_formatted_file_from_text(text, formatted_file):
            print(f"FORMATTED_FILE:{formatted_file}")

    for line in result.relations:
        print(line)

    return EXIT_OK


if __name__ == "__main__":
    sys.exit(main())
