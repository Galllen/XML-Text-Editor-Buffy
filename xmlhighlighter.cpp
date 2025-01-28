#include "xmlhighlighter.h"

XmlHighlighter::XmlHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {
    // Формат для тегов
    tagFormat.setForeground(Qt::darkYellow);
    tagFormat.setFontWeight(QFont::Bold);
    highlightingRules.append({ QRegularExpression("<[^>]*>"), tagFormat });

    // Формат для атрибутов
    attributeFormat.setForeground(Qt::darkMagenta);
    highlightingRules.append({ QRegularExpression("\\b\\w+\\s*=\\s*\".*\""), attributeFormat });

    // Формат для комментариев
    commentFormat.setForeground(Qt::gray);
    highlightingRules.append({ QRegularExpression("<!--[^>]*-->"), commentFormat });
}

void XmlHighlighter::highlightBlock(const QString &text) {
    for (const HighlightingRule &rule : highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
