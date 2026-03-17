// test_python.cpp
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <windows.h>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <set>
#include <fstream>
#include <map>

using namespace std;

// ==============================================================================
// Перевод POS-тегов на русский язык
// ==============================================================================
string translate_pos(const string& pos) {
    if (pos == "NOUN") return "сущ.";
    if (pos == "PROPN") return "им.соб.";
    if (pos == "VERB") return "глаг.";
    if (pos == "ADJ") return "прил.";
    if (pos == "ADV") return "нареч.";
    if (pos == "PRON") return "мест.";
    if (pos == "ADP") return "предл.";
    if (pos == "CONJ") return "союз";
    if (pos == "PART") return "част.";
    if (pos == "NUM") return "числ.";
    if (pos == "PUNCT") return "пункт.";
    if (pos == "INTJ") return "межд.";
    if (pos == "AUX") return "вспом.глаг.";
    if (pos == "SCONJ") return "подч.союз";
    if (pos == "DET") return "опред.";
    if (pos == "X") return "проч.";
    if (pos == "SPACE") return "пробел";
    return pos;
}

// ==============================================================================
// Перевод синтаксических связей на русский язык
// ==============================================================================
string translate_relation(const string& rel) {
    if (rel == "root") return "сказуемое";
    if (rel == "nsubj") return "подлежащее";
    if (rel == "obj") return "прям.доп.";
    if (rel == "amod") return "определение";
    if (rel == "nmod") return "косв.доп.";
    if (rel == "obl") return "обстоятельство";
    if (rel == "case") return "предлог";
    if (rel == "punct") return "пунктуация";
    if (rel == "advmod") return "нареч.обст.";
    if (rel == "conj") return "союз";
    if (rel == "cop") return "связка";
    if (rel == "aux") return "вспом.глаг.";
    if (rel == "mark") return "подч.союз";
    if (rel == "acl") return "придат.опред.";
    if (rel == "xcomp") return "инф.доп.";
    if (rel == "ccomp") return "придат.изъясн.";
    if (rel == "parataxis") return "паратаксис";
    if (rel == "discourse") return "вводное";
    if (rel == "fixed") return "уст.выраж.";
    if (rel == "flat") return "сост.имя";
    if (rel == "det") return "детерминатив";
    return rel;
}

// ==============================================================================
// Перевод строки результата
// ==============================================================================
string translate_line(const string& line) {
    vector<string> fields;
    string field;
    stringstream ss(line);
    while (getline(ss, field, '|')) {
        fields.push_back(field);
    }
    if (fields.size() == 5) {
        fields[1] = translate_pos(fields[1]);
        fields[2] = translate_relation(fields[2]);
        string result = fields[0];
        for (int i = 1; i < 5; ++i) {
            result += "|" + fields[i];
        }
        return result;
    }
    return line;
}

// ==============================================================================
// Структура для хранения информации о предложении
// ==============================================================================
struct SentenceData {
    int number;
    string full_text;  // Полный текст предложения (со знаками препинания)
    vector<vector<string>> tokens;  // Токены без пунктуации
};

// ==============================================================================
// Запуск Python-скрипта и получение результатов
// ==============================================================================
vector<string> run_python_test(const string& inputFile, string& formattedFile) {
    vector<string> results;
    string command = "python analyzer.py \"" + inputFile + "\"";
    cout << "Запуск: " << command << endl << endl;

    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        cout << "Ошибка запуска Python-скрипта!" << endl;
        return results;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        string line = buffer;
        while (!line.empty() && (line.back() == '\n' || line.back() == '\r')) {
            line.pop_back();
        }
        if (!line.empty()) {
            if (line.find("FORMATTED_FILE:") == 0) {
                formattedFile = line.substr(15);
            }
            else {
                results.push_back(translate_line(line));
            }
        }
    }
    _pclose(pipe);
    return results;
}

void display_results(const vector<string>& results, const string& inputFile) {
    set<string> unique_words;
    set<string> unique_pos;
    set<string> unique_relations;
    int total_tokens = 0;

    for (const string& line : results) {
        vector<string> fields;
        stringstream ss(line);
        string field;
        while (getline(ss, field, '|')) {
            fields.push_back(field);
        }
        if (fields.size() == 5) {
            total_tokens++;
            unique_words.insert(fields[0]);
            unique_pos.insert(fields[1]);
            unique_relations.insert(fields[2]);
        }
    }

    int current_sentence = 0;

    for (const string& line : results) {
        vector<string> fields;
        stringstream ss(line);
        string field;
        while (getline(ss, field, '|')) {
            fields.push_back(field);
        }
        if (fields.size() == 5) {
            if (current_sentence == stoi(fields[4])){
                cout  << fields[1]  << "\t";
            } else {
                current_sentence ++;
                cout  << "\n" << fields[4] << " | " << fields[1];
            }
            
        }
    }
}

// ==============================================================================
// Основная функция
// ==============================================================================
int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "\n=== АНАЛИЗАТОР СИНТАКСИЧЕСКИХ СВЯЗЕЙ (Natasha) ===\n\n";

    string testFile = "test2.txt";
    cout << "Тестовый файл: " << testFile << "\n";
    cout << "----------------------------------------\n\n";

    string formattedFile = "";
    vector<string> results = run_python_test(testFile, formattedFile);

    if (!results.empty()) {
        display_results(results, testFile);

        string htmlFilename = testFile.substr(0, testFile.find_last_of('.')) + "_report.html";
        //generate_html_report(results, testFile, formattedFile, htmlFilename);

        cout << "\n[INFO] " << htmlFilename << endl;
    }
    else {
        cout << "Результаты анализа не получены.\n";
    }

    cin.get();
    return 0;
}