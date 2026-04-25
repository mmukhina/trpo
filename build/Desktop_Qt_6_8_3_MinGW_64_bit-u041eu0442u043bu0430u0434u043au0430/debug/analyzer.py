#!/usr/bin/env python3
import os
import re
import sys
from collections import Counter
from dataclasses import dataclass
from typing import Dict, List, Optional, Sequence, Tuple

from natasha import (
    Doc,
    MorphVocab,
    NewsEmbedding,
    NewsMorphTagger,
    NewsSyntaxParser,
    Segmenter,
)

# Initialization of Natasha components.
segmenter = Segmenter()
morph_vocab = MorphVocab()
embedding = NewsEmbedding()
morph_tagger = NewsMorphTagger(embedding)
syntax_parser = NewsSyntaxParser(embedding)

DEBUG_ENABLED = os.getenv("ANALYZER_DEBUG", "1").lower() not in {"0", "false", "no"}

CYRILLIC_CHAR_RE = re.compile(r"[а-яА-ЯёЁ]")
CYRILLIC_WORD_RE = re.compile(r"[а-яА-ЯёЁ-]+")
SPACE_RE = re.compile(r"\s+")
VOWELS = set("аеёиоуыэюяАЕЁИОУЫЭЮЯ")
VOWELS_LOWER = set("аеёиоуыэюя")
KEYBOARD_ROWS = ("йцукенгшщзхъ", "фывапролджэ", "ячсмитьбю")
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

EXIT_OK = 0
EXIT_USAGE_ERROR = 2
EXIT_IO_ERROR = 3
EXIT_ANALYSIS_ERROR = 4
EXIT_FILTERED = 5


@dataclass
class ProcessResult:
    status: str
    relations: List[str]
    message: str = ""


def debug_log(message: str) -> None:
    if DEBUG_ENABLED:
        print(f"[DEBUG] {message}", file=sys.stderr)


def configure_stdio() -> None:
    # Keep existing encoding (cp1251/utf-8/etc), but never crash on non-representable characters.
    for stream in (sys.stdout, sys.stderr):
        if hasattr(stream, "reconfigure"):
            stream.reconfigure(errors="replace")


def normalize_text(text: str) -> str:
    text = text.replace("\ufeff", "")
    text = text.replace("\r\n", "\n").replace("\r", "\n")
    return text


def sanitize_output_field(value: object, fallback: str = "_") -> str:
    s = "" if value is None else str(value)
    s = s.replace("\ufeff", "")
    s = s.replace("\r", " ").replace("\n", " ")
    s = s.replace("|", "/")
    s = SPACE_RE.sub(" ", s).strip()
    return s if s else fallback


def contains_keyboard_run(word: str, min_run: int = 4) -> bool:
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


def is_weird_mixed_case(word: str) -> bool:
    letters = [ch for ch in word if ("а" <= ch.lower() <= "я") or ch.lower() == "ё"]
    if len(letters) < 3:
        return False

    has_upper = any(ch.isupper() for ch in letters)
    has_lower = any(ch.islower() for ch in letters)
    if not (has_upper and has_lower):
        return False

    # Нормальный title-case не считаем мусором.
    if letters[0].isupper() and all(ch.islower() for ch in letters[1:]):
        return False

    return True


def get_suspicious_word_reasons(word: str) -> List[str]:
    w = word.lower().strip("-")
    reasons: List[str] = []
    if len(w) < 3:
        return reasons
    if is_weird_mixed_case(word):
        reasons.append("неестественный регистр")
    if not any(ch in VOWELS_LOWER for ch in w):
        reasons.append("без гласных")
    if len(w) >= 4 and contains_keyboard_run(w, min_run=4):
        reasons.append("похоже на клавиатурный набор")
    if len(w) >= 4 and len(w) >= 2 and w[-1] == w[-2] and w[-1] in VOWELS_LOWER:
        reasons.append("подозрительное удвоение гласной")
    if len(w) >= 5 and max_repeating_run(w) >= 3:
        reasons.append("длинная повторяющаяся последовательность")
    if len(w) >= 6 and max_consonant_run(w) >= 5:
        reasons.append("слишком длинный кластер согласных")
    if len(w) >= 6:
        unique_ratio = len(set(w)) / len(w)
        if unique_ratio < 0.35:
            reasons.append("слишком низкое разнообразие символов")
    if len(w) >= 8:
        vowel_ratio = sum(1 for ch in w if ch in VOWELS_LOWER) / len(w)
        if vowel_ratio < 0.15:
            reasons.append("аномально мало гласных")
    return reasons


def evaluate_raw_text_quality(
    text: str,
    vowel_min: float = 0.30,
    vowel_max: float = 0.70,
    avg_word_max: float = 14.0,
) -> Tuple[bool, str]:
    cyrillic_chars = CYRILLIC_CHAR_RE.findall(text)
    debug_log(f"Кириллических символов: {len(cyrillic_chars)}")
    if len(cyrillic_chars) < 5:
        return True, "мало кириллицы"

    words = [w.strip("-") for w in CYRILLIC_WORD_RE.findall(text)]
    words = [w for w in words if w]
    if not words:
        return True, "нет слов"

    vowel_ratio = sum(1 for c in cyrillic_chars if c in VOWELS) / len(cyrillic_chars)
    debug_log(f"Соотношение гласных: {vowel_ratio:.3f} (норма: {vowel_min}-{vowel_max})")
    if vowel_ratio < vowel_min or vowel_ratio > vowel_max:
        return True, "аномальное соотношение гласных"

    avg_len = sum(len(w) for w in words) / len(words)
    debug_log(f"Средняя длина слова: {avg_len:.2f} (макс: {avg_word_max})")
    if avg_len > avg_word_max:
        return True, "слова слишком длинные"

    suspicious_details: List[Tuple[str, List[str]]] = []
    for w in words:
        reasons = get_suspicious_word_reasons(w)
        if reasons:
            suspicious_details.append((w, reasons))

    suspicious_words = [w for w, _ in suspicious_details]
    suspicious_ratio = len(suspicious_words) / len(words)
    debug_log(f"Подозрительных слов: {len(suspicious_words)}/{len(words)} = {suspicious_ratio:.3f}")

    stopword_count = sum(1 for w in words if w.lower() in COMMON_RU_STOPWORDS)
    stopword_ratio = stopword_count / len(words)
    debug_log(f"Служебных русских слов: {stopword_count}/{len(words)} = {stopword_ratio:.3f}")

    weird_case_count = sum(1 for _, reasons in suspicious_details if "неестественный регистр" in reasons)
    repeated_noise = max(Counter(w.lower() for w in suspicious_words).values(), default=0)
    debug_log(
        f"Неестественный регистр: {weird_case_count}, "
        f"повтор одного шумового слова: {repeated_noise}"
    )

    if (len(words) >= 3 and suspicious_ratio >= 0.55) or (len(words) >= 6 and suspicious_ratio >= 0.40):
        preview = ", ".join(suspicious_words[:5])
        return True, f"слишком много мусорных слов ({preview})"
    if len(words) >= 8 and weird_case_count >= 2 and suspicious_ratio >= 0.15:
        return True, "слишком много слов с неестественным регистром"
    if len(words) >= 8 and repeated_noise >= 2 and suspicious_ratio >= 0.15:
        return True, "много повторяющегося шумового слова"
    if len(words) >= 30 and stopword_ratio < 0.03:
        return True, "аномально мало служебных слов для русского текста"

    return False, ""


def evaluate_doc_quality(doc: Doc) -> Tuple[bool, str]:
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
        suspicious_details: List[Tuple[str, List[str]]] = []
        for w in token_words:
            reasons = get_suspicious_word_reasons(w)
            if reasons:
                suspicious_details.append((w, reasons))

        suspicious_tokens = [w for w, _ in suspicious_details]
        suspicious_ratio = len(suspicious_tokens) / len(token_words)
        debug_log(
            f"Подозрительных токенов: {len(suspicious_tokens)}/{len(token_words)} = {suspicious_ratio:.3f}"
        )

        weird_case_count = sum(1 for _, reasons in suspicious_details if "неестественный регистр" in reasons)
        repeated_noise = max(Counter(w.lower() for w in suspicious_tokens).values(), default=0)
        debug_log(
            f"Токены с неестественным регистром: {weird_case_count}, "
            f"повтор одного шумового токена: {repeated_noise}"
        )

        if (len(token_words) >= 4 and suspicious_ratio >= 0.55) or (len(token_words) >= 8 and suspicious_ratio >= 0.40):
            preview = ", ".join(suspicious_tokens[:5])
            return True, f"текст содержит много мусорных токенов ({preview})"
        if len(token_words) >= 8 and weird_case_count >= 2 and suspicious_ratio >= 0.15:
            return True, "текст содержит много токенов с неестественным регистром"
        if len(token_words) >= 8 and repeated_noise >= 2 and suspicious_ratio >= 0.15:
            return True, "текст содержит повторяющийся шумовой токен"

    return False, ""


def get_conj_member_info(token, sent) -> Tuple[str, str]:
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

        head_token = next((t for t in sent.tokens if hasattr(t, "id") and t.id == current_token.head_id), None)
        if not head_token:
            break

        rel = head_token.rel if hasattr(head_token, "rel") else None
        mapping = {
            "nsubj": ("pod", "Подлежащее"),
            "root": ("skaz", "Сказуемое"),
            "advcl": ("skaz", "Сказуемое"),
            "obj": ("dop", "Дополнение"),
            "iobj": ("dop", "Дополнение"),
            "obl": ("dop", "Дополнение"),
            "xcomp": ("dop", "Дополнение"),
            "ccomp": ("dop", "Дополнение"),
            "amod": ("opred", "Определение"),
            "det": ("opred", "Определение"),
            "nmod": ("opred", "Определение"),
            "acl": ("opred", "Определение"),
            "advmod": ("ob", "Обстоятельство"),
            "parataxis": ("ob", "Обстоятельство"),
        }
        if rel in mapping:
            return mapping[rel]
        if rel == "conj":
            current_token = head_token
            continue
        return "none", "Другое"
    return "none", "Другое"


def get_relation_description(relation, token=None, sent=None) -> str:
    base_desc = {
        "nsubj": "Подлежащее",
        "obj": "Дополнение",
        "iobj": "Дополнение",
        "obl": "Дополнение",
        "xcomp": "Дополнение",
        "ccomp": "Дополнение",
        "amod": "Определение",
        "det": "Определение",
        "nmod": "Определение",
        "acl": "Определение",
        "advmod": "Обстоятельство",
        "advcl": "Обстоятельство",
        "parataxis": "Обстоятельство",
        "root": "Сказуемое",
        "cop": "Связка",
        "case": "Предлог",
        "cc": "Союз",
        "mark": "Союз",
        "nummod": "Числительное",
        "aux": "Вспомогательный",
        "punct": "Пунктуация",
    }
    if relation == "conj" and token and sent:
        _, desc = get_conj_member_info(token, sent)
        return desc
    return base_desc.get(relation, "Другое")


def get_relation_type(relation, token=None, sent=None) -> str:
    base_type = {
        "nsubj": "pod",
        "obj": "dop",
        "iobj": "dop",
        "obl": "dop",
        "xcomp": "dop",
        "ccomp": "dop",
        "amod": "opred",
        "det": "opred",
        "nmod": "opred",
        "acl": "opred",
        "advmod": "ob",
        "parataxis": "ob",
        "root": "skaz",
        "advcl": "skaz",
    }
    if relation == "conj" and token and sent:
        rel_type, _ = get_conj_member_info(token, sent)
        return rel_type
    return base_type.get(relation, "none")


def get_pos_russian(pos: str) -> str:
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


def analyze_syntax(text: str) -> Doc:
    doc = Doc(text)
    doc.segment(segmenter)
    doc.tag_morph(morph_tagger)
    doc.parse_syntax(syntax_parser)
    for token in doc.tokens:
        token.lemmatize(morph_vocab)
    return doc


def extract_syntax_relations(doc: Doc) -> List[str]:
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

            relation_type = get_relation_type(token.rel, token, sent)
            relation_desc = get_relation_description(token.rel, token, sent)
            pos_ru = get_pos_russian(token.pos)

            relations.append(
                "|".join(
                    [
                        sanitize_output_field(token.text),
                        sanitize_output_field(token.pos),
                        sanitize_output_field(token.rel),
                        sanitize_output_field(head_word),
                        str(sent_num),
                        sanitize_output_field(relation_type),
                        sanitize_output_field(relation_desc),
                        sanitize_output_field(pos_ru),
                    ]
                )
            )
    return relations


def create_formatted_file_from_text(text: str, output_file: str) -> bool:
    try:
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


def should_create_formatted_file(input_file: str) -> bool:
    base_name = os.path.splitext(input_file)[0].lower()
    return not base_name.endswith("_formatted")


def process_text(text: str) -> ProcessResult:
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
    # utf-8-sig removes BOM automatically and prevents crashes on output.
    with open(path, "r", encoding="utf-8-sig") as f:
        return f.read()


def parse_input_path(argv: Sequence[str]) -> Optional[str]:
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
