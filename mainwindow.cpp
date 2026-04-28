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

    calculateStatisticsWithFilter();

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Сохранить отчет",
                                                    lastDirectory + "/syntax_report.html",
                                                    "HTML файлы (*.html)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    out << "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    out << "<title>Синтаксический разбор текста</title>";
    out << "<style>"
        << "body { font-family: 'Segoe UI', Arial, sans-serif; padding: 20px; background: #f0f2f5; color: #333; }"
        << ".container { max-width: 1200px; margin: auto; background: white; padding: 30px; border-radius: 12px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }"
        << "h1 { text-align: center; color: #2c3e50; margin-bottom: 20px; }"
        // Вкладки
        << ".tab { overflow: hidden; border-bottom: 1px solid #dee2e6; margin-bottom: 20px; }"
        << ".tab-link { background-color: #ecf0f1; float: left; border: none; outline: none; cursor: pointer; padding: 12px 20px; transition: 0.3s; font-size: 14px; border-radius: 8px 8px 0 0; margin-right: 2px; }"
        << ".tab-link:hover { background-color: #dfe6e9; }"
        << ".tab-link.active { background-color: #3498db; color: white; }"
        << ".tab-content { display: none; animation: fadeEffect 0.3s; }"
        << "@keyframes fadeEffect { from {opacity: 0;} to {opacity: 1;} }"
        // Дерево анализа
        << ".tree-item { margin: 5px 0; }"
        << ".tree-header { background: #f8f9fa; padding: 8px 12px; border-radius: 6px; cursor: pointer; font-weight: bold; color: #2c3e50; border: 1px solid #dee2e6; user-select: none; }"
        << ".tree-header:hover { background: #e9ecef; }"
        << ".tree-children { margin-left: 25px; margin-top: 5px; display: none; }" // Изначально закрыто
        << ".leaf { padding: 5px 12px; color: #495057; border-left: 2px solid #3498db; margin: 3px 0; font-size: 12px; }"
        << ".word-text { font-weight: 500; }"
        << ".word-role { color: #3498db; font-size: 11px; margin-left: 8px; }"
        // Статистика
        << ".stats-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(150px, 1fr)); gap: 15px; margin: 20px 0; }"
        << ".stat-card { background: #3498db; color: white; padding: 15px; border-radius: 8px; text-align: center; }"
        << ".stat-card h3 { margin: 0 0 5px 0; font-size: 14px; }"
        << ".stat-number { font-size: 28px; font-weight: bold; }"
        << ".stat-section { margin-bottom: 20px; border: 1px solid #dee2e6; border-radius: 8px; }"
        << ".stat-header { background: #f8f9fa; padding: 12px 15px; cursor: pointer; font-weight: bold; user-select: none; }"
        << ".stat-header:hover { background: #e9ecef; }"
        << ".stat-content { padding: 15px; display: none; max-height: 400px; overflow-y: auto; }" // Изначально закрыто
        << ".stat-item { background: #f8f9fa; padding: 8px 12px; margin-bottom: 8px; border-radius: 6px; border-left: 3px solid #3498db; }"
        << ".sentence-num { font-weight: bold; color: #2c3e50; }"
        // Таблица частотности + Сортировка
        << "table { width: 100%; border-collapse: collapse; margin-top: 10px; font-size: 14px; }"
        << "th { cursor: pointer; user-select: none; position: relative; padding-right: 25px; background: #f8fafc; padding: 10px; border-bottom: 2px solid #3498db; font-weight: bold; color: #2c3e50; text-align: left; }"
        << "th:hover { background-color: #e9ecef; }"
        << "th.active { color: #3498db; }"
        << "th::after { content: ''; position: absolute; right: 5px; top: 50%; transform: translateY(-50%); font-size: 12px; color: #95a5a6; transition: color 0.2s; }"
        << "th.active::after { color: #3498db; }"
        << "th[data-sort='asc']::after { content: ' ▲'; }"
        << "th[data-sort='desc']::after { content: ' ▼'; }"
        << "td { padding: 8px; border-bottom: 1px solid #ecf0f1; }"
        << "tr:hover { background: #f0f4f8; }"
        << "</style>"
        << "<script>"
        // Функции переключения
        << "function toggleSection(el, id) {"
        << "  var c = document.getElementById(id);"
        << "  var isHidden = (c.style.display === 'none' || c.style.display === '');"
        << "  c.style.display = isHidden ? 'block' : 'none';"
        << "  el.innerHTML = (isHidden ? '▼' : '▶') + el.innerHTML.substring(1);"
        << "}"
        << "function toggleTree(el, id) {"
        << "  var c = document.getElementById(id);"
        << "  var isHidden = (c.style.display === 'none' || c.style.display === '');"
        << "  c.style.display = isHidden ? 'block' : 'none';"
        << "  el.innerHTML = (isHidden ? '▼' : '▶') + el.innerHTML.substring(1);"
        << "}"
        << "function openTab(evt, tabName) {"
        << "  var i, tc, tl;"
        << "  tc = document.getElementsByClassName('tab-content');"
        << "  for (i = 0; i < tc.length; i++) tc[i].style.display = 'none';"
        << "  tl = document.getElementsByClassName('tab-link');"
        << "  for (i = 0; i < tl.length; i++) tl[i].className = tl[i].className.replace(' active', '');"
        << "  document.getElementById(tabName).style.display = 'block';"
        << "  evt.currentTarget.className += ' active';"
        << "}"
        // Функция сортировки таблицы
        << "let sortDir = {};"
        << "function sortTable(n) {"
        << "  let table = document.querySelector('#WordFreq table');"
        << "  let tbody = table.querySelector('tbody');"
        << "  let rows = Array.from(tbody.querySelectorAll('tr'));"
        << "  let dir = sortDir[n] === 'asc' ? 'desc' : 'asc';"
        << "  sortDir = {}; sortDir[n] = dir;"
        << "  table.querySelectorAll('th').forEach((h, i) => {"
        << "    h.classList.remove('active'); h.removeAttribute('data-sort');"
        << "    if (i === n) { h.classList.add('active'); h.setAttribute('data-sort', dir); }"
        << "  });"
        << "  rows.sort((a, b) => {"
        << "    let av = a.cells[n].textContent.trim();"
        << "    let bv = b.cells[n].textContent.trim();"
        << "    if (n === 2) return dir === 'asc' ? parseFloat(av) - parseFloat(bv) : parseFloat(bv) - parseFloat(av);"
        << "    return dir === 'asc' ? av.localeCompare(bv, 'ru') : bv.localeCompare(av, 'ru');"
        << "  });"
        << "  tbody.innerHTML = '';"
        << "  rows.forEach(r => tbody.appendChild(r));"
        << "}"
        // Инициализация при загрузке
        << "document.addEventListener('DOMContentLoaded', function() {"
        << "  document.getElementById('Analysis').style.display = 'block';"
        << "  sortTable(0);"
        << "});"
        << "</script>"
        << "</head><body><div class='container'>";

    out << "<h1>Синтаксический разбор текста</h1>";
    out << "<div class='tab'>"
        << "<button class='tab-link active' onclick=\"openTab(event, 'Analysis')\">Результаты анализа</button>"
        << "<button class='tab-link' onclick=\"openTab(event, 'Statistics')\">Статистика</button>"
        << "<button class='tab-link' onclick=\"openTab(event, 'WordFreq')\">Частотность слов</button>"
        << "</div>";

    // ========== ВКЛАДКА: АНАЛИЗ ПРЕДЛОЖЕНИЙ ==========
    out << "<div id='Analysis' class='tab-content'>";
    QList<int> sentNumbers = sentenceTexts.keys();
    std::sort(sentNumbers.begin(), sentNumbers.end());
    int treeId = 0;
    for (int sentNum : sentNumbers) {
        QString sentenceText = sentenceTexts[sentNum];
        QString sentId = QString("sent_%1").arg(treeId++);
        out << "<div class='tree-item'>";
        out << "<div class='tree-header' onclick='toggleTree(this, \"" << sentId << "\")'>";
        out << "▶ Предложение " << sentNum << ": " << sentenceText.toHtmlEscaped();
        out << "</div>";
        out << "<div id='" << sentId << "' class='tree-children'>";
        if (wordsBySentence.contains(sentNum)) {
            const QList<WordInfo>& words = wordsBySentence[sentNum];
            bool hasWords = false;
            for (const WordInfo& word : words) {
                if (!shouldShowWord(word) || word.speech == "PUNCT" || word.speech == "SPACE") continue;
                hasWords = true;
                QString escapedText = word.text;
                escapedText.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;");
                out << "<div class='leaf'>"
                    << "<span class='word-text'>" << escapedText << "</span>"
                    << "<span class='word-role'>— " << word.sentence << "</span>"
                    << "</div>";
            }
            if (!hasWords) {
                out << "<div class='leaf' style='color: #6c757d; font-style: italic;'>Нет слов, соответствующих выбранным фильтрам</div>";
            }
        }
        out << "</div></div>";
    }
    out << "</div>";

    // ========== ВКЛАДКА: СТАТИСТИКА ==========
    out << "<div id='Statistics' class='tab-content'>";
    out << "<div class='stats-grid'>";
    if (ui->c_pod->isChecked()) out << "<div class='stat-card'><h3>Подлежащее</h3><div class='stat-number'>" << stats.podlezhaschee << "</div></div>";
    if (ui->c_skaz->isChecked()) out << "<div class='stat-card'><h3>Сказуемое</h3><div class='stat-number'>" << stats.skazuemoe << "</div></div>";
    if (ui->c_opred->isChecked()) out << "<div class='stat-card'><h3>Определение</h3><div class='stat-number'>" << stats.opredelenie << "</div></div>";
    if (ui->c_dop->isChecked()) out << "<div class='stat-card'><h3>Дополнение</h3><div class='stat-number'>" << stats.dopolnenie << "</div></div>";
    if (ui->c_ob->isChecked()) out << "<div class='stat-card'><h3>Обстоятельство</h3><div class='stat-number'>" << stats.obstoyatelstvo << "</div></div>";
    if (ui->c_none->isChecked()) out << "<div class='stat-card'><h3>Другое</h3><div class='stat-number'>" << stats.drugoe << "</div></div>";
    out << "</div>";

    auto addStatSection = [&out](const QString& title, const QMap<int, QList<QString>>& data, const QString& id, bool isChecked) {
        if (!isChecked) return;
        int totalCount = 0;
        for (auto it = data.begin(); it != data.end(); ++it) totalCount += it.value().size();
        out << "<div class='stat-section'>"
            << "<div class='stat-header' onclick='toggleSection(this, \"" << id << "\")'>"
            << "▶ " << title
            << "</div>"
            << "<div class='stat-content' id='" << id << "'>";
        if (data.isEmpty()) {
            out << "<p style='color: #6c757d; font-style: italic; text-align: center;'>Нет данных</p>";
        } else {
            for (auto it = data.begin(); it != data.end(); ++it) {
                out << "<div class='stat-item'>"
                    << "<span class='sentence-num'>Предложение " << it.key() << ":</span> " << it.value().join(", ")
                    << "</div>";
            }
        }
        out << "</div></div>";
    };

    addStatSection("Подлежащие", stats.podlezhascheeSentences, "stat-pod", ui->c_pod->isChecked());
    addStatSection("Сказуемые", stats.skazuemoeSentences, "stat-skaz", ui->c_skaz->isChecked());
    addStatSection("Определения", stats.opredelenieSentences, "stat-opred", ui->c_opred->isChecked());
    addStatSection("Дополнения", stats.dopolnenieSentences, "stat-dop", ui->c_dop->isChecked());
    addStatSection("Обстоятельства", stats.obstoyatelstvoSentences, "stat-ob", ui->c_ob->isChecked());
    addStatSection("Другое", stats.drugoeSentences, "stat-drugoe", ui->c_none->isChecked());
    out << "</div>";

    // ========== ВКЛАДКА: ЧАСТОТНОСТЬ СЛОВ ==========
    out << "<div id='WordFreq' class='tab-content'>";
    out << "<table>"
        << "<thead><tr>"
        << "<th onclick=\"sortTable(0)\">Слово</th>"
        << "<th onclick=\"sortTable(1)\">Роль</th>"
        << "<th onclick=\"sortTable(2)\" style=\"width:120px; text-align:center;\">Количество</th>"
        << "</tr></thead><tbody>";

    QStringList words = wordRoleStats.keys();
    std::sort(words.begin(), words.end());
    for (const QString& w : words) {
        const QMap<QString, int>& roles = wordRoleStats[w];
        QStringList roleKeys = roles.keys();
        std::sort(roleKeys.begin(), roleKeys.end());
        for (const QString& role : roleKeys) {
            out << "<tr>"
                << "<td>" << w.toHtmlEscaped() << "</td>"
                << "<td>" << role << "</td>"
                << "<td style=\"text-align:center; font-weight:bold;\">" << roles[role] << "</td>"
                << "</tr>";
        }
    }
    out << "</tbody></table></div>";
    out << "</div></body></html>";

    file.close();
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pythonProcess(nullptr)
{
    lastDirectory = QDir::homePath();
    ui->setupUi(this);

    analyzedText = ""; // Изначально текст не анализировался
    connect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::onTextEdited);

    // Настройка таблицы частотности слов (созданной в UI)
    QTableWidget *tableFreq = ui->tableWordStats;
    if (tableFreq) {
        tableFreq->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }

    setupPythonProcess();
    ui->btn_download->setEnabled(false);

    // Hide the tree widget header
    ui->treeWidget->setHeaderHidden(true);

    // Show placeholder messages on all tabs with consistent styling
    showPlaceholderInResults();
    showPlaceholderStatistics();
    showPlaceholderWordFreq();

    ui->btn_search->setFocusPolicy(Qt::NoFocus);
    ui->btn_upload->setFocusPolicy(Qt::NoFocus);
    ui->btn_download->setFocusPolicy(Qt::NoFocus);

    // Убираем рамку фокуса у вкладок
    ui->tabWidget->setFocusPolicy(Qt::NoFocus);
    ui->tabWidget->tabBar()->setFocusPolicy(Qt::NoFocus);
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

void MainWindow::showPlaceholderWordFreq()
{
    // Show placeholder on word frequency tab
    ui->stackedWordFreq->setCurrentIndex(0);
    ui->wordFreqPlaceholderLabel->setText("Введите текст для анализа");
}

void MainWindow::showWordFreqResults()
{
    // Show results on word frequency tab
    ui->stackedWordFreq->setCurrentIndex(1);
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


void MainWindow::on_c_pod_stateChanged(int)
{
    updateAllCheckboxState();
    refreshAllDisplay();
}

void MainWindow::on_c_skaz_stateChanged(int)
{
    updateAllCheckboxState();
    refreshAllDisplay();
}

void MainWindow::on_c_opred_stateChanged(int)
{
    updateAllCheckboxState();
    refreshAllDisplay();
}

void MainWindow::on_c_dop_stateChanged(int)
{
    updateAllCheckboxState();
    refreshAllDisplay();
}

void MainWindow::on_c_ob_stateChanged(int)
{
    updateAllCheckboxState();
    refreshAllDisplay();
}

void MainWindow::on_c_none_stateChanged(int)
{
    updateAllCheckboxState();
    refreshAllDisplay();
}

void MainWindow::on_c_all_stateChanged(int arg1)
{
    ui->c_pod->blockSignals(true);
    ui->c_skaz->blockSignals(true);
    ui->c_opred->blockSignals(true);
    ui->c_dop->blockSignals(true);
    ui->c_ob->blockSignals(true);
    ui->c_none->blockSignals(true);
    ui->c_all->blockSignals(true);

    if (arg1 == Qt::Checked) {
        ui->c_pod->setChecked(true);
        ui->c_skaz->setChecked(true);
        ui->c_opred->setChecked(true);
        ui->c_dop->setChecked(true);
        ui->c_ob->setChecked(true);
        ui->c_none->setChecked(true);
        ui->c_all->setCheckState(Qt::Checked);
    }
    else if (arg1 == Qt::Unchecked) {
        ui->c_pod->setChecked(false);
        ui->c_skaz->setChecked(false);
        ui->c_opred->setChecked(false);
        ui->c_dop->setChecked(false);
        ui->c_ob->setChecked(false);
        ui->c_none->setChecked(false);
        ui->c_all->setCheckState(Qt::Unchecked);
    }

    ui->c_pod->blockSignals(false);
    ui->c_skaz->blockSignals(false);
    ui->c_opred->blockSignals(false);
    ui->c_dop->blockSignals(false);
    ui->c_ob->blockSignals(false);
    ui->c_none->blockSignals(false);
    ui->c_all->blockSignals(false);

    refreshAllDisplay();

    // Добавьте эту строку:
    updateButtonsState();
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

    // ОБНОВЛЯЕМ СОСТОЯНИЕ КНОПКИ ПОИСКА
    updateButtonsState();
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

void MainWindow::on_btn_search_clicked()
{
    if (!isAnyFilterSelected()) {
        QMessageBox::warning(this, "Нет активных фильтров",
                             "Не выбран ни один фильтр для отображения слов.\n\n"
                             "Пожалуйста, выберите хотя бы одну категорию.",
                             QMessageBox::Ok);
        return;
    }

    QString searchText = ui->textEdit->toPlainText();

    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "Пустой поиск",
                             "Пожалуйста, введите текст для анализа.",
                             QMessageBox::Ok);
        return;
    }

    runAnalysis(searchText);
}

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

bool MainWindow::isUtf8File(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return false;
    QByteArray data = file.readAll();
    file.close();

    if (data.isEmpty()) return true;

    const char* ptr = data.constData();
    int len = data.length();
    for (int i = 0; i < len; ) {
        unsigned char c = ptr[i];
        int extraBytes = 0;

        if (c < 0x80) {
            extraBytes = 0;
        } else if ((c & 0xE0) == 0xC0) {
            extraBytes = 1;
        } else if ((c & 0xF0) == 0xE0) {
            extraBytes = 2;
        } else if ((c & 0xF8) == 0xF0) {
            extraBytes = 3;
        } else {
            return false;
        }

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
    QMessageBox::information(this, "Требование к файлу",
                             "Внимание!\n\n"
                             "Программа работает только с текстовыми файлами в кодировке UTF-8.\n"
                             "Убедитесь, что ваш файл сохранен именно в этой кодировке.\n\n",
                             QMessageBox::Ok);

    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Выберите текстовый файл",
                                                    lastDirectory,
                                                    "Текстовые файлы (*.txt)");

    if (filePath.isEmpty()) return;

    if (!isUtf8File(filePath)) {
        QMessageBox::critical(this, "Ошибка кодировки",
                              "Выбранный файл не соответствует формату UTF-8 или поврежден.\n"
                              "Пожалуйста, сохраните файл в кодировке UTF-8 и попробуйте снова.",
                              QMessageBox::Ok);
        return;
    }

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

    if (!hasAnyLetter(content)) {
        QMessageBox::warning(this, "Нет букв",
                             "Файл не содержит ни одной буквы.\n"
                             "Пожалуйста, выберите файл с текстом, содержащим буквы для анализа.",
                             QMessageBox::Ok);
        return;
    }

    if (hasOnlyNumbers(content)) {
        QMessageBox::warning(this, "Только цифры",
                             "Файл содержит только цифры.\n"
                             "Пожалуйста, выберите файл с текстом, содержащим буквы для анализа.",
                             QMessageBox::Ok);
        return;
    }

    ui->textEdit->setPlainText(content);
    currentFileName = QFileInfo(filePath).fileName();
    lastDirectory = QFileInfo(filePath).absolutePath();
    statusBar()->showMessage("Загружен файл: " + currentFileName, 3000);

    // Обычное стандартное диалоговое окно без стилей
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Запустить анализ?",
        "Файл загружен. Запустить синтаксический анализ сейчас?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
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

        // Убираем фильтрацию, чтобы видеть все ошибки
        qDebug() << "PYTHON ERROR:" << error;  // Добавьте эту строку

        if (!error.isEmpty()) {
            QMessageBox::critical(this, "Ошибка Python скрипта",
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
        if (child->widget()) {
            child->widget()->deleteLater();
        }
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

    QStackedWidget* stackedContent = new QStackedWidget();
    stackedContent->setObjectName("contentScrollArea");
    stackedContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QList<QPushButton*> buttons;
    QList<QTextBrowser*> textBrowsers;

    for (const auto& stat : statList) {
        QPushButton* button = new QPushButton(QString("%1 (%2)").arg(stat.title).arg(stat.count));
        button->setObjectName("statButton");
        buttonsLayout->addWidget(button);
        buttons.append(button);

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

        QString htmlContent;
        if (stat.data->isEmpty()) {
            htmlContent = "<p style='color: #6c757d; font-style: italic; text-align: center; padding: 20px;'>Нет данных</p>";
        } else {
            htmlContent = "<div style='display: flex; flex-direction: column; gap: 8px;'>";
            for (auto it = stat.data->begin(); it != stat.data->end(); ++it) {
                QString wordsText = it.value().join(", ");
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

    for (int i = 0; i < buttons.size(); i++) {
        int index = i;
        connect(buttons[i], &QPushButton::clicked, [this, buttons, stackedContent, index]() {
            for (int j = 0; j < buttons.size(); j++) {
                buttons[j]->setProperty("expanded", false);
                buttons[j]->style()->unpolish(buttons[j]);
                buttons[j]->style()->polish(buttons[j]);
            }
            buttons[index]->setProperty("expanded", true);
            buttons[index]->style()->unpolish(buttons[index]);
            buttons[index]->style()->polish(buttons[index]);

            stackedContent->setCurrentIndex(index);
        });
    }

    ui->statisticsContainerLayout->addWidget(buttonsWidget);
    ui->statisticsContainerLayout->addWidget(stackedContent);

    if (!buttons.isEmpty()) {
        buttons[0]->setProperty("expanded", true);
        buttons[0]->style()->unpolish(buttons[0]);
        buttons[0]->style()->polish(buttons[0]);
        stackedContent->setCurrentIndex(0);
    }
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
        QMessageBox::critical(this, "Аварийное завершение",
                              "Программа анализа неожиданно завершила работу.\n\n"
                              "Попробуйте перезапустить приложение.");
        refreshAllDisplay();
    }
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
                             QString("Не удалось выполнить анализ: %1\n\n"
                                     "Проверьте, что установлены библиотеки:\n"
                                     "pip install natasha").arg(errorMessage));
        refreshAllDisplay();
    }
    else {
        statusBar()->showMessage("Анализ успешно завершен", 3000);
        analyzedText = ui->textEdit->toPlainText(); // 🟢 Запоминаем текст только после УСПЕШНОГО анализа
        refreshAllDisplay(); // Внутри вызовется updateButtonsState() и разблокирует кнопку
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
            if (!shouldShowWord(word)) continue;

            QString w = word.text.toLower();
            wordRoleStats[w][word.sentence]++;
        }
    }
}

void MainWindow::updateWordRoleDisplay()
{
    QTableWidget *table = ui->tableWordStats;
    if (!table) return;

    // 🔹 Отключаем сортировку на время заполнения
    table->setSortingEnabled(false);
    table->setRowCount(0);

    // 🔹 Проверяем, выбран ли хотя бы один фильтр
    if (!isAnyFilterSelected()) {
        table->hide();

        QWidget* parentPage = table->parentWidget();
        QVBoxLayout* parentLayout = qobject_cast<QVBoxLayout*>(parentPage->layout());
        if (!parentLayout) return;

        QLabel* noFilterLabel = parentPage->findChild<QLabel*>("noFilterWordFreqLabel");
        if (!noFilterLabel) {
            noFilterLabel = new QLabel(parentPage);
            noFilterLabel->setObjectName("noFilterWordFreqLabel");
            noFilterLabel->setAlignment(Qt::AlignCenter);
            noFilterLabel->setStyleSheet("color: #6c757d; padding: 40px; font-style: italic;");
            QFont labelFont;
            labelFont.setItalic(true);
            noFilterLabel->setFont(labelFont);
            parentLayout->insertWidget(0, noFilterLabel);
        }
        noFilterLabel->setText("Нет выбранных фильтров для отображения");
        noFilterLabel->show();
        return;
    }

    // 🔹 Если фильтры выбраны — скрываем метку и показываем таблицу
    if (table->parentWidget()) {
        QLabel* noFilterLabel = table->parentWidget()->findChild<QLabel*>("noFilterWordFreqLabel");
        if (noFilterLabel) noFilterLabel->hide();
    }
    table->show();

    // 🔹 Проверяем, есть ли данные
    if (wordRoleStats.isEmpty()) {
        table->setRowCount(1);
        QTableWidgetItem* messageItem = new QTableWidgetItem("Нет данных для отображения с текущими фильтрами");
        messageItem->setTextAlignment(Qt::AlignCenter);
        messageItem->setForeground(QBrush(QColor("#6c757d")));
        QFont font = messageItem->font();
        font.setItalic(true);
        messageItem->setFont(font);
        table->setItem(0, 0, messageItem);
        table->setItem(0, 1, new QTableWidgetItem(""));
        table->setItem(0, 2, new QTableWidgetItem(""));
        table->setSpan(0, 0, 1, 3);

        // 🔹 Настраиваем заголовок даже для пустой таблицы
        setupTableHeader(table);
        table->setSortingEnabled(true);
        return;
    }

    // 🔹 Заполнение таблицы с правильной типизацией данных для сортировки
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

            // 🔹 КЛЮЧЕВОЕ: сохраняем число как Qt::DisplayRole — сортировка будет числовой!
            QTableWidgetItem *countItem = new QTableWidgetItem();
            countItem->setData(Qt::DisplayRole, roles[role]);
            table->setItem(row, 2, countItem);

            table->item(row, 2)->setTextAlignment(Qt::AlignCenter);
            row++;
        }
    }

    // 🔹 Настраиваем заголовок и включаем сортировку
    setupTableHeader(table);
    table->setSortingEnabled(true);

    // 🔹 По умолчанию сортируем по первому столбцу (Слово) по возрастанию
    table->sortByColumn(0, Qt::AscendingOrder);
}

void MainWindow::setupTableHeader(QTableWidget* table)
{
    if (!table) return;
    QHeaderView* header = table->horizontalHeader();

    // 1. Отключаем встроенную стрелку Qt
    header->setSortIndicatorShown(false);
    header->setSectionsClickable(true);
    header->setHighlightSections(false);

    // 2. Применяем стили, гарантированно скрывая системный индикатор
    table->setStyleSheet(
        "QHeaderView::section {"
        "    background-color: #f8fafc;"
        "    padding: 10px;"
        "    border: none;"
        "    border-bottom: 2px solid #3498db;"
        "    font-weight: bold;"
        "    font-size: 12px;"
        "    color: #2c3e50;"
        "    text-align: left;"
        "}"
        // Принудительно скрываем встроенные стрелки Qt
        "QHeaderView::up-arrow, QHeaderView::down-arrow {"
        "    image: none !important;"
        "    width: 0px;"
        "    height: 0px;"
        "    margin: 0px;"
        "}"
        "QTableWidget::item { padding: 8px; border-bottom: 1px solid #ecf0f1; color: #495057; }"
        "QTableWidget::item:selected { background-color: #e8f4fd; color: #2c3e50; }"
        );

    // 3. Логика отрисовки ТОЛЬКО НАШИХ стрелок (▲ / ▼) в тексте заголовка
    connect(header, &QHeaderView::sortIndicatorChanged, table, [table](int logicalIndex, Qt::SortOrder order) {
        QString arrow = (order == Qt::AscendingOrder) ? " ▲" : " ▼";
        QStringList names = {"Слово", "Роль", "Количество"};

        for (int col = 0; col < 3; ++col) {
            QTableWidgetItem* item = table->horizontalHeaderItem(col);
            if (!item) continue;

            if (col == logicalIndex) {
                item->setText(names[col] + arrow);
                item->setForeground(QBrush(QColor("#3498db"))); // Синий активный
                QFont f = item->font(); f.setBold(true); item->setFont(f);
            } else {
                item->setText(names[col]);
                item->setForeground(QBrush(QColor("#2c3e50"))); // Серый неактивный
                QFont f = item->font(); f.setBold(true); item->setFont(f);
            }
        }
    });

    table->setSortingEnabled(true);
    table->sortByColumn(0, Qt::AscendingOrder);
}

void MainWindow::calculateStatisticsWithFilter()
{
    // Очищаем статистику
    stats = Statistics();

    for (auto it = wordsBySentence.begin(); it != wordsBySentence.end(); ++it) {
        int sentenceNum = it.key();
        const QList<WordInfo>& words = it.value();

        for (const WordInfo& word : words) {
            // Применяем фильтры при подсчете статистики
            if (!shouldShowWord(word)) {
                continue;  // Пропускаем слова, которые не проходят фильтр
            }

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

void MainWindow::updateStatisticsDisplayWithFilter()
{
    // Очищаем существующий layout
    QLayoutItem* child;
    while ((child = ui->statisticsContainerLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

    // Создаем виджет с кнопками
    QWidget* buttonsWidget = new QWidget();
    buttonsWidget->setObjectName("statButtonsWidget");
    QHBoxLayout* buttonsLayout = new QHBoxLayout(buttonsWidget);
    buttonsLayout->setSpacing(10);
    buttonsLayout->setContentsMargins(10, 8, 10, 8);

    struct StatData {
        QString title;
        int count;
        QMap<int, QList<QString>>* data;
        bool enabled;  // Добавляем флаг, показывать ли эту категорию
    };

    // Получаем состояние фильтров
    bool showPod = ui->c_pod->isChecked();
    bool showSkaz = ui->c_skaz->isChecked();
    bool showOpred = ui->c_opred->isChecked();
    bool showDop = ui->c_dop->isChecked();
    bool showOb = ui->c_ob->isChecked();
    bool showNone = ui->c_none->isChecked();

    QList<StatData> statList = {
        {"Подлежащие", stats.podlezhaschee, &stats.podlezhascheeSentences, showPod},
        {"Сказуемые", stats.skazuemoe, &stats.skazuemoeSentences, showSkaz},
        {"Определения", stats.opredelenie, &stats.opredelenieSentences, showOpred},
        {"Дополнения", stats.dopolnenie, &stats.dopolnenieSentences, showDop},
        {"Обстоятельства", stats.obstoyatelstvo, &stats.obstoyatelstvoSentences, showOb},
        {"Другое", stats.drugoe, &stats.drugoeSentences, showNone}
    };

    QStackedWidget* stackedContent = new QStackedWidget();
    stackedContent->setObjectName("contentScrollArea");
    stackedContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QList<QPushButton*> buttons;
    int visibleIndex = 0;

    // Создаем страницы только для видимых категорий
    for (const auto& stat : statList) {
        if (!stat.enabled) continue;  // Пропускаем отключенные категории

        // Создаем кнопку
        QPushButton* button = new QPushButton(QString("%1 (%2)").arg(stat.title).arg(stat.count));
        button->setObjectName("statButton");
        button->setFocusPolicy(Qt::NoFocus);
        buttonsLayout->addWidget(button);
        buttons.append(button);

        // Создаем страницу с контентом
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

        // Формируем HTML контент
        QString htmlContent;
        if (stat.data->isEmpty()) {
            htmlContent = "<p style='color: #6c757d; font-style: italic; text-align: center; padding: 20px;'>"
                          "Нет данных для отображения с текущими фильтрами</p>";
        } else {
            htmlContent = "<div style='display: flex; flex-direction: column; gap: 8px;'>";
            for (auto it = stat.data->begin(); it != stat.data->end(); ++it) {
                QString wordsText = it.value().join(", ");
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
        stackedContent->addWidget(statPage);
        visibleIndex++;
    }

    buttonsLayout->addStretch();

    // Подключаем кнопки к stacked widget
    for (int i = 0; i < buttons.size(); i++) {
        int index = i;
        connect(buttons[i], &QPushButton::clicked, [this, buttons, stackedContent, index]() {
            for (int j = 0; j < buttons.size(); j++) {
                buttons[j]->setProperty("expanded", false);
                buttons[j]->style()->unpolish(buttons[j]);
                buttons[j]->style()->polish(buttons[j]);
            }
            buttons[index]->setProperty("expanded", true);
            buttons[index]->style()->unpolish(buttons[index]);
            buttons[index]->style()->polish(buttons[index]);
            stackedContent->setCurrentIndex(index);
        });
    }

    // Добавляем виджеты в главный layout
    ui->statisticsContainerLayout->addWidget(buttonsWidget);
    ui->statisticsContainerLayout->addWidget(stackedContent);

    // Выбираем первую кнопку по умолчанию, если есть кнопки
    if (!buttons.isEmpty()) {
        buttons[0]->setProperty("expanded", true);
        buttons[0]->style()->unpolish(buttons[0]);
        buttons[0]->style()->polish(buttons[0]);
        stackedContent->setCurrentIndex(0);
    } else {
        // Если все категории отключены, показываем сообщение
        QLabel* noDataLabel = new QLabel("Все категории отключены фильтрами.\nВключите хотя бы одну категорию в фильтре.");
        noDataLabel->setAlignment(Qt::AlignCenter);
        noDataLabel->setStyleSheet("color: #6c757d; padding: 40px; font-style: italic;");
        ui->statisticsContainerLayout->addWidget(noDataLabel);
    }

    // Обновляем статусбар
    int totalCount = stats.podlezhaschee + stats.skazuemoe + stats.opredelenie +
                     stats.dopolnenie + stats.obstoyatelstvo + stats.drugoe;

    QStringList activeFilters;
    if (showPod) activeFilters << "Подлежащие";
    if (showSkaz) activeFilters << "Сказуемые";
    if (showOpred) activeFilters << "Определения";
    if (showDop) activeFilters << "Дополнения";
    if (showOb) activeFilters << "Обстоятельства";
    if (showNone) activeFilters << "Другое";
}

bool MainWindow::isAnyFilterSelected()
{
    return ui->c_pod->isChecked() ||
           ui->c_skaz->isChecked() ||
           ui->c_opred->isChecked() ||
           ui->c_dop->isChecked() ||
           ui->c_ob->isChecked() ||
           ui->c_none->isChecked();
}

void MainWindow::refreshAllDisplay()
{
    if (sentenceTexts.isEmpty()) {
        showPlaceholderInResults();
        showPlaceholderStatistics();
        showPlaceholderWordFreq();
        updateButtonsState(); // <-- Заменяем прямое setEnabled
        return;
    }

    if (!isAnyFilterSelected()) {
        updateDisplay();

        QLayoutItem* child;
        while ((child = ui->statisticsContainerLayout->takeAt(0)) != nullptr) {
            if (child->widget()) child->widget()->deleteLater();
            delete child;
        }
        QLabel* noFilterLabel = new QLabel("Нет выбранных фильтров для отображения");
        noFilterLabel->setAlignment(Qt::AlignCenter);
        noFilterLabel->setStyleSheet("color: #6c757d; padding: 40px; font-style: italic;");
        ui->statisticsContainerLayout->addWidget(noFilterLabel);

        updateWordRoleDisplay();
        showWordFreqResults();
        updateButtonsState(); // <-- Заменяем прямое setEnabled
        return;
    }

    calculateStatisticsWithFilter();
    calculateWordRoleStats();
    updateDisplay();
    updateStatisticsDisplayWithFilter();
    updateWordRoleDisplay();
    showWordFreqResults();

    updateButtonsState(); // <-- Единая точка управления состоянием кнопок
}

void MainWindow::updateButtonsState()
{
    bool anyFilterSelected = isAnyFilterSelected();
    bool isAnalyzing = (pythonProcess && pythonProcess->state() == QProcess::Running);
    // Текст должен совпадать с последним проанализированным
    bool textMatches = (!analyzedText.isEmpty() && ui->textEdit->toPlainText() == analyzedText);

    // Кнопка поиска
    if (isAnalyzing) {
        ui->btn_search->setEnabled(false);
    } else {
        ui->btn_search->setEnabled(anyFilterSelected);
    }

    // Кнопка сохранения: доступна ТОЛЬКО если текст не менялся, есть данные и выбраны фильтры
    if (sentenceTexts.isEmpty() || !anyFilterSelected || !textMatches) {
        ui->btn_download->setEnabled(false);
    } else {
        ui->btn_download->setEnabled(true);
    }

    // Кнопка загрузки
    ui->btn_upload->setEnabled(!isAnalyzing);
}

void MainWindow::onTextEdited()
{
    // Если текущий текст не совпадает с проанализированным, мгновенно блокируем кнопку
    if (ui->textEdit->toPlainText() != analyzedText) {
        ui->btn_download->setEnabled(false);
    }
}


