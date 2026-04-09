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
    QSpacerItem *verticalSpacer_2;
    QCheckBox *c_all;
    QSpacerItem *verticalSpacer;
    QTabWidget *tabWidget;
    QWidget *tab_analysis;
    QVBoxLayout *verticalLayout_2;
    QTreeWidget *treeWidget;
    QWidget *tab_statistics;
    QVBoxLayout *verticalLayout_3;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_4;
    QGridLayout *statsGridLayout;
    QGroupBox *group_pod;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_pod_count;
    QTreeWidget *tree_pod;
    QGroupBox *group_skaz;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_skaz_count;
    QTreeWidget *tree_skaz;
    QGroupBox *group_opred;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_opred_count;
    QTreeWidget *tree_opred;
    QGroupBox *group_dop;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_dop_count;
    QTreeWidget *tree_dop;
    QGroupBox *group_ob;
    QVBoxLayout *verticalLayout_9;
    QLabel *label_ob_count;
    QTreeWidget *tree_ob;
    QGroupBox *group_drugoe;
    QVBoxLayout *verticalLayout_10;
    QLabel *label_drugoe_count;
    QTreeWidget *tree_drugoe;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(980, 570);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"    QMainWindow, QWidget#centralwidget {\n"
"        background-color: #f5f7fa;\n"
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
        inputFrame->setStyleSheet(QString::fromUtf8("\n"
"          QFrame#inputFrame {\n"
"              background-color: white;\n"
"              border: 1px solid #d0d0d0;\n"
"              border-radius: 8px;\n"
"          }\n"
"         "));
        inputLayout = new QVBoxLayout(inputFrame);
        inputLayout->setSpacing(10);
        inputLayout->setObjectName("inputLayout");
        inputLayout->setContentsMargins(15, 15, 15, 15);
        textEdit = new QTextEdit(inputFrame);
        textEdit->setObjectName("textEdit");
        textEdit->setStyleSheet(QString::fromUtf8("\n"
"             QTextEdit {\n"
"                 border: 1px solid #d0d0d0;\n"
"                 border-radius: 6px;\n"
"                 padding: 10px;\n"
"                 font-size: 13px;\n"
"                 background-color: #fafafa;\n"
"             }\n"
"             QTextEdit:focus {\n"
"                 border-color: #3498db;\n"
"                 background-color: white;\n"
"             }\n"
"            "));

        inputLayout->addWidget(textEdit);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(10);
        buttonLayout->setObjectName("buttonLayout");
        btn_search = new QPushButton(inputFrame);
        btn_search->setObjectName("btn_search");
        btn_search->setMinimumSize(QSize(100, 34));
        btn_search->setStyleSheet(QString::fromUtf8("\n"
"               QPushButton {\n"
"                   background-color: #3498db;\n"
"                   color: white;\n"
"                   border: none;\n"
"                   border-radius: 6px;\n"
"                   font-size: 13px;\n"
"                   font-weight: 500;\n"
"                   padding: 6px 16px;\n"
"               }\n"
"               QPushButton:hover {\n"
"                   background-color: #2980b9;\n"
"               }\n"
"               QPushButton:pressed {\n"
"                   background-color: #1f618d;\n"
"               }\n"
"               QPushButton:disabled {\n"
"                   background-color: #bdc3c7;\n"
"               }\n"
"              "));

        buttonLayout->addWidget(btn_search);

        btn_upload = new QPushButton(inputFrame);
        btn_upload->setObjectName("btn_upload");
        btn_upload->setMinimumSize(QSize(100, 34));
        btn_upload->setStyleSheet(QString::fromUtf8("\n"
"               QPushButton {\n"
"                   background-color: #ecf0f1;\n"
"                   color: #2c3e50;\n"
"                   border: 1px solid #bdc3c7;\n"
"                   border-radius: 6px;\n"
"                   font-size: 13px;\n"
"                   padding: 6px 16px;\n"
"               }\n"
"               QPushButton:hover {\n"
"                   background-color: #dfe6e9;\n"
"               }\n"
"               QPushButton:pressed {\n"
"                   background-color: #bdc3c7;\n"
"               }\n"
"              "));

        buttonLayout->addWidget(btn_upload);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        btn_download = new QPushButton(inputFrame);
        btn_download->setObjectName("btn_download");
        btn_download->setMinimumSize(QSize(100, 34));
        btn_download->setStyleSheet(QString::fromUtf8("\n"
"               QPushButton:enabled {\n"
"                   background-color: #27ae60;\n"
"                   color: white;\n"
"                   border: none;\n"
"                   border-radius: 6px;\n"
"                   font-size: 13px;\n"
"                   padding: 6px 16px;\n"
"               }\n"
"               QPushButton:enabled:hover {\n"
"                   background-color: #219a52;\n"
"               }\n"
"               QPushButton:disabled {\n"
"                   background-color: #bdc3c7;\n"
"                   color: #95a5a6;\n"
"                   border: none;\n"
"                   border-radius: 6px;\n"
"               }\n"
"              "));

        buttonLayout->addWidget(btn_download);


        inputLayout->addLayout(buttonLayout);


        topLayout->addWidget(inputFrame);

        filterFrame = new QFrame(centralwidget);
        filterFrame->setObjectName("filterFrame");
        filterFrame->setStyleSheet(QString::fromUtf8("\n"
"          QFrame#filterFrame {\n"
"              background-color: white;\n"
"              border: 1px solid #d0d0d0;\n"
"              border-radius: 8px;\n"
"          }\n"
"         "));
        filterLayout = new QVBoxLayout(filterFrame);
        filterLayout->setSpacing(12);
        filterLayout->setObjectName("filterLayout");
        filterLayout->setContentsMargins(15, 15, 15, 15);
        filterLabel = new QLabel(filterFrame);
        filterLabel->setObjectName("filterLabel");
        QFont font;
        font.setPointSize(13);
        font.setBold(true);
        filterLabel->setFont(font);
        filterLabel->setStyleSheet(QString::fromUtf8("\n"
"             QLabel {\n"
"                 color: #2c3e50;\n"
"             }\n"
"            "));
        filterLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        filterLayout->addWidget(filterLabel);

        c_pod = new QCheckBox(filterFrame);
        c_pod->setObjectName("c_pod");
        c_pod->setStyleSheet(QString::fromUtf8("\n"
"                QCheckBox {\n"
"                    spacing: 8px;\n"
"                    font-size: 12px;\n"
"                    color: #2c3e50;\n"
"                }\n"
"                QCheckBox::indicator {\n"
"                    width: 16px;\n"
"                    height: 16px;\n"
"                    border-radius: 3px;\n"
"                    border: 1px solid #bdc3c7;\n"
"                    background-color: white;\n"
"                }\n"
"                QCheckBox::indicator:checked {\n"
"                    background-color: #3498db;\n"
"                    border-color: #3498db;\n"
"                }\n"
"                QCheckBox::indicator:hover {\n"
"                    border-color: #3498db;\n"
"                }\n"
"               "));
        c_pod->setChecked(true);

        filterLayout->addWidget(c_pod);

        c_skaz = new QCheckBox(filterFrame);
        c_skaz->setObjectName("c_skaz");
        c_skaz->setStyleSheet(QString::fromUtf8("\n"
"                QCheckBox {\n"
"                    spacing: 8px;\n"
"                    font-size: 12px;\n"
"                    color: #2c3e50;\n"
"                }\n"
"                QCheckBox::indicator {\n"
"                    width: 16px;\n"
"                    height: 16px;\n"
"                    border-radius: 3px;\n"
"                    border: 1px solid #bdc3c7;\n"
"                    background-color: white;\n"
"                }\n"
"                QCheckBox::indicator:checked {\n"
"                    background-color: #3498db;\n"
"                    border-color: #3498db;\n"
"                }\n"
"                QCheckBox::indicator:hover {\n"
"                    border-color: #3498db;\n"
"                }\n"
"               "));
        c_skaz->setChecked(true);

        filterLayout->addWidget(c_skaz);

        c_opred = new QCheckBox(filterFrame);
        c_opred->setObjectName("c_opred");
        c_opred->setStyleSheet(QString::fromUtf8("\n"
"                QCheckBox {\n"
"                    spacing: 8px;\n"
"                    font-size: 12px;\n"
"                    color: #2c3e50;\n"
"                }\n"
"                QCheckBox::indicator {\n"
"                    width: 16px;\n"
"                    height: 16px;\n"
"                    border-radius: 3px;\n"
"                    border: 1px solid #bdc3c7;\n"
"                    background-color: white;\n"
"                }\n"
"                QCheckBox::indicator:checked {\n"
"                    background-color: #3498db;\n"
"                    border-color: #3498db;\n"
"                }\n"
"                QCheckBox::indicator:hover {\n"
"                    border-color: #3498db;\n"
"                }\n"
"               "));
        c_opred->setChecked(true);

        filterLayout->addWidget(c_opred);

        c_dop = new QCheckBox(filterFrame);
        c_dop->setObjectName("c_dop");
        c_dop->setStyleSheet(QString::fromUtf8("\n"
"                QCheckBox {\n"
"                    spacing: 8px;\n"
"                    font-size: 12px;\n"
"                    color: #2c3e50;\n"
"                }\n"
"                QCheckBox::indicator {\n"
"                    width: 16px;\n"
"                    height: 16px;\n"
"                    border-radius: 3px;\n"
"                    border: 1px solid #bdc3c7;\n"
"                    background-color: white;\n"
"                }\n"
"                QCheckBox::indicator:checked {\n"
"                    background-color: #3498db;\n"
"                    border-color: #3498db;\n"
"                }\n"
"                QCheckBox::indicator:hover {\n"
"                    border-color: #3498db;\n"
"                }\n"
"               "));
        c_dop->setChecked(true);

        filterLayout->addWidget(c_dop);

        c_ob = new QCheckBox(filterFrame);
        c_ob->setObjectName("c_ob");
        c_ob->setStyleSheet(QString::fromUtf8("\n"
"                QCheckBox {\n"
"                    spacing: 8px;\n"
"                    font-size: 12px;\n"
"                    color: #2c3e50;\n"
"                }\n"
"                QCheckBox::indicator {\n"
"                    width: 16px;\n"
"                    height: 16px;\n"
"                    border-radius: 3px;\n"
"                    border: 1px solid #bdc3c7;\n"
"                    background-color: white;\n"
"                }\n"
"                QCheckBox::indicator:checked {\n"
"                    background-color: #3498db;\n"
"                    border-color: #3498db;\n"
"                }\n"
"                QCheckBox::indicator:hover {\n"
"                    border-color: #3498db;\n"
"                }\n"
"               "));
        c_ob->setChecked(true);

        filterLayout->addWidget(c_ob);

        c_none = new QCheckBox(filterFrame);
        c_none->setObjectName("c_none");
        c_none->setStyleSheet(QString::fromUtf8("\n"
"                QCheckBox {\n"
"                    spacing: 8px;\n"
"                    font-size: 12px;\n"
"                    color: #2c3e50;\n"
"                }\n"
"                QCheckBox::indicator {\n"
"                    width: 16px;\n"
"                    height: 16px;\n"
"                    border-radius: 3px;\n"
"                    border: 1px solid #bdc3c7;\n"
"                    background-color: white;\n"
"                }\n"
"                QCheckBox::indicator:checked {\n"
"                    background-color: #3498db;\n"
"                    border-color: #3498db;\n"
"                }\n"
"                QCheckBox::indicator:hover {\n"
"                    border-color: #3498db;\n"
"                }\n"
"               "));
        c_none->setChecked(true);

        filterLayout->addWidget(c_none);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        filterLayout->addItem(verticalSpacer_2);

        c_all = new QCheckBox(filterFrame);
        c_all->setObjectName("c_all");
        c_all->setStyleSheet(QString::fromUtf8("\n"
"             QCheckBox {\n"
"                 spacing: 8px;\n"
"                 font-size: 12px;\n"
"                 font-weight: bold;\n"
"                 color: #3498db;\n"
"             }\n"
"             QCheckBox::indicator {\n"
"                 width: 16px;\n"
"                 height: 16px;\n"
"                 border-radius: 3px;\n"
"                 border: 1px solid #3498db;\n"
"                 background-color: white;\n"
"             }\n"
"             QCheckBox::indicator:checked {\n"
"                 background-color: #3498db;\n"
"                 border-color: #3498db;\n"
"             }\n"
"            "));
        c_all->setChecked(true);

        filterLayout->addWidget(c_all);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        filterLayout->addItem(verticalSpacer);


        topLayout->addWidget(filterFrame);


        mainLayout->addLayout(topLayout);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setStyleSheet(QString::fromUtf8("\n"
"        QTabWidget::pane {\n"
"            background-color: white;\n"
"            border: 1px solid #d0d0d0;\n"
"            border-radius: 8px;\n"
"        }\n"
"        QTabBar::tab {\n"
"            background-color: #ecf0f1;\n"
"            padding: 8px 20px;\n"
"            margin-right: 2px;\n"
"            border-top-left-radius: 6px;\n"
"            border-top-right-radius: 6px;\n"
"            font-size: 12px;\n"
"            font-weight: 500;\n"
"            color: #2c3e50;\n"
"        }\n"
"        QTabBar::tab:selected {\n"
"            background-color: white;\n"
"            color: #3498db;\n"
"            border-bottom: 2px solid #3498db;\n"
"        }\n"
"        QTabBar::tab:hover:!selected {\n"
"            background-color: #dfe6e9;\n"
"        }\n"
"       "));
        tab_analysis = new QWidget();
        tab_analysis->setObjectName("tab_analysis");
        verticalLayout_2 = new QVBoxLayout(tab_analysis);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(10, 10, 10, 10);
        treeWidget = new QTreeWidget(tab_analysis);
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setFont(0, font1);
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName("treeWidget");
        treeWidget->setStyleSheet(QString::fromUtf8("\n"
"            QTreeWidget {\n"
"                background-color: white;\n"
"                border: none;\n"
"                font-size: 13px;\n"
"                outline: 0;\n"
"            }\n"
"            QTreeWidget::item {\n"
"                padding: 6px;\n"
"                border-bottom: 1px solid #ecf0f1;\n"
"            }\n"
"            QTreeWidget::item:hover {\n"
"                background-color: #f0f4f8;\n"
"            }\n"
"            QTreeWidget::item:selected {\n"
"                background-color: #3498db;\n"
"                color: white;\n"
"            }\n"
"            QTreeWidget::item:selected:hover {\n"
"                background-color: #2980b9;\n"
"            }\n"
"            QHeaderView::section {\n"
"                background-color: #f8fafc;\n"
"                padding: 8px;\n"
"                border: none;\n"
"                border-bottom: 1px solid #d0d0d0;\n"
"                font-weight: 500;\n"
"                font-size: 12px;\n"
"                color: #2c3e50;\n"
""
                        "            }\n"
"           "));
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
        verticalLayout_3 = new QVBoxLayout(tab_statistics);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(10, 10, 10, 10);
        scrollArea = new QScrollArea(tab_statistics);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setAutoFillBackground(false);
        scrollArea->setStyleSheet(QString::fromUtf8("\n"
"            QScrollArea {\n"
"                border: none;\n"
"                background-color: white;\n"
"            }\n"
"           "));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 917, 625));
        verticalLayout_4 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_4->setSpacing(15);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_4->setContentsMargins(10, 10, 10, 10);
        statsGridLayout = new QGridLayout();
        statsGridLayout->setSpacing(12);
        statsGridLayout->setObjectName("statsGridLayout");
        group_pod = new QGroupBox(scrollAreaWidgetContents);
        group_pod->setObjectName("group_pod");
        group_pod->setStyleSheet(QString::fromUtf8("\n"
"                  QGroupBox {\n"
"                      background-color: #f8fafc;\n"
"                      border: 1px solid #d0d0d0;\n"
"                      border-radius: 8px;\n"
"                      margin-top: 8px;\n"
"                      padding-top: 8px;\n"
"                  }\n"
"                  QGroupBox::title {\n"
"                      subcontrol-origin: margin;\n"
"                      left: 12px;\n"
"                      padding: 0 6px;\n"
"                      color: #2c3e50;\n"
"                      font-weight: 500;\n"
"                      font-size: 12px;\n"
"                  }\n"
"                 "));
        verticalLayout_5 = new QVBoxLayout(group_pod);
        verticalLayout_5->setSpacing(5);
        verticalLayout_5->setObjectName("verticalLayout_5");
        label_pod_count = new QLabel(group_pod);
        label_pod_count->setObjectName("label_pod_count");
        QFont font2;
        font2.setPointSize(28);
        font2.setBold(true);
        label_pod_count->setFont(font2);
        label_pod_count->setStyleSheet(QString::fromUtf8("\n"
"                     QLabel {\n"
"                         color: #3498db;\n"
"                     }\n"
"                    "));
        label_pod_count->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_5->addWidget(label_pod_count);

        tree_pod = new QTreeWidget(group_pod);
        tree_pod->setObjectName("tree_pod");
        tree_pod->setStyleSheet(QString::fromUtf8("\n"
"                     QTreeWidget {\n"
"                         border: 1px solid #e0e0e0;\n"
"                         border-radius: 6px;\n"
"                         background-color: white;\n"
"                         font-size: 12px;\n"
"                     }\n"
"                     QTreeWidget::item:selected {\n"
"                         background-color: #3498db;\n"
"                         color: white;\n"
"                     }\n"
"                    "));
        tree_pod->setHeaderHidden(true);

        verticalLayout_5->addWidget(tree_pod);


        statsGridLayout->addWidget(group_pod, 0, 0, 1, 1);

        group_skaz = new QGroupBox(scrollAreaWidgetContents);
        group_skaz->setObjectName("group_skaz");
        group_skaz->setStyleSheet(QString::fromUtf8("\n"
"                  QGroupBox {\n"
"                      background-color: #f8fafc;\n"
"                      border: 1px solid #d0d0d0;\n"
"                      border-radius: 8px;\n"
"                      margin-top: 8px;\n"
"                      padding-top: 8px;\n"
"                  }\n"
"                  QGroupBox::title {\n"
"                      subcontrol-origin: margin;\n"
"                      left: 12px;\n"
"                      padding: 0 6px;\n"
"                      color: #2c3e50;\n"
"                      font-weight: 500;\n"
"                      font-size: 12px;\n"
"                  }\n"
"                 "));
        verticalLayout_6 = new QVBoxLayout(group_skaz);
        verticalLayout_6->setSpacing(5);
        verticalLayout_6->setObjectName("verticalLayout_6");
        label_skaz_count = new QLabel(group_skaz);
        label_skaz_count->setObjectName("label_skaz_count");
        label_skaz_count->setFont(font2);
        label_skaz_count->setStyleSheet(QString::fromUtf8("\n"
"                     QLabel {\n"
"                         color: #3498db;\n"
"                     }\n"
"                    "));
        label_skaz_count->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_6->addWidget(label_skaz_count);

        tree_skaz = new QTreeWidget(group_skaz);
        tree_skaz->setObjectName("tree_skaz");
        tree_skaz->setStyleSheet(QString::fromUtf8("\n"
"                     QTreeWidget {\n"
"                         border: 1px solid #e0e0e0;\n"
"                         border-radius: 6px;\n"
"                         background-color: white;\n"
"                         font-size: 12px;\n"
"                     }\n"
"                     QTreeWidget::item:selected {\n"
"                         background-color: #3498db;\n"
"                         color: white;\n"
"                     }\n"
"                    "));
        tree_skaz->setHeaderHidden(true);

        verticalLayout_6->addWidget(tree_skaz);


        statsGridLayout->addWidget(group_skaz, 0, 1, 1, 1);

        group_opred = new QGroupBox(scrollAreaWidgetContents);
        group_opred->setObjectName("group_opred");
        group_opred->setStyleSheet(QString::fromUtf8("\n"
"                  QGroupBox {\n"
"                      background-color: #f8fafc;\n"
"                      border: 1px solid #d0d0d0;\n"
"                      border-radius: 8px;\n"
"                      margin-top: 8px;\n"
"                      padding-top: 8px;\n"
"                  }\n"
"                  QGroupBox::title {\n"
"                      subcontrol-origin: margin;\n"
"                      left: 12px;\n"
"                      padding: 0 6px;\n"
"                      color: #2c3e50;\n"
"                      font-weight: 500;\n"
"                      font-size: 12px;\n"
"                  }\n"
"                 "));
        verticalLayout_7 = new QVBoxLayout(group_opred);
        verticalLayout_7->setSpacing(5);
        verticalLayout_7->setObjectName("verticalLayout_7");
        label_opred_count = new QLabel(group_opred);
        label_opred_count->setObjectName("label_opred_count");
        label_opred_count->setFont(font2);
        label_opred_count->setStyleSheet(QString::fromUtf8("\n"
"                     QLabel {\n"
"                         color: #3498db;\n"
"                     }\n"
"                    "));
        label_opred_count->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_7->addWidget(label_opred_count);

        tree_opred = new QTreeWidget(group_opred);
        tree_opred->setObjectName("tree_opred");
        tree_opred->setStyleSheet(QString::fromUtf8("\n"
"                     QTreeWidget {\n"
"                         border: 1px solid #e0e0e0;\n"
"                         border-radius: 6px;\n"
"                         background-color: white;\n"
"                         font-size: 12px;\n"
"                     }\n"
"                     QTreeWidget::item:selected {\n"
"                         background-color: #3498db;\n"
"                         color: white;\n"
"                     }\n"
"                    "));
        tree_opred->setHeaderHidden(true);

        verticalLayout_7->addWidget(tree_opred);


        statsGridLayout->addWidget(group_opred, 1, 0, 1, 1);

        group_dop = new QGroupBox(scrollAreaWidgetContents);
        group_dop->setObjectName("group_dop");
        group_dop->setStyleSheet(QString::fromUtf8("\n"
"                  QGroupBox {\n"
"                      background-color: #f8fafc;\n"
"                      border: 1px solid #d0d0d0;\n"
"                      border-radius: 8px;\n"
"                      margin-top: 8px;\n"
"                      padding-top: 8px;\n"
"                  }\n"
"                  QGroupBox::title {\n"
"                      subcontrol-origin: margin;\n"
"                      left: 12px;\n"
"                      padding: 0 6px;\n"
"                      color: #2c3e50;\n"
"                      font-weight: 500;\n"
"                      font-size: 12px;\n"
"                  }\n"
"                 "));
        verticalLayout_8 = new QVBoxLayout(group_dop);
        verticalLayout_8->setSpacing(5);
        verticalLayout_8->setObjectName("verticalLayout_8");
        label_dop_count = new QLabel(group_dop);
        label_dop_count->setObjectName("label_dop_count");
        label_dop_count->setFont(font2);
        label_dop_count->setStyleSheet(QString::fromUtf8("\n"
"                     QLabel {\n"
"                         color: #3498db;\n"
"                     }\n"
"                    "));
        label_dop_count->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_8->addWidget(label_dop_count);

        tree_dop = new QTreeWidget(group_dop);
        tree_dop->setObjectName("tree_dop");
        tree_dop->setStyleSheet(QString::fromUtf8("\n"
"                     QTreeWidget {\n"
"                         border: 1px solid #e0e0e0;\n"
"                         border-radius: 6px;\n"
"                         background-color: white;\n"
"                         font-size: 12px;\n"
"                     }\n"
"                     QTreeWidget::item:selected {\n"
"                         background-color: #3498db;\n"
"                         color: white;\n"
"                     }\n"
"                    "));
        tree_dop->setHeaderHidden(true);

        verticalLayout_8->addWidget(tree_dop);


        statsGridLayout->addWidget(group_dop, 1, 1, 1, 1);

        group_ob = new QGroupBox(scrollAreaWidgetContents);
        group_ob->setObjectName("group_ob");
        group_ob->setStyleSheet(QString::fromUtf8("\n"
"                  QGroupBox {\n"
"                      background-color: #f8fafc;\n"
"                      border: 1px solid #d0d0d0;\n"
"                      border-radius: 8px;\n"
"                      margin-top: 8px;\n"
"                      padding-top: 8px;\n"
"                  }\n"
"                  QGroupBox::title {\n"
"                      subcontrol-origin: margin;\n"
"                      left: 12px;\n"
"                      padding: 0 6px;\n"
"                      color: #2c3e50;\n"
"                      font-weight: 500;\n"
"                      font-size: 12px;\n"
"                  }\n"
"                 "));
        verticalLayout_9 = new QVBoxLayout(group_ob);
        verticalLayout_9->setSpacing(5);
        verticalLayout_9->setObjectName("verticalLayout_9");
        label_ob_count = new QLabel(group_ob);
        label_ob_count->setObjectName("label_ob_count");
        label_ob_count->setFont(font2);
        label_ob_count->setStyleSheet(QString::fromUtf8("\n"
"                     QLabel {\n"
"                         color: #3498db;\n"
"                     }\n"
"                    "));
        label_ob_count->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_9->addWidget(label_ob_count);

        tree_ob = new QTreeWidget(group_ob);
        tree_ob->setObjectName("tree_ob");
        tree_ob->setStyleSheet(QString::fromUtf8("\n"
"                     QTreeWidget {\n"
"                         border: 1px solid #e0e0e0;\n"
"                         border-radius: 6px;\n"
"                         background-color: white;\n"
"                         font-size: 12px;\n"
"                     }\n"
"                     QTreeWidget::item:selected {\n"
"                         background-color: #3498db;\n"
"                         color: white;\n"
"                     }\n"
"                    "));
        tree_ob->setHeaderHidden(true);

        verticalLayout_9->addWidget(tree_ob);


        statsGridLayout->addWidget(group_ob, 2, 0, 1, 1);

        group_drugoe = new QGroupBox(scrollAreaWidgetContents);
        group_drugoe->setObjectName("group_drugoe");
        group_drugoe->setStyleSheet(QString::fromUtf8("\n"
"                  QGroupBox {\n"
"                      background-color: #f8fafc;\n"
"                      border: 1px solid #d0d0d0;\n"
"                      border-radius: 8px;\n"
"                      margin-top: 8px;\n"
"                      padding-top: 8px;\n"
"                  }\n"
"                  QGroupBox::title {\n"
"                      subcontrol-origin: margin;\n"
"                      left: 12px;\n"
"                      padding: 0 6px;\n"
"                      color: #2c3e50;\n"
"                      font-weight: 500;\n"
"                      font-size: 12px;\n"
"                  }\n"
"                 "));
        verticalLayout_10 = new QVBoxLayout(group_drugoe);
        verticalLayout_10->setSpacing(5);
        verticalLayout_10->setObjectName("verticalLayout_10");
        label_drugoe_count = new QLabel(group_drugoe);
        label_drugoe_count->setObjectName("label_drugoe_count");
        label_drugoe_count->setFont(font2);
        label_drugoe_count->setStyleSheet(QString::fromUtf8("\n"
"                     QLabel {\n"
"                         color: #3498db;\n"
"                     }\n"
"                    "));
        label_drugoe_count->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_10->addWidget(label_drugoe_count);

        tree_drugoe = new QTreeWidget(group_drugoe);
        tree_drugoe->setObjectName("tree_drugoe");
        tree_drugoe->setStyleSheet(QString::fromUtf8("\n"
"                     QTreeWidget {\n"
"                         border: 1px solid #e0e0e0;\n"
"                         border-radius: 6px;\n"
"                         background-color: white;\n"
"                         font-size: 12px;\n"
"                     }\n"
"                     QTreeWidget::item:selected {\n"
"                         background-color: #3498db;\n"
"                         color: white;\n"
"                     }\n"
"                    "));
        tree_drugoe->setHeaderHidden(true);

        verticalLayout_10->addWidget(tree_drugoe);


        statsGridLayout->addWidget(group_drugoe, 2, 1, 1, 1);


        verticalLayout_4->addLayout(statsGridLayout);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_3->addWidget(scrollArea);

        tabWidget->addTab(tab_statistics, QString());

        mainLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        statusbar->setStyleSheet(QString::fromUtf8("\n"
"     QStatusBar {\n"
"         background-color: #2c3e50;\n"
"         color: white;\n"
"         padding: 4px 8px;\n"
"         font-size: 11px;\n"
"     }\n"
"    "));
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
        QTreeWidgetItem *___qtreewidgetitem1 = tree_pod->headerItem();
        ___qtreewidgetitem1->setText(0, QCoreApplication::translate("MainWindow", "\320\241\320\273\320\276\320\262\320\260", nullptr));
        group_skaz->setTitle(QCoreApplication::translate("MainWindow", "\320\241\320\272\320\260\320\267\321\203\320\265\320\274\320\276\320\265", nullptr));
        label_skaz_count->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        QTreeWidgetItem *___qtreewidgetitem2 = tree_skaz->headerItem();
        ___qtreewidgetitem2->setText(0, QCoreApplication::translate("MainWindow", "\320\241\320\273\320\276\320\262\320\260", nullptr));
        group_opred->setTitle(QCoreApplication::translate("MainWindow", "\320\236\320\277\321\200\320\265\320\264\320\265\320\273\320\265\320\275\320\270\320\265", nullptr));
        label_opred_count->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        QTreeWidgetItem *___qtreewidgetitem3 = tree_opred->headerItem();
        ___qtreewidgetitem3->setText(0, QCoreApplication::translate("MainWindow", "\320\241\320\273\320\276\320\262\320\260", nullptr));
        group_dop->setTitle(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\277\320\276\320\273\320\275\320\265\320\275\320\270\320\265", nullptr));
        label_dop_count->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        QTreeWidgetItem *___qtreewidgetitem4 = tree_dop->headerItem();
        ___qtreewidgetitem4->setText(0, QCoreApplication::translate("MainWindow", "\320\241\320\273\320\276\320\262\320\260", nullptr));
        group_ob->setTitle(QCoreApplication::translate("MainWindow", "\320\236\320\261\321\201\321\202\320\276\321\217\321\202\320\265\320\273\321\214\321\201\321\202\320\262\320\276", nullptr));
        label_ob_count->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        QTreeWidgetItem *___qtreewidgetitem5 = tree_ob->headerItem();
        ___qtreewidgetitem5->setText(0, QCoreApplication::translate("MainWindow", "\320\241\320\273\320\276\320\262\320\260", nullptr));
        group_drugoe->setTitle(QCoreApplication::translate("MainWindow", "\320\224\321\200\321\203\320\263\320\276\320\265", nullptr));
        label_drugoe_count->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        QTreeWidgetItem *___qtreewidgetitem6 = tree_drugoe->headerItem();
        ___qtreewidgetitem6->setText(0, QCoreApplication::translate("MainWindow", "\320\241\320\273\320\276\320\262\320\260", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_statistics), QCoreApplication::translate("MainWindow", "\320\241\321\202\320\260\321\202\320\270\321\201\321\202\320\270\320\272\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
