#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QMap>
#include <QTextEdit>

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

    void on_actionopendota_triggered();

private:
    Ui::MainWindow *ui;
    QTabWidget *tabWidget;
    QMap<QString, QTextEdit*> fileMap;
    QString baseName;
    QString fileName;
};

#endif // MAINWINDOW_H
