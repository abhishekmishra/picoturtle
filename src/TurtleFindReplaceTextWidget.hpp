#pragma once

#ifndef TURTLEFINDREPLACETEXTWIDGET_H
#define TURTLEFINDREPLACETEXTWIDGET_H

#include "TurtleCodeEditorTextWidget.hpp"
#include <QWidget>

namespace turtle
{

    class TurtleFindReplaceTextWidget: public QWidget
    {
        Q_OBJECT

    public:
        TurtleFindReplaceTextWidget(TurtleCodeEditorTextWidget* ed, QWidget *parent = nullptr);
        bool is_replace_enabled();

    public slots:
        void set_replace_enabled(bool flag);

    private:
        bool replace_enabled;
        TurtleCodeEditorTextWidget* editor;
    };

};

#endif // TURTLEFINDREPLACETEXTWIDGET_H
