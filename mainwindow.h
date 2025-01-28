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
#include <QDrag>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QDragLeaveEvent>
#include <QUrl>
#include <QMimeData>
#include <QFile>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include "searchdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void OpenFile();


private slots:
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_2_triggered();
    void on_actionExit_triggered();
    void closeTab(int index);
    void performSearch(const QString &text);
    void setDarkTheme();
    void setLightTheme();
    QString iconFile(QString suffix);
    void on_actionthem_triggered();
    void increaseTextSize();
    void decreaseTextSize();
    void undo();
    void redo();
    void validateXml();
    bool isValidXml(const QString &xml);
    void keyPressEvent(QKeyEvent *event) override;
    void openSearchDialog(); // Открыть окно поиска
    void findNext(); // Поиск вперед
    void findPrevious();
    void updateMatchPosition();
    void highlightAllOccurrences(const QString &text);
private:

    Ui::MainWindow *ui;
    QTabWidget *tabWidget;
    QLineEdit *searchLineEdit; // Поле для ввода поискового запроса
    QPushButton *findNextButton; // Кнопка "Найти далее"
    QPushButton *findPreviousButton;
    SearchDialog *searchDialog;
    QMap<QString, CodeEditor*> fileMap;
    QMap<CodeEditor*, QString> filePathMap;

    QString baseName;

    int currentMatchIndex; // Текущий индекс найденного слова
    int totalMatches;
    bool isDarkTheme=false;
    bool isLightTheme;

    bool readFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QTabWidget *castumtabWidget;
    QLabel *TxtSize;
    QPushButton *increaseButton;
    QPushButton *decreaseButton;
    QPushButton *validatorButton;
    int textSizePercent;

    void updateAllTabsFontSize();
};


inline QString fileName;
inline int fonSize,newSize;


class CustomTabWidget : public QTabWidget {
    Q_OBJECT
public:
    CustomTabWidget(QWidget *parent = nullptr) : QTabWidget(parent) {
        setAcceptDrops(true); // Разрешаем перетаскивание
    }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override {
        if (event->mimeData()->hasUrls()) {
            event->acceptProposedAction();
        } else {
            event->ignore();
        }
    }

    void dropEvent(QDropEvent *event) override {
        if (!event->mimeData()->hasUrls()) {
            event->ignore();
            return;
        }

        MainWindow *mainWindow = qobject_cast<MainWindow *>(this->window());

        const QList<QUrl> urls = event->mimeData()->urls();
        for (const QUrl &url : urls) {
            QString filePath = url.toLocalFile();
            if (mainWindow) {
                fileName = filePath;
                mainWindow->OpenFile(); // Вызываем метод загрузки файла
            }
        }

        event->acceptProposedAction();
    }
};


#endif // MAINWINDOW_H
