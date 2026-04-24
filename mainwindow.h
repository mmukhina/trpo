#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QTextBrowser>
#include <QMap>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QFrame>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct WordInfo {
    QString text;
    QString speech;
    QString sentence;
    int sentenceNum;
    QString posRussian;
};

struct Statistics {
    int podlezhaschee = 0;
    int skazuemoe = 0;
    int opredelenie = 0;
    int dopolnenie = 0;
    int obstoyatelstvo = 0;
    int drugoe = 0;

    QMap<int, QList<QString>> podlezhascheeSentences;
    QMap<int, QList<QString>> skazuemoeSentences;
    QMap<int, QList<QString>> opredelenieSentences;
    QMap<int, QList<QString>> dopolnenieSentences;
    QMap<int, QList<QString>> obstoyatelstvoSentences;
    QMap<int, QList<QString>> drugoeSentences;
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
    void handlePythonOutput();
    void handlePythonError();
    void handlePythonFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    Ui::MainWindow *ui;
    QProcess *pythonProcess;
    QString currentTempFile;
    QString currentFileName;
    QString lastDirectory;
    QMap<int, QString> sentenceTexts;
    QMap<int, QList<WordInfo>> wordsBySentence;
    QMap<QString, QMap<QString, int>> wordRoleStats;
    Statistics stats;

    void calculateWordRoleStats();
    void updateWordRoleDisplay();

    void setupPythonProcess();
    void updateAllCheckboxState();
    QString createTempFileWithText(const QString& text);
    bool loadFile(const QString& filePath);
    void processPythonOutput(const QString& output);
    bool shouldShowWord(const WordInfo& word);
    void updateDisplay();
    void runAnalysis(const QString& text);
    void generateHtmlReport();
    bool callNatasha(const QString path);
    bool hasAnyLetter(const QString& text);
    bool hasOnlyNumbers(const QString& text);
    void calculateStatistics();
    void updateStatisticsDisplay();
    void showPlaceholderStatistics();
    void showPlaceholderInResults();
    void showResultsPage();
    bool isUtf8File(const QString& filePath);
};

#endif // MAINWINDOW_H
