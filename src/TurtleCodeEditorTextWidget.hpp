#pragma once

#ifndef TURTLECODEEDITORTEXTWIDGET_H
#define TURTLECODEEDITORTEXTWIDGET_H

#include <QPlainTextEdit>

// this file is based largely on the example at https://doc.qt.io/qt-6/qtwidgets-widgets-codeeditor-example.html

namespace turtle
{
    class TurtleLineNumberAreaWidget;

    class TurtleCodeEditorTextWidget : public QPlainTextEdit
    {
        Q_OBJECT

    public:
        TurtleCodeEditorTextWidget(QWidget *parent = nullptr);

        void lineNumberAreaPaintEvent(QPaintEvent *event);
        int lineNumberAreaWidth();
        int current_line();
        int current_column();
        void move_cursor_to_end();

    public slots:

        /**
         * if the current cursor has a selection,
         * we want to indent the entire selection.
         * i.e. add tabs at the beginning of every line
         * occuring in the selection.
         *
         * however if there is no selection,
         * a tab should be added at the beginning of the
         * current line.

         * NOTE: that this is different from what just pressing TAB key
         * does - which is to add a tab at the current position.
         */
        void indent_line_or_selection();

        /**
         * reduce the number of tabs at the beginning of the text.
         * only if found.
         */
        void deindent_line_or_selection();

    protected:
        void resizeEvent(QResizeEvent *event) override;
        void keyPressEvent(QKeyEvent* e) override;

    private slots:
        void updateLineNumberAreaWidth(int newBlockCount);
        void highlightCurrentLine();
        void updateLineNumberArea(const QRect &rect, int dy);

    private:
        QWidget *lineNumberArea;
    };

};

#endif // TURTLECODEEDITORTEXTWIDGET_H
