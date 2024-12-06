#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QTabWidget>
#include <QIcon>
#include <QTabBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    ui->statusbar->showMessage("Ok", 5000);
    this->setWindowTitle("beefy's");

    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    tabWidget->setTabsClosable(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_triggered()
{
    fileName = QFileDialog::getOpenFileName(this, "Open File", ".txt","All files (*.*);;Text files (*.txt);;Configuration files (*.ini *.xml *.json *.yaml *.yml *.toml *.conf *.cfg)");
    if (fileName.isEmpty()) {
        return;
    }
    if (fileMap.contains(fileName)) {
        int index = tabWidget->indexOf(fileMap[fileName]);
        tabWidget->setCurrentIndex(index);
        QMessageBox::warning(this,"Error","File already opened");
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

      // Выбор иконки в зависимости от расширения файла
    QString suffix = QFileInfo(fileName).suffix();
    QString iconPath;
    if (suffix == "txt") {
        iconPath = ":/icons/icons/txt.png";
    } else if (suffix == "html") {
        iconPath = ":/icons/icons/html.png";
    } else if (suffix == "json") {
        iconPath = ":/icons/icons/json.png";
    } else if (suffix == "xml") {
        iconPath = ":/icons/icons/xml.png";
    } else if (suffix == "ini") {
        iconPath = ":/icons/icon/ini.png";
    } else if (suffix == "cfg") {
        iconPath = ":/icons/icons/cfg.png";
    } else {
        iconPath = ":/icons/icons/default.png"; // Иконка по умолчанию, если расширение неизвестно
    }

    int index = tabWidget->addTab(textEdit, QIcon(iconPath), QFileInfo(fileName).fileName());
    tabWidget->setCurrentIndex(index);
    fileMap[fileName] = textEdit;
    qDebug() << "Selected icon path:" << iconPath;
    baseName = QFileInfo(fileName).fileName();
    this->setWindowTitle(QFileInfo(fileName).fileName() + " beefy's");
    ui->statusbar->showMessage("File opened", 5000);
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", baseName);
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

    ui->statusbar->showMessage("File saved", 5000);
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

void MainWindow::closeTab(int index)
{
    QTextEdit *textEdit = qobject_cast<QTextEdit*>(tabWidget->widget(index));
    if (!textEdit) {
        return;
    }

    // Проверяем, были ли внесены изменения
    if (textEdit->document()->isModified()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Save Changes","Do you want to save changes before closing?",
                                      QMessageBox::Save |QMessageBox::Discard| QMessageBox::Cancel);
        if (reply == QMessageBox::Save) {
            on_actionSave_2_triggered();
        } else if (reply == QMessageBox::Cancel) {
            return;
        }
    }
    else{
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Close Tab", "Are you sure you want to close this tab?",
                                      QMessageBox::Yes | QMessageBox::No );
        if (reply == QMessageBox::No) {
            return;
        }
    }


    // Удаляем вкладку и связанные с ней данные
    QString fileName;
    for (auto it = fileMap.begin(); it != fileMap.end(); ++it) {
        if (it.value() == textEdit) {
            fileName = it.key();
            break;
        }
    }

    if (!fileName.isEmpty()) {
        fileMap.remove(fileName);
    }

    tabWidget->removeTab(index);
    delete textEdit;
}

void MainWindow::on_actionopendota_triggered()
{
    system("start steam://rungameid/570");
}

