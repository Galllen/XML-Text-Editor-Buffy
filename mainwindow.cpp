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
#include <QMimeData>
#include <QUrl>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include "xmlhighlighter.h"
#include <QtXml>
#include <QDomDocument>
#include <QKeyEvent>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , searchDialog(new SearchDialog(this))
    , currentMatchIndex(0)
    , totalMatches(0)
{
    ui->setupUi(this);
    setAcceptDrops(true);

    tabWidget =new CustomTabWidget(this);
    setCentralWidget(tabWidget);


    fonSize=15;
    increaseButton = new QPushButton("+", this);
    TxtSize=new QLabel("100%",this);
    decreaseButton = new QPushButton("-", this);
    validatorButton= new QPushButton("Check", this);
    connect(validatorButton, &QPushButton::clicked,this,&MainWindow::validateXml);
    connect(increaseButton, &QPushButton::clicked, this, &MainWindow::increaseTextSize);
    connect(decreaseButton, &QPushButton::clicked, this, &MainWindow::decreaseTextSize);

    //connect(ui->actionthem, &QAction::triggered, this, &MainWindow::on_actionthem_triggered);



    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(validatorButton);
    buttonLayout->addWidget(decreaseButton);
    buttonLayout->addWidget(TxtSize);
    buttonLayout->addWidget(increaseButton);
    validatorButton->hide();

    // Создаем действия для горячих клавиш
    QAction *saveAction = new QAction("Save", this);
    saveAction->setShortcut(QKeySequence::Save);

    QAction *undoAction = new QAction("Undo", this);
    undoAction->setShortcut(QKeySequence::Undo); // Ctrl+Z
    connect(undoAction, &QAction::triggered, this, &MainWindow::undo);

    QAction *redoAction = new QAction("Redo", this);
    redoAction->setShortcut(QKeySequence::Redo); // Ctrl+Y
    connect(redoAction, &QAction::triggered, this, &MainWindow::redo);


    QAction *searchAction = new QAction("Найти", this);
    searchAction->setShortcut(QKeySequence::Find); // Ctrl+F
    connect(searchAction, &QAction::triggered, this, &MainWindow::openSearchDialog);

    QPushButton *undoButton = new QPushButton("↩", this);
    QPushButton *redoButton = new QPushButton("↪", this);
    connect(undoButton, &QPushButton::clicked, this, &MainWindow::undo);
    connect(redoButton, &QPushButton::clicked, this, &MainWindow::redo);

    buttonLayout->addWidget(undoButton);
    buttonLayout->addWidget(redoButton);

    QMenu *searchMenu = menuBar()->addMenu("Поиск");
    searchMenu->addAction(searchAction);


    QWidget *buttonWidget = new QWidget(this);
    buttonWidget->setLayout(buttonLayout);
    statusBar()->addPermanentWidget(buttonWidget);


    ui->statusbar->showMessage("Ok", 5000);
    this->setWindowTitle("Beefy's");


    connect(searchDialog, &SearchDialog::findNextRequested, this, &MainWindow::findNext);
    connect(searchDialog, &SearchDialog::findPreviousRequested, this, &MainWindow::findPrevious);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(searchDialog, &SearchDialog::searchRequested, this, &MainWindow::performSearch);
    tabWidget->setTabsClosable(true);

    //style
    QString styleSheet = "QMenuBar{background: qlineargradient(x1:0,x2:0,y1:0,y2:1,stop:0 #cccccc, stop:0.4 gray)} QStatusBar{background: qlineargradient(x1:0,x2:0,y1:0,y2:1,stop:0 #cccccc, stop:0.4 gray);color:white;} ";
    this->setStyleSheet(styleSheet);
    QString Stle = "QMenuBar::item{background-color: qlineargradient(x1:0,x2:0,y1:0,y2:1,stop:0 #cccccc, stop:0.4 gray)}";
    this->setStyleSheet(Stle);
    setLightTheme();
    ui->actionthem->setText("Dark");
    int new_size=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    fileName = QFileDialog::getOpenFileName(this, "Open File", ".txt","All files (*.*);;Text files (*.txt);;Configuration files (*.ini *.xml *.json *.yaml *.yml *.toml *.conf *.cfg)");
    OpenFile();
}


void MainWindow::OpenFile(){
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

    QString suffix = QFileInfo(fileName).suffix();
    QString iconPath = iconFile(suffix);

    int index = tabWidget->addTab(codeEditor, QIcon(iconPath), QFileInfo(fileName).fileName());
    tabWidget->setCurrentIndex(index);
    fileMap[fileName] = codeEditor;
    filePathMap[codeEditor] = fileName;
    tabWidget->setTabToolTip(index, fileName);

    baseName = QFileInfo(fileName).fileName();

    this->setWindowTitle(QFileInfo(fileName).fileName() + " - beefy's");
    ui->statusbar->showMessage("File opened", 5000);

    if (suffix == "xml") {
        new XmlHighlighter(codeEditor->document());
        validatorButton->show();
    }
    newSize=100;
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

    QString oldFileName = filePathMap.value(currentCodeEditor);
    if (!oldFileName.isEmpty()) {
        fileMap.remove(oldFileName);
    }
    fileMap[fileName] = currentCodeEditor;
    filePathMap[currentCodeEditor] = fileName;

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
    qDebug() << "Save action triggered";
    CodeEditor *currentTextEdit = qobject_cast<CodeEditor*>(tabWidget->currentWidget());
    if (!currentTextEdit) {
        QMessageBox::critical(this, "Error", "No active text editor found.");
        return;
    }
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
            font-size: %1px;
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
    darkStyle = darkStyle.arg(fonSize);
    qApp->setStyleSheet(darkStyle); // Применяем стиль ко всему приложению
    qDebug("set dark");
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
            font-size: %1px;
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
    lightStyle = lightStyle.arg(fonSize);
    qApp->setStyleSheet(lightStyle);
    qDebug("set lig");

}


void MainWindow::on_actionthem_triggered()
{
    qDebug("swth");
    if (isDarkTheme) {
        qDebug("lig");
        setLightTheme();
        ui->actionthem->setText("Dark");
     }
    else
    {
        qDebug("dark ch");
        setDarkTheme();
        qDebug("chngd");
        ui->actionthem->setText("Light");
     }
    isDarkTheme = !isDarkTheme;
}

void MainWindow::increaseTextSize() {
    fonSize++;
    newSize=newSize+10;
    if (isDarkTheme) {
        setDarkTheme();
    } else {
        setLightTheme();
    }
    TxtSize->setText(QString::number(newSize)+"%");
}
void MainWindow::decreaseTextSize() {
    if(fonSize<11)return;
    fonSize--;
    newSize=newSize-10;
    if (isDarkTheme) {
        setDarkTheme();
    } else {
        setLightTheme();
    }
    TxtSize->setText(QString::number(newSize)+"%");
}

void MainWindow::undo() {
    CodeEditor *currentCodeEditor = qobject_cast<CodeEditor*>(tabWidget->currentWidget());
    if (currentCodeEditor) {
        currentCodeEditor->undo();
    }
}

void MainWindow::redo() {
    CodeEditor *currentCodeEditor = qobject_cast<CodeEditor*>(tabWidget->currentWidget());
    if (currentCodeEditor) {
        currentCodeEditor->redo();
    }
}

bool MainWindow::isValidXml(const QString &xml) {
    QDomDocument doc;
    QString errorMsg;
    int errorLine, errorColumn;

    // Пытаемся загрузить XML
    if (!doc.setContent(xml, &errorMsg, &errorLine, &errorColumn)) {
        // Если XML невалиден, выводим сообщение об ошибке
        QMessageBox::critical(this, "XML Validation Error",
                              QString("Error at line %1, column %2: %3")
                                  .arg(errorLine)
                                  .arg(errorColumn)
                                  .arg(errorMsg));
        return false;
    }
    return true;
}
void MainWindow::validateXml() {
    // Получаем текущий редактор
    CodeEditor *currentCodeEditor = qobject_cast<CodeEditor*>(tabWidget->currentWidget());
    if (!currentCodeEditor) {
        QMessageBox::critical(this, "Error", "No active text editor found.");
        return;
    }

    // Получаем текст из редактора
    QString xmlContent = currentCodeEditor->toPlainText();

    // Проверяем валидность XML
    if (isValidXml(xmlContent)) {
        QMessageBox::information(this, "Validation", "XML is valid.");
    } else {
        QMessageBox::critical(this, "Validation", "XML is not valid.");
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_F) {
        openSearchDialog();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}
void MainWindow::openSearchDialog() {
    searchDialog->show();
    searchDialog->activateWindow(); // Активируем окно
}

void MainWindow::findNext() {
    QString searchText = searchDialog->getSearchText();
    if (searchText.isEmpty()) {
        QMessageBox::information(this, "Поиск", "Введите текст для поиска.");
        return;
    }

    CodeEditor *currentCodeEditor = qobject_cast<CodeEditor*>(tabWidget->currentWidget());
    if (!currentCodeEditor) {
        QMessageBox::critical(this, "Ошибка", "Нет активного редактора.");
        return;
    }

    QTextDocument *document = currentCodeEditor->document();
    QTextCursor cursor = currentCodeEditor->textCursor();

    cursor = document->find(searchText, cursor, QTextDocument::FindCaseSensitively);
    if (totalMatches == 0) {
        QMessageBox::information(this, "Поиск", "Нет совпадений.");
        return;
    }
    if (cursor.isNull()) {
        QMessageBox::information(this, "Поиск", "Достигнут конец документа.");
        currentMatchIndex = 0; // Сброс индекса
    } else {

        currentCodeEditor->setTextCursor(cursor);
        currentMatchIndex = (currentMatchIndex + 1) % totalMatches; // Обновляем индекс
    }

    highlightAllOccurrences(searchText); // Подсветка всех вхождений
    updateMatchPosition(); // Обновление положения
}

void MainWindow::findPrevious() {
    QString searchText = searchDialog->getSearchText();
    if (searchText.isEmpty()) {
        QMessageBox::information(this, "Поиск", "Введите текст для поиска.");
        return;
    }

    CodeEditor *currentCodeEditor = qobject_cast<CodeEditor*>(tabWidget->currentWidget());
    if (!currentCodeEditor) {
        QMessageBox::critical(this, "Ошибка", "Нет активного редактора.");
        return;
    }

    QTextDocument *document = currentCodeEditor->document();
    QTextCursor cursor = currentCodeEditor->textCursor();

    cursor = document->find(searchText, cursor, QTextDocument::FindBackward | QTextDocument::FindCaseSensitively);
    if (totalMatches == 0) {
        QMessageBox::information(this, "Поиск", "Нет совпадений.");
        return;
    }
    if (cursor.isNull()) {
        QMessageBox::information(this, "Поиск", "Достигнуто начало документа.");
        currentMatchIndex = totalMatches - 1; // Переход к последнему вхождению
    } else {
        currentCodeEditor->setTextCursor(cursor);
        currentMatchIndex = (currentMatchIndex - 1 + totalMatches) % totalMatches; // Обновляем индекс
    }

    highlightAllOccurrences(searchText); // Подсветка всех вхождений
    updateMatchPosition(); // Обновление положения
}

void MainWindow::updateMatchPosition() {
    if (totalMatches > 0) {
        searchDialog->setStatusText(QString("%1/%2").arg(currentMatchIndex + 1).arg(totalMatches));
    } else {
        searchDialog->setStatusText("0/0");
    }
}
void MainWindow::highlightAllOccurrences(const QString &text) {
    CodeEditor *currentCodeEditor = qobject_cast<CodeEditor*>(tabWidget->currentWidget());
    if (!currentCodeEditor) {
        return;
    }

    QTextDocument *document = currentCodeEditor->document();
    QTextCursor cursor(document);

    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection selection;

    selection.format.setBackground(Qt::yellow);

    cursor = document->find(text, cursor, QTextDocument::FindCaseSensitively);
    totalMatches = 0; // Сбрасываем счетчик

    while (!cursor.isNull()) {
        selection.cursor = cursor;
        extraSelections.append(selection);
        cursor = document->find(text, cursor, QTextDocument::FindCaseSensitively);
        totalMatches++; // Увеличиваем счетчик найденных слов
    }

    currentCodeEditor->setExtraSelections(extraSelections);
}
void MainWindow::performSearch(const QString &text) {
    if (text.isEmpty()) {
        QMessageBox::information(this, "Поиск", "Введите текст для поиска.");
        return;
    }

    // Подсвечиваем все совпадения и обновляем счетчик
    highlightAllOccurrences(text);

    if (totalMatches == 0) {
        QMessageBox::information(this, "Поиск", "Ничего не найдено.");
        searchDialog->setStatusText("0/0");
        searchDialog->hideFindButtons();  // Скрываем кнопки "Найти далее" и "Найти предыдущие"
    } else {
        searchDialog->setStatusText(QString("%1/%2").arg(currentMatchIndex + 1).arg(totalMatches));
        searchDialog->showFindButtons();  // Показываем кнопки "Найти далее" и "Найти предыдущие"
    }
}
