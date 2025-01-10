//отображение номера строк
#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>
#include <QPlainTextEdit>

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(QPlainTextEdit *editor); // Конструктор
    // Возвращает размер области для номеров строк
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPlainTextEdit *codeEditor;
};

#endif // LINENUMBERAREA_H
