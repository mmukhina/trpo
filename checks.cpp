#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>

// Проверка, содержит ли текст хотя бы одну букву
bool MainWindow::hasAnyLetter(const QString& text)
{
    for (int i = 0; i < text.length(); ++i) {
        QChar ch = text[i];
        if (ch.isLetter()) {
            return true;
        }
    }
    return false;
}

// Проверка, состоит ли текст только из цифр
bool MainWindow::hasOnlyNumbers(const QString& text)
{
    for (int i = 0; i < text.length(); ++i) {
        QChar ch = text[i];
        // Если есть буква или русская буква - возвращаем false
        if (ch.isLetter() || (ch.unicode() >= 0x0400 && ch.unicode() <= 0x04FF)) return false;
        // Пропускаем знаки препинания и пробелы
        if (ch.isPunct() || ch.isSpace()) continue;
        // Если не цифра - возвращаем false
        if (!ch.isDigit()) return false;
    }
    return true;
}

// Проверка, является ли файл корректным UTF-8
bool MainWindow::isUtf8File(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return false;
    QByteArray data = file.readAll();
    file.close();

    if (data.isEmpty()) return true;

    // Проверка UTF-8 последовательностей
    const char* ptr = data.constData();
    int len = data.length();
    for (int i = 0; i < len; ) {
        unsigned char c = ptr[i];
        int extraBytes = 0;

        // Определяем количество байт в UTF-8 символе по старшим битам
        if (c < 0x80) {
            extraBytes = 0; // ASCII символ
        } else if ((c & 0xE0) == 0xC0) {
            extraBytes = 1; // 2-байтовый UTF-8
        } else if ((c & 0xF0) == 0xE0) {
            extraBytes = 2; // 3-байтовый UTF-8
        } else if ((c & 0xF8) == 0xF0) {
            extraBytes = 3; // 4-байтовый UTF-8
        } else {
            return false; // Некорректный начальный байт
        }

        // Проверяем последующие байты (должны иметь вид 10xxxxxx)
        for (int j = 0; j < extraBytes; j++) {
            i++;
            if (i >= len || (ptr[i] & 0xC0) != 0x80) return false;
        }
        i++;
    }
    return true;
}

// Определяет, нужно ли показывать слово в результатах на основе выбранных фильтров
bool MainWindow::shouldShowWord(const WordInfo& word)
{
    QString type = word.sentence;

    if (type == "Подлежащее") return ui->c_pod->isChecked();
    if (type == "Сказуемое") return ui->c_skaz->isChecked();
    if (type == "Определение") return ui->c_opred->isChecked();
    if (type == "Дополнение") return ui->c_dop->isChecked();
    if (type == "Обстоятельство") return ui->c_ob->isChecked();
    return ui->c_none->isChecked(); // Все остальные роли идут в "Другое"
}

// Проверка, выбран ли хотя бы один фильтр для отображения
bool MainWindow::isAnyFilterSelected()
{
    return ui->c_pod->isChecked() ||
           ui->c_skaz->isChecked() ||
           ui->c_opred->isChecked() ||
           ui->c_dop->isChecked() ||
           ui->c_ob->isChecked() ||
           ui->c_none->isChecked();
}
