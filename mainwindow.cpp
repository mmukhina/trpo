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
#include <QProgressDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDesktopServices>
#include <QUrl>
#include <QTextEdit>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QScrollArea>
#include <QGridLayout>

void MainWindow::generateHtmlReport()
{
    if (sentenceTexts.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Нет данных для сохранения.");
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить отчет",
                                                    QDir::currentPath() + "/syntax_report.html", "HTML файлы (*.html)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    out << "<title>Синтаксический разбор текста</title>";
    out << "<style>"
        << "body { font-family: 'Segoe UI', Arial, sans-serif; padding: 30px; background: #f0f2f5; color: #333; }"
        << ".container { max-width: 1200px; margin: auto; background: white; padding: 40px; border-radius: 12px; box-shadow: 0 4px 15px rgba(0,0,0,0.1); }"
        << "h1 { text-align: center; color: #2c3e50; margin-bottom: 30px; }"

        << ".tabs { overflow: hidden; border-bottom: 1px solid #dee2e6; margin-bottom: 20px; }"
        << ".tab-button { background-color: inherit; float: left; border: none; outline: none; cursor: pointer; padding: 14px 16px; transition: 0.3s; font-size: 17px; border-radius: 8px 8px 0 0; }"
        << ".tab-button:hover { background-color: #e9ecef; }"
        << ".tab-button.active { background-color: #3498db; color: white; }"
        << ".tab-content { display: none; padding: 20px 0; animation: fadeEffect 0.5s; }"
        << "@keyframes fadeEffect { from {opacity: 0;} to {opacity: 1;} }"

        << ".legend { background: #f8f9fa; border: 1px solid #dee2e6; padding: 20px; border-radius: 8px; margin-bottom: 40px; }"
        << ".legend h3 { margin-top: 0; font-size: 18px; color: #495057; }"
        << ".legend-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; }"
        << ".legend-item { display: flex; align-items: center; gap: 10px; font-size: 14px; }"

        << ".stats-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 20px; margin-bottom: 30px; }"
        << ".stat-card { background: #3498db; color: white; padding: 20px; border-radius: 12px; text-align: center; }"
        << ".stat-card h3 { margin: 0 0 10px 0; font-size: 16px; }"
        << ".stat-number { font-size: 36px; font-weight: bold; margin: 10px 0; }"
        << ".stat-list { background: #f8f9fa; padding: 15px; border-radius: 8px; margin-bottom: 20px; border: 1px solid #dee2e6; }"
        << ".stat-list h4 { margin: 0 0 10px 0; color: #2c3e50; }"
        << ".stat-list ul { margin: 0; padding-left: 20px; }"
        << ".stat-list li { margin: 5px 0; }"

        << ".sentence-block { margin-bottom: 40px; line-height: 3; }"
        << ".word-container { display: inline-flex; flex-direction: column; align-items: center; margin: 0 5px; vertical-align: bottom; }"
        << ".pos-label { font-size: 11px; color: #2c3e50; text-transform: none; line-height: 1.2; margin-bottom: 5px; font-weight: 600; background: #e8f4fd; padding: 2px 8px; border-radius: 4px; }"
        << ".word-text { font-size: 18px; white-space: nowrap; padding: 0 2px; }"

        << ".pod { border-bottom: 2px solid black; } "
        << ".skaz { border-bottom: 4px double black; } "
        << ".opred { text-decoration: underline wavy #3498db; } "
        << ".dop { border-bottom: 2px dashed #3498db; } "
        << ".ob { border-bottom: 2px dotted #3498db; } "
        << ".none { border-bottom: none; }"
        << "</style>"
        << "<script>"
        << "function openTab(evt, tabName) {"
        << "  var i, tabcontent, tabbuttons;"
        << "  tabcontent = document.getElementsByClassName('tab-content');"
        << "  for (i = 0; i < tabcontent.length; i++) {"
        << "    tabcontent[i].style.display = 'none';"
        << "  }"
        << "  tabbuttons = document.getElementsByClassName('tab-button');"
        << "  for (i = 0; i < tabbuttons.length; i++) {"
        << "    tabbuttons[i].className = tabbuttons[i].className.replace(' active', '');"
        << "  }"
        << "  document.getElementById(tabName).style.display = 'block';"
        << "  evt.currentTarget.className += ' active';"
        << "}"
        << "</script>"
        << "</head><body><div class='container'>";

    out << "<h1>Синтаксический разбор текста</h1>";

    out << "<div class='tabs'>"
        << "<button class='tab-button active' onclick=\"openTab(event, 'Analysis')\">Анализ предложений</button>"
        << "<button class='tab-button' onclick=\"openTab(event, 'Statistics')\">Статистика</button>"
        << "</div>";

    out << "<div id='Analysis' class='tab-content' style='display:block'>";

    out << "<div class='legend'>"
        << "<h3>Условные обозначения:</h3>"
        << "<div class='legend-grid'>"
        << "<div class='legend-item'><span class='word-text pod'>Слово</span> — Подлежащее</div>"
        << "<div class='legend-item'><span class='word-text skaz'>Слово</span> — Сказуемое</div>"
        << "<div class='legend-item'><span class='word-text opred'>Слово</span> — Определение</div>"
        << "<div class='legend-item'><span class='word-text dop'>Слово</span> — Дополнение</div>"
        << "<div class='legend-item'><span class='word-text ob'>Слово</span> — Обстоятельство</div>"
        << "</div></div>";

    QList<int> sentNumbers = sentenceTexts.keys();
    std::sort(sentNumbers.begin(), sentNumbers.end());
    for (int sentNum : sentNumbers) {
        out << "<div class='sentence-block'>";
        out << "<div style='color: #999; font-size: 12px; margin-bottom: 10px;'>Предложение " << sentNum << "</div>";
        for (const WordInfo& word : wordsBySentence[sentNum]) {
            QString cssClass;
            if (word.sentence == "Подлежащее") cssClass = "pod";
            else if (word.sentence == "Сказуемое") cssClass = "skaz";
            else if (word.sentence == "Определение") cssClass = "opred";
            else if (word.sentence == "Дополнение") cssClass = "dop";
            else if (word.sentence == "Обстоятельство") cssClass = "ob";
            else cssClass = "none";

            QString displayPos = word.posRussian;
            if (displayPos.isEmpty() || word.speech == "PUNCT") {
                displayPos = "";
            }

            QString escapedText = word.text;
            escapedText.replace("&", "&amp;");
            escapedText.replace("<", "&lt;");
            escapedText.replace(">", "&gt;");

            out << "<div class='word-container'>"
                << "<span class='pos-label'>" << displayPos << "</span>"
                << "<span class='word-text " << cssClass << "'>" << escapedText << "</span>"
                << "</div>";
        }
        out << "</div><hr style='border: 0; border-top: 1px solid #eee;'>";
    }

    out << "</div>";

    out << "<div id='Statistics' class='tab-content'>";

    out << "<div class='stats-grid'>"
        << "<div class='stat-card'><h3>Подлежащее</h3><div class='stat-number'>" << stats.podlezhaschee << "</div></div>"
        << "<div class='stat-card'><h3>Сказуемое</h3><div class='stat-number'>" << stats.skazuemoe << "</div></div>"
        << "<div class='stat-card'><h3>Определение</h3><div class='stat-number'>" << stats.opredelenie << "</div></div>"
        << "<div class='stat-card'><h3>Дополнение</h3><div class='stat-number'>" << stats.dopolnenie << "</div></div>"
        << "<div class='stat-card'><h3>Обстоятельство</h3><div class='stat-number'>" << stats.obstoyatelstvo << "</div></div>"
        << "<div class='stat-card'><h3>Другое</h3><div class='stat-number'>" << stats.drugoe << "</div></div>"
        << "</div>";

    out << "<h3>Детальная статистика по предложениям</h3>";

    if (!stats.podlezhascheeSentences.isEmpty()) {
        out << "<div class='stat-list'><h4>Подлежащие:</h4><ul>";
        for (auto it = stats.podlezhascheeSentences.begin(); it != stats.podlezhascheeSentences.end(); ++it) {
            out << "<li>Предложение " << it.key() << ": " << it.value().join(", ") << "</li>";
        }
        out << "</ul></div>";
    }

    if (!stats.skazuemoeSentences.isEmpty()) {
        out << "<div class='stat-list'><h4>Сказуемые:</h4><ul>";
        for (auto it = stats.skazuemoeSentences.begin(); it != stats.skazuemoeSentences.end(); ++it) {
            out << "<li>Предложение " << it.key() << ": " << it.value().join(", ") << "</li>";
        }
        out << "</ul></div>";
    }

    if (!stats.opredelenieSentences.isEmpty()) {
        out << "<div class='stat-list'><h4>Определения:</h4><ul>";
        for (auto it = stats.opredelenieSentences.begin(); it != stats.opredelenieSentences.end(); ++it) {
            out << "<li>Предложение " << it.key() << ": " << it.value().join(", ") << "</li>";
        }
        out << "</ul></div>";
    }

    if (!stats.dopolnenieSentences.isEmpty()) {
        out << "<div class='stat-list'><h4>Дополнения:</h4><ul>";
        for (auto it = stats.dopolnenieSentences.begin(); it != stats.dopolnenieSentences.end(); ++it) {
            out << "<li>Предложение " << it.key() << ": " << it.value().join(", ") << "</li>";
        }
        out << "</ul></div>";
    }

    if (!stats.obstoyatelstvoSentences.isEmpty()) {
        out << "<div class='stat-list'><h4>Обстоятельства:</h4><ul>";
        for (auto it = stats.obstoyatelstvoSentences.begin(); it != stats.obstoyatelstvoSentences.end(); ++it) {
            out << "<li>Предложение " << it.key() << ": " << it.value().join(", ") << "</li>";
        }
        out << "</ul></div>";
    }

    if (!stats.drugoeSentences.isEmpty()) {
        out << "<div class='stat-list'><h4>Другое:</h4><ul>";
        for (auto it = stats.drugoeSentences.begin(); it != stats.drugoeSentences.end(); ++it) {
            out << "<li>Предложение " << it.key() << ": " << it.value().join(", ") << "</li>";
        }
        out << "</ul></div>";
    }

    out << "</div>";
    out << "</div></body></html>";
    file.close();
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pythonProcess(nullptr)
{
    ui->setupUi(this);
    setupPythonProcess();

    ui->btn_download->setEnabled(false);
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
    ui->c_pod->blockSignals(true);
    ui->c_skaz->blockSignals(true);
    ui->c_opred->blockSignals(true);
    ui->c_dop->blockSignals(true);
    ui->c_ob->blockSignals(true);
    ui->c_none->blockSignals(true);

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

    ui->c_pod->blockSignals(false);
    ui->c_skaz->blockSignals(false);
    ui->c_opred->blockSignals(false);
    ui->c_dop->blockSignals(false);
    ui->c_ob->blockSignals(false);
    ui->c_none->blockSignals(false);

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
    ui->c_all->blockSignals(true);

    bool allChecked = ui->c_pod->isChecked() &&
                      ui->c_skaz->isChecked() &&
                      ui->c_opred->isChecked() &&
                      ui->c_dop->isChecked() &&
                      ui->c_ob->isChecked() &&
                      ui->c_none->isChecked();

    bool allUnchecked = !ui->c_pod->isChecked() &&
                        !ui->c_skaz->isChecked() &&
                        !ui->c_opred->isChecked() &&
                        !ui->c_dop->isChecked() &&
                        !ui->c_ob->isChecked() &&
                        !ui->c_none->isChecked();

    if (allChecked) {
        ui->c_all->setCheckState(Qt::Checked);
    } else if (allUnchecked) {
        ui->c_all->setCheckState(Qt::Unchecked);
    } else {
        ui->c_all->setCheckState(Qt::PartiallyChecked);
    }

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
        QMessageBox::warning(this, "Предупреждение", "Файл пуст. Пожалуйста, выберите файл с текстом.");
        return false;
    }

    if (hasOnlyNumbers(content)) {
        QMessageBox::warning(this, "Предупреждение",
                             "Файл содержит только цифры и символы.\n"
                             "Пожалуйста, выберите файл с текстом на русском языке.",
                             QMessageBox::Ok);
        return false;
    }

    ui->textEdit->setPlainText(content);
    currentFileName = QFileInfo(filePath).fileName();
    statusBar()->showMessage("Загружен файл: " + currentFileName, 3000);

    return true;
}

bool MainWindow::callNatasha(const QString path){
    QString scriptPath = QCoreApplication::applicationDirPath() + "/analyzer.py";
    QFileInfo scriptFile(scriptPath);
    if (!scriptFile.exists()) {
        QMessageBox::critical(this, "Скрипт не найден",
                              "Python скрипт не найден по пути:\n" + scriptPath +
                                  "\nУбедитесь, что файл analyzer.py находится в папке с программой.",
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
    pythonProcess->setProcessEnvironment(env);

    sentenceTexts.clear();
    wordsBySentence.clear();
    ui->treeWidget->clear();

    statusBar()->showMessage("Выполняется синтаксический анализ...");

    pythonProcess->start("python", arguments);

    if (!pythonProcess->waitForStarted(3000)) {
        QMessageBox::critical(this, "Ошибка запуска",
                              "Не удалось запустить Python скрипт.\n"
                              "Убедитесь, что Python установлен и доступен в командной строке.\n"
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

bool MainWindow::hasAnyLetter(const QString& text)
{
    for (QChar ch : text) {
        if (ch.isLetter()) {
            return true;
        }
    }
    return false;
}

bool MainWindow::hasOnlyNumbers(const QString& text)
{
    for (QChar ch : text) {
        if (ch.isLetter() || (ch.unicode() >= 0x0400 && ch.unicode() <= 0x04FF)) {
            return false;
        }
        if (ch.isPunct() || ch.isSpace()) {
            continue;
        }
        if (!ch.isDigit()) {
            return false;
        }
    }
    return true;
}

void MainWindow::runAnalysis(const QString& text)
{
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

    ui->btn_download->setEnabled(false);
    ui->btn_search->setEnabled(false);
    ui->btn_search->setText("Поиск...");
    ui->btn_upload->setEnabled(false);

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

void MainWindow::on_btn_search_clicked()
{
    QString searchText = ui->textEdit->toPlainText();

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
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Выберите текстовый файл",
                                                    QDir::homePath(),
                                                    "Текстовые файлы (*.txt)");
    if (filePath.isEmpty()) {
        return;
    }

    if (loadFile(filePath)) {
        QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                                  "Запустить анализ?",
                                                                  "Файл загружен. Запустить синтаксический анализ сейчас?",
                                                                  QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            runAnalysis(ui->textEdit->toPlainText());
        }
    }
}

void MainWindow::handlePythonOutput()
{
    if (pythonProcess) {
        QByteArray outputData = pythonProcess->readAllStandardOutput();
        QString output = QString::fromUtf8(outputData);
        processPythonOutput(output);
    }
}

void MainWindow::handlePythonError()
{
    if (pythonProcess) {
        QByteArray errorData = pythonProcess->readAllStandardError();
        QString error = QString::fromUtf8(errorData);
        if (!error.isEmpty() && !error.contains("chcp")) {
            QMessageBox::warning(this, "Ошибка Python скрипта",
                                 "Ошибка от Python скрипта:\n" + error,
                                 QMessageBox::Ok);
        }
    }
}

void MainWindow::calculateStatistics()
{
    stats = Statistics();

    for (auto it = wordsBySentence.begin(); it != wordsBySentence.end(); ++it) {
        int sentenceNum = it.key();
        const QList<WordInfo>& words = it.value();

        for (const WordInfo& word : words) {
            if (word.sentence == "Подлежащее") {
                stats.podlezhaschee++;
                stats.podlezhascheeSentences[sentenceNum].append(word.text);
            }
            else if (word.sentence == "Сказуемое") {
                stats.skazuemoe++;
                stats.skazuemoeSentences[sentenceNum].append(word.text);
            }
            else if (word.sentence == "Определение") {
                stats.opredelenie++;
                stats.opredelenieSentences[sentenceNum].append(word.text);
            }
            else if (word.sentence == "Дополнение") {
                stats.dopolnenie++;
                stats.dopolnenieSentences[sentenceNum].append(word.text);
            }
            else if (word.sentence == "Обстоятельство") {
                stats.obstoyatelstvo++;
                stats.obstoyatelstvoSentences[sentenceNum].append(word.text);
            }
            else {
                stats.drugoe++;
                stats.drugoeSentences[sentenceNum].append(word.text);
            }
        }
    }
}

void MainWindow::updateStatisticsDisplay()
{
    // Update count labels
    ui->label_pod_count->setText(QString::number(stats.podlezhaschee));
    ui->label_skaz_count->setText(QString::number(stats.skazuemoe));
    ui->label_opred_count->setText(QString::number(stats.opredelenie));
    ui->label_dop_count->setText(QString::number(stats.dopolnenie));
    ui->label_ob_count->setText(QString::number(stats.obstoyatelstvo));
    ui->label_drugoe_count->setText(QString::number(stats.drugoe));

    // Clear all text browsers
    ui->text_pod->clear();
    ui->text_skaz->clear();
    ui->text_opred->clear();
    ui->text_dop->clear();
    ui->text_ob->clear();
    ui->text_drugoe->clear();

    // Fill Подлежащее
    QString podText;
    for (auto it = stats.podlezhascheeSentences.begin(); it != stats.podlezhascheeSentences.end(); ++it) {
        podText += QString("Предложение %1: %2\n").arg(it.key()).arg(it.value().join(", "));
    }
    if (podText.isEmpty()) podText = "Нет данных";
    ui->text_pod->setPlainText(podText);

    // Fill Сказуемое
    QString skazText;
    for (auto it = stats.skazuemoeSentences.begin(); it != stats.skazuemoeSentences.end(); ++it) {
        skazText += QString("Предложение %1: %2\n").arg(it.key()).arg(it.value().join(", "));
    }
    if (skazText.isEmpty()) skazText = "Нет данных";
    ui->text_skaz->setPlainText(skazText);

    // Fill Определение
    QString opredText;
    for (auto it = stats.opredelenieSentences.begin(); it != stats.opredelenieSentences.end(); ++it) {
        opredText += QString("Предложение %1: %2\n").arg(it.key()).arg(it.value().join(", "));
    }
    if (opredText.isEmpty()) opredText = "Нет данных";
    ui->text_opred->setPlainText(opredText);

    // Fill Дополнение
    QString dopText;
    for (auto it = stats.dopolnenieSentences.begin(); it != stats.dopolnenieSentences.end(); ++it) {
        dopText += QString("Предложение %1: %2\n").arg(it.key()).arg(it.value().join(", "));
    }
    if (dopText.isEmpty()) dopText = "Нет данных";
    ui->text_dop->setPlainText(dopText);

    // Fill Обстоятельство
    QString obText;
    for (auto it = stats.obstoyatelstvoSentences.begin(); it != stats.obstoyatelstvoSentences.end(); ++it) {
        obText += QString("Предложение %1: %2\n").arg(it.key()).arg(it.value().join(", "));
    }
    if (obText.isEmpty()) obText = "Нет данных";
    ui->text_ob->setPlainText(obText);

    // Fill Другое
    QString drugoeText;
    for (auto it = stats.drugoeSentences.begin(); it != stats.drugoeSentences.end(); ++it) {
        drugoeText += QString("Предложение %1: %2\n").arg(it.key()).arg(it.value().join(", "));
    }
    if (drugoeText.isEmpty()) drugoeText = "Нет данных";
    ui->text_drugoe->setPlainText(drugoeText);

    QString statsText = QString("Статистика: Подлежащих: %1 | Сказуемых: %2 | Определений: %3 | Дополнений: %4 | Обстоятельств: %5 | Прочих: %6")
                            .arg(stats.podlezhaschee)
                            .arg(stats.skazuemoe)
                            .arg(stats.opredelenie)
                            .arg(stats.dopolnenie)
                            .arg(stats.obstoyatelstvo)
                            .arg(stats.drugoe);

    statusBar()->showMessage(statsText, 5000);
}

void MainWindow::handlePythonFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    ui->btn_search->setEnabled(true);
    ui->btn_search->setText("Поиск");
    ui->btn_upload->setEnabled(true);
    statusBar()->clearMessage();

    if (!currentTempFile.isEmpty() && QFile::exists(currentTempFile)) {
        QFile::remove(currentTempFile);
        currentTempFile.clear();
    }

    if (exitStatus == QProcess::CrashExit) {
        QMessageBox::critical(this, "Процесс аварийно завершен",
                              "Python скрипт аварийно завершился.",
                              QMessageBox::Ok);
        ui->btn_download->setEnabled(false);
    } else if (exitCode != 0) {
        QMessageBox::warning(this, "Ошибка скрипта",
                             QString("Python скрипт завершился с кодом ошибки: %1\n"
                                     "Проверьте, что установлены все необходимые библиотеки:\n"
                                     "pip install natasha").arg(exitCode),
                             QMessageBox::Ok);
        ui->btn_download->setEnabled(false);
    } else {
        statusBar()->showMessage("Синтаксический анализ успешно завершен!", 3000);
        if (!sentenceTexts.isEmpty()) {
            calculateStatistics();
            updateStatisticsDisplay();
            updateDisplay();
            ui->btn_download->setEnabled(true);
        } else {
            QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
            item->setText(0, "Анализ завершен, но данные не получены");
            ui->btn_download->setEnabled(false);
        }
    }
}

bool MainWindow::shouldShowWord(const WordInfo& word)
{
    QString type = word.sentence;
    if (type == "Подлежащее") return ui->c_pod->isChecked();
    if (type == "Сказуемое") return ui->c_skaz->isChecked();
    if (type == "Определение") return ui->c_opred->isChecked();
    if (type == "Дополнение") return ui->c_dop->isChecked();
    if (type == "Обстоятельство") return ui->c_ob->isChecked();
    if (type == "Однородный член") return ui->c_none->isChecked();

    return ui->c_none->isChecked();
}

void MainWindow::updateDisplay()
{
    ui->treeWidget->clear();
    if (sentenceTexts.isEmpty() || wordsBySentence.isEmpty()) {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, "Нет данных для отображения");
        return;
    }

    QList<int> sentNumbers = sentenceTexts.keys();
    std::sort(sentNumbers.begin(), sentNumbers.end());
    for (int sentNum : sentNumbers) {
        QString sentenceText = sentenceTexts[sentNum];
        QTreeWidgetItem* sentItem = new QTreeWidgetItem(ui->treeWidget);
        sentItem->setText(0, QString("Предложение %1: %2").arg(sentNum).arg(sentenceText));
        sentItem->setExpanded(false);

        if (wordsBySentence.contains(sentNum)) {
            const QList<WordInfo>& words = wordsBySentence[sentNum];
            bool hasWords = false;
            for (const WordInfo& word : words) {
                if (shouldShowWord(word)) {
                    hasWords = true;
                    QString displayText = QString("%1 — %2 (%3)")
                                              .arg(word.text)
                                              .arg(word.sentence)
                                              .arg(word.posRussian);
                    QTreeWidgetItem* wordItem = new QTreeWidgetItem(sentItem);
                    wordItem->setText(0, displayText);
                }
            }
            if (!hasWords) {
                QTreeWidgetItem* noWordsItem = new QTreeWidgetItem(sentItem);
                noWordsItem->setText(0, "Нет слов, соответствующих выбранным фильтрам");
            }
        }
    }
    ui->treeWidget->collapseAll();
}

void MainWindow::processPythonOutput(const QString& output)
{
    qDebug() << output;
    QStringList lines = output.split("\n", Qt::SkipEmptyParts);
    for (QString& line : lines) {
        line = line.trimmed();
    }

    if (lines.isEmpty()) {
        return;
    }

    QString formattedFilePath;
    for (const QString& line : lines) {
        if (line.startsWith("FORMATTED_FILE:")) {
            formattedFilePath = line.mid(15);
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

void MainWindow::on_btn_download_clicked() {
    if (sentenceTexts.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Сначала проведите анализ текста.");
        return;
    }
    generateHtmlReport();
}
