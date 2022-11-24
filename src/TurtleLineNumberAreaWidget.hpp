#pragma once

#ifndef TURTLELINENUMBERAREAWIDGET_H
#define TURTLELINENUMBERAREAWIDGET_H

#include <QWidget>
#include "TurtleCodeEditorTextWidget.hpp"

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
