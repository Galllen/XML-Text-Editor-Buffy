#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "file.h"
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    File *wnd=  new File(this);
    wnd->show();
    connect(wnd,SIGNAL(filePath(QString)),this,SLOT(openFl(QStirng)));
}


void MainWindow::on_actionSave_triggered()
{
    File *wnd=  new File(this);
    wnd->show();
    connect(wnd,SIGNAL(filePath(QString)),this,SLOT(saveFl(QStirng)));
}


void MainWindow::openFl(const QString &filePath){
    QFile mFile(filePath);
    if(!mFile.open((QFile::ReadOnly|QFile::Text))){
        QMessageBox::information(this,"Error","Error open file");
        ui->statusbar->showMessage("Error open file");
        return;
    }
    QTextStream stream(&mFile);
    QString buffer=stream.readAll();
    ui->textEdit->setText(buffer);
    ui->statusbar->showMessage("Read to file: "+ filePath);

    mFile.flush();//оптимизация памяти(не теряются данные)
    mFile.close();//закрыли поток

}

void MainWindow::saveFl(const QString &filePath){
    QFile mFile(filePath);
    if(!mFile.open((QFile::WriteOnly|QFile::Text))){
        QMessageBox::information(this,"Error","Error save file");
        ui->statusbar->showMessage("Error save file");
        return;
    }
    QTextStream stream(&mFile);
    stream<<ui->textEdit->toPlainText();
    ui->statusbar->showMessage("Write to file"+ filePath);
    mFile.close();//закрыли поток

}
void MainWindow::on_actionExit_triggered()
{
    close();
}

