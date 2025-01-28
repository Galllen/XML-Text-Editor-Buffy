#include "searchdialog.h"
#include <QMessageBox>

#include <QVBoxLayout>

SearchDialog::SearchDialog(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Поиск");

    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText("Введите текст для поиска...");

    searchButton = new QPushButton("Поиск", this);  // Новая кнопка "Поиск"
    findNextButton = new QPushButton("Найти далее", this);
    findPreviousButton = new QPushButton("Найти предыдущее", this);
    closeButton = new QPushButton("Закрыть", this);

    statusLabel = new QLabel("0/0", this); // Метка для отображения статуса

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(searchLineEdit);
    layout->addWidget(searchButton);  // Добавляем кнопку "Поиск"
    layout->addWidget(findNextButton);
    layout->addWidget(findPreviousButton);
    layout->addWidget(statusLabel);
    layout->addWidget(closeButton);

    // Скрываем кнопки "Найти далее" и "Найти предыдущие" по умолчанию
    findNextButton->hide();
    findPreviousButton->hide();

    // Соединяем кнопки с сигналами
    connect(searchButton, &QPushButton::clicked, this, &SearchDialog::performSearch);
    connect(findNextButton, &QPushButton::clicked, this, &SearchDialog::findNextRequested);
    connect(findPreviousButton, &QPushButton::clicked, this, &SearchDialog::findPreviousRequested);
    connect(closeButton, &QPushButton::clicked, this, &SearchDialog::close);
}

QString SearchDialog::getSearchText() const {
    return searchLineEdit->text();
}

void SearchDialog::setStatusText(const QString &text) {
    statusLabel->setText(text);
}
void SearchDialog::performSearch() {
    QString searchText = searchLineEdit->text();
    if (searchText.isEmpty()) {
        QMessageBox::information(this, "Поиск", "Введите текст для поиска.");
        return;
    }

    // Отправляем сигнал для выполнения поиска
    emit searchRequested(searchText);
}
void SearchDialog::hideFindButtons() {
    findNextButton->hide();
    findPreviousButton->hide();
}

void SearchDialog::showFindButtons() {
    findNextButton->show();
    findPreviousButton->show();
}
