#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::showPlaceholderInResults()
{
    ui->stackedResults->setCurrentIndex(0);

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

void MainWindow::showPlaceholderStatistics()
{
    QLayoutItem* child;
    while ((child = ui->statisticsContainerLayout->takeAt(0)) != nullptr) {
        if (child->widget()) delete child->widget();
        delete child;
    }

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
    ui->stackedWordFreq->setCurrentIndex(0);
    ui->wordFreqPlaceholderLabel->setText("Введите текст для анализа");
}

void MainWindow::showWordFreqResults()
{
    ui->stackedWordFreq->setCurrentIndex(1);
}

void MainWindow::updateDisplay()
{
    if (sentenceTexts.isEmpty() || wordsBySentence.isEmpty()) {
        showPlaceholderInResults();
        return;
    }

    ui->stackedResults->setCurrentIndex(1);
    ui->treeWidget->clear();
    ui->treeWidget->setRootIsDecorated(true);
    ui->treeWidget->setExpandsOnDoubleClick(true);

    QList<int> sentNumbers = sentenceTexts.keys();
    std::sort(sentNumbers.begin(), sentNumbers.end());

    for (int idx = 0; idx < sentNumbers.size(); ++idx) {
        int sentNum = sentNumbers[idx];
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

            for (int i = 0; i < words.size(); ++i) {
                const WordInfo& word = words[i];
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

void MainWindow::updateWordRoleDisplay()
{
    QTableWidget *table = ui->tableWordStats;
    if (!table) return;

    table->setSortingEnabled(false);
    table->setRowCount(0);

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

    if (table->parentWidget()) {
        QLabel* noFilterLabel = table->parentWidget()->findChild<QLabel*>("noFilterWordFreqLabel");
        if (noFilterLabel) noFilterLabel->hide();
    }
    table->show();

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

        setupTableHeader(table);
        table->setSortingEnabled(true);
        return;
    }

    int row = 0;
    QStringList words = wordRoleStats.keys();
    std::sort(words.begin(), words.end());
    for (int i = 0; i < words.size(); ++i) {
        const QString& w = words[i];
        const QMap<QString, int>& roles = wordRoleStats[w];
        QStringList roleKeys = roles.keys();
        std::sort(roleKeys.begin(), roleKeys.end());
        for (int j = 0; j < roleKeys.size(); ++j) {
            const QString& role = roleKeys[j];
            table->insertRow(row);
            table->setItem(row, 0, new QTableWidgetItem(w));
            table->setItem(row, 1, new QTableWidgetItem(role));

            QTableWidgetItem *countItem = new QTableWidgetItem();
            countItem->setData(Qt::DisplayRole, roles[role]);
            table->setItem(row, 2, countItem);

            table->item(row, 2)->setTextAlignment(Qt::AlignCenter);
            row++;
        }
    }

    setupTableHeader(table);
    table->setSortingEnabled(true);

    table->sortByColumn(0, Qt::AscendingOrder);
}

void MainWindow::setupTableHeader(QTableWidget* table)
{
    if (!table) return;
    QHeaderView* header = table->horizontalHeader();

    header->setSortIndicatorShown(false);
    header->setSectionsClickable(true);
    header->setHighlightSections(false);

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
        "QHeaderView::up-arrow, QHeaderView::down-arrow {"
        "    image: none !important;"
        "    width: 0px;"
        "    height: 0px;"
        "    margin: 0px;"
        "}"
        "QTableWidget::item { padding: 8px; border-bottom: 1px solid #ecf0f1; color: #495057; }"
        "QTableWidget::item:selected { background-color: #e8f4fd; color: #2c3e50; }"
        );

    connect(header, &QHeaderView::sortIndicatorChanged, table, [table](int logicalIndex, Qt::SortOrder order) {
        QString arrow = (order == Qt::AscendingOrder) ? " ▲" : " ▼";
        QStringList names = {"Слово", "Роль", "Количество"};

        for (int col = 0; col < 3; ++col) {
            QTableWidgetItem* item = table->horizontalHeaderItem(col);
            if (!item) continue;

            if (col == logicalIndex) {
                item->setText(names[col] + arrow);
                item->setForeground(QBrush(QColor("#3498db")));
                QFont f = item->font(); f.setBold(true); item->setFont(f);
            } else {
                item->setText(names[col]);
                item->setForeground(QBrush(QColor("#2c3e50")));
                QFont f = item->font(); f.setBold(true); item->setFont(f);
            }
        }
    });

    table->setSortingEnabled(true);
    table->sortByColumn(0, Qt::AscendingOrder);
}

void MainWindow::updateStatisticsDisplayWithFilter()
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
        bool enabled;
    };

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

    for (int i = 0; i < statList.size(); ++i) {
        const StatData& stat = statList[i];
        if (!stat.enabled) continue;

        QPushButton* button = new QPushButton(QString("%1 (%2)").arg(stat.title, QString::number(stat.count)));
        button->setObjectName("statButton");
        button->setFocusPolicy(Qt::NoFocus);
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
            htmlContent = "<p style='color: #6c757d; font-style: italic; text-align: center; padding: 20px;'>"
                          "Нет данных для отображения с текущими фильтрами</p>";
        } else {
            htmlContent = "<div style='display: flex; flex-direction: column; gap: 8px;'>";
            QList<int> keys = stat.data->keys();
            for (int j = 0; j < keys.size(); ++j) {
                int key = keys[j];
                QStringList values = stat.data->value(key);
                QString wordsText = values.join(", ");
                if (wordsText.length() > 200) {
                    wordsText = wordsText.left(197) + "...";
                }
                htmlContent += QString(
                                   "<div style='background: #f8f9fa; padding: 8px 12px; border-radius: 6px; border-left: 3px solid #3498db;'>"
                                   "<span style='font-weight: bold; color: #2c3e50;'>Предложение %1:</span> "
                                   "<span style='color: #495057;'>%2</span>"
                                   "</div>"
                                   ).arg(key).arg(wordsText);
            }
            htmlContent += "</div>";
        }

        textBrowser->setHtml(htmlContent);
        pageLayout->addWidget(textBrowser);
        stackedContent->addWidget(statPage);
    }

    buttonsLayout->addStretch();

    for (int i = 0; i < buttons.size(); ++i) {
        int index = i;
        connect(buttons[i], &QPushButton::clicked, [buttons, stackedContent, index]() {
            for (int j = 0; j < buttons.size(); ++j) {
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
    } else {
        QLabel* noDataLabel = new QLabel("Все категории отключены фильтрами.\nВключите хотя бы одну категорию в фильтре.");
        noDataLabel->setAlignment(Qt::AlignCenter);
        noDataLabel->setStyleSheet("color: #6c757d; padding: 40px; font-style: italic;");
        ui->statisticsContainerLayout->addWidget(noDataLabel);
    }
}


void MainWindow::refreshAllDisplay()
{
    if (sentenceTexts.isEmpty()) {
        showPlaceholderInResults();
        showPlaceholderStatistics();
        showPlaceholderWordFreq();
        updateButtonsState();
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
        updateButtonsState();
        return;
    }

    calculateStatisticsWithFilter();
    calculateWordRoleStats();
    updateDisplay();
    updateStatisticsDisplayWithFilter();
    updateWordRoleDisplay();
    showWordFreqResults();

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

    updateButtonsState();
}


void MainWindow::updateButtonsState()
{
    bool anyFilterSelected = isAnyFilterSelected();
    bool isAnalyzing = (pythonProcess && pythonProcess->state() == QProcess::Running);
    bool textMatches = (!analyzedText.isEmpty() && ui->textEdit->toPlainText() == analyzedText);

    if (isAnalyzing) {
        ui->btn_search->setEnabled(false);
    } else {
        ui->btn_search->setEnabled(anyFilterSelected);
    }

    if (sentenceTexts.isEmpty() || !anyFilterSelected || !textMatches) {
        ui->btn_download->setEnabled(false);
    } else {
        ui->btn_download->setEnabled(true);
    }

    ui->btn_upload->setEnabled(!isAnalyzing);
}

void MainWindow::calculateWordRoleStats()
{
    wordRoleStats.clear();

    QList<int> keys = wordsBySentence.keys();
    for (int i = 0; i < keys.size(); ++i) {
        int sentenceNum = keys[i];
        const QList<WordInfo>& words = wordsBySentence[sentenceNum];
        for (int j = 0; j < words.size(); ++j) {
            const WordInfo& word = words[j];
            if (word.speech == "PUNCT" || word.speech == "SPACE") continue;
            if (!shouldShowWord(word)) continue;

            QString w = word.text.toLower();
            wordRoleStats[w][word.sentence]++;
        }
    }
}


void MainWindow::calculateStatisticsWithFilter()
{
    stats = Statistics();

    QList<int> keys = wordsBySentence.keys();
    for (int i = 0; i < keys.size(); ++i) {
        int sentenceNum = keys[i];
        const QList<WordInfo>& words = wordsBySentence[sentenceNum];

        for (int j = 0; j < words.size(); ++j) {
            const WordInfo& word = words[j];

            if (!shouldShowWord(word)) {
                continue;
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
