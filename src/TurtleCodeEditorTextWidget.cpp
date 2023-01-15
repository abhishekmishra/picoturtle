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
        /* get the beginning and end of selection*/
        int start = textCursor().selectionStart();
        int end = textCursor().selectionEnd();

        /* get a cursor object to use for indentation related insertions */
        QTextCursor indent_cur = textCursor();
        
        /* begin atomic undoable option */
        indent_cur.beginEditBlock();

        /* set the indentation cursor to start of selection */
        indent_cur.setPosition(start);

        /* if the indentation cursor is ahead of the end of selection */
        while (indent_cur.position() < end)
        {
            //qDebug() << "start = " << start << " end = " << end 
            //     << " current = " << indent_cur.position();

            /* move the indentation cursor to the beginning of current line */
            indent_cur.movePosition(QTextCursor::StartOfLine);
            /* insert the tab */
            indent_cur.insertText("\t");
            /* now move to one character beyond the end of the current line*/
            indent_cur.movePosition(QTextCursor::EndOfLine);
            indent_cur.setPosition(indent_cur.position() + 1);

            /* since there has been an insertion get the 
               current selection end from the current text cursor
            */
            end = textCursor().selectionEnd();
        }

        /* begin atomic undoable option */
        indent_cur.endEditBlock();
    }
    else
    {
        /* if there is no selction get the current position
           and move to the beginning of the line*/
        int pos = textCursor().position();

        /* get a cursor object to use for indentation related insertions */
        QTextCursor indent_cur = textCursor();
        indent_cur.movePosition(QTextCursor::StartOfLine);

        /* insert a tab */
        indent_cur.insertText("\t");

        /* change the current position to one beyond the original,
           as a new tab has been added.*/
        indent_cur.setPosition(pos + 1);

        /* set the cursor so that position is set back to the editor */
        setTextCursor(indent_cur);
    }
}

/**
 * reduce the number of tabs at the beginning of the text.
 * only if found.
 */
void TurtleCodeEditorTextWidget::deindent_line_or_selection()
{
    if (textCursor().hasSelection())
    {
        /* get the beginning and end of selection*/
        int start = textCursor().selectionStart();
        int end = textCursor().selectionEnd();

        /* get a cursor object to use for indentation related insertions */
        QTextCursor indent_cur = textCursor();

        /* begin atomic undoable option */
        indent_cur.beginEditBlock();

        /* set the indentation cursor to start of selection */
        indent_cur.setPosition(start);

        /* if the indentation cursor is ahead of the end of selection */
        while (indent_cur.position() < end)
        {
            //qDebug() << "start = " << start << " end = " << end 
            //     << " current = " << indent_cur.position();

            /* move the indentation cursor to the beginning of current line */
            indent_cur.movePosition(QTextCursor::StartOfLine);
            
            /* delete the tab */
            if (toPlainText().at(indent_cur.position()) == QString("\t"))
            {
                indent_cur.deleteChar();
            }

            /* now move to one character beyond the end of the current line*/
            indent_cur.movePosition(QTextCursor::EndOfLine);
            indent_cur.setPosition(indent_cur.position() + 1);

            /* since there has been an insertion get the
               current selection end from the current text cursor
            */
            end = textCursor().selectionEnd();
        }

        /* begin atomic undoable option */
        indent_cur.endEditBlock();
    }
    else
    {
        /* if there is no selction get the current position
           and move to the beginning of the line*/
        int pos = textCursor().position();

        /* get a cursor object to use for indentation related insertions */
        QTextCursor indent_cur = textCursor();
        indent_cur.movePosition(QTextCursor::StartOfLine);

        /* delete a tab */
        if (toPlainText().at(indent_cur.position()) == QString("\t"))
        {
            indent_cur.deleteChar();
        }

        /* set the cursor so that position is set back to the editor */
        setTextCursor(indent_cur);
    }

}

int TurtleCodeEditorTextWidget::find_first_comment(QTextCursor cur)
{
    cur.movePosition(QTextCursor::EndOfLine);
    int end_pos = cur.position();
    cur.movePosition(QTextCursor::StartOfLine);
    int start_pos = cur.position();
    int cur_pos = cur.position();

    int whitespace = 1;

    while(cur_pos < end_pos)
    {
        // gobble whitespace
        // and first two non-whitespace should be two dashes for lua comment.
        QChar cur_char = toPlainText().at(cur_pos);
        QChar next_char = toPlainText().at(cur_pos + 1);
        if(whitespace)
        {
            if (cur_char == '\t' || cur_char == ' ')
            {
                cur.setPosition(cur.position() + 1);
                cur_pos = cur.position();
            }
            else
            {
                whitespace = 0;
            }
        }
        else
        {
            if(cur_char == '-' && next_char == '-')
            {
                return cur_pos;
            }
            else
            {
                return -1;
            }
        }
    }
    return -1;
}

void TurtleCodeEditorTextWidget::toggle_comment_selection()
{
    if (textCursor().hasSelection())
    {
        /* get the beginning and end of selection*/
        int start = textCursor().selectionStart();
        int end = textCursor().selectionEnd();

        /* get a cursor object to use for indentation related insertions */
        QTextCursor cmt_cur = textCursor();

        /* begin atomic undoable option */
        cmt_cur.beginEditBlock();

        /* set the indentation cursor to start of selection */
        cmt_cur.setPosition(start);

        int all_comment = 1;

        /* if the indentation cursor is ahead of the end of selection */
        while (cmt_cur.position() < end)
        {
            /* move the indentation cursor to the beginning of current line */
            cmt_cur.movePosition(QTextCursor::StartOfLine);
            
            if(find_first_comment(cmt_cur) == -1)
            {
                all_comment = 0;
                break;
            }

            /* now move to one character beyond the end of the current line*/
            cmt_cur.movePosition(QTextCursor::EndOfLine);
            cmt_cur.setPosition(cmt_cur.position() + 1);

            /* since there has been an insertion get the
               current selection end from the current text cursor
            */
            end = textCursor().selectionEnd();
        }

        /* set the indentation cursor to start of selection */
        cmt_cur.setPosition(start);

        //if all lines are comments, uncomment
        //else add comments
        if (all_comment == 1)
        {
            /* if the indentation cursor is ahead of the end of selection */
            while (cmt_cur.position() < end)
            {
                /* move the indentation cursor to the beginning of current line */
                cmt_cur.movePosition(QTextCursor::StartOfLine);
                
                int cmt_loc = find_first_comment(cmt_cur);
                cmt_cur.setPosition(cmt_loc);
                cmt_cur.deleteChar();
                cmt_cur.deleteChar();

                /* now move to one character beyond the end of the current line*/
                cmt_cur.movePosition(QTextCursor::EndOfLine);
                cmt_cur.setPosition(cmt_cur.position() + 1);

                /* since there has been an insertion get the
                current selection end from the current text cursor
                */
                end = textCursor().selectionEnd();
            }
        }
        else
        {
            /* if the indentation cursor is ahead of the end of selection */
            while (cmt_cur.position() < end)
            {
                /* move the indentation cursor to the beginning of current line */
                cmt_cur.movePosition(QTextCursor::StartOfLine);
                
                cmt_cur.insertText("--");

                /* now move to one character beyond the end of the current line*/
                cmt_cur.movePosition(QTextCursor::EndOfLine);
                cmt_cur.setPosition(cmt_cur.position() + 1);

                /* since there has been an insertion get the
                current selection end from the current text cursor
                */
                end = textCursor().selectionEnd();
            }
        }

        /* begin atomic undoable option */
        cmt_cur.endEditBlock();
    }
    else
    {
        /* if there is no selction get the current position
           and move to the beginning of the line*/
        int pos = textCursor().position();

        /* get a cursor object to use for indentation related insertions */
        QTextCursor cmt_cur = textCursor();
        cmt_cur.movePosition(QTextCursor::StartOfLine);

        int cmt_loc = find_first_comment(cmt_cur);
        if(cmt_loc == -1)
        {
            cmt_cur.insertText("--");
        }
        else
        {
            cmt_cur.setPosition(cmt_loc);
            cmt_cur.deleteChar();
            cmt_cur.deleteChar();
        }
    }
}

void TurtleCodeEditorTextWidget::keyPressEvent(QKeyEvent* e)
{
    if (e->modifiers() == Qt::ShiftModifier) 
    {
        if (e->key() == Qt::Key_Backtab)
        {
            deindent_line_or_selection();
        }
        else
        {
            QPlainTextEdit::keyPressEvent(e);
        }
    }
    else if (e->key() == Qt::Key_Tab)
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
