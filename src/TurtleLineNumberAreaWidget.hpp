#pragma once

#ifndef TURTLELINENUMBERAREAWIDGET_H
#define TURTLELINENUMBERAREAWIDGET_H

#include <QWidget>
#include "TurtleCodeEditorTextWidget.hpp"

// this file is based largely on the example at https://doc.qt.io/qt-6/qtwidgets-widgets-codeeditor-example.html

namespace turtle
{

    class TurtleLineNumberAreaWidget : public QWidget
    {

    public:
        TurtleLineNumberAreaWidget(TurtleCodeEditorTextWidget *ce);
        QSize sizeHint() const override;

    protected:
        void paintEvent(QPaintEvent *event) override;

    private:
        TurtleCodeEditorTextWidget *code_editor;
    };

};

#endif // TURTLELINENUMBERAREAWIDGET_H
