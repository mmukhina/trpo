import os
import sys
from natasha import Segmenter, MorphVocab, NewsEmbedding, NewsMorphTagger, NewsSyntaxParser, Doc

# Инициализация компонентов Natasha
segmenter = Segmenter()              # Сегментатор токенов
morph_vocab = MorphVocab()           # Словарь для лемматизации
embedding = NewsEmbedding()          # Векторные представления
morph_tagger = NewsMorphTagger(embedding)  # Морфологический таггер
syntax_parser = NewsSyntaxParser(embedding)  # Синтаксический парсер

def get_relation_description(relation):
    """
    Получение русского описания синтаксической связи
    Аналог getRelationDescription из C++
    """
    desc = {
        "nsubj": "Подлежащее",
        "obj": "Дополнение",
        "iobj": "Дополнение",
        "obl": "Дополнение",
        "amod": "Определение",
        "det": "Определение",
        "nmod": "Определение",
        "advmod": "Обстоятельство",
        "advcl": "Обстоятельство",
        "root": "Сказуемое",
        "cop": "Связка",
        "case": "Предлог",
        "conj": "Союз",
        "mark": "Союз",
        "parataxis": "Обстоятельство",
        "nummod": "Числительное",
        "aux": "Вспомогательный",
        "punct": "Пунктуация",
        "cc": "Союз",
        "acl": "Определение",
        "xcomp": "Дополнение",
        "ccomp": "Дополнение"
    }
    return desc.get(relation, "Другое")

def get_relation_type(relation):
    """
    Определение типа отношения для фильтрации
    Аналог getRelationType из C++
    """
    if relation == "nsubj":
        return "pod"  # подлежащее
    elif relation == "root" or relation == "advcl":
        return "skaz"  # сказуемое
    elif relation in ["amod", "det", "nmod", "acl"]:
        return "opred"  # определение
    elif relation in ["obj", "iobj", "obl", "xcomp", "ccomp"]:
        return "dop"  # дополнение
    elif relation in ["advmod", "parataxis"]:
        return "ob"  # обстоятельство
    else:
        return "none"

def create_formatted_file(input_file, output_file):
    """
    Создание файла с пронумерованными предложениями
    """
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            text = f.read().strip()
        
        doc = Doc(text)
        doc.segment(segmenter)
        
        with open(output_file, 'w', encoding='utf-8') as f:
            for i, sent in enumerate(doc.sents, 1):
                cleaned_sent = sent.text.strip()
                if cleaned_sent:
                    f.write(f"({i}) {cleaned_sent}")
                    if i < len(doc.sents):
                        f.write("\n")
        return True
    except Exception as e:
        print(f"Ошибка создания файла: {e}", file=sys.stderr)
        return False

def analyze_syntax(text):
    """
    Полный лингвистический анализ с акцентом на синтаксис
    """
    doc = Doc(text)
    # Последовательная обработка компонентами (раздел 2.1)
    doc.segment(segmenter)
    doc.tag_morph(morph_tagger)
    doc.parse_syntax(syntax_parser)

    # Лемматизация
    for token in doc.tokens:
        token.lemmatize(morph_vocab)

    return doc

def extract_syntax_relations(doc):
    """
    Извлечение синтаксических связей между токенами.
    Формат вывода:
    - SENT|номер|текст_предложения (для оригинального текста)
    - Слово|POS|Relation|Head_Word|Sentence_Num|Relation_Type|Relation_Desc (для токенов)
    """
    relations = []
    for sent_num, sent in enumerate(doc.sents, 1):
        # Выводим оригинальное предложение целиком
        original_text = sent.text.strip()
        relations.append(f"SENT|{sent_num}|{original_text}")
        
        for token in sent.tokens:
            # Пропускаем пробелы и пунктуацию
            if token.pos == 'SPACE' or token.pos == 'PUNCT':
                continue
            
            # Получаем информацию о главном слове
            head_word = "_"
            head_id = getattr(token, 'head_id', None)
            
            if head_id is not None:
                for head_token in sent.tokens:
                    if head_token.id == head_id:
                        head_word = head_token.text
                        break
            
            # Получаем преобразованные типы отношений
            relation_type = get_relation_type(token.rel)
            relation_desc = get_relation_description(token.rel)
            
            # Формируем строку результата с дополнительными полями
            # Формат: слово|POS|relation|head_word|sent_num|relation_type|relation_desc
            relations.append(f"{token.text}|{token.pos}|{token.rel}|{head_word}|{sent_num}|{relation_type}|{relation_desc}")
            
    return relations

def process_text(text):
    """
    Основная функция обработки (точка входа)
    """
    try:
        doc = analyze_syntax(text)
        relations = extract_syntax_relations(doc)
        return relations
    except Exception as e:
        print(f"Ошибка при анализе текста: {e}", file=sys.stderr)
        return None

def main():
    """
    Обработка аргументов командной строки
    """
    if len(sys.argv) < 2:
        print("Укажите путь к файлу для анализа")
        return
    
    input_file = sys.argv[1]

    if not os.path.exists(input_file):
        print(f"Файл не найден: {input_file}")
        return

    # Создаем форматированный файл с нумерацией
    base_name = os.path.splitext(input_file)[0]
    formatted_file = f"{base_name}_formatted.txt"

    if create_formatted_file(input_file, formatted_file):
        print(f"FORMATTED_FILE:{formatted_file}")

    # Чтение текста
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            text = f.read().strip()
    except Exception as e:
        print(f"Ошибка чтения файла: {e}")
        return

    if not text:
        print("Файл пустой")
        return

    # Запуск анализа
    results = process_text(text)

    if results:
        # Вывод в stdout для перехвата C++ программой
        for item in results:
            print(item)
    else:
        print("Анализ не дал результатов")

if __name__ == "__main__":
    main()
