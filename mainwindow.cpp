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

        << ".stat-section { margin-bottom: 15px; border: 1px solid #dee2e6; border-radius: 8px; overflow: hidden; }"
        << ".stat-header { background: #f8f9fa; padding: 12px 20px; cursor: pointer; user-select: none; transition: background 0.2s; }"
        << ".stat-header:hover { background: #e9ecef; }"
        << ".toggle-icon { display: inline-block; transition: transform 0.2s; font-size: 14px; margin-right: 8px; }"
        << ".stat-header .item-count { font-size: 12px; color: #6c757d; font-weight: normal; margin-left: 8px; }"
        << ".stat-content { padding: 15px 20px; background: white; border-top: 1px solid #dee2e6; max-height: 400px; overflow-y: auto; }"
        << ".stat-list-grid { display: grid; grid-template-columns: repeat(auto-fill, minmax(300px, 1fr)); gap: 12px; }"
        << ".stat-list-item { background: #f8f9fa; padding: 10px 15px; border-radius: 6px; border-left: 3px solid #3498db; }"
        << ".sentence-num { font-weight: bold; color: #2c3e50; margin-bottom: 5px; font-size: 13px; }"
        << ".word-list { color: #495057; font-size: 13px; line-height: 1.4; }"
        << ".no-data { color: #6c757d; font-style: italic; padding: 20px; text-align: center; }"

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

        << "::-webkit-scrollbar { width: 8px; height: 8px; }"
        << "::-webkit-scrollbar-track { background: #f1f1f1; border-radius: 4px; }"
        << "::-webkit-scrollbar-thumb { background: #c1c1c1; border-radius: 4px; }"
        << "::-webkit-scrollbar-thumb:hover { background: #a8a8a8; }"
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
        << "function toggleSection(sectionId) {"
        << "  var content = document.getElementById(sectionId);"
        << "  var icon = document.getElementById(sectionId + '-icon');"
        << "  if (content.style.display === 'none' || content.style.display === '') {"
        << "    content.style.display = 'block';"
        << "    icon.innerHTML = '▼';"
        << "    icon.style.transform = 'rotate(0deg)';"
        << "  } else {"
        << "    content.style.display = 'none';"
        << "    icon.innerHTML = '▶';"
        << "  }"
        << "}"
        << "document.addEventListener('DOMContentLoaded', function() {"
        << "  var sections = document.querySelectorAll('.stat-content');"
        << "  for (var i = 0; i < sections.length; i++) {"
        << "    sections[i].style.display = 'none';"
        << "  }"
        << "});"
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

    // Statistics tab with expandable sections
    out << "<div id='Statistics' class='tab-content'>";

    // Summary cards
    out << "<div class='stats-grid'>"
        << "<div class='stat-card'><h3>Подлежащее</h3><div class='stat-number'>" << stats.podlezhaschee << "</div></div>"
        << "<div class='stat-card'><h3>Сказуемое</h3><div class='stat-number'>" << stats.skazuemoe << "</div></div>"
        << "<div class='stat-card'><h3>Определение</h3><div class='stat-number'>" << stats.opredelenie << "</div></div>"
        << "<div class='stat-card'><h3>Дополнение</h3><div class='stat-number'>" << stats.dopolnenie << "</div></div>"
        << "<div class='stat-card'><h3>Обстоятельство</h3><div class='stat-number'>" << stats.obstoyatelstvo << "</div></div>"
        << "<div class='stat-card'><h3>Другое</h3><div class='stat-number'>" << stats.drugoe << "</div></div>"
        << "</div>";

    out << "<h3 style='margin-top: 30px; margin-bottom: 15px;'>Детальная статистика по предложениям</h3>";

    // Helper lambda to create expandable sections
    auto createExpandableSection = [&out](const QString& title, const QMap<int, QList<QString>>& data, const QString& sectionId) {
        int totalItems = 0;
        for (auto it = data.begin(); it != data.end(); ++it) {
            totalItems += it.value().size();
        }

        out << "<div class='stat-section'>"
            << "<div class='stat-header' onclick=\"toggleSection('" << sectionId << "')\">"
            << "<span class='toggle-icon' id='" << sectionId << "-icon'>▶</span> "
            << "<strong>" << title << "</strong> <span class='item-count'>(" << totalItems << " элементов)</span>"
            << "</div>"
            << "<div id='" << sectionId << "' class='stat-content' style='display: none;'>";

        if (data.isEmpty()) {
            out << "<p class='no-data'>Нет данных</p>";
        } else {
            out << "<div class='stat-list-grid'>";
            for (auto it = data.begin(); it != data.end(); ++it) {
                out << "<div class='stat-list-item'>"
                    << "<div class='sentence-num'>Предложение " << it.key() << "</div>"
                    << "<div class='word-list'>" << it.value().join(", ") << "</div>"
                    << "</div>";
            }
            out << "</div>";
        }
        out << "</div></div>";
    };

    createExpandableSection("Подлежащие", stats.podlezhascheeSentences, "section-pod");
    createExpandableSection("Сказуемые", stats.skazuemoeSentences, "section-skaz");
    createExpandableSection("Определения", stats.opredelenieSentences, "section-opred");
    createExpandableSection("Дополнения", stats.dopolnenieSentences, "section-dop");
    createExpandableSection("Обстоятельства", stats.obstoyatelstvoSentences, "section-ob");
    createExpandableSection("Другое", stats.drugoeSentences, "section-drugoe");

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

    // Hide the tree widget header
    ui->treeWidget->setHeaderHidden(true);

    // Show placeholder messages on both tabs with consistent styling
    showPlaceholderInResults();
    showPlaceholderStatistics();
}

MainWindow::~MainWindow()
{
    if (pythonProcess && pythonProcess->state() == QProcess::Running) {
        pythonProcess->terminate();
        pythonProcess->waitForFinished(3000);
    }
    delete ui;
}

void MainWindow::showPlaceholderInResults()
{
    // Switch to placeholder page
    ui->stackedResults->setCurrentIndex(0);

    // Ensure the placeholder label has consistent styling
    ui->placeholderLabel->setStyleSheet(
        "QLabel#placeholderLabel {"
        "    color: #6c757d;"
        "    font-size: 14px;"
        "    font-style: italic;"
        "    padding: 40px;"
        "    text-align: center;"
        "}"
        );
    ui->placeholderLabel->setText("Введите текст для анализа");
}

void MainWindow::showResultsPage()
{
    // Switch to results page with tree widget
    ui->stackedResults->setCurrentIndex(1);
}

void MainWindow::showPlaceholderStatistics()
{
    // Clear the existing statistics container
    QLayoutItem* child;
    while ((child = ui->statisticsContainerLayout->takeAt(0)) != nullptr) {
        if (child->widget()) delete child->widget();
        delete child;
    }

    // Create a simple centered placeholder similar to results tab
    QWidget* placeholderWidget = new QWidget();
    QVBoxLayout* placeholderLayout = new QVBoxLayout(placeholderWidget);
    placeholderLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* placeholderLabel = new QLabel("Введите текст для анализа");
    placeholderLabel->setObjectName("placeholderLabel");
    placeholderLabel->setAlignment(Qt::AlignCenter);
    placeholderLabel->setStyleSheet(
        "QLabel#placeholderLabel {"
        "    color: #6c757d;"
        "    font-size: 14px;"
        "    font-style: italic;"
        "    padding: 40px;"
        "    text-align: center;"
        "}"
        );

    placeholderLayout->addWidget(placeholderLabel);

    ui->statisticsContainerLayout->addWidget(placeholderWidget);
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

void MainWindow::on_c_pod_stateChanged(int) { updateAllCheckboxState(); if (!sentenceTexts.isEmpty()) updateDisplay(); }
void MainWindow::on_c_skaz_stateChanged(int) { updateAllCheckboxState(); if (!sentenceTexts.isEmpty()) updateDisplay(); }
void MainWindow::on_c_opred_stateChanged(int) { updateAllCheckboxState(); if (!sentenceTexts.isEmpty()) updateDisplay(); }
void MainWindow::on_c_dop_stateChanged(int) { updateAllCheckboxState(); if (!sentenceTexts.isEmpty()) updateDisplay(); }
void MainWindow::on_c_ob_stateChanged(int) { updateAllCheckboxState(); if (!sentenceTexts.isEmpty()) updateDisplay(); }
void MainWindow::on_c_none_stateChanged(int) { updateAllCheckboxState(); if (!sentenceTexts.isEmpty()) updateDisplay(); }

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
        QMessageBox::warning(this, "Предупреждение", "Файл пуст.");
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
    pythonProcess->setProcessEnvironment(env);

    sentenceTexts.clear();
    wordsBySentence.clear();
    ui->treeWidget->clear();

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
        if (ch.isLetter() || (ch.unicode() >= 0x0400 && ch.unicode() <= 0x04FF)) return false;
        if (ch.isPunct() || ch.isSpace()) continue;
        if (!ch.isDigit()) return false;
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

    // First, read the file content without loading it into the UI
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл: " + filePath);
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    QString content = in.readAll();
    file.close();

    if (content.isEmpty()) {
        QMessageBox::warning(this, "Пустой файл",
                             "Файл пуст. Пожалуйста, выберите файл с текстом для анализа.",
                             QMessageBox::Ok);
        return;
    }

    // Check if file contains any letters
    if (!hasAnyLetter(content)) {
        QMessageBox::warning(this, "Нет букв",
                             "Файл не содержит ни одной буквы.\n"
                             "Пожалуйста, выберите файл с текстом, содержащим буквы для анализа.",
                             QMessageBox::Ok);
        return;
    }

    // Check if file contains only numbers
    if (hasOnlyNumbers(content)) {
        QMessageBox::warning(this, "Только цифры",
                             "Файл содержит только цифры.\n"
                             "Пожалуйста, выберите файл с текстом, содержащим буквы для анализа.",
                             QMessageBox::Ok);
        return;
    }

    // All checks passed - now load the text into the UI
    ui->textEdit->setPlainText(content);
    currentFileName = QFileInfo(filePath).fileName();
    statusBar()->showMessage("Загружен файл: " + currentFileName, 3000);

    // Ask user if they want to run analysis
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Запустить анализ?");
    msgBox.setText("Файл загружен. Запустить синтаксический анализ сейчас?");
    msgBox.setIcon(QMessageBox::Question);

    // Apply proper stylesheet targeting QMessageBox buttons
    msgBox.setStyleSheet(
        "QMessageBox {"
        "    background-color: white;"
        "}"
        "QMessageBox QPushButton {"
        "    background-color: #ecf0f1;"
        "    color: #2c3e50;"
        "    border: 1px solid #bdc3c7;"
        "    border-radius: 6px;"
        "    padding: 5px 14px;"
        "    font-size: 12px;"
        "    min-width: 80px;"
        "}"
        "QMessageBox QPushButton:hover {"
        "    background-color: #dfe6e9;"
        "    border: 1px solid #bdc3c7;"
        "}"
        "QMessageBox QPushButton:pressed {"
        "    background-color: #bdc3c7;"
        "}"
        "QLabel {"
        "    color: black;"
        "    background-color: white;"
        "    font-size: 12px;"
        "}"
        );

    QPushButton *yesButton = msgBox.addButton("Да", QMessageBox::YesRole);
    QPushButton *noButton = msgBox.addButton("Нет", QMessageBox::NoRole);
    msgBox.setDefaultButton(yesButton);

    // Style the buttons directly
    yesButton->setCursor(Qt::PointingHandCursor);
    noButton->setCursor(Qt::PointingHandCursor);

    msgBox.exec();

    if (msgBox.clickedButton() == yesButton) {
        runAnalysis(ui->textEdit->toPlainText());
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
    QLayoutItem* child;
    while ((child = ui->statisticsContainerLayout->takeAt(0)) != nullptr) {
        if (child->widget()) delete child->widget();
        delete child;
    }

    QWidget* buttonsWidget = new QWidget();
    buttonsWidget->setObjectName("statButtonsWidget");
    QHBoxLayout* buttonsLayout = new QHBoxLayout(buttonsWidget);
    buttonsLayout->setSpacing(10);
    buttonsLayout->setContentsMargins(10, 8, 10, 8);

    struct StatData {
        QString title;
        int count;
        QMap<int, QList<QString>>* data;
    };

    QList<StatData> statList = {
        {"Подлежащие", stats.podlezhaschee, &stats.podlezhascheeSentences},
        {"Сказуемые", stats.skazuemoe, &stats.skazuemoeSentences},
        {"Определения", stats.opredelenie, &stats.opredelenieSentences},
        {"Дополнения", stats.dopolnenie, &stats.dopolnenieSentences},
        {"Обстоятельства", stats.obstoyatelstvo, &stats.obstoyatelstvoSentences},
        {"Другое", stats.drugoe, &stats.drugoeSentences}
    };

    QScrollArea* contentArea = new QScrollArea();
    contentArea->setObjectName("contentScrollArea");
    contentArea->setWidgetResizable(true);
    contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget* contentWidget = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    contentArea->setWidget(contentWidget);

    QList<QPushButton*> buttons;
    QList<QWidget*> contentWidgets;

    for (const auto& stat : statList) {
        QPushButton* button = new QPushButton(QString("%1 (%2)").arg(stat.title).arg(stat.count));
        button->setObjectName("statButton");
        buttonsLayout->addWidget(button);
        buttons.append(button);

        QWidget* statContent = new QWidget();
        QVBoxLayout* statContentLayout = new QVBoxLayout(statContent);
        statContentLayout->setSpacing(8);
        statContentLayout->setContentsMargins(15, 15, 15, 15);

        if (stat.data->isEmpty()) {
            QLabel* noDataLabel = new QLabel("Нет данных");
            noDataLabel->setObjectName("noDataLabel");
            noDataLabel->setAlignment(Qt::AlignCenter);
            statContentLayout->addWidget(noDataLabel);
        } else {
            for (auto it = stat.data->begin(); it != stat.data->end(); ++it) {
                QString sentenceText = QString("Предложение %1: %2").arg(it.key()).arg(it.value().join(", "));
                QLabel* sentenceLabel = new QLabel(sentenceText);
                sentenceLabel->setObjectName("sentenceLabel");
                sentenceLabel->setWordWrap(true);
                statContentLayout->addWidget(sentenceLabel);
            }
        }

        statContent->setVisible(false);
        contentLayout->addWidget(statContent);
        contentWidgets.append(statContent);
    }

    buttonsLayout->addStretch();
    ui->statisticsContainerLayout->addWidget(buttonsWidget);
    ui->statisticsContainerLayout->addWidget(contentArea);

    for (int i = 0; i < buttons.size(); i++) {
        int index = i;
        connect(buttons[i], &QPushButton::clicked, [this, buttons, contentWidgets, index]() {
            for (int j = 0; j < contentWidgets.size(); j++) {
                contentWidgets[j]->setVisible(false);
                buttons[j]->setProperty("expanded", false);
                buttons[j]->style()->unpolish(buttons[j]);
                buttons[j]->style()->polish(buttons[j]);
            }

            contentWidgets[index]->setVisible(true);
            buttons[index]->setProperty("expanded", true);
            buttons[index]->style()->unpolish(buttons[index]);
            buttons[index]->style()->polish(buttons[index]);
        });
    }

    if (!buttons.isEmpty() && !contentWidgets.isEmpty()) {
        contentWidgets[0]->setVisible(true);
        buttons[0]->setProperty("expanded", true);
        buttons[0]->style()->unpolish(buttons[0]);
        buttons[0]->style()->polish(buttons[0]);
    }

    QString statsText = QString("Статистика: Подлежащих: %1 | Сказуемых: %2 | Определений: %3 | Дополнений: %4 | Обстоятельств: %5 | Прочих: %6")
                            .arg(stats.podlezhaschee).arg(stats.skazuemoe).arg(stats.opredelenie)
                            .arg(stats.dopolnenie).arg(stats.obstoyatelstvo).arg(stats.drugoe);
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
        showPlaceholderInResults();
        showPlaceholderStatistics();
        ui->btn_download->setEnabled(false);
    } else if (exitCode != 0) {
        QMessageBox::warning(this, "Ошибка скрипта",
                             QString("Python скрипт завершился с кодом ошибки: %1\n\n"
                                     "Проверьте, что установлены все необходимые библиотеки:\n"
                                     "pip install natasha").arg(exitCode),
                             QMessageBox::Ok);
        showPlaceholderInResults();
        showPlaceholderStatistics();
        ui->btn_download->setEnabled(false);
    } else {
        statusBar()->showMessage("Синтаксический анализ успешно завершен!", 3000);
        if (!sentenceTexts.isEmpty()) {
            calculateStatistics();
            updateStatisticsDisplay();
            updateDisplay();
            ui->btn_download->setEnabled(true);
        } else {
            showPlaceholderInResults();
            showPlaceholderStatistics();
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
    return ui->c_none->isChecked();
}

void MainWindow::updateDisplay()
{
    if (sentenceTexts.isEmpty() || wordsBySentence.isEmpty()) {
        showPlaceholderInResults();
        return;
    }

    showResultsPage();
    ui->treeWidget->clear();
    ui->treeWidget->setRootIsDecorated(true);
    ui->treeWidget->setExpandsOnDoubleClick(true);

    QList<int> sentNumbers = sentenceTexts.keys();
    std::sort(sentNumbers.begin(), sentNumbers.end());

    for (int sentNum : sentNumbers) {
        QString sentenceText = sentenceTexts[sentNum];

        QTreeWidgetItem* sentItem = new QTreeWidgetItem(ui->treeWidget);
        sentItem->setText(0, QString("Предложение %1: %2").arg(sentNum).arg(sentenceText));
        sentItem->setExpanded(false);
        sentItem->setForeground(0, QBrush(QColor("#495057")));

        QFont sentFont = sentItem->font(0);
        sentFont.setBold(true);
        sentItem->setFont(0, sentFont);

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
                noWordsItem->setForeground(0, QBrush(QColor("#6c757d")));
                noWordsItem->setTextAlignment(0, Qt::AlignCenter);

                QFont italicFont = noWordsItem->font(0);
                italicFont.setItalic(true);
                noWordsItem->setFont(0, italicFont);
            }
        }
    }

    ui->treeWidget->collapseAll();
}

void MainWindow::processPythonOutput(const QString& output)
{
    QStringList lines = output.split("\n", Qt::SkipEmptyParts);

    for (QString& line : lines) {
        line = line.trimmed();
    }

    if (lines.isEmpty()) {
        return;
    }

    for (const QString& line : lines) {
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

void MainWindow::on_btn_download_clicked() {
    if (sentenceTexts.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Сначала проведите анализ текста.");
        return;
    }
    generateHtmlReport();
}
