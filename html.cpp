#include "mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include "ui_mainwindow.h"

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
        << ".tab { overflow: hidden; border-bottom: 1px solid #dee2e6; margin-bottom: 20px; }"
        << ".tab-link { background-color: #ecf0f1; float: left; border: none; outline: none; cursor: pointer; padding: 12px 20px; transition: 0.3s; font-size: 14px; border-radius: 8px 8px 0 0; margin-right: 2px; }"
        << ".tab-link:hover { background-color: #dfe6e9; }"
        << ".tab-link.active { background-color: #3498db; color: white; }"
        << ".tab-content { display: none; animation: fadeEffect 0.3s; }"
        << "@keyframes fadeEffect { from {opacity: 0;} to {opacity: 1;} }"
        << ".tree-item { margin: 5px 0; }"
        << ".tree-header { background: #f8f9fa; padding: 8px 12px; border-radius: 6px; cursor: pointer; font-weight: bold; color: #2c3e50; border: 1px solid #dee2e6; user-select: none; }"
        << ".tree-header:hover { background: #e9ecef; }"
        << ".tree-children { margin-left: 25px; margin-top: 5px; display: none; }"
        << ".leaf { padding: 5px 12px; color: #495057; border-left: 2px solid #3498db; margin: 3px 0; font-size: 12px; }"
        << ".word-text { font-weight: 500; }"
        << ".word-role { color: #3498db; font-size: 11px; margin-left: 8px; }"
        << ".stats-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(150px, 1fr)); gap: 15px; margin: 20px 0; }"
        << ".stat-card { background: #3498db; color: white; padding: 15px; border-radius: 8px; text-align: center; }"
        << ".stat-card h3 { margin: 0 0 5px 0; font-size: 14px; }"
        << ".stat-number { font-size: 28px; font-weight: bold; }"
        << ".stat-section { margin-bottom: 20px; border: 1px solid #dee2e6; border-radius: 8px; }"
        << ".stat-header { background: #f8f9fa; padding: 12px 15px; cursor: pointer; font-weight: bold; user-select: none; }"
        << ".stat-header:hover { background: #e9ecef; }"
        << ".stat-content { padding: 15px; display: none; max-height: 400px; overflow-y: auto; }"
        << ".stat-item { background: #f8f9fa; padding: 8px 12px; margin-bottom: 8px; border-radius: 6px; border-left: 3px solid #3498db; }"
        << ".sentence-num { font-weight: bold; color: #2c3e50; }"
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
