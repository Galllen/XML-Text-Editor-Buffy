#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class SearchDialog : public QDialog {
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr);
    QString getSearchText() const; // Получить текст для поиска
    void setStatusText(const QString &text);
    void hideFindButtons();
    void showFindButtons();
    void performSearch();

signals:
    void findNextRequested(); // Сигнал для поиска вперед
    void findPreviousRequested(); // Сигнал для поиска назад
    void searchRequested(const QString &text);


private:
    QLineEdit *searchLineEdit;
    QPushButton *findNextButton;
    QPushButton *findPreviousButton;
    QPushButton *closeButton;
    QPushButton *searchButton;
    QLabel *statusLabel;
};

#endif // SEARCHDIALOG_H
