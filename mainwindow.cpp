#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QTabWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    ui->statusbar->showMessage("Ok",5000);
    this->setWindowTitle("beefy's");
}

MainWindow::~MainWindow()
{

    delete ui;
}


QString baseName;
QString fileName;

void MainWindow::on_actionOpen_triggered()
{
    fileName = QFileDialog::getOpenFileName(this, "Open File", ".txt","All files (*.*);;Text files (*.txt);;Configuration files (*.ini *.xml *.json *.yaml *.yml *.toml *.conf *.cfg)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Could not open file for reading.");
        return;
    }
    QString log = file.readAll();
    file.close();

    QTextEdit *textEdit = new QTextEdit(this);
    textEdit->setText(log);
    int index = tabWidget->addTab(textEdit, QFileInfo(fileName).fileName());
    tabWidget->setCurrentIndex(index);

    fileMap[fileName] = textEdit;

    baseName = QFileInfo(fileName).fileName();
    this->setWindowTitle(QFileInfo(fileName).fileName() + " beefy's");
    ui->statusbar->showMessage("File opened",5000);
}



void MainWindow::on_actionSave_triggered()
{

    QString fileName = QFileDialog::getSaveFileName(this, "Save File",baseName);
    if (fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Error", "Could not open file for writing.");
        return;
    }

    QTextEdit *currentTextEdit = qobject_cast<QTextEdit*>(tabWidget->currentWidget());
    if (!currentTextEdit) {
        QMessageBox::critical(this, "Error", "No active text editor found.");
        return;
    }

    QTextStream out(&file);
    out << currentTextEdit->toPlainText();
    file.flush();
    file.close();

}

void MainWindow::on_actionSave_2_triggered()
{
    // Получаем указатель на текущую активную вкладку
    QTextEdit *currentTextEdit = qobject_cast<QTextEdit*>(tabWidget->currentWidget());
    if (!currentTextEdit) {
        QMessageBox::critical(this, "Error", "No active text editor found.");
        return;
    }

    // Ищем имя файла, связанное с текущей вкладкой
    QString fileName;
    for (auto it = fileMap.begin(); it != fileMap.end(); ++it) {
        if (it.value() == currentTextEdit) {
            fileName = it.key();
            break;
        }
    }

    if (fileName.isEmpty()) {
        QMessageBox::critical(this, "Error", "No file associated with the current tab.");
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Error", "Could not open file for writing.");
        return;
    }

    QTextStream out(&file);
    out << currentTextEdit->toPlainText();
    file.flush();
    file.close();

    ui->statusbar->showMessage("File saved", 5000);
}

void MainWindow::on_actionExit_triggered()
{
    close();
}
