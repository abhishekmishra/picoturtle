#pragma once
#ifndef LUA_SYNTAX_HIGHLIGHTER_H
#define LUA_SYNTAX_HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class LuaSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    LuaSyntaxHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QList<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QRegularExpression quoteStartExpression;
    QRegularExpression quoteEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat multiLineQuotationFormat;
    QTextCharFormat functionFormat;
};
#endif //LUA_SYNTAX_HIGHLIGHTER_H