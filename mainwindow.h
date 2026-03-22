#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QTreeWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Структура для хранения информации о слове
struct WordInfo {
    QString text;
    QString pos;
    QString relation;
    QString headWord;
    int sentenceNum;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_c_all_stateChanged(int arg1);
    void on_c_pod_stateChanged(int arg1);
    void on_c_skaz_stateChanged(int arg1);
    void on_c_opred_stateChanged(int arg1);
    void on_c_dop_stateChanged(int arg1);
    void on_c_ob_stateChanged(int arg1);
    void on_c_none_stateChanged(int arg1);
    void on_btn_search_clicked();
    void on_btn_upload_clicked();
    void on_btn_download_clicked();
    void on_btn_test_clicked();

    void handlePythonOutput();
    void handlePythonError();
    void handlePythonFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    Ui::MainWindow *ui;
    QProcess *pythonProcess;
    QString currentTempFile;
    QString currentFileName;  // Имя текущего файла для отображения

    // Данные от Python
    QMap<int, QString> sentenceTexts;  // номер предложения -> текст
    QMap<int, QList<WordInfo>> wordsBySentence;  // номер предложения -> список слов

    void setupPythonProcess();
    void updateAllCheckboxState();
    QString createTempFileWithText(const QString& text);
    bool loadFile(const QString& filePath);  // Загрузка файла
    void processPythonOutput(const QString& output);
    QString getRelationDescription(const QString& relation);
    QString getRelationType(const QString& relation);
    bool shouldShowWord(const WordInfo& word);
    void updateDisplay();
    void setupTestData();
    void runAnalysis(const QString& text);  // Запуск анализа для текста
    void generateHtmlReport();
};

#endif // MAINWINDOW_H
