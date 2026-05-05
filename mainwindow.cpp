#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>

// Конструктор главного окна. Инициализирует UI, настраивает виджеты и подготавливает Python процесс
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pythonProcess(nullptr)
{
    lastDirectory = QDir::homePath();
    ui->setupUi(this);

    analyzedText = "";
    // Подключаем сигнал изменения текста к слоту onTextEdited
    connect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::onTextEdited);

    // Отключаем фокус у главного окна и всех виджетов для чистого визуального стиля
    setFocusPolicy(Qt::NoFocus);

    // Отключаем фокус у всех виджетов-фильтров
    ui->filterFrame->setFocusPolicy(Qt::NoFocus);
    ui->c_all->setFocusPolicy(Qt::NoFocus);
    ui->c_pod->setFocusPolicy(Qt::NoFocus);
    ui->c_skaz->setFocusPolicy(Qt::NoFocus);
    ui->c_opred->setFocusPolicy(Qt::NoFocus);
    ui->c_dop->setFocusPolicy(Qt::NoFocus);
    ui->c_ob->setFocusPolicy(Qt::NoFocus);
    ui->c_none->setFocusPolicy(Qt::NoFocus);

    // Отключаем фокус у кнопок
    ui->btn_search->setFocusPolicy(Qt::NoFocus);
    ui->btn_upload->setFocusPolicy(Qt::NoFocus);
    ui->btn_download->setFocusPolicy(Qt::NoFocus);

    // Отключаем фокус у дерева результатов и отключаем выделение элементов
    ui->treeWidget->setFocusPolicy(Qt::NoFocus);
    ui->treeWidget->setSelectionMode(QAbstractItemView::NoSelection);

    // Отключаем фокус у вкладок и их панели
    ui->tabWidget->setFocusPolicy(Qt::NoFocus);
    ui->tabWidget->tabBar()->setFocusPolicy(Qt::NoFocus);

    // Отключаем фокус у таблицы частотности слов
    ui->tableWordStats->setFocusPolicy(Qt::NoFocus);

    QTableWidget *tableFreq = ui->tableWordStats;
    if (tableFreq) {
        // Растягиваем колонки таблицы на всю доступную ширину
        tableFreq->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }

    setupPythonProcess(); // Инициализируем процесс для вызова Python скрипта
    ui->btn_download->setEnabled(false); // Кнопка сохранения недоступна до анализа

    ui->treeWidget->setHeaderHidden(true); // Скрываем заголовок дерева результатов

    // Показываем плейсхолдеры (сообщения "Нет данных") на всех вкладках
    showPlaceholderInResults();
    showPlaceholderStatistics();
    showPlaceholderWordFreq();
}

// Деструктор. Завершает Python процесс если он ещё работает и удаляет UI
MainWindow::~MainWindow()
{
    if (pythonProcess && pythonProcess->state() == QProcess::Running) {
        pythonProcess->terminate(); // Отправляем сигнал завершения
        pythonProcess->waitForFinished(3000); // Ждём 3 секунды
    }
    delete ui;
}
