#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>
#include <QStatusBar>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDateTime>
#include <QTreeWidget>
#include <QHeaderView>
#include <QBrush>
#include <QFont>
#include <QMap>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pythonProcess(nullptr)
{
    ui->setupUi(this);

    setupPythonProcess();

    // Setup tree widget
    QStringList headers;
    headers << "Результаты синтаксического анализа";
    ui->treeWidget->setHeaderLabels(headers);
    ui->treeWidget->setAlternatingRowColors(true);
    ui->treeWidget->setAnimated(true);
    ui->treeWidget->setIndentation(20);
    ui->treeWidget->setSortingEnabled(false);
    ui->treeWidget->setExpandsOnDoubleClick(true);

    // Настройка чекбокса "Выбрать все"
    ui->c_all->setTristate(false);
    ui->c_all->setChecked(true);

    // Включаем все чекбоксы по умолчанию
    ui->c_pod->setChecked(true);
    ui->c_skaz->setChecked(true);
    ui->c_opred->setChecked(true);
    ui->c_dop->setChecked(true);
    ui->c_ob->setChecked(true);
    ui->c_none->setChecked(true);

    statusBar()->showMessage("Готов к работе", 3000);
}

MainWindow::~MainWindow()
{
    if (pythonProcess && pythonProcess->state() == QProcess::Running) {
        pythonProcess->terminate();
        pythonProcess->waitForFinished(3000);
    }
    delete ui;
}

void MainWindow::setupPythonProcess()
{
    pythonProcess = new QProcess(this);
    pythonProcess->setProcessChannelMode(QProcess::MergedChannels);

    connect(pythonProcess, &QProcess::readyReadStandardOutput,
            this, &MainWindow::handlePythonOutput);
    connect(pythonProcess, &QProcess::readyReadStandardError,
            this, &MainWindow::handlePythonError);
    connect(pythonProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &MainWindow::handlePythonFinished);
}

void MainWindow::on_c_all_stateChanged(int arg1)
{
    // Блокируем сигналы
    ui->c_pod->blockSignals(true);
    ui->c_skaz->blockSignals(true);
    ui->c_opred->blockSignals(true);
    ui->c_dop->blockSignals(true);
    ui->c_ob->blockSignals(true);
    ui->c_none->blockSignals(true);

    // Устанавливаем состояние всех чекбоксов
    if (arg1 == Qt::Checked) {
        ui->c_pod->setChecked(true);
        ui->c_skaz->setChecked(true);
        ui->c_opred->setChecked(true);
        ui->c_dop->setChecked(true);
        ui->c_ob->setChecked(true);
        ui->c_none->setChecked(true);
    } else {
        ui->c_pod->setChecked(false);
        ui->c_skaz->setChecked(false);
        ui->c_opred->setChecked(false);
        ui->c_dop->setChecked(false);
        ui->c_ob->setChecked(false);
        ui->c_none->setChecked(false);
    }

    // Разблокируем сигналы
    ui->c_pod->blockSignals(false);
    ui->c_skaz->blockSignals(false);
    ui->c_opred->blockSignals(false);
    ui->c_dop->blockSignals(false);
    ui->c_ob->blockSignals(false);
    ui->c_none->blockSignals(false);

    // Обновляем отображение, если есть данные
    if (!sentenceTexts.isEmpty()) {
        updateDisplay();
    }
}

void MainWindow::on_c_pod_stateChanged(int)
{
    updateAllCheckboxState();
    if (!sentenceTexts.isEmpty()) {
        updateDisplay();
    }
}

void MainWindow::on_c_skaz_stateChanged(int)
{
    updateAllCheckboxState();
    if (!sentenceTexts.isEmpty()) {
        updateDisplay();
    }
}

void MainWindow::on_c_opred_stateChanged(int)
{
    updateAllCheckboxState();
    if (!sentenceTexts.isEmpty()) {
        updateDisplay();
    }
}

void MainWindow::on_c_dop_stateChanged(int)
{
    updateAllCheckboxState();
    if (!sentenceTexts.isEmpty()) {
        updateDisplay();
    }
}

void MainWindow::on_c_ob_stateChanged(int)
{
    updateAllCheckboxState();
    if (!sentenceTexts.isEmpty()) {
        updateDisplay();
    }
}

void MainWindow::on_c_none_stateChanged(int)
{
    updateAllCheckboxState();
    if (!sentenceTexts.isEmpty()) {
        updateDisplay();
    }
}

void MainWindow::updateAllCheckboxState()
{
    // Блокируем сигнал для c_all
    ui->c_all->blockSignals(true);

    // Проверяем, все ли чекбоксы выбраны
    bool allChecked = ui->c_pod->isChecked() &&
                      ui->c_skaz->isChecked() &&
                      ui->c_opred->isChecked() &&
                      ui->c_dop->isChecked() &&
                      ui->c_ob->isChecked() &&
                      ui->c_none->isChecked();

    // Проверяем, все ли чекбоксы не выбраны
    bool allUnchecked = !ui->c_pod->isChecked() &&
                        !ui->c_skaz->isChecked() &&
                        !ui->c_opred->isChecked() &&
                        !ui->c_dop->isChecked() &&
                        !ui->c_ob->isChecked() &&
                        !ui->c_none->isChecked();

    // Устанавливаем состояние c_all
    if (allChecked) {
        ui->c_all->setCheckState(Qt::Checked);
    } else if (allUnchecked) {
        ui->c_all->setCheckState(Qt::Unchecked);
    } else {
        ui->c_all->setCheckState(Qt::PartiallyChecked);
    }

    // Разблокируем сигнал
    ui->c_all->blockSignals(false);
}

QString MainWindow::createTempFileWithText(const QString& text)
{
    QString tempDir = QCoreApplication::applicationDirPath() + "/temp";
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

bool MainWindow::loadFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл: " + filePath);
        return false;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    QString content = in.readAll();
    file.close();

    if (content.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Файл пуст.");
        return false;
    }

    // Показываем содержимое в поле ввода
    ui->lineEdit->setText(content);
    currentFileName = QFileInfo(filePath).fileName();

    statusBar()->showMessage("Загружен файл: " + currentFileName, 3000);

    return true;
}

void MainWindow::runAnalysis(const QString& text)
{
    // Проверяем, выбран ли хотя бы один чекбокс
    bool anyCheckboxSelected = ui->c_pod->isChecked() ||
                               ui->c_skaz->isChecked() ||
                               ui->c_opred->isChecked() ||
                               ui->c_dop->isChecked() ||
                               ui->c_ob->isChecked() ||
                               ui->c_none->isChecked();

    if (!anyCheckboxSelected) {
        QMessageBox::warning(this, "Нет выбора",
                             "Пожалуйста, выберите хотя бы один тип слов для отображения.",
                             QMessageBox::Ok);
        return;
    }

    // Проверяем, не запущен ли уже процесс
    if (pythonProcess && pythonProcess->state() == QProcess::Running) {
        QMessageBox::warning(this, "Процесс занят",
                             "Анализ уже выполняется. Пожалуйста, подождите.",
                             QMessageBox::Ok);
        return;
    }

    // Блокируем кнопки
    ui->btn_search->setEnabled(false);
    ui->btn_search->setText("Поиск...");
    ui->btn_upload->setEnabled(false);

    // Создаем временный файл
    QString tempFilePath = createTempFileWithText(text);
    qDebug() << "Создан временный файл:" << tempFilePath;

    if (tempFilePath.isEmpty()) {
        QMessageBox::critical(this, "Ошибка файла",
                              "Не удалось создать временный файл для анализа.",
                              QMessageBox::Ok);
        ui->btn_search->setEnabled(true);
        ui->btn_search->setText("Поиск");
        ui->btn_upload->setEnabled(true);
        return;
    }

    // Путь к Python скрипту
    QString scriptPath = QCoreApplication::applicationDirPath() + "/analyzer.py";
    QFileInfo scriptFile(scriptPath);

    qDebug() << "Путь к скрипту:" << scriptPath;
    qDebug() << "Скрипт существует:" << scriptFile.exists();

    if (!scriptFile.exists()) {
        QMessageBox::critical(this, "Скрипт не найден",
                              "Python скрипт не найден по пути:\n" + scriptPath +
                                  "\n\nУбедитесь, что файл analyzer.py находится в папке с программой.",
                              QMessageBox::Ok);
        ui->btn_search->setEnabled(true);
        ui->btn_search->setText("Поиск");
        ui->btn_upload->setEnabled(true);
        QFile::remove(tempFilePath);
        return;
    }

    // Подготавливаем аргументы
    QStringList arguments;
    arguments << scriptPath;
    arguments << tempFilePath;

    qDebug() << "Аргументы:" << arguments;

    // Устанавливаем окружение для UTF-8
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PYTHONIOENCODING", "utf-8");
    env.insert("LANG", "en_US.UTF-8");
    pythonProcess->setProcessEnvironment(env);

    // Очищаем старые данные
    sentenceTexts.clear();
    wordsBySentence.clear();
    ui->treeWidget->clear();

    // Показываем сообщение в статусбаре
    statusBar()->showMessage("Выполняется синтаксический анализ...");

    // Сохраняем путь к временному файлу для последующего удаления
    currentTempFile = tempFilePath;

    // Запускаем Python скрипт
    qDebug() << "Запускаем Python скрипт...";

#ifdef Q_OS_WIN
    pythonProcess->start("python", arguments);
#else
    pythonProcess->start("python3", arguments);
#endif

    // Проверяем, запустился ли процесс
    if (!pythonProcess->waitForStarted(3000)) {
        qDebug() << "Ошибка запуска процесса:" << pythonProcess->errorString();

        QMessageBox::critical(this, "Ошибка запуска",
                              "Не удалось запустить Python скрипт.\n"
                              "Убедитесь, что Python установлен и доступен в командной строке.\n\n"
                              "Ошибка: " + pythonProcess->errorString(),
                              QMessageBox::Ok);

        ui->btn_search->setEnabled(true);
        ui->btn_search->setText("Поиск");
        ui->btn_upload->setEnabled(true);
        statusBar()->clearMessage();
        QFile::remove(tempFilePath);
    } else {
        qDebug() << "Python процесс успешно запущен";
    }
}

void MainWindow::on_btn_search_clicked()
{
    qDebug() << "Кнопка поиска нажата";

    // Получаем текст для поиска
    QString searchText = ui->lineEdit->text();

    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "Пустой поиск",
                             "Пожалуйста, введите текст для анализа.",
                             QMessageBox::Ok);
        return;
    }

    runAnalysis(searchText);
}

void MainWindow::on_btn_upload_clicked()
{
    qDebug() << "Кнопка загрузки нажата";

    // Открываем диалог выбора файла
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Выберите текстовый файл",
                                                    QDir::homePath(),
                                                    "Текстовые файлы (*.txt);;Все файлы (*.*)");

    if (filePath.isEmpty()) {
        return; // Пользователь отменил выбор
    }

    qDebug() << "Выбран файл:" << filePath;

    // Загружаем файл
    if (loadFile(filePath)) {
        // Спрашиваем, хочет ли пользователь выполнить анализ сразу
        QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                                  "Запустить анализ?",
                                                                  "Файл загружен. Запустить синтаксический анализ сейчас?",
                                                                  QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            runAnalysis(ui->lineEdit->text());
        }
    }
}

void MainWindow::handlePythonOutput()
{
    if (pythonProcess) {
        QByteArray outputData = pythonProcess->readAllStandardOutput();
        QString output = QString::fromUtf8(outputData);
        qDebug() << "Получен вывод от Python:";
        qDebug() << output;
        processPythonOutput(output);
    }
}

void MainWindow::handlePythonError()
{
    if (pythonProcess) {
        QByteArray errorData = pythonProcess->readAllStandardError();
        QString error = QString::fromUtf8(errorData);
        qDebug() << "Ошибка от Python:" << error;

        if (!error.isEmpty() && !error.contains("chcp")) {
            QMessageBox::warning(this, "Ошибка Python скрипта",
                                 "Ошибка от Python скрипта:\n" + error,
                                 QMessageBox::Ok);
        }
    }
}

void MainWindow::handlePythonFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Python процесс завершен. Код выхода:" << exitCode << "Статус:" << exitStatus;

    // Разблокируем кнопки
    ui->btn_search->setEnabled(true);
    ui->btn_search->setText("Поиск");
    ui->btn_upload->setEnabled(true);
    statusBar()->clearMessage();

    // Удаляем временный файл
    if (!currentTempFile.isEmpty() && QFile::exists(currentTempFile)) {
        QFile::remove(currentTempFile);
        qDebug() << "Временный файл удален:" << currentTempFile;
        currentTempFile.clear();
    }

    if (exitStatus == QProcess::CrashExit) {
        QMessageBox::critical(this, "Процесс аварийно завершен",
                              "Python скрипт аварийно завершился.",
                              QMessageBox::Ok);
    } else if (exitCode != 0) {
        QMessageBox::warning(this, "Ошибка скрипта",
                             QString("Python скрипт завершился с кодом ошибки: %1\n\n"
                                     "Проверьте, что установлены все необходимые библиотеки:\n"
                                     "pip install natasha").arg(exitCode),
                             QMessageBox::Ok);
    } else {
        statusBar()->showMessage("Синтаксический анализ успешно завершен!", 3000);
        // Обновляем отображение с учетом фильтров
        if (!sentenceTexts.isEmpty()) {
            updateDisplay();
        } else {
            QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
            item->setText(0, "Анализ завершен, но данные не получены");
        }
    }
}

QString MainWindow::getRelationDescription(const QString& relation)
{
    static QMap<QString, QString> desc;
    if (desc.isEmpty()) {
        desc["nsubj"] = "Подлежащее";
        desc["root"] = "Сказуемое";
        desc["advcl"] = "Обстоятельство";
        desc["amod"] = "Определение";
        desc["det"] = "Определение";
        desc["obj"] = "Дополнение";
        desc["iobj"] = "Дополнение";
        desc["obl"] = "Дополнение";
        desc["advmod"] = "Обстоятельство";
        desc["parataxis"] = "Обстоятельство";
        desc["punct"] = "Пунктуация";
        desc["nmod"] = "Определение";
        desc["conj"] = "Союз";
        desc["cc"] = "Союз";
        desc["case"] = "Предлог";
        desc["mark"] = "Союз";
        desc["nummod"] = "Числительное";
        desc["aux"] = "Вспомогательный";
        desc["cop"] = "Связка";
    }
    return desc.value(relation, "Другое");
}

QString MainWindow::getRelationType(const QString& relation)
{
    // Определяем тип отношения для фильтрации
    if (relation == "nsubj") return "pod";
    if (relation == "root" || relation == "advcl") return "skaz";
    if (relation == "amod" || relation == "det" || relation == "nmod") return "opred";
    if (relation == "obj" || relation == "iobj" || relation == "obl") return "dop";
    if (relation == "advmod" || relation == "parataxis") return "ob";
    return "none";
}

bool MainWindow::shouldShowWord(const WordInfo& word)
{
    QString type = getRelationType(word.relation);

    if (type == "pod" && ui->c_pod->isChecked()) return true;
    if (type == "skaz" && ui->c_skaz->isChecked()) return true;
    if (type == "opred" && ui->c_opred->isChecked()) return true;
    if (type == "dop" && ui->c_dop->isChecked()) return true;
    if (type == "ob" && ui->c_ob->isChecked()) return true;
    if (type == "none" && ui->c_none->isChecked()) return true;

    return false;
}

void MainWindow::updateDisplay()
{
    ui->treeWidget->clear();

    if (sentenceTexts.isEmpty() || wordsBySentence.isEmpty()) {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, "Нет данных для отображения");
        return;
    }

    QFont font("Arial", 10);

    // Для каждого предложения
    QList<int> sentNumbers = sentenceTexts.keys();
    std::sort(sentNumbers.begin(), sentNumbers.end());

    for (int sentNum : sentNumbers) {
        QString sentenceText = sentenceTexts[sentNum];

        QTreeWidgetItem* sentItem = new QTreeWidgetItem(ui->treeWidget);
        sentItem->setText(0, QString("Предложение %1: %2").arg(sentNum).arg(sentenceText));
        sentItem->setFont(0, font);
        sentItem->setForeground(0, QBrush(Qt::darkBlue));
        sentItem->setExpanded(false);

        // Добавляем слова, которые проходят фильтр
        if (wordsBySentence.contains(sentNum)) {
            const QList<WordInfo>& words = wordsBySentence[sentNum];
            bool hasWords = false;

            for (const WordInfo& word : words) {
                if (shouldShowWord(word)) {
                    hasWords = true;

                    // Получаем описание части речи
                    QString posDescription;
                    if (word.pos == "NOUN") posDescription = "сущ.";
                    else if (word.pos == "VERB") posDescription = "гл.";
                    else if (word.pos == "ADJ") posDescription = "прил.";
                    else if (word.pos == "ADV") posDescription = "нар.";
                    else if (word.pos == "PRON") posDescription = "мест.";
                    else if (word.pos == "DET") posDescription = "опр.";
                    else if (word.pos == "NUM") posDescription = "числ.";
                    else if (word.pos == "ADP") posDescription = "предл.";
                    else if (word.pos == "CCONJ") posDescription = "союз";
                    else if (word.pos == "SCONJ") posDescription = "союз";
                    else if (word.pos == "PART") posDescription = "част.";
                    else posDescription = word.pos.toLower();

                    // Формируем отображаемый текст
                    QString displayText = QString("%1 — %2 (%3)")
                                              .arg(word.text)
                                              .arg(posDescription)
                                              .arg(getRelationDescription(word.relation));

                    QTreeWidgetItem* wordItem = new QTreeWidgetItem(sentItem);
                    wordItem->setText(0, displayText);
                    wordItem->setFont(0, font);

                    // Добавляем всплывающую подсказку
                    QString tooltip = QString("Слово: %1\nЧасть речи: %2\nОтношение: %3\nГлавное слово: %4")
                                          .arg(word.text)
                                          .arg(word.pos)
                                          .arg(getRelationDescription(word.relation))
                                          .arg(word.headWord);
                    wordItem->setToolTip(0, tooltip);
                }
            }

            // Если нет слов после фильтрации, показываем сообщение
            if (!hasWords) {
                QTreeWidgetItem* noWordsItem = new QTreeWidgetItem(sentItem);
                noWordsItem->setText(0, "Нет слов, соответствующих выбранным фильтрам");
                noWordsItem->setFont(0, font);
                noWordsItem->setForeground(0, QBrush(Qt::gray));
            }
        }
    }

    ui->treeWidget->expandAll();
    ui->treeWidget->collapseAll();
    ui->treeWidget->resizeColumnToContents(0);
}

void MainWindow::processPythonOutput(const QString& output)
{
    qDebug() << "========== НАЧАЛО ОБРАБОТКИ ДАННЫХ ==========";

    // Разделяем строки и очищаем от символов \r
    QStringList lines = output.split("\n", Qt::SkipEmptyParts);

    for (QString& line : lines) {
        line = line.trimmed();
    }

    qDebug() << "Количество строк:" << lines.size();

    if (lines.isEmpty()) {
        qDebug() << "Нет строк для обработки";
        return;
    }

    QString formattedFilePath;

    // Обрабатываем все строки
    for (const QString& line : lines) {
        qDebug() << "Обрабатываем строку:" << line;

        if (line.startsWith("FORMATTED_FILE:")) {
            formattedFilePath = line.mid(15);
            qDebug() << "Найден форматированный файл:" << formattedFilePath;
            continue;
        }

        QStringList parts = line.split("|");

        if (parts.isEmpty()) continue;

        if (parts[0] == "SENT" && parts.size() >= 3) {
            // Это предложение
            int sentNum = parts[1].toInt();
            QString sentenceText = parts[2];
            sentenceTexts[sentNum] = sentenceText;
            qDebug() << "Сохранено предложение" << sentNum << ":" << sentenceText;
        }
        else if (parts.size() >= 5) {
            // Это слово
            WordInfo word;
            word.text = parts[0];
            word.pos = parts[1];
            word.relation = parts[2];
            word.headWord = parts[3];
            word.sentenceNum = parts[4].toInt();

            wordsBySentence[word.sentenceNum].append(word);
            qDebug() << "Сохранено слово:" << word.text << "для предложения" << word.sentenceNum;
        }
    }

    qDebug() << "Всего предложений:" << sentenceTexts.size();
    qDebug() << "Всего слов по предложениям:" << wordsBySentence.size();
    qDebug() << "========== КОНЕЦ ОБРАБОТКИ ДАННЫХ ==========";
}

void MainWindow::on_btn_test_clicked()
{
    setupTestData();
}

void MainWindow::setupTestData()
{
    qDebug() << "Создаем тестовые данные...";

    // Очищаем старые данные
    sentenceTexts.clear();
    wordsBySentence.clear();

    // Предложение 1
    sentenceTexts[1] = "Мама мыла раму";

    WordInfo w1;
    w1.text = "Мама"; w1.pos = "NOUN"; w1.relation = "nsubj"; w1.headWord = "мыла"; w1.sentenceNum = 1;
    wordsBySentence[1].append(w1);

    WordInfo w2;
    w2.text = "мыла"; w2.pos = "VERB"; w2.relation = "root"; w2.headWord = "_"; w2.sentenceNum = 1;
    wordsBySentence[1].append(w2);

    WordInfo w3;
    w3.text = "раму"; w3.pos = "NOUN"; w3.relation = "obj"; w3.headWord = "мыла"; w3.sentenceNum = 1;
    wordsBySentence[1].append(w3);

    // Предложение 2
    sentenceTexts[2] = "Папа читает интересную книгу";

    WordInfo w4;
    w4.text = "Папа"; w4.pos = "NOUN"; w4.relation = "nsubj"; w4.headWord = "читает"; w4.sentenceNum = 2;
    wordsBySentence[2].append(w4);

    WordInfo w5;
    w5.text = "читает"; w5.pos = "VERB"; w5.relation = "root"; w5.headWord = "_"; w5.sentenceNum = 2;
    wordsBySentence[2].append(w5);

    WordInfo w6;
    w6.text = "интересную"; w6.pos = "ADJ"; w6.relation = "amod"; w6.headWord = "книгу"; w6.sentenceNum = 2;
    wordsBySentence[2].append(w6);

    WordInfo w7;
    w7.text = "книгу"; w7.pos = "NOUN"; w7.relation = "obj"; w7.headWord = "читает"; w7.sentenceNum = 2;
    wordsBySentence[2].append(w7);

    // Предложение 3
    sentenceTexts[3] = "Быстро бежит рыжая лиса по лесу";

    WordInfo w8;
    w8.text = "Быстро"; w8.pos = "ADV"; w8.relation = "advmod"; w8.headWord = "бежит"; w8.sentenceNum = 3;
    wordsBySentence[3].append(w8);

    WordInfo w9;
    w9.text = "бежит"; w9.pos = "VERB"; w9.relation = "root"; w9.headWord = "_"; w9.sentenceNum = 3;
    wordsBySentence[3].append(w9);

    WordInfo w10;
    w10.text = "рыжая"; w10.pos = "ADJ"; w10.relation = "amod"; w10.headWord = "лиса"; w10.sentenceNum = 3;
    wordsBySentence[3].append(w10);

    WordInfo w11;
    w11.text = "лиса"; w11.pos = "NOUN"; w11.relation = "nsubj"; w11.headWord = "бежит"; w11.sentenceNum = 3;
    wordsBySentence[3].append(w11);

    WordInfo w12;
    w12.text = "по"; w12.pos = "ADP"; w12.relation = "case"; w12.headWord = "лесу"; w12.sentenceNum = 3;
    wordsBySentence[3].append(w12);

    WordInfo w13;
    w13.text = "лесу"; w13.pos = "NOUN"; w13.relation = "obl"; w13.headWord = "бежит"; w13.sentenceNum = 3;
    wordsBySentence[3].append(w13);

    // Обновляем отображение
    updateDisplay();

    qDebug() << "Тестовые данные созданы";
}

void MainWindow::on_btn_download_clicked()
{
    QMessageBox::information(this, "Скачивание", "Кнопка скачивания нажата");
}
