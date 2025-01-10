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
#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>
#include <QPushButton>


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

    QString styleSheet = "QMenuBar{background: qlineargradient(x1:0,x2:0,y1:0,y2:1,stop:0 #cccccc, stop:0.4 gray)} QStatusBar{background: qlineargradient(x1:0,x2:0,y1:0,y2:1,stop:0 #cccccc, stop:0.4 gray);color:white;} ";

    this->setStyleSheet(styleSheet);
    QString Stle = "QMenuBar::item{background-color: qlineargradient(x1:0,x2:0,y1:0,y2:1,stop:0 #cccccc, stop:0.4 gray)}";
    this->setStyleSheet(Stle);
    setLightTheme();
    ui->actionthem->setText("Dark");
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
        return ;
    }
    QString log = file.readAll();
    file.close();
    CodeEditor *codeEditor = new CodeEditor(this);
    codeEditor->setPlainText(log);

    // Выбор иконки в зависимости от расширения файла
    QString suffix = QFileInfo(fileName).suffix();
    QString iconPath = iconFile(suffix);
    int index = tabWidget->addTab(codeEditor, QIcon(iconPath), QFileInfo(fileName).fileName());
    tabWidget->setCurrentIndex(index);
    fileMap[fileName] = codeEditor;
    filePathMap[codeEditor] = fileName;
    tabWidget->setTabToolTip(index, fileName);
    qDebug() << "Selected icon path:" << iconPath;
    baseName = QFileInfo(fileName).fileName();
    this->setWindowTitle(QFileInfo(fileName).fileName() + " beefy's");
    ui->statusbar->showMessage("File opened", 5000);
}

QString MainWindow::iconFile(QString suffix){
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
        iconPath = ":/icons/icons/file.png";
    }
    return iconPath;
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

    CodeEditor *currentCodeEditor = qobject_cast<CodeEditor*>(tabWidget->currentWidget());
    if (!currentCodeEditor) {
        QMessageBox::critical(this, "Error", "No active text editor found.");
        return;
    }

    QTextStream out(&file);
    out << currentCodeEditor->toPlainText();
    file.flush();
    file.close();

    // Обновляем QMap и название вкладки
    QString oldFileName = filePathMap.value(currentCodeEditor);
    if (!oldFileName.isEmpty()) {
        fileMap.remove(oldFileName);
    }
    fileMap[fileName] = currentCodeEditor;
    filePathMap[currentCodeEditor] = fileName;

    // Обновляем название вкладки
    int index = tabWidget->indexOf(currentCodeEditor);
    if (index != -1) {
        tabWidget->setTabText(index, QFileInfo(fileName).fileName());
        tabWidget->setTabToolTip(index, fileName);
    }

    baseName = QFileInfo(fileName).fileName();
    this->setWindowTitle(baseName + " beefy's");
    ui->statusbar->showMessage("File saved", 5000);

}

void MainWindow::on_actionSave_2_triggered()
{
    // Получаем указатель на текущую активную вкладку
    CodeEditor *currentTextEdit = qobject_cast<CodeEditor*>(tabWidget->currentWidget());
    if (!currentTextEdit) {
        QMessageBox::critical(this, "Error", "No active text editor found.");
        return;
    }

    // Ищем имя файла, связанное с текущей вкладкой
    QString fileName = filePathMap.value(currentTextEdit);

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

    ui->statusbar->showMessage("File saved", 10000);
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::closeTab(int index)
{
    CodeEditor *codeEditor = qobject_cast<CodeEditor*>(tabWidget->widget(index));
    if (!codeEditor) {
        return;
    }

    // Проверяем, были ли внесены изменения
    if (codeEditor->document()->isModified()) {
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
        reply = QMessageBox::question(this, "Close Tab", "Are you sure you want to close this file?",
                                      QMessageBox::Yes | QMessageBox::No );
        if (reply == QMessageBox::No) {
            return;
        }
    }

    // Удаляем вкладку и связанные с ней данные
    QString fileName;
    for (auto it = fileMap.begin(); it != fileMap.end(); ++it) {
        if (it.value() == codeEditor) {
            fileName = it.key();
            break;
        }
    }

    if (!fileName.isEmpty()) {
        fileMap.remove(fileName);
         filePathMap.remove(codeEditor);
    }



    tabWidget->removeTab(index);
    delete codeEditor;
}
void MainWindow::setDarkTheme()
{
    QString darkStyle = R"(
        QWidget {
            background-color: #1E2A47; /* Темно-синий */
            color: #FFFFFF;
            font-family: "Segoe UI";
            font-size: 14px;
        }
        QTextEdit, QPlainTextEdit {
            background-color: #0F1A2F; /* Очень темно-синий */
            color: #FFFFFF;
            selection-background-color: #2A3F5F; /* Средний темно-синий */
            selection-color: #FFFFFF;
            border: 1px solid #555555;
        }
        QMenuBar {
            background-color: #1E2A47; /* Темно-синий */
            color: #FFFFFF;
        }
        QMenuBar::item {
            background-color: transparent;
        }
        QMenuBar::item:selected {
            background-color: #2A3F5F; /* Средний темно-синий */
        }
        QMenu {
            background-color: #1E2A47; /* Темно-синий */
            color: #FFFFFF;
            border: 1px solid #555555;
        }
        QMenu::item:selected {
            background-color: #2A3F5F; /* Средний темно-синий */
        }
        QToolBar {
            background-color: #1E2A47; /* Темно-синий */
            border: none;
        }
        QToolButton {
            background-color: transparent;
            color: #FFFFFF;
        }
        QToolButton:hover {
            background-color: #2A3F5F; /* Средний темно-синий */
        }
        QStatusBar {
            background-color: #1E2A47; /* Темно-синий */
            color: #FFFFFF;
        }
        QTabWidget::pane {
            border: 1px solid #555555;
            background-color: #1E2A47; /* Темно-синий */
        }
        QTabBar::tab {
            background-color: #1E2A47; /* Темно-синий */
            color: #FFFFFF;
            border: 1px solid #555555;
            padding: 5px;
        }
        QTabBar::tab:selected {
            background-color: #0F1A2F; /* Очень темно-синий */
        }
    )";

    qApp->setStyleSheet(darkStyle); // Применяем стиль ко всему приложению
}
void MainWindow::setLightTheme()
{
    QString lightStyle = R"(
        QWidget {
            background-color: #F5F5F5; /* Светло-серый */
            color: #000000;
            font-family: "Segoe UI";
            font-size: 14px;
        }
        QTextEdit, QPlainTextEdit {
            background-color: #FFFFFF; /* Белый */
            color: #000000;
            selection-background-color: #ADD8E6; /* Светло-голубой */
            selection-color: #000000;
            border: 1px solid #CCCCCC;
        }
        QMenuBar {
            background-color: #F5F5F5; /* Светло-серый */
            color: #000000;
        }
        QMenuBar::item {
            background-color: transparent;
        }
        QMenuBar::item:selected {
            background-color: #ADD8E6; /* Светло-голубой */
        }
        QMenu {
            background-color: #F5F5F5; /* Светло-серый */
            color: #000000;
            border: 1px solid #CCCCCC;
        }
        QMenu::item:selected {
            background-color: #ADD8E6; /* Светло-голубой */
        }
        QToolBar {
            background-color: #F5F5F5; /* Светло-серый */
            border: none;
        }
        QToolButton {
            background-color: transparent;
            color: #000000;
        }
        QToolButton:hover {
            background-color: #ADD8E6; /* Светло-голубой */
        }
        QStatusBar {
            background-color: #F5F5F5; /* Светло-серый */
            color: #000000;
        }
        QTabWidget::pane {
            border: 1px solid #CCCCCC;
            background-color: #F5F5F5; /* Светло-серый */
        }
        QTabBar::tab {
            background-color: #F5F5F5; /* Светло-серый */
            color: #000000;
            border: 1px solid #CCCCCC;
            padding: 5px;
        }
        QTabBar::tab:selected {
            background-color: #FFFFFF; /* Белый */
        }
    )";

    qApp->setStyleSheet(lightStyle);
}

void MainWindow::on_actionopendota_triggered()
{
    system("start steam://rungameid/570");
}




void MainWindow::on_actionthem_triggered()
{
     if (isDarkTheme) {
        setLightTheme();
        ui->actionthem->setText("Dark");
     } else {
        setDarkTheme();
        ui->actionthem->setText("Light");
     }
     isDarkTheme = !isDarkTheme; // Переключаем состояние
}

