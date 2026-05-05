#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QDir>

const QString SCRIPT_PATH = "/analyzer.py";
const QString TEMP_DIR = "/temp";

// Инициализация процесса для вызова Python скрипта
void MainWindow::setupPythonProcess()
{
    pythonProcess = new QProcess(this);
    // Объединяем стандартный вывод и вывод ошибок в один канал
    pythonProcess->setProcessChannelMode(QProcess::MergedChannels);

    // Подключаем функции для обработки данных от Python скрипта
    connect(pythonProcess, &QProcess::readyReadStandardOutput,
            this, &MainWindow::handlePythonOutput);
    connect(pythonProcess, &QProcess::readyReadStandardError,
            this, &MainWindow::handlePythonError);
    connect(pythonProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &MainWindow::handlePythonFinished);
}

// Обработка стандартного вывода от Python скрипта
void MainWindow::handlePythonOutput()
{
    if (pythonProcess) {
        QByteArray outputData = pythonProcess->readAllStandardOutput();
        QString output = QString::fromUtf8(outputData);
        processPythonOutput(output); // Передаем данные на парсинг
    }
}

// Обработка ошибок от Python скрипта
void MainWindow::handlePythonError()
{
    if (pythonProcess) {
        QByteArray errorData = pythonProcess->readAllStandardError();
        QString error = QString::fromUtf8(errorData);

        if (!error.isEmpty()) {
            QMessageBox::critical(this, "Ошибка Python скрипта",
                                  "Ошибка от Python скрипта:\n" + error,
                                  QMessageBox::Ok);
        }
    }
}

// Обработка завершения Python процесса
void MainWindow::handlePythonFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    // Возвращаем кнопкам активное состояние
    ui->btn_search->setEnabled(true);
    ui->btn_search->setText("Поиск");
    ui->btn_upload->setEnabled(true);
    statusBar()->clearMessage();

    // Удаляем временный файл с текстом, если он существует
    if (!currentTempFile.isEmpty() && QFile::exists(currentTempFile)) {
        QFile::remove(currentTempFile);
        currentTempFile.clear();
    }

    // Обрабатываем аварийное завершение
    if (exitStatus == QProcess::CrashExit) {
        QMessageBox::critical(this, "Аварийное завершение",
                              "Программа анализа неожиданно завершила работу.\n\n"
                              "Попробуйте перезапустить приложение.");
        refreshAllDisplay();
    }

    // Обрабатываем ненулевой код возврата (ошибка в скрипте)
    else if (exitCode != 0) {
        QString errorMessage;
        switch (exitCode) {
        case 2: errorMessage = "Неверные параметры скрипта"; break;
        case 3: errorMessage = "Ошибка чтения файла"; break;
        case 4: errorMessage = "Ошибка синтаксического анализа"; break;
        case 5: errorMessage = "Текст не распознан как русскоязычный"; break;
        default: errorMessage = "Неизвестная ошибка";
        }

        QMessageBox::warning(this, "Ошибка анализа",
                             QString("Не удалось выполнить анализ: %1\n\n").arg(errorMessage));
        refreshAllDisplay();
    }
    else {
        // Анализ успешно завершен
        statusBar()->showMessage("Анализ успешно завершен", 3000);
        // Запоминаем проанализированный текст для отслеживания изменений
        analyzedText = ui->textEdit->toPlainText();
        refreshAllDisplay(); // Обновляем все вкладки с результатами
    }
}

// Запуск Python скрипта для анализа текста
bool MainWindow::callNatasha(const QString path){
    // Формируем путь к Python скрипту
    QString scriptPath = QCoreApplication::applicationDirPath() + SCRIPT_PATH;
    QFileInfo scriptFile(scriptPath);

    // Проверяем существование скрипта
    if (!scriptFile.exists()) {
        QMessageBox::critical(this, "Скрипт не найден",
                              "Python скрипт не найден по пути:\n" + scriptPath +
                                  "\n\nУбедитесь, что файл analyzer.py находится в папке с программой.",
                              QMessageBox::Ok);
        ui->btn_search->setEnabled(true);
        ui->btn_search->setText("Поиск");
        ui->btn_upload->setEnabled(true);
        if (!path.isEmpty() && QFile::exists(path)) {
            QFile::remove(path);
        }
        return false;
    }

    // Аргументы для Python: путь к скрипту и путь к файлу с текстом
    QStringList arguments;
    arguments << scriptPath;
    arguments << path;

    // Устанавливаем переменные окружения для корректной работы с UTF-8
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PYTHONIOENCODING", "utf-8");
    env.insert("LANG", "en_US.UTF-8");
    pythonProcess->setProcessEnvironment(env);

    // Очищаем старые данные перед новым анализом
    sentenceTexts.clear();
    wordsBySentence.clear();
    ui->treeWidget->clear();

    statusBar()->showMessage("Выполняется синтаксический анализ...");

    // Запускаем Python процесс
    pythonProcess->start("python", arguments);

    // Проверяем, успешно ли запустился процесс
    if (!pythonProcess->waitForStarted(3000)) {
        QMessageBox::critical(this, "Ошибка запуска",
                              "Не удалось запустить Python скрипт.\n"
                              "Убедитесь, что Python установлен и доступен в командной строке.\n\n"
                              "Ошибка: " + pythonProcess->errorString(),
                              QMessageBox::Ok);

        ui->btn_search->setEnabled(true);
        ui->btn_search->setText("Поиск");
        ui->btn_upload->setEnabled(true);
        statusBar()->clearMessage();
        if (!path.isEmpty() && QFile::exists(path)) {
            QFile::remove(path);
        }
        return false;
    }
    return true;
}

// Запуск полного цикла анализа текста
void MainWindow::runAnalysis(const QString& text)
{
    // Проверяем, выбран ли хоть один фильтр
    if (!isAnyFilterSelected()) {
        QMessageBox::warning(this, "Нет активных фильтров",
                             "Не выбран ни один фильтр для отображения слов.\n\n"
                             "Пожалуйста, выберите хотя бы одну категорию.",
                             QMessageBox::Ok);
        return;
    }

    // Проверяем, есть ли в тексте буквы
    if (!hasAnyLetter(text)) {
        QMessageBox::warning(this, "Нет букв",
                             "Текст не содержит ни одной буквы.\n"
                             "Пожалуйста, введите текст с буквами для анализа.",
                             QMessageBox::Ok);
        return;
    }

    // Проверяем, не запущен ли уже анализ
    if (pythonProcess && pythonProcess->state() == QProcess::Running) {
        QMessageBox::warning(this, "Процесс занят",
                             "Анализ уже выполняется. Пожалуйста, подождите.",
                             QMessageBox::Ok);
        return;
    }

    // Блокируем кнопки на время анализа
    ui->btn_search->setEnabled(false);
    ui->btn_search->setText("Поиск...");
    ui->btn_upload->setEnabled(false);
    ui->btn_download->setEnabled(false);

    // Создаем временный файл с текстом для передачи в Python
    QString tempFilePath = createTempFileWithText(text);

    if (tempFilePath.isEmpty()) {
        QMessageBox::critical(this, "Ошибка файла",
                              "Не удалось создать временный файл для анализа.",
                              QMessageBox::Ok);
        ui->btn_search->setEnabled(true);
        ui->btn_search->setText("Поиск");
        ui->btn_upload->setEnabled(true);
        return;
    }

    // Запускаем Python скрипт
    if (!callNatasha(tempFilePath)) {
        ui->btn_search->setEnabled(true);
        ui->btn_search->setText("Поиск");
        ui->btn_upload->setEnabled(true);
        if (!tempFilePath.isEmpty() && QFile::exists(tempFilePath)) {
            QFile::remove(tempFilePath);
        }
        return;
    }

    currentTempFile = tempFilePath; // Сохраняем путь для последующего удаления
}

// Парсинг вывода Python скрипта
void MainWindow::processPythonOutput(const QString& output)
{
    // Разбиваем вывод на строки, игнорируя пустые
    QStringList lines = output.split("\n", Qt::SkipEmptyParts);

    // Очищаем каждую строку от пробелов в начале и конце
    for (int i = 0; i < lines.size(); ++i) {
        QString& line = lines[i];
        line = line.trimmed();
    }

    if (lines.isEmpty()) {
        return;
    }

    // Обрабатываем каждую строку вывода
    for (int i = 0; i < lines.size(); ++i) {
        const QString& line = lines[i];
        // Пропускаем строку с информацией о форматированном файле
        if (line.startsWith("FORMATTED_FILE:")) {
            continue;
        }

        // Разделяем строку на части по символу "|"
        QStringList parts = line.split("|");

        if (parts.isEmpty()) continue;

        // Обработка строки с информацией о предложении
        if (parts[0] == "SENT" && parts.size() >= 3) {
            int sentNum = parts[1].toInt();      // Номер предложения
            QString sentenceText = parts[2];     // Текст предложения
            sentenceTexts[sentNum] = sentenceText;
        }
        // Обработка строки с информацией о слове (должно быть минимум 8 полей)
        else if (parts.size() >= 8) {
            WordInfo word;
            word.text = parts[0].trimmed();           // Текст слова
            word.speech = parts[1].trimmed();         // Часть речи
            word.sentence = parts[6].trimmed();       // Синтаксическая роль на русском
            word.sentenceNum = parts[4].toInt();      // Номер предложения
            word.posRussian = parts[7].trimmed();     // Часть речи на русском
            wordsBySentence[word.sentenceNum].append(word); // Добавляем слово в список
        }
    }
}

// Создание временного файла с текстом для передачи в Python
QString MainWindow::createTempFileWithText(const QString& text)
{
    // Папка для временных файлов в директории программы
    QString tempDir = QCoreApplication::applicationDirPath() + TEMP_DIR;
    QDir dir;
    if (!dir.exists(tempDir)) {
        dir.mkpath(tempDir); // Создаем папку если её нет
    }

    // Генерируем уникальное имя файла с использованием временной метки
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
    QString tempFilePath = tempDir + "/input_" + timestamp + ".txt";

    QFile file(tempFilePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf8);
        out << text;
        file.close();
        return tempFilePath;
    }

    return QString(); // Возвращаем пустую строку при ошибке
}
