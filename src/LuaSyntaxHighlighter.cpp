#include "LuaSyntaxHighlighter.hpp"

// see https://www.lua.org/manual/5.4/manual.html#3.1
// for list of keywords
/*
     and       break     do        else      elseif    end
     false     for       function  goto      if        in
     local     nil       not       or        repeat    return
     then      true      until     while

*/
LuaSyntaxHighlighter::LuaSyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywordPatterns[] = {
        QStringLiteral("\\band\\b"), QStringLiteral("\\bbreak\\b"), QStringLiteral("\\bdo\\b"),
        QStringLiteral("\\belse\\b"), QStringLiteral("\\belseif\\b"), QStringLiteral("\\bend\\b"),
        QStringLiteral("\\bfalse\\b"), QStringLiteral("\\bfor\\b"), QStringLiteral("\\bfunction\\b"),
        QStringLiteral("\\bgoto\\b"), QStringLiteral("\\bif\\b"), QStringLiteral("\\bin\\b"),
        QStringLiteral("\\blocal\\b"), QStringLiteral("\\bnil\\b"), QStringLiteral("\\bnot\\b"),
        QStringLiteral("\\bor\\b"), QStringLiteral("\\brepeat\\b"), QStringLiteral("\\breturn\\b"),
        QStringLiteral("\\bthen\\b"), QStringLiteral("\\btrue\\b"), QStringLiteral("\\buntil\\b"),
        QStringLiteral("\\bwhile\\b")
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
    rule.format = classFormat;
    highlightingRules.append(rule);

    // lua strings can use single or double quotes
    // TODO: this matches more than one quotations as one, if they are on a single line
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("(\".*\")|('.*')"));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    multiLineQuotationFormat.setForeground(Qt::yellow);

    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression(QStringLiteral("--[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegularExpression(QStringLiteral("^--\\[\\["));
    commentEndExpression = QRegularExpression(QStringLiteral("\\]\\]"));

    quoteStartExpression = QRegularExpression(QStringLiteral("\\[\\["));
    quoteEndExpression = QRegularExpression(QStringLiteral("\\]\\]"));
}

void LuaSyntaxHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);


    int quoteStartIndex = 0;
    if (previousBlockState() != 2)
        quoteStartIndex = text.indexOf(quoteStartExpression);

    while (quoteStartIndex >= 0) {
        QRegularExpressionMatch match = quoteEndExpression.match(text, quoteStartIndex);
        int quoteEndIndex = match.capturedStart();
        int quoteLength = 0;
        if (quoteEndIndex == -1) {
            setCurrentBlockState(2);
            quoteLength = text.length() - quoteStartIndex;
        } else {
            quoteLength = quoteEndIndex - quoteStartIndex
                            + match.capturedLength();
        }
        setFormat(quoteStartIndex, quoteLength, multiLineQuotationFormat);
        quoteStartIndex = text.indexOf(quoteStartExpression, quoteStartIndex + quoteLength);
    }

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }

}
