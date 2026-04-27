/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *topLayout;
    QFrame *inputFrame;
    QVBoxLayout *inputLayout;
    QTextEdit *textEdit;
    QHBoxLayout *buttonLayout;
    QPushButton *btn_search;
    QPushButton *btn_upload;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_download;
    QFrame *filterFrame;
    QVBoxLayout *filterLayout;
    QLabel *filterLabel;
    QCheckBox *c_pod;
    QCheckBox *c_skaz;
    QCheckBox *c_opred;
    QCheckBox *c_dop;
    QCheckBox *c_ob;
    QCheckBox *c_none;
    QSpacerItem *verticalSpacer;
    QCheckBox *c_all;
    QTabWidget *tabWidget;
    QWidget *tab_analysis;
    QVBoxLayout *verticalLayout_2;
    QStackedWidget *stackedResults;
    QWidget *placeholderPage;
    QVBoxLayout *placeholderLayout;
    QWidget *placeholderContainer;
    QVBoxLayout *placeholderContainerLayout;
    QLabel *placeholderLabel;
    QWidget *resultsPage;
    QVBoxLayout *resultsLayout;
    QTreeWidget *treeWidget;
    QWidget *tab_statistics;
    QVBoxLayout *verticalLayout_3;
    QWidget *statisticsContainer;
    QVBoxLayout *statisticsContainerLayout;
    QWidget *tab_word_frequency;
    QVBoxLayout *verticalLayout_4;
    QStackedWidget *stackedWordFreq;
    QWidget *wordFreqPlaceholderPage;
    QVBoxLayout *wordFreqPlaceholderLayout;
    QWidget *wordFreqPlaceholderContainer;
    QVBoxLayout *wordFreqPlaceholderContainerLayout;
    QLabel *wordFreqPlaceholderLabel;
    QWidget *wordFreqResultsPage;
    QVBoxLayout *wordFreqResultsLayout;
    QTableWidget *tableWordStats;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(997, 803);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"    /* Force light background - override dark mode */\n"
"    QMainWindow, QWidget#centralwidget {\n"
"        background-color: #e8ecf1;\n"
"    }\n"
"    \n"
"    /* Input frame */\n"
"    QFrame#inputFrame {\n"
"        background-color: #ffffff;\n"
"        border: 1px solid #d0d0d0;\n"
"        border-radius: 8px;\n"
"    }\n"
"    \n"
"    /* Text edit styling - remove extra scrollbar */\n"
"    QTextEdit {\n"
"        border: 1px solid #d0d0d0;\n"
"        border-radius: 6px;\n"
"        padding: 6px;\n"
"        font-size: 12px;\n"
"        background-color: #fafbfc;\n"
"        color: #2c3e50;\n"
"    }\n"
"    QTextEdit:focus {\n"
"        border-color: #3498db;\n"
"        background-color: #ffffff;\n"
"        color: #2c3e50;\n"
"    }\n"
"    \n"
"    /* Button styling */\n"
"    QPushButton {\n"
"        border-radius: 5px;\n"
"        padding: 4px 10px;\n"
"    }\n"
"    \n"
"    QPushButton#btn_search {\n"
"        background-color: #3498db;\n"
"        color: #ffffff;\n"
"        border: "
                        "none;\n"
"        border-radius: 6px;\n"
"        font-size: 12px;\n"
"        font-weight: 500;\n"
"        padding: 5px 14px;\n"
"    }\n"
"    QPushButton#btn_search:hover {\n"
"        background-color: #2980b9;\n"
"    }\n"
"    QPushButton#btn_search:pressed {\n"
"        background-color: #1f618d;\n"
"    }\n"
"    QPushButton#btn_search:disabled {\n"
"        background-color: #bdc3c7;\n"
"    }\n"
"    \n"
"    QPushButton#btn_upload {\n"
"        background-color: #ecf0f1;\n"
"        color: #2c3e50;\n"
"        border: 1px solid #bdc3c7;\n"
"        border-radius: 6px;\n"
"        font-size: 12px;\n"
"        padding: 5px 14px;\n"
"    }\n"
"    QPushButton#btn_upload:hover {\n"
"        background-color: #dfe6e9;\n"
"    }\n"
"    QPushButton#btn_upload:pressed {\n"
"        background-color: #bdc3c7;\n"
"    }\n"
"    \n"
"    QPushButton#btn_download {\n"
"        background-color: #27ae60;\n"
"        color: #ffffff;\n"
"        border: none;\n"
"        border-radius: 6px;\n"
"        font-size"
                        ": 12px;\n"
"        padding: 5px 14px;\n"
"    }\n"
"    QPushButton#btn_download:disabled {\n"
"        background-color: #bdc3c7;\n"
"    }\n"
"    QPushButton#btn_download:hover:enabled {\n"
"        background-color: #219a52;\n"
"    }\n"
"    \n"
"    /* Filter frame */\n"
"    QFrame#filterFrame {\n"
"        background-color: #ffffff;\n"
"        border: 1px solid #d0d0d0;\n"
"        border-radius: 8px;\n"
"    }\n"
"    \n"
"    QLabel#filterLabel {\n"
"        color: #2c3e50;\n"
"        font-size: 12px;\n"
"        font-weight: bold;\n"
"    }\n"
"    \n"
"    /* Checkbox styling */\n"
"    QCheckBox {\n"
"        spacing: 6px;\n"
"        font-size: 11px;\n"
"        color: #2c3e50;\n"
"        background-color: transparent;\n"
"    }\n"
"    QCheckBox::indicator {\n"
"        width: 14px;\n"
"        height: 14px;\n"
"        border-radius: 3px;\n"
"        border: 1px solid #bdc3c7;\n"
"        background-color: #ffffff;\n"
"    }\n"
"    QCheckBox::indicator:checked {\n"
"        background-colo"
                        "r: #3498db;\n"
"        border-color: #3498db;\n"
"    }\n"
"    QCheckBox::indicator:hover {\n"
"        border-color: #3498db;\n"
"    }\n"
"    \n"
"    QCheckBox#c_all {\n"
"        font-weight: bold;\n"
"        color: #3498db;\n"
"    }\n"
"    QCheckBox#c_all::indicator {\n"
"        border-color: #3498db;\n"
"    }\n"
"    \n"
"    /* Tab widget styling - UNIFIED */\n"
"    QTabWidget::pane {\n"
"        background-color: #ffffff;\n"
"        border: 1px solid #d0d0d0;\n"
"        border-radius: 8px;\n"
"    }\n"
"    QTabBar::tab {\n"
"        background-color: #ecf0f1;\n"
"        padding: 8px 20px;\n"
"        margin-right: 2px;\n"
"        border-top-left-radius: 6px;\n"
"        border-top-right-radius: 6px;\n"
"        font-size: 12px;\n"
"        font-weight: 500;\n"
"        color: #2c3e50;\n"
"        min-width: 120px;\n"
"    }\n"
"    QTabBar::tab:selected {\n"
"        background-color: #ffffff;\n"
"        color: #3498db;\n"
"        border-bottom: 2px solid #3498db;\n"
"    }\n"
"    QTab"
                        "Bar::tab:hover:!selected {\n"
"        background-color: #ecf0f1;\n"
"    }\n"
"    \n"
"    /* UNIFIED Tree and Table Widget Styling - NO HIGHLIGHTING */\n"
"    QTreeWidget, QTableWidget {\n"
"        background-color: #ffffff;\n"
"        color: #495057;\n"
"        border: none;\n"
"        font-size: 12px;\n"
"        outline: 0;\n"
"        alternate-background-color: #f8f9fa;\n"
"        selection-background-color: transparent;\n"
"        selection-color: #495057;\n"
"    }\n"
"    QTreeWidget::item, QTableWidget::item {\n"
"        padding: 8px;\n"
"        border-bottom: 1px solid #ecf0f1;\n"
"        color: #495057;\n"
"    }\n"
"    \n"
"    /* \320\236\321\202\320\272\320\273\321\216\321\207\320\260\320\265\320\274 \320\262\321\213\320\264\320\265\320\273\320\265\320\275\320\270\320\265 \320\270 \320\270\320\267\320\274\320\265\320\275\320\265\320\275\320\270\320\265 \321\206\320\262\320\265\321\202\320\260 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320"
                        "\270 */\n"
"    QTreeWidget::item:hover,\n"
"    QTableWidget::item:hover {\n"
"        background-color: transparent;\n"
"    }\n"
"    \n"
"    QTreeWidget::item:selected,\n"
"    QTableWidget::item:selected {\n"
"        background-color: transparent;\n"
"        color: #495057;\n"
"    }\n"
"    \n"
"    QTreeWidget::item:selected:hover,\n"
"    QTableWidget::item:selected:hover {\n"
"        background-color: transparent;\n"
"    }\n"
"    \n"
"    QTreeWidget::item:focus,\n"
"    QTableWidget::item:focus {\n"
"        outline: none;\n"
"        background-color: transparent;\n"
"        color: #495057;\n"
"    }\n"
"    \n"
"    QTableWidget::item:hover:!selected,\n"
"    QTreeWidget::item:hover:!selected {\n"
"        background-color: transparent;\n"
"    }\n"
"    \n"
"    QTreeWidget::branch:hover {\n"
"        color: #2980b9;\n"
"    }\n"
"    \n"
"    QTreeWidget {\n"
"        selection-background-color: transparent;\n"
"        selection-color: #495057;\n"
"    }\n"
"    \n"
"    QTableWidget {\n"
""
                        "        selection-background-color: transparent;\n"
"        selection-color: #495057;\n"
"    }\n"
"    \n"
"    /* UNIFIED Header Styling */\n"
"    QHeaderView::section {\n"
"        background-color: #f8fafc;\n"
"        padding: 10px;\n"
"        border: none;\n"
"        border-bottom: 2px solid #3498db;\n"
"        font-weight: bold;\n"
"        font-size: 12px;\n"
"        color: #2c3e50;\n"
"    }\n"
"    \n"
"    QHeaderView::section:hover {\n"
"        background-color: #f8fafc;\n"
"    }\n"
"    \n"
"    QHeaderView::section:pressed {\n"
"        background-color: #f8fafc;\n"
"    }\n"
"    \n"
"    QTableCornerButton::section {\n"
"        background-color: #f8fafc;\n"
"        border: none;\n"
"    }\n"
"    \n"
"    /* Statistics container */\n"
"    QWidget#statisticsContainer {\n"
"        background-color: #ffffff;\n"
"        min-height: 520px;\n"
"        max-height: 520px;\n"
"    }\n"
"    \n"
"    /* Statistics buttons row */\n"
"    QWidget#statButtonsWidget {\n"
"        background-col"
                        "or: #f8f9fa;\n"
"        border-bottom: 1px solid #dee2e6;\n"
"        padding: 8px;\n"
"    }\n"
"    \n"
"    QPushButton#statButton {\n"
"        background-color: #e9ecef;\n"
"        border: 1px solid #dee2e6;\n"
"        border-radius: 6px;\n"
"        padding: 8px 12px;\n"
"        font-size: 12px;\n"
"        font-weight: bold;\n"
"        color: #495057;\n"
"        min-width: 100px;\n"
"    }\n"
"    QPushButton#statButton:hover {\n"
"        background-color: #dee2e6;\n"
"    }\n"
"    QPushButton#statButton[expanded=\"true\"] {\n"
"        background-color: #3498db;\n"
"        color: white;\n"
"        border-color: #3498db;\n"
"    }\n"
"    \n"
"    /* Expandable content area */\n"
"    QScrollArea#contentScrollArea {\n"
"        border: none;\n"
"        background-color: white;\n"
"    }\n"
"    QScrollArea#contentScrollArea QScrollBar:vertical {\n"
"        border: none;\n"
"        background: #f0f0f0;\n"
"        width: 8px;\n"
"        border-radius: 4px;\n"
"    }\n"
"    QScrollArea#cont"
                        "entScrollArea QScrollBar::handle:vertical {\n"
"        background: #c0c0c0;\n"
"        border-radius: 4px;\n"
"        min-height: 30px;\n"
"    }\n"
"    QScrollArea#contentScrollArea QScrollBar::handle:vertical:hover {\n"
"        background: #3498db;\n"
"    }\n"
"    QScrollArea#contentScrollArea QScrollBar::handle:vertical:pressed {\n"
"        background: #2980b9;\n"
"    }\n"
"    \n"
"    QLabel#sentenceLabel {\n"
"        color: #2c3e50;\n"
"        font-size: 12px;\n"
"        padding: 6px 8px;\n"
"        background-color: #f8f9fa;\n"
"        border-radius: 4px;\n"
"        margin: 2px 0;\n"
"    }\n"
"    QLabel#sentenceLabel:hover {\n"
"        background-color: #e9ecef;\n"
"    }\n"
"    \n"
"    QLabel#noDataLabel {\n"
"        color: #6c757d;\n"
"        font-style: italic;\n"
"        padding: 20px;\n"
"        text-align: center;\n"
"    }\n"
"    \n"
"    /* UNIFIED Placeholder styling */\n"
"    QLabel#placeholderLabel, QLabel#wordFreqPlaceholderLabel {\n"
"        color: #6c757d;\n"
"  "
                        "      font-size: 14px;\n"
"        font-style: italic;\n"
"        padding: 40px;\n"
"        text-align: center;\n"
"    }\n"
"    \n"
"    /* Scrollbar styling - prevent black color on press */\n"
"    QScrollBar:vertical {\n"
"        border: none;\n"
"        background: #f0f0f0;\n"
"        width: 8px;\n"
"        border-radius: 4px;\n"
"        margin: 0px;\n"
"    }\n"
"    QScrollBar::handle:vertical {\n"
"        background: #c0c0c0;\n"
"        min-height: 30px;\n"
"        border-radius: 4px;\n"
"    }\n"
"    QScrollBar::handle:vertical:hover {\n"
"        background: #3498db;\n"
"    }\n"
"    QScrollBar::handle:vertical:pressed {\n"
"        background: #2980b9;\n"
"    }\n"
"    QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {\n"
"        height: 0px;\n"
"    }\n"
"    QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\n"
"        background: none;\n"
"    }\n"
"    \n"
"    /* Horizontal scrollbar styling */\n"
"    QScrollBar:horizontal {\n"
"        border: none;\n"
""
                        "        background: #f0f0f0;\n"
"        height: 8px;\n"
"        border-radius: 4px;\n"
"        margin: 0px;\n"
"    }\n"
"    QScrollBar::handle:horizontal {\n"
"        background: #c0c0c0;\n"
"        min-width: 30px;\n"
"        border-radius: 4px;\n"
"    }\n"
"    QScrollBar::handle:horizontal:hover {\n"
"        background: #3498db;\n"
"    }\n"
"    QScrollBar::handle:horizontal:pressed {\n"
"        background: #2980b9;\n"
"    }\n"
"    QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {\n"
"        width: 0px;\n"
"    }\n"
"    QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {\n"
"        background: none;\n"
"    }\n"
"    \n"
"    /* Status bar styling */\n"
"    QStatusBar {\n"
"        background-color: #2c3e50;\n"
"        color: #ffffff;\n"
"        padding: 3px 6px;\n"
"        font-size: 11px;\n"
"    }\n"
"    \n"
"    /* QTextBrowser styling for statistics details */\n"
"    QTextBrowser {\n"
"        background-color: transparent;\n"
"        border: none"
                        ";\n"
"        font-size: 12px;\n"
"        font-family: 'Segoe UI', Arial, sans-serif;\n"
"        color: #495057;\n"
"    }\n"
"   "));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QVBoxLayout(centralwidget);
        mainLayout->setSpacing(8);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(10, 10, 10, 10);
        topLayout = new QHBoxLayout();
        topLayout->setSpacing(10);
        topLayout->setObjectName("topLayout");
        inputFrame = new QFrame(centralwidget);
        inputFrame->setObjectName("inputFrame");
        inputLayout = new QVBoxLayout(inputFrame);
        inputLayout->setSpacing(8);
        inputLayout->setObjectName("inputLayout");
        inputLayout->setContentsMargins(8, 8, 8, 8);
        textEdit = new QTextEdit(inputFrame);
        textEdit->setObjectName("textEdit");
        textEdit->setMinimumSize(QSize(0, 100));
        textEdit->setMaximumSize(QSize(16777215, 16777215));
        textEdit->setLineWrapMode(QTextEdit::LineWrapMode::WidgetWidth);

        inputLayout->addWidget(textEdit);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(8);
        buttonLayout->setObjectName("buttonLayout");
        btn_search = new QPushButton(inputFrame);
        btn_search->setObjectName("btn_search");
        btn_search->setMinimumSize(QSize(80, 28));

        buttonLayout->addWidget(btn_search);

        btn_upload = new QPushButton(inputFrame);
        btn_upload->setObjectName("btn_upload");
        btn_upload->setMinimumSize(QSize(80, 28));

        buttonLayout->addWidget(btn_upload);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        btn_download = new QPushButton(inputFrame);
        btn_download->setObjectName("btn_download");
        btn_download->setMinimumSize(QSize(80, 28));

        buttonLayout->addWidget(btn_download);


        inputLayout->addLayout(buttonLayout);


        topLayout->addWidget(inputFrame);

        filterFrame = new QFrame(centralwidget);
        filterFrame->setObjectName("filterFrame");
        filterLayout = new QVBoxLayout(filterFrame);
        filterLayout->setSpacing(5);
        filterLayout->setObjectName("filterLayout");
        filterLayout->setContentsMargins(8, 8, 8, 8);
        filterLabel = new QLabel(filterFrame);
        filterLabel->setObjectName("filterLabel");
        filterLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        filterLayout->addWidget(filterLabel);

        c_pod = new QCheckBox(filterFrame);
        c_pod->setObjectName("c_pod");
        c_pod->setChecked(true);

        filterLayout->addWidget(c_pod);

        c_skaz = new QCheckBox(filterFrame);
        c_skaz->setObjectName("c_skaz");
        c_skaz->setChecked(true);

        filterLayout->addWidget(c_skaz);

        c_opred = new QCheckBox(filterFrame);
        c_opred->setObjectName("c_opred");
        c_opred->setChecked(true);

        filterLayout->addWidget(c_opred);

        c_dop = new QCheckBox(filterFrame);
        c_dop->setObjectName("c_dop");
        c_dop->setChecked(true);

        filterLayout->addWidget(c_dop);

        c_ob = new QCheckBox(filterFrame);
        c_ob->setObjectName("c_ob");
        c_ob->setChecked(true);

        filterLayout->addWidget(c_ob);

        c_none = new QCheckBox(filterFrame);
        c_none->setObjectName("c_none");
        c_none->setChecked(true);

        filterLayout->addWidget(c_none);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        filterLayout->addItem(verticalSpacer);

        c_all = new QCheckBox(filterFrame);
        c_all->setObjectName("c_all");
        c_all->setChecked(true);

        filterLayout->addWidget(c_all);


        topLayout->addWidget(filterFrame);


        mainLayout->addLayout(topLayout);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setStyleSheet(QString::fromUtf8(""));
        tabWidget->setMovable(false);
        tab_analysis = new QWidget();
        tab_analysis->setObjectName("tab_analysis");
        verticalLayout_2 = new QVBoxLayout(tab_analysis);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        stackedResults = new QStackedWidget(tab_analysis);
        stackedResults->setObjectName("stackedResults");
        placeholderPage = new QWidget();
        placeholderPage->setObjectName("placeholderPage");
        placeholderLayout = new QVBoxLayout(placeholderPage);
        placeholderLayout->setObjectName("placeholderLayout");
        placeholderLayout->setContentsMargins(0, 0, 0, 0);
        placeholderContainer = new QWidget(placeholderPage);
        placeholderContainer->setObjectName("placeholderContainer");
        placeholderContainerLayout = new QVBoxLayout(placeholderContainer);
        placeholderContainerLayout->setObjectName("placeholderContainerLayout");
        placeholderContainerLayout->setContentsMargins(0, 0, 0, 0);
        placeholderLabel = new QLabel(placeholderContainer);
        placeholderLabel->setObjectName("placeholderLabel");
        placeholderLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        placeholderContainerLayout->addWidget(placeholderLabel);


        placeholderLayout->addWidget(placeholderContainer);

        stackedResults->addWidget(placeholderPage);
        resultsPage = new QWidget();
        resultsPage->setObjectName("resultsPage");
        resultsLayout = new QVBoxLayout(resultsPage);
        resultsLayout->setObjectName("resultsLayout");
        resultsLayout->setContentsMargins(0, 0, 0, 0);
        treeWidget = new QTreeWidget(resultsPage);
        treeWidget->setObjectName("treeWidget");
        treeWidget->setAlternatingRowColors(true);
        treeWidget->setIndentation(20);
        treeWidget->setHeaderHidden(true);
        treeWidget->setExpandsOnDoubleClick(true);
        treeWidget->header()->setVisible(false);

        resultsLayout->addWidget(treeWidget);

        stackedResults->addWidget(resultsPage);

        verticalLayout_2->addWidget(stackedResults);

        tabWidget->addTab(tab_analysis, QString());
        tab_statistics = new QWidget();
        tab_statistics->setObjectName("tab_statistics");
        verticalLayout_3 = new QVBoxLayout(tab_statistics);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        statisticsContainer = new QWidget(tab_statistics);
        statisticsContainer->setObjectName("statisticsContainer");
        statisticsContainer->setMinimumSize(QSize(0, 520));
        statisticsContainer->setMaximumSize(QSize(16777215, 520));
        statisticsContainerLayout = new QVBoxLayout(statisticsContainer);
        statisticsContainerLayout->setSpacing(0);
        statisticsContainerLayout->setObjectName("statisticsContainerLayout");
        statisticsContainerLayout->setContentsMargins(0, 0, 0, 0);

        verticalLayout_3->addWidget(statisticsContainer);

        tabWidget->addTab(tab_statistics, QString());
        tab_word_frequency = new QWidget();
        tab_word_frequency->setObjectName("tab_word_frequency");
        verticalLayout_4 = new QVBoxLayout(tab_word_frequency);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        stackedWordFreq = new QStackedWidget(tab_word_frequency);
        stackedWordFreq->setObjectName("stackedWordFreq");
        wordFreqPlaceholderPage = new QWidget();
        wordFreqPlaceholderPage->setObjectName("wordFreqPlaceholderPage");
        wordFreqPlaceholderLayout = new QVBoxLayout(wordFreqPlaceholderPage);
        wordFreqPlaceholderLayout->setObjectName("wordFreqPlaceholderLayout");
        wordFreqPlaceholderLayout->setContentsMargins(0, 0, 0, 0);
        wordFreqPlaceholderContainer = new QWidget(wordFreqPlaceholderPage);
        wordFreqPlaceholderContainer->setObjectName("wordFreqPlaceholderContainer");
        wordFreqPlaceholderContainerLayout = new QVBoxLayout(wordFreqPlaceholderContainer);
        wordFreqPlaceholderContainerLayout->setObjectName("wordFreqPlaceholderContainerLayout");
        wordFreqPlaceholderContainerLayout->setContentsMargins(0, 0, 0, 0);
        wordFreqPlaceholderLabel = new QLabel(wordFreqPlaceholderContainer);
        wordFreqPlaceholderLabel->setObjectName("wordFreqPlaceholderLabel");
        wordFreqPlaceholderLabel->setEnabled(false);
        wordFreqPlaceholderLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        wordFreqPlaceholderContainerLayout->addWidget(wordFreqPlaceholderLabel);


        wordFreqPlaceholderLayout->addWidget(wordFreqPlaceholderContainer);

        stackedWordFreq->addWidget(wordFreqPlaceholderPage);
        wordFreqResultsPage = new QWidget();
        wordFreqResultsPage->setObjectName("wordFreqResultsPage");
        wordFreqResultsLayout = new QVBoxLayout(wordFreqResultsPage);
        wordFreqResultsLayout->setObjectName("wordFreqResultsLayout");
        wordFreqResultsLayout->setContentsMargins(0, 0, 0, 0);
        tableWordStats = new QTableWidget(wordFreqResultsPage);
        if (tableWordStats->columnCount() < 3)
            tableWordStats->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWordStats->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWordStats->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWordStats->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWordStats->setObjectName("tableWordStats");
        tableWordStats->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableWordStats->setAlternatingRowColors(true);
        tableWordStats->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
        tableWordStats->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tableWordStats->horizontalHeader()->setVisible(true);
        tableWordStats->horizontalHeader()->setStretchLastSection(true);
        tableWordStats->verticalHeader()->setVisible(false);

        wordFreqResultsLayout->addWidget(tableWordStats);

        stackedWordFreq->addWidget(wordFreqResultsPage);

        verticalLayout_4->addWidget(stackedWordFreq);

        tabWidget->addTab(tab_word_frequency, QString());

        mainLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        stackedResults->setCurrentIndex(1);
        stackedWordFreq->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\320\241\320\270\320\275\321\202\320\260\320\272\321\201\320\270\321\207\320\265\321\201\320\272\320\270\320\271 \320\260\320\275\320\260\320\273\320\270\320\267\320\260\321\202\320\276\321\200", nullptr));
        textEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \321\202\320\265\320\272\321\201\321\202 \320\264\320\273\321\217 \320\260\320\275\320\260\320\273\320\270\320\267\320\260...", nullptr));
        btn_search->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\270\321\201\320\272", nullptr));
        btn_upload->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214", nullptr));
        btn_download->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        filterLabel->setText(QCoreApplication::translate("MainWindow", "\320\244\320\270\320\273\321\214\321\202\321\200", nullptr));
        c_pod->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\264\320\273\320\265\320\266\320\260\321\211\320\265\320\265", nullptr));
        c_skaz->setText(QCoreApplication::translate("MainWindow", "\320\241\320\272\320\260\320\267\321\203\320\265\320\274\320\276\320\265", nullptr));
        c_opred->setText(QCoreApplication::translate("MainWindow", "\320\236\320\277\321\200\320\265\320\264\320\265\320\273\320\265\320\275\320\270\320\265", nullptr));
        c_dop->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\277\320\276\320\273\320\275\320\265\320\275\320\270\320\265", nullptr));
        c_ob->setText(QCoreApplication::translate("MainWindow", "\320\236\320\261\321\201\321\202\320\276\321\217\321\202\320\265\320\273\321\214\321\201\321\202\320\262\320\276", nullptr));
        c_none->setText(QCoreApplication::translate("MainWindow", "\320\224\321\200\321\203\320\263\320\276\320\265", nullptr));
        c_all->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\261\321\200\320\260\321\202\321\214 \320\262\321\201\320\265", nullptr));
        placeholderLabel->setText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \321\202\320\265\320\272\321\201\321\202 \320\264\320\273\321\217 \320\260\320\275\320\260\320\273\320\270\320\267\320\260", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MainWindow", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202\321\213 \321\201\320\270\320\275\321\202\320\260\320\272\321\201\320\270\321\207\320\265\321\201\320\272\320\276\320\263\320\276 \320\260\320\275\320\260\320\273\320\270\320\267\320\260", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_analysis), QCoreApplication::translate("MainWindow", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202\321\213 \320\260\320\275\320\260\320\273\320\270\320\267\320\260", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_statistics), QCoreApplication::translate("MainWindow", "\320\241\321\202\320\260\321\202\320\270\321\201\321\202\320\270\320\272\320\260", nullptr));
        wordFreqPlaceholderLabel->setText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \321\202\320\265\320\272\321\201\321\202 \320\264\320\273\321\217 \320\260\320\275\320\260\320\273\320\270\320\267\320\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWordStats->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "\320\241\320\273\320\276\320\262\320\276", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWordStats->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "\320\240\320\276\320\273\321\214", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWordStats->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_word_frequency), QCoreApplication::translate("MainWindow", "\320\247\320\260\321\201\321\202\320\276\321\202\320\275\320\276\321\201\321\202\321\214 \321\201\320\273\320\276\320\262", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
