#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QMap>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QWidget>
#include <QPushButton>
#include "codeeditor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_2_triggered();
    void on_actionExit_triggered();
    void closeTab(int index);
    void setDarkTheme();
    void setLightTheme();
    void on_actionopendota_triggered();
    QString iconFile(QString suffix);

    void on_actionthem_triggered();

private:
    Ui::MainWindow *ui;
    QTabWidget *tabWidget;
    QMap<QString, CodeEditor*> fileMap;
    QMap<CodeEditor*, QString> filePathMap;
    QString baseName;
    QString fileName;
    bool isDarkTheme = false;
    bool isLightTheme = false;

};

#endif // MAINWINDOW_H
