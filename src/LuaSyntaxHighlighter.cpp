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

    // identifiers
    identifierFormat.setFontWeight(QFont::Bold);
    identifierFormat.setForeground(Qt::magenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[_A-Za-z][_\\-A-Za-z0-9]*\\b"));
    rule.format = identifierFormat;
    highlightingRules.append(rule);

    // keywords
    keywordFormat.setForeground(Qt::white);
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywordPatterns[] = {
        QStringLiteral("\\band\\b"), QStringLiteral("\\bbreak\\b"), QStringLiteral("\\bdo\\b"),
        QStringLiteral("\\belse\\b"), QStringLiteral("\\belseif\\b"), QStringLiteral("\\bend\\b"),
        QStringLiteral("\\bfalse\\b"), QStringLiteral("\\bfor\\b"), QStringLiteral("\\bfunction\\b"),
        QStringLiteral("\\bgoto\\b"), QStringLiteral("\\bif\\b"), QStringLiteral("\\bin\\b"),
        QStringLiteral("\\blocal\\b"), QStringLiteral("\\bnil\\b"), QStringLiteral("\\bnot\\b"),
        QStringLiteral("\\bor\\b"), QStringLiteral("\\brepeat\\b"), QStringLiteral("\\breturn\\b"),
        QStringLiteral("\\bthen\\b"), QStringLiteral("\\btrue\\b"), QStringLiteral("\\buntil\\b"),
        QStringLiteral("\\bwhile\\b")};
    for (const QString &pattern : keywordPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // functions
    functionFormat.setFontWeight(QFont::Bold);
    functionFormat.setForeground(Qt::yellow);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // lua strings can use single or double quotes
    quotationFormat.setFontItalic(true);
    quotationFormat.setForeground(Qt::yellow);
    // regex matches anything in between quotes which does not have a quote inside it.
    // the regext has two parts one for single quotes pair another for double quotes pair.
    rule.pattern = QRegularExpression(QStringLiteral("(\"[^\"]*\")|('[^']*')"));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    multiLineQuotationFormat.setFontItalic(true);
    multiLineQuotationFormat.setForeground(Qt::yellow);

    singleLineCommentFormat.setForeground(Qt::cyan);
    rule.pattern = QRegularExpression(QStringLiteral("--[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::cyan);

    commentStartExpression = QRegularExpression(QStringLiteral("^--\\[\\["));
    commentEndExpression = QRegularExpression(QStringLiteral("\\]\\]"));

    quoteStartExpression = QRegularExpression(QStringLiteral("\\[\\["));
    quoteEndExpression = QRegularExpression(QStringLiteral("\\]\\]"));
}

void LuaSyntaxHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules))
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int quoteStartIndex = 0;
    if (previousBlockState() != 2)
        quoteStartIndex = text.indexOf(quoteStartExpression);

    while (quoteStartIndex >= 0)
    {
        QRegularExpressionMatch match = quoteEndExpression.match(text, quoteStartIndex);
        int quoteEndIndex = match.capturedStart();
        int quoteLength = 0;
        if (quoteEndIndex == -1)
        {
            setCurrentBlockState(2);
            quoteLength = text.length() - quoteStartIndex;
        }
        else
        {
            quoteLength = quoteEndIndex - quoteStartIndex + match.capturedLength();
        }
        setFormat(quoteStartIndex, quoteLength, multiLineQuotationFormat);
        quoteStartIndex = text.indexOf(quoteStartExpression, quoteStartIndex + quoteLength);
    }

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0)
    {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
