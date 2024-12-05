#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QTextStream>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->textEdit);
    ui->statusbar->showMessage("Ok");
    this->setWindowTitle("beefy's");
}

MainWindow::~MainWindow()
{

    delete ui;
}
QString baseName;
void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", ".txt","All files (*.*);;Text files (*.txt);;Configuration files (*.ini *.xml *.json *.yaml *.yml *.toml *.conf *.cfg)");
    if (fileName.isEmpty()) {
        return;
    }
    baseName = QFileInfo(fileName).fileName();
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QString log = file.readAll();
    ui->textEdit->setText(log);
    file.flush();
    file.close();
    this->setWindowTitle(baseName +" beefy's");
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
        QTextStream out(&file);
        out << ui->textEdit->toPlainText();
        file.flush();
        file.close();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

