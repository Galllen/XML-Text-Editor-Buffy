#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class XmlHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    XmlHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat tagFormat;
    QTextCharFormat attributeFormat;
    QTextCharFormat commentFormat;
};
