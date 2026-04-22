#!/usr/bin/env python3
import os
import sys
from natasha import Segmenter, MorphVocab, NewsEmbedding, NewsMorphTagger, NewsSyntaxParser, Doc

# Инициализация компонентов Natasha
segmenter = Segmenter()
morph_vocab = MorphVocab()
embedding = NewsEmbedding()
morph_tagger = NewsMorphTagger(embedding)
syntax_parser = NewsSyntaxParser(embedding)

def get_conj_member_info(token, sent):
    """
    Определяет тип и русское описание для однородного члена предложения (relation == 'conj').
    Итеративно поднимается по дереву зависимостей до главного слова (без рекурсии).
    Возвращает кортеж: (тип_фильтра, русское_описание)
    """
    if not hasattr(token, 'head_id') or token.head_id is None:
        return "none", "Другое"
    
    # Итеративный поиск главного слова
    current_token = token
    max_iterations = 1000  # Защита от бесконечного цикла
    iteration = 0
    visited = set()
    
    while current_token and iteration < max_iterations:
        iteration += 1
        
        # Предотвращаем зацикливание
        if hasattr(current_token, 'id') and current_token.id in visited:
            break
        if hasattr(current_token, 'id'):
            visited.add(current_token.id)
        
        # Находим головное слово
        head_token = None
        for t in sent.tokens:
            if hasattr(t, 'id') and hasattr(current_token, 'head_id') and t.id == current_token.head_id:
                head_token = t
                break
        
        if not head_token:
            break
            
        rel = head_token.rel if hasattr(head_token, 'rel') else None
        
        # Прямые соответствия
        mapping = {
            'nsubj': ("pod", "Подлежащее"),
            'root': ("skaz", "Сказуемое"),
            'advcl': ("skaz", "Сказуемое"),
            'obj': ("dop", "Дополнение"),
            'iobj': ("dop", "Дополнение"),
            'obl': ("dop", "Дополнение"),
            'xcomp': ("dop", "Дополнение"),
            'ccomp': ("dop", "Дополнение"),
            'amod': ("opred", "Определение"),
            'det': ("opred", "Определение"),
            'nmod': ("opred", "Определение"),
            'acl': ("opred", "Определение"),
            'advmod': ("ob", "Обстоятельство"),
            'parataxis': ("ob", "Обстоятельство"),
        }
        
        if rel in mapping:
            return mapping[rel]
        
        # Если главное слово тоже однородный член, продолжаем итерацию
        if rel == 'conj':
            current_token = head_token
            continue
            
        return "none", "Другое"
    
    return "none", "Другое"

def get_relation_description(relation, token=None, sent=None):
    """Получение русского описания синтаксической связи"""
    base_desc = {
        "nsubj": "Подлежащее", "obj": "Дополнение", "iobj": "Дополнение", "obl": "Дополнение",
        "xcomp": "Дополнение", "ccomp": "Дополнение",
        "amod": "Определение", "det": "Определение", "nmod": "Определение", "acl": "Определение",
        "advmod": "Обстоятельство", "advcl": "Обстоятельство", "parataxis": "Обстоятельство",
        "root": "Сказуемое", "cop": "Связка", "case": "Предлог",
        "cc": "Союз", "mark": "Союз",
        "nummod": "Числительное", "aux": "Вспомогательный",
        "punct": "Пунктуация"
    }
    
    # Специальная обработка для однородных членов
    if relation == "conj" and token and sent:
        _, desc = get_conj_member_info(token, sent)
        return desc
        
    return base_desc.get(relation, "Другое")

def get_relation_type(relation, token=None, sent=None):
    """Определение типа отношения для фильтрации в C++"""
    base_type = {
        "nsubj": "pod", "obj": "dop", "iobj": "dop", "obl": "dop",
        "xcomp": "dop", "ccomp": "dop",
        "amod": "opred", "det": "opred", "nmod": "opred", "acl": "opred",
        "advmod": "ob", "parataxis": "ob",
        "root": "skaz", "advcl": "skaz"
    }
    
    if relation == "conj" and token and sent:
        t, _ = get_conj_member_info(token, sent)
        return t
        
    return base_type.get(relation, "none")

def get_pos_russian(pos):
    """Получение русского названия части речи"""
    pos_map = {
        "NOUN": "существительное", "VERB": "глагол", "ADJ": "прилагательное",
        "ADV": "наречие", "PRON": "местоимение", "NUM": "числительное",
        "ADP": "предлог", "CONJ": "союз", "CCONJ": "союз", "PART": "частица",
        "INTJ": "междометие", "PUNCT": "знак препинания",
        "PROPN": "имя собственное", "DET": "определитель",
        "AUX": "вспомогательный глагол", "SPACE": "пробел"
    }
    return pos_map.get(pos, pos)

def create_formatted_file(input_file, output_file):
    """Создание файла с пронумерованными предложениями (для отладки/отчета)"""
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            text = f.read().strip()
        
        doc = Doc(text)
        doc.segment(segmenter)
        
        with open(output_file, 'w', encoding='utf-8') as f:
            for i, sent in enumerate(doc.sents, 1):
                cleaned = sent.text.strip()
                if cleaned:
                    f.write(f"({i}) {cleaned}\n")
        return True
    except Exception as e:
        print(f"Ошибка создания файла: {e}", file=sys.stderr)
        return False

def analyze_syntax(text):
    """Полный лингвистический анализ текста"""
    doc = Doc(text)
    doc.segment(segmenter)
    doc.tag_morph(morph_tagger)
    doc.parse_syntax(syntax_parser)
    for token in doc.tokens:
        token.lemmatize(morph_vocab)
    return doc

def extract_syntax_relations(doc):
    """Извлечение синтаксических связей. Формат: 8 полей, разделенных '|'"""
    relations = []
    
    for sent_num, sent in enumerate(doc.sents, 1):
        original_text = sent.text.strip()
        if not original_text:
            continue
            
        relations.append(f"SENT|{sent_num}|{original_text}")
        
        for token in sent.tokens:
            # Пропускаем пробелы и знаки препинания
            if token.pos in ('SPACE', 'PUNCT'):
                continue
                
            head_word = "_"
            if token.head_id is not None:
                for h in sent.tokens:
                    if h.id == token.head_id:
                        head_word = h.text
                        break
                        
            # Передаем token и sent для корректного определения conj
            relation_type = get_relation_type(token.rel, token, sent)
            relation_desc = get_relation_description(token.rel, token, sent)
            pos_ru = get_pos_russian(token.pos)
            
            # Формируем строку: 0-text, 1-POS, 2-rel, 3-head, 4-sent_num, 5-type, 6-desc, 7-pos_ru
            relations.append(f"{token.text}|{token.pos}|{token.rel}|{head_word}|{sent_num}|{relation_type}|{relation_desc}|{pos_ru}")
            
    return relations

def process_text(text):
    """Основная функция обработки"""
    text = text.strip()
    if not text:
        return None
    try:
        doc = analyze_syntax(text)
        return extract_syntax_relations(doc)
    except Exception as e:
        print(f"Ошибка анализа: {e}", file=sys.stderr)
        return None

def main():
    if len(sys.argv) < 2:
        print("Укажите путь к файлу для анализа")
        return
        
    input_file = sys.argv[1]
    if not os.path.exists(input_file):
        print(f"Файл не найден: {input_file}")
        return
        
    # Создаем форматированный файл (нужен для C++ парсера)
    base_name = os.path.splitext(input_file)[0]
    formatted_file = f"{base_name}_formatted.txt"
    if create_formatted_file(input_file, formatted_file):
        print(f"FORMATTED_FILE:{formatted_file}")
        
    # Читаем и анализируем
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            text = f.read()
    except Exception as e:
        print(f"Ошибка чтения: {e}", file=sys.stderr)
        return
        
    results = process_text(text)
    if results:
        for line in results:
            print(line)
    else:
        print("Анализ не дал результатов", file=sys.stderr)

if __name__ == "__main__":
    main()