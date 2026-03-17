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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QFrame *searchFrame;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineEdit;
    QPushButton *btn_search;
    QPushButton *btn_upload;
    QPushButton *btn_download;
    QPushButton *btn_test;
    QFrame *filterFrame;
    QHBoxLayout *horizontalLayout;
    QCheckBox *c_pod;
    QCheckBox *c_skaz;
    QCheckBox *c_opred;
    QCheckBox *c_dop;
    QCheckBox *c_ob;
    QCheckBox *c_none;
    QSpacerItem *horizontalSpacer;
    QCheckBox *c_all;
    QTreeWidget *treeWidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 700);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"    /* \320\223\320\273\320\260\320\262\320\275\320\276\320\265 \320\276\320\272\320\275\320\276 */\n"
"    QMainWindow {\n"
"        background-color: #f5f7fa;\n"
"    }\n"
"    \n"
"    /* \320\246\320\265\320\275\321\202\321\200\320\260\320\273\321\214\320\275\321\213\320\271 \320\262\320\270\320\264\320\266\320\265\321\202 */\n"
"    QWidget#centralwidget {\n"
"        background-color: #f5f7fa;\n"
"    }\n"
"    \n"
"    /* \320\227\320\260\320\263\320\276\320\273\320\276\320\262\320\276\320\272 \320\276\320\272\320\275\320\260 */\n"
"    QMainWindow::title {\n"
"        background-color: #2c3e50;\n"
"        color: white;\n"
"    }\n"
"   "));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setSpacing(15);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(20, 20, 20, 20);
        searchFrame = new QFrame(centralwidget);
        searchFrame->setObjectName("searchFrame");
        searchFrame->setStyleSheet(QString::fromUtf8("\n"
"        QFrame#searchFrame {\n"
"            background-color: white;\n"
"            border-radius: 10px;\n"
"            padding: 10px;\n"
"        }\n"
"       "));
        horizontalLayout_2 = new QHBoxLayout(searchFrame);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(15, 10, 15, 10);
        lineEdit = new QLineEdit(searchFrame);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setStyleSheet(QString::fromUtf8("\n"
"           QLineEdit {\n"
"               border: 2px solid #e0e4e8;\n"
"               border-radius: 8px;\n"
"               padding: 8px 15px;\n"
"               font-size: 14px;\n"
"               background-color: #f8fafc;\n"
"           }\n"
"           QLineEdit:focus {\n"
"               border-color: #3498db;\n"
"               background-color: white;\n"
"           }\n"
"          "));

        horizontalLayout_2->addWidget(lineEdit);

        btn_search = new QPushButton(searchFrame);
        btn_search->setObjectName("btn_search");
        btn_search->setMinimumSize(QSize(100, 40));
        btn_search->setStyleSheet(QString::fromUtf8("\n"
"           QPushButton {\n"
"               background-color: #3498db;\n"
"               color: white;\n"
"               border: none;\n"
"               border-radius: 8px;\n"
"               font-size: 14px;\n"
"               font-weight: bold;\n"
"               padding: 8px 20px;\n"
"           }\n"
"           QPushButton:hover {\n"
"               background-color: #2980b9;\n"
"           }\n"
"           QPushButton:pressed {\n"
"               background-color: #1f618d;\n"
"           }\n"
"           QPushButton:disabled {\n"
"               background-color: #bdc3c7;\n"
"           }\n"
"          "));

        horizontalLayout_2->addWidget(btn_search);

        btn_upload = new QPushButton(searchFrame);
        btn_upload->setObjectName("btn_upload");
        btn_upload->setMinimumSize(QSize(100, 40));
        btn_upload->setStyleSheet(QString::fromUtf8("\n"
"           QPushButton {\n"
"               background-color: #3498db;\n"
"               color: white;\n"
"               border: none;\n"
"               border-radius: 8px;\n"
"               font-size: 14px;\n"
"               font-weight: bold;\n"
"               padding: 8px 20px;\n"
"           }\n"
"           QPushButton:hover {\n"
"               background-color: #2980b9;\n"
"           }\n"
"           QPushButton:pressed {\n"
"               background-color: #1f618d;\n"
"           }\n"
"           QPushButton:disabled {\n"
"               background-color: #bdc3c7;\n"
"           }\n"
"          "));

        horizontalLayout_2->addWidget(btn_upload);

        btn_download = new QPushButton(searchFrame);
        btn_download->setObjectName("btn_download");
        btn_download->setMinimumSize(QSize(100, 40));
        btn_download->setStyleSheet(QString::fromUtf8("\n"
"           QPushButton {\n"
"               background-color: #3498db;\n"
"               color: white;\n"
"               border: none;\n"
"               border-radius: 8px;\n"
"               font-size: 14px;\n"
"               font-weight: bold;\n"
"               padding: 8px 20px;\n"
"           }\n"
"           QPushButton:hover {\n"
"               background-color: #2980b9;\n"
"           }\n"
"           QPushButton:pressed {\n"
"               background-color: #1f618d;\n"
"           }\n"
"           QPushButton:disabled {\n"
"               background-color: #bdc3c7;\n"
"           }\n"
"          "));

        horizontalLayout_2->addWidget(btn_download);

        btn_test = new QPushButton(searchFrame);
        btn_test->setObjectName("btn_test");
        btn_test->setMinimumSize(QSize(100, 40));
        btn_test->setStyleSheet(QString::fromUtf8("\n"
"           QPushButton {\n"
"               background-color: #3498db;\n"
"               color: white;\n"
"               border: none;\n"
"               border-radius: 8px;\n"
"               font-size: 14px;\n"
"               font-weight: bold;\n"
"               padding: 8px 20px;\n"
"           }\n"
"           QPushButton:hover {\n"
"               background-color: #2980b9;\n"
"           }\n"
"           QPushButton:pressed {\n"
"               background-color: #1f618d;\n"
"           }\n"
"           QPushButton:disabled {\n"
"               background-color: #bdc3c7;\n"
"           }\n"
"          "));

        horizontalLayout_2->addWidget(btn_test);


        gridLayout->addWidget(searchFrame, 0, 0, 1, 1);

        filterFrame = new QFrame(centralwidget);
        filterFrame->setObjectName("filterFrame");
        filterFrame->setStyleSheet(QString::fromUtf8("\n"
"        QFrame#filterFrame {\n"
"            background-color: white;\n"
"            border-radius: 10px;\n"
"            padding: 5px;\n"
"        }\n"
"       "));
        horizontalLayout = new QHBoxLayout(filterFrame);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(15, 5, 15, 5);
        c_pod = new QCheckBox(filterFrame);
        c_pod->setObjectName("c_pod");
        c_pod->setStyleSheet(QString::fromUtf8("\n"
"           QCheckBox {\n"
"               spacing: 8px;\n"
"               font-size: 13px;\n"
"               color: #2c3e50;\n"
"           }\n"
"           QCheckBox::indicator {\n"
"               width: 18px;\n"
"               height: 18px;\n"
"               border-radius: 4px;\n"
"               border: 2px solid #e0e4e8;\n"
"           }\n"
"           QCheckBox::indicator:checked {\n"
"               background-color: #3498db;\n"
"               border-color: #3498db;\n"
"           }\n"
"           QCheckBox::indicator:hover {\n"
"               border-color: #3498db;\n"
"           }\n"
"          "));

        horizontalLayout->addWidget(c_pod);

        c_skaz = new QCheckBox(filterFrame);
        c_skaz->setObjectName("c_skaz");
        c_skaz->setStyleSheet(QString::fromUtf8("\n"
"           QCheckBox {\n"
"               spacing: 8px;\n"
"               font-size: 13px;\n"
"               color: #2c3e50;\n"
"           }\n"
"           QCheckBox::indicator {\n"
"               width: 18px;\n"
"               height: 18px;\n"
"               border-radius: 4px;\n"
"               border: 2px solid #e0e4e8;\n"
"           }\n"
"           QCheckBox::indicator:checked {\n"
"               background-color: #3498db;\n"
"               border-color: #3498db;\n"
"           }\n"
"           QCheckBox::indicator:hover {\n"
"               border-color: #3498db;\n"
"           }\n"
"          "));

        horizontalLayout->addWidget(c_skaz);

        c_opred = new QCheckBox(filterFrame);
        c_opred->setObjectName("c_opred");
        c_opred->setStyleSheet(QString::fromUtf8("\n"
"           QCheckBox {\n"
"               spacing: 8px;\n"
"               font-size: 13px;\n"
"               color: #2c3e50;\n"
"           }\n"
"           QCheckBox::indicator {\n"
"               width: 18px;\n"
"               height: 18px;\n"
"               border-radius: 4px;\n"
"               border: 2px solid #e0e4e8;\n"
"           }\n"
"           QCheckBox::indicator:checked {\n"
"               background-color: #3498db;\n"
"               border-color: #3498db;\n"
"           }\n"
"           QCheckBox::indicator:hover {\n"
"               border-color: #3498db;\n"
"           }\n"
"          "));

        horizontalLayout->addWidget(c_opred);

        c_dop = new QCheckBox(filterFrame);
        c_dop->setObjectName("c_dop");
        c_dop->setStyleSheet(QString::fromUtf8("\n"
"           QCheckBox {\n"
"               spacing: 8px;\n"
"               font-size: 13px;\n"
"               color: #2c3e50;\n"
"           }\n"
"           QCheckBox::indicator {\n"
"               width: 18px;\n"
"               height: 18px;\n"
"               border-radius: 4px;\n"
"               border: 2px solid #e0e4e8;\n"
"           }\n"
"           QCheckBox::indicator:checked {\n"
"               background-color: #3498db;\n"
"               border-color: #3498db;\n"
"           }\n"
"           QCheckBox::indicator:hover {\n"
"               border-color: #3498db;\n"
"           }\n"
"          "));

        horizontalLayout->addWidget(c_dop);

        c_ob = new QCheckBox(filterFrame);
        c_ob->setObjectName("c_ob");
        c_ob->setStyleSheet(QString::fromUtf8("\n"
"           QCheckBox {\n"
"               spacing: 8px;\n"
"               font-size: 13px;\n"
"               color: #2c3e50;\n"
"           }\n"
"           QCheckBox::indicator {\n"
"               width: 18px;\n"
"               height: 18px;\n"
"               border-radius: 4px;\n"
"               border: 2px solid #e0e4e8;\n"
"           }\n"
"           QCheckBox::indicator:checked {\n"
"               background-color: #3498db;\n"
"               border-color: #3498db;\n"
"           }\n"
"           QCheckBox::indicator:hover {\n"
"               border-color: #3498db;\n"
"           }\n"
"          "));

        horizontalLayout->addWidget(c_ob);

        c_none = new QCheckBox(filterFrame);
        c_none->setObjectName("c_none");
        c_none->setStyleSheet(QString::fromUtf8("\n"
"           QCheckBox {\n"
"               spacing: 8px;\n"
"               font-size: 13px;\n"
"               color: #2c3e50;\n"
"           }\n"
"           QCheckBox::indicator {\n"
"               width: 18px;\n"
"               height: 18px;\n"
"               border-radius: 4px;\n"
"               border: 2px solid #e0e4e8;\n"
"           }\n"
"           QCheckBox::indicator:checked {\n"
"               background-color: #3498db;\n"
"               border-color: #3498db;\n"
"           }\n"
"           QCheckBox::indicator:hover {\n"
"               border-color: #3498db;\n"
"           }\n"
"          "));

        horizontalLayout->addWidget(c_none);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        c_all = new QCheckBox(filterFrame);
        c_all->setObjectName("c_all");
        c_all->setStyleSheet(QString::fromUtf8("\n"
"           QCheckBox {\n"
"               spacing: 8px;\n"
"               font-size: 13px;\n"
"               font-weight: bold;\n"
"               color: #3498db;\n"
"           }\n"
"           QCheckBox::indicator {\n"
"               width: 18px;\n"
"               height: 18px;\n"
"               border-radius: 4px;\n"
"               border: 2px solid #e0e4e8;\n"
"           }\n"
"           QCheckBox::indicator:checked {\n"
"               background-color: #3498db;\n"
"               border-color: #3498db;\n"
"           }\n"
"           QCheckBox::indicator:hover {\n"
"               border-color: #3498db;\n"
"           }\n"
"          "));

        horizontalLayout->addWidget(c_all);


        gridLayout->addWidget(filterFrame, 1, 0, 1, 1);

        treeWidget = new QTreeWidget(centralwidget);
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setFont(0, font);
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName("treeWidget");
        treeWidget->setStyleSheet(QString::fromUtf8("\n"
"        QTreeWidget {\n"
"            background-color: white;\n"
"            border: none;\n"
"            border-radius: 10px;\n"
"            padding: 10px;\n"
"            font-size: 13px;\n"
"            alternate-background-color: #f8fafc;\n"
"        }\n"
"        QTreeWidget::item {\n"
"            padding: 8px;\n"
"            border-bottom: 1px solid #e0e4e8;\n"
"        }\n"
"        QTreeWidget::item:hover {\n"
"            background-color: #f0f4f8;\n"
"        }\n"
"        QTreeWidget::item:selected {\n"
"            background-color: #e1f0fa;\n"
"            color: #2c3e50;\n"
"        }\n"
"        QHeaderView::section {\n"
"            background-color: #f8fafc;\n"
"            padding: 8px;\n"
"            border: none;\n"
"            border-bottom: 2px solid #3498db;\n"
"            font-weight: bold;\n"
"            color: #2c3e50;\n"
"        }\n"
"       "));
        treeWidget->setAlternatingRowColors(true);
        treeWidget->setIndentation(25);
        treeWidget->setExpandsOnDoubleClick(true);
        treeWidget->header()->setVisible(true);
        treeWidget->header()->setMinimumSectionSize(150);
        treeWidget->header()->setDefaultSectionSize(300);
        treeWidget->header()->setStretchLastSection(true);

        gridLayout->addWidget(treeWidget, 2, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        statusbar->setStyleSheet(QString::fromUtf8("\n"
"     QStatusBar {\n"
"         background-color: #2c3e50;\n"
"         color: white;\n"
"         padding: 5px;\n"
"         font-size: 12px;\n"
"     }\n"
"    "));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\320\241\320\270\320\275\321\202\320\260\320\272\321\201\320\270\321\207\320\265\321\201\320\272\320\270\320\271 \320\260\320\275\320\260\320\273\320\270\320\267\320\260\321\202\320\276\321\200", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \321\202\320\265\320\272\321\201\321\202 \320\264\320\273\321\217 \320\260\320\275\320\260\320\273\320\270\320\267\320\260...", nullptr));
        btn_search->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\270\321\201\320\272", nullptr));
        btn_upload->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214", nullptr));
        btn_download->setText(QCoreApplication::translate("MainWindow", "\320\241\320\272\320\260\321\207\320\260\321\202\321\214", nullptr));
        btn_test->setText(QCoreApplication::translate("MainWindow", "\320\242\320\265\321\201\321\202", nullptr));
        c_pod->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\264\320\273\320\265\320\266\320\260\321\211\320\265\320\265", nullptr));
        c_skaz->setText(QCoreApplication::translate("MainWindow", "\320\241\320\272\320\260\320\267\321\203\320\265\320\274\320\276\320\265", nullptr));
        c_opred->setText(QCoreApplication::translate("MainWindow", "\320\236\320\277\321\200\320\265\320\264\320\265\320\273\320\265\320\275\320\270\320\265", nullptr));
        c_dop->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\277\320\276\320\273\320\275\320\265\320\275\320\270\320\265", nullptr));
        c_ob->setText(QCoreApplication::translate("MainWindow", "\320\236\320\261\321\201\321\202\320\276\321\217\321\202\320\265\320\273\321\214\321\201\321\202\320\262\320\276", nullptr));
        c_none->setText(QCoreApplication::translate("MainWindow", "\320\224\321\200\321\203\320\263\320\276\320\265", nullptr));
        c_all->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\261\321\200\320\260\321\202\321\214 \320\262\321\201\320\265", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MainWindow", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202\321\213 \321\201\320\270\320\275\321\202\320\260\320\272\321\201\320\270\321\207\320\265\321\201\320\272\320\276\320\263\320\276 \320\260\320\275\320\260\320\273\320\270\320\267\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
