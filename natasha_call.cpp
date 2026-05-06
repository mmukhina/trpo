#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QDir>

// константы
const QString SCRIPT_PATH = "/analyzer.py";
const QString TEMP_DIR = "/temp";

// коды возврата python скрипта
const int EXIT_OK = 0;
const int EXIT_USAGE_ERROR = 2;
const int EXIT_IO_ERROR = 3;
const int EXIT_ANALYSIS_ERROR = 4;
const int EXIT_FILTERED = 5;

// инициализация процесса для вызова python скрипта
void MainWindow::setupPythonProcess()
{
    pythonProcess = new QProcess(this);
    // разделяем stdout и stderr для правильного захвата ошибок
    pythonProcess->setProcessChannelMode(QProcess::SeparateChannels);

    // подключаем функции для обработки данных от python скрипта
    connect(pythonProcess, &QProcess::readyReadStandardOutput,
            this, &MainWindow::handlePythonOutput);
    connect(pythonProcess, &QProcess::readyReadStandardError,
            this, &MainWindow::handlePythonError);
    connect(pythonProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &MainWindow::handlePythonFinished);
}

// обработка стандартного вывода от python скрипта
void MainWindow::handlePythonOutput()
{
    if (pythonProcess) {
        QByteArray outputData = pythonProcess->readAllStandardOutput();
        QString output = QString::fromUtf8(outputData);
        processPythonOutput(output);
    }
}

// обработка ошибок от python скрипта
void MainWindow::handlePythonError()
{
    if (pythonProcess) {
        QByteArray errorData = pythonProcess->readAllStandardError();
        QString error = QString::fromUtf8(errorData);

        if (!error.isEmpty()) {
            // сохраняем ошибку для отображения после завершения процесса
            lastPythonError = error;
        }
    }
}

// обработка завершения python процесса
void MainWindow::handlePythonFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    // возвращаем кнопкам активное состояние
    ui->btn_search->setEnabled(true);
    ui->btn_search->setText("Поиск");
    ui->btn_upload->setEnabled(true);
    statusBar()->clearMessage();

    // удаляем временный файл с текстом, если он существует
    if (!currentTempFile.isEmpty() && QFile::exists(currentTempFile)) {
        QFile::remove(currentTempFile);
        currentTempFile.clear();
    }

    // обрабатываем аварийное завершение
    if (exitStatus == QProcess::CrashExit) {
        QMessageBox::critical(this, "Аварийное завершение",
                              "Программа анализа неожиданно завершила работу.\n\n"
                              "Попробуйте перезапустить приложение.");
        refreshAllDisplay();
        return;
    }

    // обрабатываем коды возврата из python скрипта
    switch (exitCode) {
    case EXIT_OK:
        statusBar()->showMessage("Анализ успешно завершен", 3000);
        analyzedText = ui->textEdit->toPlainText();
        refreshAllDisplay();
        break;

    case EXIT_USAGE_ERROR:
        QMessageBox::warning(this, "Ошибка параметров",
                             "Неверные параметры скрипта.\n"
                             "Убедитесь, что передан правильный путь к файлу.",
                             QMessageBox::Ok);
        refreshAllDisplay();
        break;

    case EXIT_IO_ERROR:
        QMessageBox::warning(this, "Ошибка чтения файла",
                             "Не удалось прочитать файл с текстом.\n"
                             "Проверьте права доступа к файлу.",
                             QMessageBox::Ok);
        refreshAllDisplay();
        break;

    case EXIT_ANALYSIS_ERROR:
        QMessageBox::warning(this, "Ошибка анализа",
                             "Ошибка при синтаксическом анализе текста.\n"
                             "Проверьте, что текст содержит русские буквы.",
                             QMessageBox::Ok);
        refreshAllDisplay();
        break;

    case EXIT_FILTERED:
        if (!lastPythonError.isEmpty()) {
            QMessageBox::information(this, "Текст не распознан",
                                     "Текст не соответствует критериям:\n" + lastPythonError,
                                     QMessageBox::Ok);
        } else {
            QMessageBox::information(this, "Текст не распознан",
                                     "Текст не распознан как русскоязычный или содержит слишком много мусора.",
                                     QMessageBox::Ok);
        }
        refreshAllDisplay();
        break;

    default:
        QMessageBox::warning(this, "Неизвестная ошибка",
                             QString("Анализ завершился с кодом ошибки: %1\n"
                                     "Проверьте, что установлены все библиотеки:\n"
                                     "pip install natasha").arg(exitCode),
                             QMessageBox::Ok);
        refreshAllDisplay();
        break;
    }

    lastPythonError.clear();
}

// запуск python скрипта для анализа текста
bool MainWindow::callNatasha(const QString path){
    QString scriptPath = QCoreApplication::applicationDirPath() + SCRIPT_PATH;
    QFileInfo scriptFile(scriptPath);

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

    QStringList arguments;
    arguments << scriptPath;
    arguments << path;

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PYTHONIOENCODING", "utf-8");
    env.insert("LANG", "en_US.UTF-8");
    env.insert("ANALYZER_DEBUG", "0");
    pythonProcess->setProcessEnvironment(env);

    sentenceTexts.clear();
    wordsBySentence.clear();
    ui->treeWidget->clear();
    lastPythonError.clear();

    statusBar()->showMessage("Выполняется синтаксический анализ...");

    pythonProcess->start("python", arguments);

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

// запуск полного цикла анализа текста
void MainWindow::runAnalysis(const QString& text)
{
    if (!isAnyFilterSelected()) {
        QMessageBox::warning(this, "Нет активных фильтров",
                             "Не выбран ни один фильтр для отображения слов.\n\n"
                             "Пожалуйста, выберите хотя бы одну категорию.",
                             QMessageBox::Ok);
        return;
    }

    if (!hasAnyLetter(text)) {
        QMessageBox::warning(this, "Нет букв",
                             "Текст не содержит ни одной буквы.\n"
                             "Пожалуйста, введите текст с буквами для анализа.",
                             QMessageBox::Ok);
        return;
    }

    if (pythonProcess && pythonProcess->state() == QProcess::Running) {
        QMessageBox::warning(this, "Процесс занят",
                             "Анализ уже выполняется. Пожалуйста, подождите.",
                             QMessageBox::Ok);
        return;
    }

    ui->btn_search->setEnabled(false);
    ui->btn_search->setText("Поиск...");
    ui->btn_upload->setEnabled(false);
    ui->btn_download->setEnabled(false);

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

    if (!callNatasha(tempFilePath)) {
        ui->btn_search->setEnabled(true);
        ui->btn_search->setText("Поиск");
        ui->btn_upload->setEnabled(true);
        if (!tempFilePath.isEmpty() && QFile::exists(tempFilePath)) {
            QFile::remove(tempFilePath);
        }
        return;
    }

    currentTempFile = tempFilePath;
}

// парсинг вывода python скрипта
void MainWindow::processPythonOutput(const QString& output)
{
    QStringList lines = output.split("\n", Qt::SkipEmptyParts);

    for (int i = 0; i < lines.size(); ++i) {
        QString& line = lines[i];
        line = line.trimmed();
    }

    if (lines.isEmpty()) {
        return;
    }

    for (int i = 0; i < lines.size(); ++i) {
        const QString& line = lines[i];
        if (line.startsWith("FORMATTED_FILE:")) {
            continue;
        }

        QStringList parts = line.split("|");

        if (parts.isEmpty()) continue;

        if (parts[0] == "SENT" && parts.size() >= 3) {
            int sentNum = parts[1].toInt();
            QString sentenceText = parts[2];
            sentenceTexts[sentNum] = sentenceText;
        }
        else if (parts.size() >= 8) {
            WordInfo word;
            word.text = parts[0].trimmed();
            word.speech = parts[1].trimmed();
            word.sentence = parts[6].trimmed();
            word.sentenceNum = parts[4].toInt();
            word.posRussian = parts[7].trimmed();
            wordsBySentence[word.sentenceNum].append(word);
        }
    }
}

// создание временного файла с текстом для передачи в python
QString MainWindow::createTempFileWithText(const QString& text)
{
    QString tempDir = QCoreApplication::applicationDirPath() + TEMP_DIR;
    QDir dir;
    if (!dir.exists(tempDir)) {
        dir.mkpath(tempDir);
    }

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

    return QString();
}
