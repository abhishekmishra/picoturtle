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

    protected:
        void resizeEvent(QResizeEvent *event) override;

    private slots:
        void updateLineNumberAreaWidth(int newBlockCount);
        void highlightCurrentLine();
        void updateLineNumberArea(const QRect &rect, int dy);

    private:
        QWidget *lineNumberArea;
    };

};

#endif // TURTLECODEEDITORTEXTWIDGET_H
