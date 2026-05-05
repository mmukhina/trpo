#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>

// Изменение состояния чекбокса "Подлежащее"
void MainWindow::on_c_pod_stateChanged(int)
{
    updateAllCheckboxState(); // Обновляем состояние чекбокса "Выбрать все"
    refreshAllDisplay();      // Обновляем отображение результатов
}

// Изменение состояния чекбокса "Сказуемое"
void MainWindow::on_c_skaz_stateChanged(int)
{
    updateAllCheckboxState();
    refreshAllDisplay();
}

// Изменение состояния чекбокса "Определение"
void MainWindow::on_c_opred_stateChanged(int)
{
    updateAllCheckboxState();
    refreshAllDisplay();
}

// Изменение состояния чекбокса "Дополнение"
void MainWindow::on_c_dop_stateChanged(int)
{
    updateAllCheckboxState();
    refreshAllDisplay();
}

// Изменение состояния чекбокса "Обстоятельство"
void MainWindow::on_c_ob_stateChanged(int)
{
    updateAllCheckboxState();
    refreshAllDisplay();
}

// Изменение состояния чекбокса "Другое"
void MainWindow::on_c_none_stateChanged(int)
{
    updateAllCheckboxState();
    refreshAllDisplay();
}

// Изменение состояния чекбокса "Выбрать все"
void MainWindow::on_c_all_stateChanged(int arg1)
{
    // Блокируем сигналы всех чекбоксов, чтобы избежать рекурсивных вызовов
    ui->c_pod->blockSignals(true);
    ui->c_skaz->blockSignals(true);
    ui->c_opred->blockSignals(true);
    ui->c_dop->blockSignals(true);
    ui->c_ob->blockSignals(true);
    ui->c_none->blockSignals(true);
    ui->c_all->blockSignals(true);

    if (arg1 == Qt::Checked) {
        // Если "Выбрать все" отмечен - включаем все чекбоксы
        ui->c_pod->setChecked(true);
        ui->c_skaz->setChecked(true);
        ui->c_opred->setChecked(true);
        ui->c_dop->setChecked(true);
        ui->c_ob->setChecked(true);
        ui->c_none->setChecked(true);
        ui->c_all->setCheckState(Qt::Checked);
    }
    else if (arg1 == Qt::Unchecked) {
        // Если "Выбрать все" не отмечен - выключаем все чекбоксы
        ui->c_pod->setChecked(false);
        ui->c_skaz->setChecked(false);
        ui->c_opred->setChecked(false);
        ui->c_dop->setChecked(false);
        ui->c_ob->setChecked(false);
        ui->c_none->setChecked(false);
        ui->c_all->setCheckState(Qt::Unchecked);
    }

    // Разблокируем сигналы
    ui->c_pod->blockSignals(false);
    ui->c_skaz->blockSignals(false);
    ui->c_opred->blockSignals(false);
    ui->c_dop->blockSignals(false);
    ui->c_ob->blockSignals(false);
    ui->c_none->blockSignals(false);
    ui->c_all->blockSignals(false);

    refreshAllDisplay();  // Обновляем отображение
    updateButtonsState(); // Обновляем состояние кнопок
}

// Нажатие кнопки "Поиск"
void MainWindow::on_btn_search_clicked()
{
    // Проверяем, выбран ли хотя бы один фильтр
    if (!isAnyFilterSelected()) {
        QMessageBox::warning(this, "Нет активных фильтров",
                             "Не выбран ни один фильтр для отображения слов.\n\n"
                             "Пожалуйста, выберите хотя бы одну категорию.",
                             QMessageBox::Ok);
        return;
    }

    QString searchText = ui->textEdit->toPlainText();

    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "Пустой поиск",
                             "Пожалуйста, введите текст для анализа.",
                             QMessageBox::Ok);
        return;
    }

    runAnalysis(searchText); // Запускаем анализ
}

// Нажатие кнопки "Загрузить" (загрузка файла)
void MainWindow::on_btn_upload_clicked()
{
    // Предупреждение о требовании к кодировке файла
    QMessageBox::information(this, "Требование к файлу",
                             "Внимание!\n\n"
                             "Программа работает только с текстовыми файлами в кодировке UTF-8.\n"
                             "Убедитесь, что ваш файл сохранен именно в этой кодировке.\n\n",
                             QMessageBox::Ok);

    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Выберите текстовый файл",
                                                    lastDirectory,
                                                    "Текстовые файлы (*.txt)");

    if (filePath.isEmpty()) return;

    // Проверяем кодировку файла
    if (!isUtf8File(filePath)) {
        QMessageBox::critical(this, "Ошибка кодировки",
                              "Выбранный файл не соответствует формату UTF-8 или поврежден.\n"
                              "Пожалуйста, сохраните файл в кодировке UTF-8 и попробуйте снова.",
                              QMessageBox::Ok);
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл: " + filePath);
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    QString content = in.readAll();
    file.close();

    // Проверка содержимого файла на пустоту
    if (content.isEmpty()) {
        QMessageBox::warning(this, "Пустой файл",
                             "Файл пуст. Пожалуйста, выберите файл с текстом для анализа.",
                             QMessageBox::Ok);
        return;
    }

    // Проверка содержимого файла на наличие букв
    if (!hasAnyLetter(content)) {
        QMessageBox::warning(this, "Нет букв",
                             "Файл не содержит ни одной буквы.\n"
                             "Пожалуйста, выберите файл с текстом, содержащим буквы для анализа.",
                             QMessageBox::Ok);
        return;
    }

    // Проверка содержимого файла на наличие только цифр
    if (hasOnlyNumbers(content)) {
        QMessageBox::warning(this, "Только цифры",
                             "Файл содержит только цифры.\n"
                             "Пожалуйста, выберите файл с текстом, содержащим буквы для анализа.",
                             QMessageBox::Ok);
        return;
    }

    // Отображаем содержимое файла в текстовом поле
    ui->textEdit->setPlainText(content);
    currentFileName = QFileInfo(filePath).fileName();
    lastDirectory = QFileInfo(filePath).absolutePath();
    statusBar()->showMessage("Загружен файл: " + currentFileName, 3000);

    // Спрашиваем, запускать ли анализ сразу
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Запустить анализ?",
        "Файл загружен. Запустить синтаксический анализ сейчас?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        runAnalysis(ui->textEdit->toPlainText());
    }
}

// Нажатие кнопки "Сохранить"
void MainWindow::on_btn_download_clicked() {
    if (sentenceTexts.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Сначала проведите анализ текста.");
        return;
    }
    generateHtmlReport(); // Генерируем HTML отчет
}

// Изменение текста в редакторе
void MainWindow::onTextEdited()
{
    // Если текущий текст не совпадает с проанализированным, блокируем кнопку сохранения
    if (ui->textEdit->toPlainText() != analyzedText) {
        ui->btn_download->setEnabled(false);
    }
}
