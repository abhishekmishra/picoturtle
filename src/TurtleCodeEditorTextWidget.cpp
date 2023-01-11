#include "TurtleCodeEditorTextWidget.hpp"
#include "TurtleLineNumberAreaWidget.hpp"
#include <QPainter>
#include <QTextBlock>

using namespace turtle;

TurtleCodeEditorTextWidget::TurtleCodeEditorTextWidget(QWidget *parent) : QPlainTextEdit(parent)
{

    // Set the default monospace font for now
    // TODO: perhaps include a decent open source font
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPointSize(10);
    QFontInfo info(font);
    // qDebug() << font << info.family() << info.fixedPitch();
    setFont(font);

    QFontMetrics fontMetrics = QFontMetrics(font);
    int fontWidth = fontMetrics.averageCharWidth();
    setTabStopDistance(fontWidth * 4);

    QPalette p = palette();

    // set background colour
    p.setColor(QPalette::Active, QPalette::Base, Qt::darkBlue);
    p.setColor(QPalette::Inactive, QPalette::Base, Qt::darkBlue);

    // set foreground colour
    p.setColor(QPalette::Active, QPalette::Text, Qt::yellow);
    p.setColor(QPalette::Inactive, QPalette::Text, Qt::yellow);

    setPalette(p);

    // set minimum width of code_edit
    setMinimumWidth(512);

    lineNumberArea = new TurtleLineNumberAreaWidget(this);

    connect(this, &TurtleCodeEditorTextWidget::blockCountChanged, this, &TurtleCodeEditorTextWidget::updateLineNumberAreaWidth);
    connect(this, &TurtleCodeEditorTextWidget::updateRequest, this, &TurtleCodeEditorTextWidget::updateLineNumberArea);
    connect(this, &TurtleCodeEditorTextWidget::cursorPositionChanged, this, &TurtleCodeEditorTextWidget::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

int TurtleCodeEditorTextWidget::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void TurtleCodeEditorTextWidget::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void TurtleCodeEditorTextWidget::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void TurtleCodeEditorTextWidget::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void TurtleCodeEditorTextWidget::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::darkBlue).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void TurtleCodeEditorTextWidget::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor(Qt::darkBlue).darker(160));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::lightGray);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

int TurtleCodeEditorTextWidget::current_line()
{
    return textCursor().blockNumber() + 1;
}

int TurtleCodeEditorTextWidget::current_column()
{
    return textCursor().positionInBlock() + 1;
}

void TurtleCodeEditorTextWidget::move_cursor_to_end()
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);
}

void TurtleCodeEditorTextWidget::indent_line_or_selection()
{
    if (textCursor().hasSelection())
    {
        //int start = textCursor().selectionStart();
        //int end = textCursor().selectionEnd();

        //QTextCursor indent_cur = textCursor();
        //indent_cur.setPosition(start);

        //while (indent_cur.position() < end)
        //{
        //    qDebug() << "start = " << start << " end = " << end << " current = " << indent_cur.position();

        //    indent_cur.movePosition(QTextCursor::StartOfLine);
        //    indent_cur.insertText("\t");
        //    indent_cur.movePosition(QTextCursor::EndOfLine);
        //}
    }
    else
    {
        int pos = textCursor().position();
        textCursor().movePosition(QTextCursor::StartOfLine);
        insertPlainText("\t");
        textCursor().setPosition(pos + 1);
    }
}

void TurtleCodeEditorTextWidget::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Tab)
    {
        if (!textCursor().hasSelection())
        {
            insertPlainText("\t");
        }
        else
        {
            indent_line_or_selection();
        }
    }
    else
    {
        QPlainTextEdit::keyPressEvent(e);
    }
}
