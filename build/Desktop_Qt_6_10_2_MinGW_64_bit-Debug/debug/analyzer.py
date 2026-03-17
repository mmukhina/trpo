import os
import sys
import io
from natasha import Segmenter, MorphVocab, NewsEmbedding, NewsMorphTagger, NewsSyntaxParser, Doc

# Принудительная установка UTF-8 для вывода
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8')

# Для Windows
if sys.platform == 'win32':
    os.system('chcp 65001 > nul')

segmenter = Segmenter()
morph_vocab = MorphVocab()
embedding = NewsEmbedding()
morph_tagger = NewsMorphTagger(embedding)
syntax_parser = NewsSyntaxParser(embedding)

def create_formatted_file(input_file, output_file):
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
        print(f"FORMATTED_FILE:{output_file}", flush=True)
        return True
    except Exception as e:
        print(f"Ошибка создания файла: {e}", file=sys.stderr, flush=True)
        return False

def analyze_syntax(text):
    doc = Doc(text)
    doc.segment(segmenter)
    doc.tag_morph(morph_tagger)
    doc.parse_syntax(syntax_parser)
    
    for token in doc.tokens:
        token.lemmatize(morph_vocab)
    
    return doc

def extract_syntax_relations(doc):
    relations = []
    
    for sent_num, sent in enumerate(doc.sents, 1):
        original_text = sent.text.strip()
        relations.append(f"SENT|{sent_num}|{original_text}")
        print(f"SENT|{sent_num}|{original_text}", flush=True)
        
        for token in sent.tokens:
            if token.pos == 'SPACE' or token.pos == 'PUNCT':
                continue
            
            head_word = "_"
            head_id = getattr(token, 'head_id', None)
            
            if head_id is not None:
                for head_token in sent.tokens:
                    if head_token.id == head_id:
                        head_word = head_token.text
                        break
            
            result = f"{token.text}|{token.pos}|{token.rel}|{head_word}|{sent_num}"
            relations.append(result)
            print(result, flush=True)
            
    return relations

def process_text(text):
    try:
        doc = analyze_syntax(text)
        relations = extract_syntax_relations(doc)
        return relations
    except Exception as e:
        print(f"Ошибка при анализе текста: {e}", file=sys.stderr, flush=True)
        return None

def main():
    if len(sys.argv) < 2:
        print("Укажите путь к файлу для анализа", flush=True)
        return
    
    input_file = sys.argv[1]
    
    if not os.path.exists(input_file):
        print(f"Файл не найден: {input_file}", flush=True)
        return
    
    base_name = os.path.splitext(input_file)[0]
    formatted_file = f"{base_name}_formatted.txt"
    
    create_formatted_file(input_file, formatted_file)
    
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            text = f.read().strip()
    except Exception as e:
        print(f"Ошибка чтения файла: {e}", flush=True)
        return
    
    if not text:
        print("Файл пустой", flush=True)
        return
    
    results = process_text(text)
    
    if not results:
        print("Анализ не дал результатов", flush=True)

if __name__ == "__main__":
    main()