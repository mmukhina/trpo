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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
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
    QTreeWidget *treeWidget;
    QWidget *tab_statistics;
    QVBoxLayout *verticalLayout_3;
    QScrollArea *statisticsScrollArea;
    QWidget *statisticsContainer;
    QVBoxLayout *statisticsContainerLayout;
    QGridLayout *statsGridLayout;
    QGroupBox *group_pod;
    QVBoxLayout *verticalLayout_pod;
    QLabel *label_pod_count;
    QTextBrowser *text_pod;
    QGroupBox *group_skaz;
    QVBoxLayout *verticalLayout_skaz;
    QLabel *label_skaz_count;
    QTextBrowser *text_skaz;
    QGroupBox *group_opred;
    QVBoxLayout *verticalLayout_opred;
    QLabel *label_opred_count;
    QTextBrowser *text_opred;
    QGroupBox *group_dop;
    QVBoxLayout *verticalLayout_dop;
    QLabel *label_dop_count;
    QTextBrowser *text_dop;
    QGroupBox *group_ob;
    QVBoxLayout *verticalLayout_ob;
    QLabel *label_ob_count;
    QTextBrowser *text_ob;
    QGroupBox *group_drugoe;
    QVBoxLayout *verticalLayout_drugoe;
    QLabel *label_drugoe_count;
    QTextBrowser *text_drugoe;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(960, 727);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"    /* Global background */\n"
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
"    /* Text edit styling */\n"
"    QTextEdit {\n"
"        border: 1px solid #d0d0d0;\n"
"        border-radius: 6px;\n"
"        padding: 10px;\n"
"        font-size: 13px;\n"
"        background-color: #fafbfc;\n"
"    }\n"
"    QTextEdit:focus {\n"
"        border-color: #3498db;\n"
"        background-color: #ffffff;\n"
"    }\n"
"    \n"
"    /* Button styling */\n"
"    QPushButton {\n"
"        border-radius: 5px;\n"
"        padding: 6px 12px;\n"
"    }\n"
"    \n"
"    QPushButton#btn_search {\n"
"        background-color: #3498db;\n"
"        color: #ffffff;\n"
"        border: none;\n"
"        border-radius: 6px;\n"
"        font-size: 13px;\n"
"        font-weight: 500;\n"
"     "
                        "   padding: 6px 16px;\n"
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
"        font-size: 13px;\n"
"        padding: 6px 16px;\n"
"    }\n"
"    QPushButton#btn_upload:hover {\n"
"        background-color: #dfe6e9;\n"
"    }\n"
"    QPushButton#btn_upload:pressed {\n"
"        background-color: #bdc3c7;\n"
"    }\n"
"    \n"
"    QPushButton#btn_download:enabled {\n"
"        background-color: #27ae60;\n"
"        color: #ffffff;\n"
"        border: none;\n"
"        border-radius: 6px;\n"
"        font-size: 13px;\n"
"        padding: 6px 16px;\n"
"    }\n"
"    QPushButton#btn_download:enabled:hover {\n"
""
                        "        background-color: #219a52;\n"
"    }\n"
"    QPushButton#btn_download:disabled {\n"
"        background-color: #bdc3c7;\n"
"        color: #95a5a6;\n"
"        border: none;\n"
"        border-radius: 6px;\n"
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
"        font-size: 13px;\n"
"        font-weight: bold;\n"
"    }\n"
"    \n"
"    QFrame#filterInnerFrame {\n"
"        background-color: #f8fafc;\n"
"        border-radius: 6px;\n"
"    }\n"
"    \n"
"    /* Checkbox styling */\n"
"    QCheckBox {\n"
"        spacing: 8px;\n"
"        font-size: 12px;\n"
"        color: #2c3e50;\n"
"        background-color: transparent;\n"
"    }\n"
"    QCheckBox::indicator {\n"
"        width: 16px;\n"
"        height: 16px;\n"
"        border-radius: 3px;\n"
"        border: 1px solid #bdc3c7;\n"
"   "
                        "     background-color: #ffffff;\n"
"    }\n"
"    QCheckBox::indicator:checked {\n"
"        background-color: #3498db;\n"
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
"    /* Tab widget styling */\n"
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
"    }\n"
"    QTabBar::tab:selected {\n"
"        background-color: #ffffff;\n"
"        color: #3498db;\n"
""
                        "        border-bottom: 2px solid #3498db;\n"
"    }\n"
"    QTabBar::tab:hover:!selected {\n"
"        background-color: #dfe6e9;\n"
"    }\n"
"    \n"
"    /* Tree widget styling */\n"
"    QTreeWidget {\n"
"        background-color: #ffffff;\n"
"        border: none;\n"
"        font-size: 13px;\n"
"        outline: 0;\n"
"    }\n"
"    QTreeWidget::item {\n"
"        padding: 6px;\n"
"        border-bottom: 1px solid #ecf0f1;\n"
"        background-color: #ffffff;\n"
"    }\n"
"    QTreeWidget::item:hover {\n"
"        background-color: #f0f4f8;\n"
"    }\n"
"    QTreeWidget::item:selected {\n"
"        background-color: #3498db;\n"
"        color: #ffffff;\n"
"    }\n"
"    QTreeWidget::item:selected:hover {\n"
"        background-color: #2980b9;\n"
"    }\n"
"    QHeaderView::section {\n"
"        background-color: #f8fafc;\n"
"        padding: 8px;\n"
"        border: none;\n"
"        border-bottom: 1px solid #d0d0d0;\n"
"        font-weight: 500;\n"
"        font-size: 12px;\n"
"        color: #2c3e50;"
                        "\n"
"    }\n"
"    \n"
"    /* Statistics container */\n"
"    QWidget#statisticsContainer {\n"
"        background-color: #ffffff;\n"
"    }\n"
"    \n"
"    /* Group box styling for statistics */\n"
"    QGroupBox {\n"
"        background-color: #f8fafc;\n"
"        border: 1px solid #d0d0d0;\n"
"        border-radius: 8px;\n"
"        margin-top: 8px;\n"
"        padding-top: 8px;\n"
"    }\n"
"    QGroupBox::title {\n"
"        subcontrol-origin: margin;\n"
"        left: 12px;\n"
"        padding: 0 6px;\n"
"        color: #2c3e50;\n"
"        font-weight: 500;\n"
"        font-size: 12px;\n"
"        background-color: #f8fafc;\n"
"    }\n"
"    \n"
"    /* Label styling for statistics counts */\n"
"    QLabel[objectName*=\"_count\"] {\n"
"        color: #3498db;\n"
"        font-size: 28px;\n"
"        font-weight: bold;\n"
"        background-color: transparent;\n"
"    }\n"
"    \n"
"    /* Text browser styling for statistics lists */\n"
"    QTextBrowser {\n"
"        border: 1px solid #e0e0e0;\n"
"  "
                        "      border-radius: 6px;\n"
"        background-color: #ffffff;\n"
"        font-size: 12px;\n"
"        padding: 8px;\n"
"    }\n"
"    \n"
"    /* Scroll area styling */\n"
"    QScrollArea#statisticsScrollArea {\n"
"        border: none;\n"
"        background-color: #ffffff;\n"
"    }\n"
"    \n"
"    /* Scrollbar styling */\n"
"    QScrollBar:vertical {\n"
"        border: none;\n"
"        background: #f0f0f0;\n"
"        width: 10px;\n"
"        border-radius: 5px;\n"
"        margin: 0px;\n"
"    }\n"
"    QScrollBar::handle:vertical {\n"
"        background: #c0c0c0;\n"
"        min-height: 30px;\n"
"        border-radius: 5px;\n"
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
"        back"
                        "ground: none;\n"
"    }\n"
"    \n"
"    QScrollBar:horizontal {\n"
"        border: none;\n"
"        background: #f0f0f0;\n"
"        height: 10px;\n"
"        border-radius: 5px;\n"
"        margin: 0px;\n"
"    }\n"
"    QScrollBar::handle:horizontal {\n"
"        background: #c0c0c0;\n"
"        min-width: 30px;\n"
"        border-radius: 5px;\n"
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
"    \n"
"    /* Specific scrollbar for tree widget */\n"
"    QTreeWidget QScrollBar:vertical {\n"
"        width: 8px;\n"
"        border-radius: 4px;\n"
"        background: #f0f0f0;\n"
"    }\n"
"    QTreeWidget QScrollBar::handle:vertical {\n"
"        border-radius: 4px;\n"
"        background: #c0c0c0;\n"
"    }\n"
"    QTreeWidget QScrollBar::handle:vertical:hov"
                        "er {\n"
"        background: #3498db;\n"
"    }\n"
"    \n"
"    /* Specific scrollbar for text edit */\n"
"    QTextEdit QScrollBar:vertical {\n"
"        width: 8px;\n"
"        border-radius: 4px;\n"
"        background: #f0f0f0;\n"
"    }\n"
"    QTextEdit QScrollBar::handle:vertical {\n"
"        border-radius: 4px;\n"
"        background: #c0c0c0;\n"
"    }\n"
"    QTextEdit QScrollBar::handle:vertical:hover {\n"
"        background: #3498db;\n"
"    }\n"
"    \n"
"    /* Specific scrollbar for text browser */\n"
"    QTextBrowser QScrollBar:vertical {\n"
"        width: 8px;\n"
"        border-radius: 4px;\n"
"        background: #f0f0f0;\n"
"    }\n"
"    QTextBrowser QScrollBar::handle:vertical {\n"
"        border-radius: 4px;\n"
"        background: #c0c0c0;\n"
"    }\n"
"    QTextBrowser QScrollBar::handle:vertical:hover {\n"
"        background: #3498db;\n"
"    }\n"
"    \n"
"    /* Status bar styling */\n"
"    QStatusBar {\n"
"        background-color: #2c3e50;\n"
"        color: #ffffff;\n"
" "
                        "       padding: 4px 8px;\n"
"        font-size: 11px;\n"
"    }\n"
"   "));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QVBoxLayout(centralwidget);
        mainLayout->setSpacing(12);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(12, 12, 12, 12);
        topLayout = new QHBoxLayout();
        topLayout->setSpacing(12);
        topLayout->setObjectName("topLayout");
        inputFrame = new QFrame(centralwidget);
        inputFrame->setObjectName("inputFrame");
        inputLayout = new QVBoxLayout(inputFrame);
        inputLayout->setSpacing(10);
        inputLayout->setObjectName("inputLayout");
        inputLayout->setContentsMargins(15, 15, 15, 15);
        textEdit = new QTextEdit(inputFrame);
        textEdit->setObjectName("textEdit");

        inputLayout->addWidget(textEdit);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(10);
        buttonLayout->setObjectName("buttonLayout");
        btn_search = new QPushButton(inputFrame);
        btn_search->setObjectName("btn_search");
        btn_search->setMinimumSize(QSize(100, 34));

        buttonLayout->addWidget(btn_search);

        btn_upload = new QPushButton(inputFrame);
        btn_upload->setObjectName("btn_upload");
        btn_upload->setMinimumSize(QSize(100, 34));

        buttonLayout->addWidget(btn_upload);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        btn_download = new QPushButton(inputFrame);
        btn_download->setObjectName("btn_download");
        btn_download->setMinimumSize(QSize(100, 34));

        buttonLayout->addWidget(btn_download);


        inputLayout->addLayout(buttonLayout);


        topLayout->addWidget(inputFrame);

        filterFrame = new QFrame(centralwidget);
        filterFrame->setObjectName("filterFrame");
        filterLayout = new QVBoxLayout(filterFrame);
        filterLayout->setSpacing(12);
        filterLayout->setObjectName("filterLayout");
        filterLayout->setContentsMargins(15, 15, 15, 15);
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

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        filterLayout->addItem(verticalSpacer);

        c_all = new QCheckBox(filterFrame);
        c_all->setObjectName("c_all");
        c_all->setChecked(true);

        filterLayout->addWidget(c_all);


        topLayout->addWidget(filterFrame);


        mainLayout->addLayout(topLayout);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setStyleSheet(QString::fromUtf8("\n"
"QTabWidget {\n"
"padding: 0;\n"
"margin:0;\n"
"  }"));
        tab_analysis = new QWidget();
        tab_analysis->setObjectName("tab_analysis");
        tab_analysis->setStyleSheet(QString::fromUtf8("\n"
"QWidget {\n"
"padding: 0;\n"
"margin:0;\n"
"  }"));
        verticalLayout_2 = new QVBoxLayout(tab_analysis);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(10, 10, 10, 10);
        treeWidget = new QTreeWidget(tab_analysis);
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setFont(0, font);
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName("treeWidget");
        treeWidget->setAlternatingRowColors(false);
        treeWidget->setIndentation(20);
        treeWidget->setExpandsOnDoubleClick(true);
        treeWidget->header()->setVisible(true);
        treeWidget->header()->setMinimumSectionSize(150);
        treeWidget->header()->setDefaultSectionSize(300);
        treeWidget->header()->setStretchLastSection(true);

        verticalLayout_2->addWidget(treeWidget);

        tabWidget->addTab(tab_analysis, QString());
        tab_statistics = new QWidget();
        tab_statistics->setObjectName("tab_statistics");
        tab_statistics->setAutoFillBackground(false);
        tab_statistics->setStyleSheet(QString::fromUtf8("\n"
"QWidget {\n"
"padding: 0;\n"
"margin:0;\n"
"  }"));
        verticalLayout_3 = new QVBoxLayout(tab_statistics);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(10, 10, 10, 10);
        statisticsScrollArea = new QScrollArea(tab_statistics);
        statisticsScrollArea->setObjectName("statisticsScrollArea");
        statisticsScrollArea->setStyleSheet(QString::fromUtf8("\n"
"QScrollArea {\n"
"padding: 0;\n"
"margin:0;\n"
"  }"));
        statisticsScrollArea->setWidgetResizable(true);
        statisticsContainer = new QWidget();
        statisticsContainer->setObjectName("statisticsContainer");
        statisticsContainer->setGeometry(QRect(0, 0, 904, 424));
        statisticsContainerLayout = new QVBoxLayout(statisticsContainer);
        statisticsContainerLayout->setSpacing(15);
        statisticsContainerLayout->setObjectName("statisticsContainerLayout");
        statisticsContainerLayout->setContentsMargins(10, 10, 10, 10);
        statsGridLayout = new QGridLayout();
        statsGridLayout->setSpacing(12);
        statsGridLayout->setObjectName("statsGridLayout");
        group_pod = new QGroupBox(statisticsContainer);
        group_pod->setObjectName("group_pod");
        verticalLayout_pod = new QVBoxLayout(group_pod);
        verticalLayout_pod->setObjectName("verticalLayout_pod");
        label_pod_count = new QLabel(group_pod);
        label_pod_count->setObjectName("label_pod_count");
        label_pod_count->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_pod->addWidget(label_pod_count);

        text_pod = new QTextBrowser(group_pod);
        text_pod->setObjectName("text_pod");

        verticalLayout_pod->addWidget(text_pod);


        statsGridLayout->addWidget(group_pod, 0, 0, 1, 1);

        group_skaz = new QGroupBox(statisticsContainer);
        group_skaz->setObjectName("group_skaz");
        verticalLayout_skaz = new QVBoxLayout(group_skaz);
        verticalLayout_skaz->setObjectName("verticalLayout_skaz");
        label_skaz_count = new QLabel(group_skaz);
        label_skaz_count->setObjectName("label_skaz_count");
        label_skaz_count->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_skaz->addWidget(label_skaz_count);

        text_skaz = new QTextBrowser(group_skaz);
        text_skaz->setObjectName("text_skaz");

        verticalLayout_skaz->addWidget(text_skaz);


        statsGridLayout->addWidget(group_skaz, 0, 1, 1, 1);

        group_opred = new QGroupBox(statisticsContainer);
        group_opred->setObjectName("group_opred");
        verticalLayout_opred = new QVBoxLayout(group_opred);
        verticalLayout_opred->setObjectName("verticalLayout_opred");
        label_opred_count = new QLabel(group_opred);
        label_opred_count->setObjectName("label_opred_count");
        label_opred_count->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_opred->addWidget(label_opred_count);

        text_opred = new QTextBrowser(group_opred);
        text_opred->setObjectName("text_opred");

        verticalLayout_opred->addWidget(text_opred);


        statsGridLayout->addWidget(group_opred, 1, 0, 1, 1);

        group_dop = new QGroupBox(statisticsContainer);
        group_dop->setObjectName("group_dop");
        group_dop->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        group_dop->setFlat(false);
        verticalLayout_dop = new QVBoxLayout(group_dop);
        verticalLayout_dop->setObjectName("verticalLayout_dop");
        verticalLayout_dop->setContentsMargins(11, -1, -1, -1);
        label_dop_count = new QLabel(group_dop);
        label_dop_count->setObjectName("label_dop_count");
        label_dop_count->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_dop->addWidget(label_dop_count);

        text_dop = new QTextBrowser(group_dop);
        text_dop->setObjectName("text_dop");

        verticalLayout_dop->addWidget(text_dop);


        statsGridLayout->addWidget(group_dop, 1, 1, 1, 1);

        group_ob = new QGroupBox(statisticsContainer);
        group_ob->setObjectName("group_ob");
        verticalLayout_ob = new QVBoxLayout(group_ob);
        verticalLayout_ob->setObjectName("verticalLayout_ob");
        label_ob_count = new QLabel(group_ob);
        label_ob_count->setObjectName("label_ob_count");
        label_ob_count->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_ob->addWidget(label_ob_count);

        text_ob = new QTextBrowser(group_ob);
        text_ob->setObjectName("text_ob");

        verticalLayout_ob->addWidget(text_ob);


        statsGridLayout->addWidget(group_ob, 2, 0, 1, 1);

        group_drugoe = new QGroupBox(statisticsContainer);
        group_drugoe->setObjectName("group_drugoe");
        verticalLayout_drugoe = new QVBoxLayout(group_drugoe);
        verticalLayout_drugoe->setObjectName("verticalLayout_drugoe");
        label_drugoe_count = new QLabel(group_drugoe);
        label_drugoe_count->setObjectName("label_drugoe_count");
        label_drugoe_count->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_drugoe->addWidget(label_drugoe_count);

        text_drugoe = new QTextBrowser(group_drugoe);
        text_drugoe->setObjectName("text_drugoe");

        verticalLayout_drugoe->addWidget(text_drugoe);


        statsGridLayout->addWidget(group_drugoe, 2, 1, 1, 1);


        statisticsContainerLayout->addLayout(statsGridLayout);

        statisticsScrollArea->setWidget(statisticsContainer);

        verticalLayout_3->addWidget(statisticsScrollArea);

        tabWidget->addTab(tab_statistics, QString());

        mainLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\320\241\320\270\320\275\321\202\320\260\320\272\321\201\320\270\321\207\320\265\321\201\320\272\320\270\320\271 \320\260\320\275\320\260\320\273\320\270\320\267\320\260\321\202\320\276\321\200", nullptr));
        textEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \321\202\320\265\320\272\321\201\321\202 \320\264\320\273\321\217 \320\260\320\275\320\260\320\273\320\270\320\267\320\260...", nullptr));
        btn_search->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\270\321\201\320\272", nullptr));
        btn_upload->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214", nullptr));
        btn_download->setText(QCoreApplication::translate("MainWindow", "\320\241\320\272\320\260\321\207\320\260\321\202\321\214", nullptr));
        filterLabel->setText(QCoreApplication::translate("MainWindow", "\320\244\320\270\320\273\321\214\321\202\321\200", nullptr));
        c_pod->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\264\320\273\320\265\320\266\320\260\321\211\320\265\320\265", nullptr));
        c_skaz->setText(QCoreApplication::translate("MainWindow", "\320\241\320\272\320\260\320\267\321\203\320\265\320\274\320\276\320\265", nullptr));
        c_opred->setText(QCoreApplication::translate("MainWindow", "\320\236\320\277\321\200\320\265\320\264\320\265\320\273\320\265\320\275\320\270\320\265", nullptr));
        c_dop->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\277\320\276\320\273\320\275\320\265\320\275\320\270\320\265", nullptr));
        c_ob->setText(QCoreApplication::translate("MainWindow", "\320\236\320\261\321\201\321\202\320\276\321\217\321\202\320\265\320\273\321\214\321\201\321\202\320\262\320\276", nullptr));
        c_none->setText(QCoreApplication::translate("MainWindow", "\320\224\321\200\321\203\320\263\320\276\320\265", nullptr));
        c_all->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\261\321\200\320\260\321\202\321\214 \320\262\321\201\320\265", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MainWindow", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202\321\213 \321\201\320\270\320\275\321\202\320\260\320\272\321\201\320\270\321\207\320\265\321\201\320\272\320\276\320\263\320\276 \320\260\320\275\320\260\320\273\320\270\320\267\320\260", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_analysis), QCoreApplication::translate("MainWindow", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202\321\213 \320\260\320\275\320\260\320\273\320\270\320\267\320\260", nullptr));
        group_pod->setTitle(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\264\320\273\320\265\320\266\320\260\321\211\320\265\320\265", nullptr));
        label_pod_count->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        group_skaz->setTitle(QCoreApplication::translate("MainWindow", "\320\241\320\272\320\260\320\267\321\203\320\265\320\274\320\276\320\265", nullptr));
        label_skaz_count->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        group_opred->setTitle(QCoreApplication::translate("MainWindow", "\320\236\320\277\321\200\320\265\320\264\320\265\320\273\320\265\320\275\320\270\320\265", nullptr));
        label_opred_count->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        group_dop->setTitle(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\277\320\276\320\273\320\275\320\265\320\275\320\270\320\265", nullptr));
        label_dop_count->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        group_ob->setTitle(QCoreApplication::translate("MainWindow", "\320\236\320\261\321\201\321\202\320\276\321\217\321\202\320\265\320\273\321\214\321\201\321\202\320\262\320\276", nullptr));
        label_ob_count->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        group_drugoe->setTitle(QCoreApplication::translate("MainWindow", "\320\224\321\200\321\203\320\263\320\276\320\265", nullptr));
        label_drugoe_count->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_statistics), QCoreApplication::translate("MainWindow", "\320\241\321\202\320\260\321\202\320\270\321\201\321\202\320\270\320\272\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
