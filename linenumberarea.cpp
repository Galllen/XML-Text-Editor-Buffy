#include "linenumberarea.h"
#include "codeeditor.h" // Подключаем заголовочный файл CodeEditor

LineNumberArea::LineNumberArea(QPlainTextEdit *editor) : QWidget(editor), codeEditor(editor)
{}

QSize LineNumberArea::sizeHint() const
{
    // Приводим codeEditor к типу CodeEditor* и вызываем lineNumberAreaWidth
    CodeEditor *editor = qobject_cast<CodeEditor*>(codeEditor);
    if (editor) {
        return QSize(editor->lineNumberAreaWidth(), 0);
    }
    return QSize(0, 0); // Возвращаем размер по умолчанию, если приведение не удалось
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    // Приводим codeEditor к типу CodeEditor* и вызываем lineNumberAreaPaintEvent
    CodeEditor *editor = qobject_cast<CodeEditor*>(codeEditor);
    if (editor) {
        editor->lineNumberAreaPaintEvent(event);
    }
}
