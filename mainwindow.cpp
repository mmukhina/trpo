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
#include <QTableWidget>
#include <QHeaderView>

void MainWindow::generateHtmlReport()
{
    if (sentenceTexts.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Нет данных для сохранения.");
        return;
    }

    // Пересчитываем статистику
    calculateStatistics();

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Сохранить отчет",
                                                    lastDirectory + "/syntax_report.html", // <-- Тут используем сохраненную папку
                                                    "HTML файлы (*.html)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    // Начинаем генерацию HTML
    out << "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    out << "<title>Синтаксический разбор текста</title>";
    out << "<style>"
        << "body { font-family: 'Segoe UI', Arial, sans-serif; padding: 20px; background: #f0f2f5; color: #333; }"
        << ".container { max-width: 1200px; margin: auto; background: white; padding: 30px; border-radius: 12px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }"
        << "h1 { text-align: center; color: #2c3e50; margin-bottom: 20px; }"
        << "h2 { color: #2c3e50; margin-top: 30px; margin-bottom: 15px; border-bottom: 2px solid #3498db; padding-bottom: 5px; }"
        << "h3 { color: #2c3e50; margin-top: 20px; margin-bottom: 10px; }"

        // Стили для предложений
        << ".sentence { margin-bottom: 30px; border: 1px solid #dee2e6; border-radius: 8px; overflow: hidden; }"
        << ".sentence-header { background: #f8f9fa; padding: 10px 15px; font-weight: bold; color: #2c3e50; border-bottom: 1px solid #dee2e6; }"
        << ".sentence-content { padding: 15px; line-height: 2.5; }"

        // Стили для слов
        << ".word { display: inline-block; margin: 0 3px; text-align: center; }"
        << ".pos { font-size: 10px; color: #666; background: #e8f4fd; padding: 2px 6px; border-radius: 4px; display: block; }"
        << ".text { font-size: 16px; display: block; }"

        // Подчеркивания для членов предложения
        << ".pod { border-bottom: 2px solid black; }"
        << ".skaz { border-bottom: 3px double black; }"
        << ".opred { text-decoration: underline wavy #27ae60; }"
        << ".dop { border-bottom: 2px dashed #2980b9; }"
        << ".ob { border-bottom: 2px dotted #e67e22; }"

        // Статистика - карточки
        << ".stats-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(150px, 1fr)); gap: 15px; margin: 20px 0; }"
        << ".stat-card { background: #3498db; color: white; padding: 15px; border-radius: 8px; text-align: center; }"
        << ".stat-card h3 { margin: 0 0 5px 0; font-size: 14px; }"
        << ".stat-number { font-size: 28px; font-weight: bold; }"

        // Статистика - детальные секции
        << ".stat-section { margin-bottom: 20px; border: 1px solid #dee2e6; border-radius: 8px; }"
        << ".stat-header { background: #f8f9fa; padding: 12px 15px; cursor: pointer; font-weight: bold; }"
        << ".stat-header:hover { background: #e9ecef; }"
        << ".stat-content { padding: 15px; display: none; max-height: 400px; overflow-y: auto; }"
        << ".stat-item { background: #f8f9fa; padding: 8px 12px; margin-bottom: 8px; border-radius: 6px; border-left: 3px solid #3498db; }"
        << ".sentence-num { font-weight: bold; color: #2c3e50; }"
        << ".legend { display: flex; flex-wrap: wrap; gap: 20px; justify-content: center; padding: 12px; margin: 20px 0; background: #f8f9fa; border: 1px solid #dee2e6; border-radius: 6px; }"
        << ".legend-item { display: flex; align-items: center; gap: 8px; color: #6c757d; font-size: 14px; }"
        << ".legend-word { font-weight: 600; color: #343a40; }"

        << "</style>"
        << "<script>"
        << "function toggleSection(id) {"
        << "  var content = document.getElementById(id);"
        << "  if (content.style.display === 'none' || content.style.display === '') {"
        << "    content.style.display = 'block';"
        << "  } else {"
        << "    content.style.display = 'none';"
        << "  }"
        << "}"

        // Функция для открытия вкладок
        << "function openTab(evt, tabName) {"
        << "  var i, tabcontent, tablinks;"
        << "  tabcontent = document.getElementsByClassName('tab-content');"
        << "  for (i = 0; i < tabcontent.length; i++) {"
        << "    tabcontent[i].style.display = 'none';"
        << "  }"
        << "  tablinks = document.getElementsByClassName('tab-link');"
        << "  for (i = 0; i < tablinks.length; i++) {"
        << "    tablinks[i].className = tablinks[i].className.replace(' active', '');"
        << "  }"
        << "  document.getElementById(tabName).style.display = 'block';"
        << "  evt.currentTarget.className += ' active';"
        << "}"

        // Открыть первую вкладку по умолчанию
        << "document.addEventListener('DOMContentLoaded', function() {"
        << "  document.getElementById('Analysis').style.display = 'block';"
        << "});"
        << "</script>"

        // Стили для вкладок
        << "<style>"
        << ".tab { overflow: hidden; border-bottom: 1px solid #dee2e6; margin-bottom: 20px; }"
        << ".tab-link { background-color: inherit; float: left; border: none; outline: none; cursor: pointer; padding: 12px 20px; transition: 0.3s; font-size: 16px; border-radius: 8px 8px 0 0; }"
        << ".tab-link:hover { background-color: #e9ecef; }"
        << ".tab-link.active { background-color: #3498db; color: white; }"
        << ".tab-content { display: none; animation: fadeEffect 0.3s; }"
        << "@keyframes fadeEffect { from {opacity: 0;} to {opacity: 1;} }"
        << "</style>"

        << "</head><body><div class='container'>";

    out << "<div class='legend'>"
        << "<div class='legend-item'><span class='legend-word pod'>пример</span> — подлежащее</div>"
        << "<div class='legend-item'><span class='legend-word skaz'>пример</span> — сказуемое</div>"
        << "<div class='legend-item'><span class='legend-word opred'>пример</span> — определение</div>"
        << "<div class='legend-item'><span class='legend-word dop'>пример</span> — дополнение</div>"
        << "<div class='legend-item'><span class='legend-word ob'>пример</span> — обстоятельство</div>"
        << "</div>";

    // Заголовок
    out << "<h1>Синтаксический разбор текста</h1>";

    // Вкладки
    out << "<div class='tab'>"
        << "<button class='tab-link active' onclick=\"openTab(event, 'Analysis')\">Анализ предложений</button>"
        << "<button class='tab-link' onclick=\"openTab(event, 'Statistics')\">Статистика</button>"
        << "<button class='tab-link' onclick=\"openTab(event, 'WordFreq')\">Частотность слов</button>"
        << "</div>";

    // ========== ВКЛАДКА: АНАЛИЗ ПРЕДЛОЖЕНИЙ ==========
    out << "<div id='Analysis' class='tab-content'>";

    QList<int> sentNumbers = sentenceTexts.keys();
    std::sort(sentNumbers.begin(), sentNumbers.end());

    for (int sentNum : sentNumbers) {
        out << "<div class='sentence'>";
        out << "<div class='sentence-header'>Предложение " << sentNum << "</div>";
        out << "<div class='sentence-content'>";

        for (const WordInfo& word : wordsBySentence[sentNum]) {
            QString cssClass;
            if (word.sentence == "Подлежащее") cssClass = "pod";
            else if (word.sentence == "Сказуемое") cssClass = "skaz";
            else if (word.sentence == "Определение") cssClass = "opred";
            else if (word.sentence == "Дополнение") cssClass = "dop";
            else if (word.sentence == "Обстоятельство") cssClass = "ob";
            else cssClass = "";

            QString displayPos = word.posRussian;
            if (displayPos.isEmpty() || word.speech == "PUNCT") {
                displayPos = "";
            }

            QString escapedText = word.text;
            escapedText.replace("&", "&amp;");
            escapedText.replace("<", "&lt;");
            escapedText.replace(">", "&gt;");

            out << "<div class='word'>"
                << "<span class='text " << cssClass << "'>" << escapedText << "</span>"
                << "</div>";
        }

        out << "</div></div>";
    }
    out << "</div>";

    // ========== ВКЛАДКА: ЧАСТОТНОСТЬ СЛОВ ==========
    out << "<div id='WordFreq' class='tab-content'>";
    out << "<h2>Частота употребления слов по ролям</h2>";
    out << "<table style='width:100%; border-collapse: collapse; margin-top: 10px; font-size: 14px;'>";
    out << "<thead><tr style='background:#f8f9fa; border-bottom: 2px solid #dee2e6;'>"
        << "<th style='padding:10px; text-align:left;'>Слово</th>"
        << "<th style='padding:10px; text-align:left;'>Роль</th>"
        << "<th style='padding:10px; text-align:center; width:80px;'>Количество</th>"
        << "</tr></thead><tbody>";

    QStringList words = wordRoleStats.keys();
    std::sort(words.begin(), words.end());
    for (const QString& w : words) {
        const QMap<QString, int>& roles = wordRoleStats[w];
        QStringList roleKeys = roles.keys();
        std::sort(roleKeys.begin(), roleKeys.end());
        for (const QString& role : roleKeys) {
            out << "<tr style='border-bottom: 1px solid #ecf0f1;'>"
                << "<td style='padding:8px; font-weight:500;'>" << w.toHtmlEscaped() << "</td>"
                << "<td style='padding:8px;'>" << role << "</td>"
                << "<td style='padding:8px; text-align:center; font-weight:bold;'>" << roles[role] << "</td>"
                << "</tr>";
        }
    }
    out << "</tbody></table></div>";

    // ========== ВКЛАДКА: СТАТИСТИКА ==========
    out << "<div id='Statistics' class='tab-content'>";

    // Карточки с общей статистикой
    out << "<div class='stats-grid'>"
        << "<div class='stat-card'><h3>Подлежащее</h3><div class='stat-number'>" << stats.podlezhaschee << "</div></div>"
        << "<div class='stat-card'><h3>Сказуемое</h3><div class='stat-number'>" << stats.skazuemoe << "</div></div>"
        << "<div class='stat-card'><h3>Определение</h3><div class='stat-number'>" << stats.opredelenie << "</div></div>"
        << "<div class='stat-card'><h3>Дополнение</h3><div class='stat-number'>" << stats.dopolnenie << "</div></div>"
        << "<div class='stat-card'><h3>Обстоятельство</h3><div class='stat-number'>" << stats.obstoyatelstvo << "</div></div>"
        << "<div class='stat-card'><h3>Другое</h3><div class='stat-number'>" << stats.drugoe << "</div></div>"
        << "</div>";

    // Детальная статистика по категориям
    auto addStatSection = [&out](const QString& title, const QMap<int, QList<QString>>& data, const QString& id) {
        if (data.isEmpty()) {
            out << "<div class='stat-section'>"
                << "<div class='stat-header' onclick='toggleSection(\"" << id << "\")'>"
                << "▼ " << title << " (0 предложений)"
                << "</div>"
                << "<div class='stat-content' id='" << id << "'>"
                << "<p style='color: #6c757d; font-style: italic; text-align: center;'>Нет данных</p>"
                << "</div></div>";
            return;
        }

        out << "<div class='stat-section'>"
            << "<div class='stat-header' onclick='toggleSection(\"" << id << "\")'>"
            << "▼ " << title << " (" << data.size() << " предложений)"
            << "</div>"
            << "<div class='stat-content' id='" << id << "'>";

        for (auto it = data.begin(); it != data.end(); ++it) {
            QString words = it.value().join(", ");
            out << "<div class='stat-item'>"
                << "<span class='sentence-num'>Предложение " << it.key() << ":</span> "
                << words
                << "</div>";
        }

        out << "</div></div>";
    };

    addStatSection("Подлежащие", stats.podlezhascheeSentences, "stat-pod");
    addStatSection("Сказуемые", stats.skazuemoeSentences, "stat-skaz");
    addStatSection("Определения", stats.opredelenieSentences, "stat-opred");
    addStatSection("Дополнения", stats.dopolnenieSentences, "stat-dop");
    addStatSection("Обстоятельства", stats.obstoyatelstvoSentences, "stat-ob");
    addStatSection("Другое", stats.drugoeSentences, "stat-drugoe");

    out << "</div>"; // Закрываем Statistics tab
    out << "</div></body></html>";
    file.close();

    // Открываем файл в браузере
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pythonProcess(nullptr)
{
    lastDirectory = QDir::homePath();
    ui->setupUi(this);

    QWidget *tabFreq = new QWidget();
    QVBoxLayout *layoutFreq = new QVBoxLayout(tabFreq);
    layoutFreq->setContentsMargins(0, 0, 0, 0);

    QTableWidget *tableFreq = new QTableWidget();
    tableFreq->setObjectName("tableWordStats");
    tableFreq->setColumnCount(3);
    tableFreq->setHorizontalHeaderLabels({"Слово", "Роль", "Количество"});
    tableFreq->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableFreq->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableFreq->verticalHeader()->setVisible(false);
    layoutFreq->addWidget(tableFreq);

    ui->tabWidget->addTab(tabFreq, "Частотность слов");

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
bool MainWindow::isUtf8File(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return false;
    QByteArray data = file.readAll();
    file.close();

    if (data.isEmpty()) return true; // Пустой файл считаем допустимым

    // Побайтовая валидация UTF-8
    const char* ptr = data.constData();
    int len = data.length();
    for (int i = 0; i < len; ) {
        unsigned char c = ptr[i];
        int extraBytes = 0;

        if (c < 0x80) {          // ASCII символ (0xxxxxxx)
            extraBytes = 0;
        } else if ((c & 0xE0) == 0xC0) { // 2 байта (110xxxxx)
            extraBytes = 1;
        } else if ((c & 0xF0) == 0xE0) { // 3 байта (1110xxxx)
            extraBytes = 2;
        } else if ((c & 0xF8) == 0xF0) { // 4 байта (11110xxx)
            extraBytes = 3;
        } else {
            return false; // Недопустимый стартовый байт
        }

        // Проверяем байты продолжения (должны быть 10xxxxxx)
        for (int j = 0; j < extraBytes; j++) {
            i++;
            if (i >= len || (ptr[i] & 0xC0) != 0x80) return false;
        }
        i++;
    }
    return true;
}

void MainWindow::on_btn_upload_clicked()
{
    // 1. Предупреждение пользователю ПЕРЕД открытием диалога
    QMessageBox::information(this, "Требование к файлу",
                             "Внимание!\n\n"
                             "Программа работает только с текстовыми файлами в кодировке UTF-8.\n"
                             "Убедитесь, что ваш файл сохранен именно в этой кодировке.\n\n",
                             QMessageBox::Ok);

    // 2. Открываем диалог выбора файла
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Выберите текстовый файл",
                                                    lastDirectory, // Если вы уже добавили lastDirectory, используйте её. Иначе оставьте QDir::homePath()
                                                    "Текстовые файлы (*.txt)");

    if (filePath.isEmpty()) return;

    // 3. Строгая проверка кодировки
    if (!isUtf8File(filePath)) {
        QMessageBox::critical(this, "Ошибка кодировки",
                              "Выбранный файл не соответствует формату UTF-8 или поврежден.\n"
                              "Пожалуйста, сохраните файл в кодировке UTF-8 и попробуйте снова.",
                              QMessageBox::Ok);
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
    lastDirectory = QFileInfo(filePath).absolutePath();
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
    // Clear existing layout efficiently
    QLayoutItem* child;
    while ((child = ui->statisticsContainerLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

    // Create buttons widget
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

    // Use QStackedWidget for faster switching instead of showing/hiding
    QStackedWidget* stackedContent = new QStackedWidget();
    stackedContent->setObjectName("contentScrollArea");
    stackedContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QList<QPushButton*> buttons;
    QList<QTextBrowser*> textBrowsers;

    // Pre-create all content pages
    for (const auto& stat : statList) {
        // Create button
        QPushButton* button = new QPushButton(QString("%1 (%2)").arg(stat.title).arg(stat.count));
        button->setObjectName("statButton");
        buttonsLayout->addWidget(button);
        buttons.append(button);

        // Create content page with QTextBrowser (faster than many QLabels)
        QWidget* statPage = new QWidget();
        QVBoxLayout* pageLayout = new QVBoxLayout(statPage);
        pageLayout->setSpacing(8);
        pageLayout->setContentsMargins(15, 15, 15, 15);

        QTextBrowser* textBrowser = new QTextBrowser();
        textBrowser->setObjectName("statTextBrowser");
        textBrowser->setStyleSheet(
            "QTextBrowser {"
            "    background-color: transparent;"
            "    border: none;"
            "    font-size: 12px;"
            "    font-family: 'Segoe UI', Arial, sans-serif;"
            "}"
            );

        // Build HTML content
        QString htmlContent;
        if (stat.data->isEmpty()) {
            htmlContent = "<p style='color: #6c757d; font-style: italic; text-align: center; padding: 20px;'>Нет данных</p>";
        } else {
            htmlContent = "<div style='display: flex; flex-direction: column; gap: 8px;'>";
            for (auto it = stat.data->begin(); it != stat.data->end(); ++it) {
                QString wordsText = it.value().join(", ");
                // Truncate if too long for performance
                if (wordsText.length() > 200) {
                    wordsText = wordsText.left(197) + "...";
                }
                htmlContent += QString(
                                   "<div style='background: #f8f9fa; padding: 8px 12px; border-radius: 6px; border-left: 3px solid #3498db;'>"
                                   "<span style='font-weight: bold; color: #2c3e50;'>Предложение %1:</span> "
                                   "<span style='color: #495057;'>%2</span>"
                                   "</div>"
                                   ).arg(it.key()).arg(wordsText);
            }
            htmlContent += "</div>";
        }

        textBrowser->setHtml(htmlContent);
        pageLayout->addWidget(textBrowser);
        textBrowsers.append(textBrowser);
        stackedContent->addWidget(statPage);
    }

    buttonsLayout->addStretch();

    // Connect buttons to stacked widget
    for (int i = 0; i < buttons.size(); i++) {
        int index = i;
        connect(buttons[i], &QPushButton::clicked, [this, buttons, stackedContent, index]() {
            // Update button styles
            for (int j = 0; j < buttons.size(); j++) {
                buttons[j]->setProperty("expanded", false);
                buttons[j]->style()->unpolish(buttons[j]);
                buttons[j]->style()->polish(buttons[j]);
            }
            buttons[index]->setProperty("expanded", true);
            buttons[index]->style()->unpolish(buttons[index]);
            buttons[index]->style()->polish(buttons[index]);

            // Switch page instantly
            stackedContent->setCurrentIndex(index);
        });
    }

    // Add everything to main layout
    ui->statisticsContainerLayout->addWidget(buttonsWidget);
    ui->statisticsContainerLayout->addWidget(stackedContent);

    // Select first button by default
    if (!buttons.isEmpty()) {
        buttons[0]->setProperty("expanded", true);
        buttons[0]->style()->unpolish(buttons[0]);
        buttons[0]->style()->polish(buttons[0]);
        stackedContent->setCurrentIndex(0);
    }

    // Status bar message
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
            calculateWordRoleStats();
            updateStatisticsDisplay();
            updateDisplay();
            updateWordRoleDisplay();
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
                    QString displayText = QString("%1 — %2")
                                              .arg(word.text)
                                              .arg(word.sentence);

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

    void MainWindow::calculateWordRoleStats()
    {
        wordRoleStats.clear();
        for (auto it = wordsBySentence.begin(); it != wordsBySentence.end(); ++it) {
            for (const WordInfo& word : it.value()) {
                if (word.speech == "PUNCT" || word.speech == "SPACE") continue;
                QString w = word.text.toLower(); // Группируем без учета регистра
                wordRoleStats[w][word.sentence]++;
            }
        }
    }

    void MainWindow::updateWordRoleDisplay()
    {
        QTableWidget *table = findChild<QTableWidget*>("tableWordStats");
        if (!table) return;

        table->setRowCount(0);
        int row = 0;

        QStringList words = wordRoleStats.keys();
        std::sort(words.begin(), words.end());

        for (const QString& w : words) {
            const QMap<QString, int>& roles = wordRoleStats[w];
            QStringList roleKeys = roles.keys();
            std::sort(roleKeys.begin(), roleKeys.end());

            for (const QString& role : roleKeys) {
                table->insertRow(row);
                table->setItem(row, 0, new QTableWidgetItem(w));
                table->setItem(row, 1, new QTableWidgetItem(role));
                table->setItem(row, 2, new QTableWidgetItem(QString::number(roles[role])));
                row++;
            }
        }
    }
