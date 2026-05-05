#include "mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include "ui_mainwindow.h"

// Генерация HTML-отчет
void MainWindow::generateHtmlReport()
{
    if (sentenceTexts.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Нет данных для сохранения.");
        return;
    }

    // Пересчитываем статистику с учетом текущих фильтров
    calculateStatisticsWithFilter();

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Сохранить отчет",
                                                    lastDirectory,
                                                    "HTML файлы (*.html)");
    if (fileName.isEmpty()) return; // Пользователь отменил сохранение

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    out << "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    out << "<title>Синтаксический разбор текста</title>";
    
    out << "<style>"
        // Основнвые стили страницы
        << "body { font-family: 'Segoe UI', Arial, sans-serif; padding: 20px; background: #f0f2f5; color: #333; }"
        << ".container { max-width: 1200px; margin: auto; background: white; padding: 30px; border-radius: 12px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }"
        << "h1 { text-align: center; color: #2c3e50; margin-bottom: 20px; }"
        
        // Контейнер вкладок
        << ".tab { overflow: hidden; border-bottom: 1px solid #dee2e6; margin-bottom: 20px; }"
        // Кнопка вкладки
        << ".tab-link { background-color: #ecf0f1; float: left; border: none; outline: none; cursor: pointer; padding: 12px 20px; transition: 0.3s; font-size: 14px; border-radius: 8px 8px 0 0; margin-right: 2px; }"
        << ".tab-link:hover { background-color: #dfe6e9; }"
        // Активная вкладка
        << ".tab-link.active { background-color: #3498db; color: white; }"
        // Контент вкладки изначально скрыт
        << ".tab-content { display: none; animation: fadeEffect 0.3s; }"
        // Анимация появления
        << "@keyframes fadeEffect { from {opacity: 0;} to {opacity: 1;} }"
        
        // Стили дерева результатов
        // Заголовок предложения
        << ".tree-header { background: #f8f9fa; padding: 8px 12px; border-radius: 6px; cursor: pointer; font-weight: bold; color: #2c3e50; border: 1px solid #dee2e6; user-select: none; }"
        << ".tree-header:hover { background: #e9ecef; }"
        // Контейнер со словами предложения
        << ".tree-children { margin-left: 25px; margin-top: 5px; display: none; }"
        // Отдельное слово
        << ".leaf { padding: 5px 12px; color: #495057; border-left: 2px solid #3498db; margin: 3px 0; font-size: 12px; }"
        
        // Стили статистики
        // Сетка карточек с количествами
        << ".stats-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(150px, 1fr)); gap: 15px; margin: 20px 0; }"
        // Карточка статистики
        << ".stat-card { background: #3498db; color: white; padding: 15px; border-radius: 8px; text-align: center; }"
        << ".stat-card h3 { margin: 0 0 5px 0; font-size: 14px; }"
        << ".stat-number { font-size: 28px; font-weight: bold; }"
        // Раскрывающаяся секция для каждой роли
        << ".stat-section { margin-bottom: 20px; border: 1px solid #dee2e6; border-radius: 8px; }"
        << ".stat-header { background: #f8f9fa; padding: 12px 15px; cursor: pointer; font-weight: bold; user-select: none; }"
        << ".stat-header:hover { background: #e9ecef; }"
        // Контент секции (изначально скрыт)
        << ".stat-content { padding: 15px; display: none; max-height: 400px; overflow-y: auto; }"
        // Элемент внутри секции
        << ".stat-item { background: #f8f9fa; padding: 8px 12px; margin-bottom: 8px; border-radius: 6px; border-left: 3px solid #3498db; }"
        
        // Стили таблицы частотности слов
        << "table { width: 100%; border-collapse: collapse; margin-top: 10px; font-size: 14px; }"
        // Заголовок таблицы
        << "th { cursor: pointer; user-select: none; position: relative; padding-right: 25px; background: #f8fafc; padding: 10px; border-bottom: 2px solid #3498db; font-weight: bold; color: #2c3e50; text-align: left; }"
        << "th:hover { background-color: #e9ecef; }"
        << "th.active { color: #3498db; }"
        // Стрелки сортировки
        << "th[data-sort='asc']::after { content: ' ▲'; }"
        << "th[data-sort='desc']::after { content: ' ▼'; }"
        // Ячейки таблицы
        << "td { padding: 8px; border-bottom: 1px solid #ecf0f1; }"
        << "tr:hover { background: #f0f4f8; }"
        << "</style>"
        
        // JavaScript
        << "<script>"
        
        // Функция переключения вкладок
        << "function openTab(evt, tabName) {"
        << "  var i, tc, tl;"
        << "  tc = document.getElementsByClassName('tab-content');"
        << "  for (i = 0; i < tc.length; i++) tc[i].style.display = 'none';"  // Скрыть все вкладки
        << "  tl = document.getElementsByClassName('tab-link');"
        << "  for (i = 0; i < tl.length; i++) tl[i].className = tl[i].className.replace(' active', '');" // Убрать активный класс у всех кнопок
        << "  document.getElementById(tabName).style.display = 'block';"  // Показать выбранную вкладку
        << "  evt.currentTarget.className += ' active';"  // Добавить активный класс кнопке
        << "}"
        
        // Функция раскрытия/сворачивания дерева предложений
        << "function toggleTree(el, id) {"
        << "  var c = document.getElementById(id);"
        << "  var isHidden = (c.style.display === 'none' || c.style.display === '');"  // Проверка текущего состояния
        << "  c.style.display = isHidden ? 'block' : 'none';"  // Переключение видимости
        << "  el.innerHTML = (isHidden ? '▼' : '▶') + el.innerHTML.substring(1);"  // Меняем иконку ▶/▼
        << "}"
        
        // Функция раскрытия/сворачивания секций статистики
        << "function toggleSection(el, id) {"
        << "  var c = document.getElementById(id);"
        << "  var isHidden = (c.style.display === 'none' || c.style.display === '');"
        << "  c.style.display = isHidden ? 'block' : 'none';"
        << "  el.innerHTML = (isHidden ? '▼' : '▶') + el.innerHTML.substring(1);"
        << "}"
        
        // Функция сортировки таблицы
        << "let sortDir = {};"
        << "function sortTable(n) {"
        << "  let table = document.querySelector('#WordFreq table');"  // Находим таблицу частотности
        << "  let tbody = table.querySelector('tbody');"
        << "  let rows = Array.from(tbody.querySelectorAll('tr'));"    // Получаем все строки
        << "  let dir = sortDir[n] === 'asc' ? 'desc' : 'asc';"        // Переключаем направление
        << "  sortDir = {}; sortDir[n] = dir;"
        
        // Обновляет визуальное отображение стрелок в заголовках
        << "  table.querySelectorAll('th').forEach((h, i) => {"
        << "    h.classList.remove('active'); h.removeAttribute('data-sort');"
        << "    if (i === n) { h.classList.add('active'); h.setAttribute('data-sort', dir); }"
        << "  });"
        
        // Сортирует строки
        << "  rows.sort((a, b) => {"
        << "    let av = a.cells[n].textContent.trim();"
        << "    let bv = b.cells[n].textContent.trim();"
        << "    if (n === 2) return dir === 'asc' ? parseFloat(av) - parseFloat(bv) : parseFloat(bv) - parseFloat(av);"
        << "    return dir === 'asc' ? av.localeCompare(bv, 'ru') : bv.localeCompare(av, 'ru');"
        << "  });"
        
        // Перестраиваем таблицу с отсортированными строками
        << "  tbody.innerHTML = '';"
        << "  rows.forEach(r => tbody.appendChild(r));"
        << "}"
        
        // Инициализация при загрузке страницы
        << "document.addEventListener('DOMContentLoaded', function() {"
        << "  document.getElementById('Analysis').style.display = 'block';"
        << "  sortTable(0);"
        << "});"
        << "</script>"
        << "</head><body><div class='container'>";

    // Заголовок и панель вкладок
    out << "<h1>Синтаксический разбор текста</h1>";
    out << "<div class='tab'>"
        << "<button class='tab-link active' onclick=\"openTab(event, 'Analysis')\">Результаты анализа</button>"
        << "<button class='tab-link' onclick=\"openTab(event, 'Statistics')\">Статистика</button>"
        << "<button class='tab-link' onclick=\"openTab(event, 'WordFreq')\">Частотность слов</button>"
        << "</div>";

    // Вкладка 1: Результаты анализа
    out << "<div id='Analysis' class='tab-content'>";
    
    // Получить все номера предложений и сортировать их по возрастанию
    QList<int> sentNumbers = sentenceTexts.keys();
    std::sort(sentNumbers.begin(), sentNumbers.end());
    
    int treeId = 0;  // Уникальный идентификатор для каждого дерева предложения
    for (int sentNum : sentNumbers) {
        QString sentenceText = sentenceTexts[sentNum];
        QString sentId = QString("sent_%1").arg(treeId++);
        
        // Блок одного предложения
        out << "<div class='tree-item'>";
        // Заголовок - кликабельный для раскрытия/сворачивания
        out << "<div class='tree-header' onclick='toggleTree(this, \"" << sentId << "\")'>";
        out << "▶ Предложение " << sentNum << ": " << sentenceText.toHtmlEscaped();  // toHtmlEscaped() экранирует спецсимволы
        out << "</div>";
        
        // Контейнер со словами
        out << "<div id='" << sentId << "' class='tree-children'>";
        
        if (wordsBySentence.contains(sentNum)) {
            const QList<WordInfo>& words = wordsBySentence[sentNum];
            bool hasWords = false;
            
            for (const WordInfo& word : words) {
                // Пропускаем слова, не прошедшие фильтр, а также знаки препинания и пробелы
                if (!shouldShowWord(word) || word.speech == "PUNCT" || word.speech == "SPACE") continue;
                hasWords = true;
                // Выводим слово и его синтаксическую роль
                out << "<div class='leaf'>" << word.text.toHtmlEscaped() << " — " << word.sentence << "</div>";
            }
            
            // Если после фильтрации не осталось слов - выводим информационное сообщение
            if (!hasWords) {
                out << "<div class='leaf' style='color: #6c757d; font-style: italic;'>Нет слов, соответствующих выбранным фильтрам</div>";
            }
        }
        out << "</div></div>";
    }
    out << "</div>";

    // Вкладка 2: Статистика
    out << "<div id='Statistics' class='tab-content'>";
    
    // Карточки с общим количеством каждой роли
    out << "<div class='stats-grid'>";
    if (ui->c_pod->isChecked()) 
        out << "<div class='stat-card'><h3>Подлежащее</h3><div class='stat-number'>" << stats.podlezhaschee << "</div></div>";
    if (ui->c_skaz->isChecked()) 
        out << "<div class='stat-card'><h3>Сказуемое</h3><div class='stat-number'>" << stats.skazuemoe << "</div></div>";
    if (ui->c_opred->isChecked()) 
        out << "<div class='stat-card'><h3>Определение</h3><div class='stat-number'>" << stats.opredelenie << "</div></div>";
    if (ui->c_dop->isChecked()) 
        out << "<div class='stat-card'><h3>Дополнение</h3><div class='stat-number'>" << stats.dopolnenie << "</div></div>";
    if (ui->c_ob->isChecked()) 
        out << "<div class='stat-card'><h3>Обстоятельство</h3><div class='stat-number'>" << stats.obstoyatelstvo << "</div></div>";
    if (ui->c_none->isChecked()) 
        out << "<div class='stat-card'><h3>Другое</h3><div class='stat-number'>" << stats.drugoe << "</div></div>";
    out << "</div>";
    
    // Лямбда-функция для добавления раскрывающейся секции статистики
    auto addStatSection = [&out](const QString& title, 
                                  const QMap<int, QList<QString>>& data, 
                                  const QString& id, 
                                  bool isChecked) {
        if (!isChecked) return;  // Если фильтр отключен - не показываем секцию
        
        out << "<div class='stat-section'>"
            << "<div class='stat-header' onclick='toggleSection(this, \"" << id << "\")'>"
            << "▶ " << title
            << "</div>"
            << "<div class='stat-content' id='" << id << "'>";
        
        if (data.isEmpty()) {
            out << "<p style='color: #6c757d; font-style: italic; text-align: center;'>Нет данных</p>";
        } else {
            // Перебираем все предложения, в которых есть слова данной роли
            for (auto it = data.begin(); it != data.end(); ++it) {
                out << "<div class='stat-item'>"
                    << "<span style='font-weight: bold; color: #2c3e50;'>Предложение " << it.key() << ":</span> "
                    << "<span style='color: #495057;'>" << it.value().join(", ") << "</span>"
                    << "</div>";
            }
        }
        out << "</div></div>";
    };
    
    // Добавляем секции для каждой роли (только если соответствующий фильтр включен)
    addStatSection("Подлежащие", stats.podlezhascheeSentences, "stat-pod", ui->c_pod->isChecked());
    addStatSection("Сказуемые", stats.skazuemoeSentences, "stat-skaz", ui->c_skaz->isChecked());
    addStatSection("Определения", stats.opredelenieSentences, "stat-opred", ui->c_opred->isChecked());
    addStatSection("Дополнения", stats.dopolnenieSentences, "stat-dop", ui->c_dop->isChecked());
    addStatSection("Обстоятельства", stats.obstoyatelstvoSentences, "stat-ob", ui->c_ob->isChecked());
    addStatSection("Другое", stats.drugoeSentences, "stat-drugoe", ui->c_none->isChecked());
    out << "</div>";

    // Вкладка 3: Частотность слов
    out << "<div id='WordFreq' class='tab-content'>";
    out << "erte<table><thead><tr>"
        << "<th onclick=\"sortTable(0)\">Слово</th>"
        << "<th onclick=\"sortTable(1)\">Роль</th>"
        << "<th onclick=\"sortTable(2)\" style=\"width:120px; text-align:center;\">Количество</th>"
        << "<tr></thead><tbody>";
    
    // Получаем все уникальные слова и сортируем их
    QStringList words = wordRoleStats.keys();
    std::sort(words.begin(), words.end());
    
    // Для каждого слова выводим все его роли с количеством
    for (const QString& w : words) {
        const QMap<QString, int>& roles = wordRoleStats[w];
        QStringList roleKeys = roles.keys();
        std::sort(roleKeys.begin(), roleKeys.end());  // Сортируем роли для консистентности
        
        for (const QString& role : roleKeys) {
            out << "<tr>"
                << "<td>" << w.toHtmlEscaped() << "</td>"  
                << "<td>" << role << "</td>"               
                << "<td style=\"text-align:center; font-weight:bold;\">" << roles[role] << "</td>"  // Количество
                << "</tr>";
        }
    }
    
    out << "</tbody><table></div>";
    out << "</div></body></html>";
    
    file.close();
    
    // Автоматически открываем созданный отчет в браузере по умолчанию
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}
