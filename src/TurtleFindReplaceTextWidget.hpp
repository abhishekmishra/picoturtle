#pragma once

#ifndef TURTLEFINDREPLACETEXTWIDGET_H
#define TURTLEFINDREPLACETEXTWIDGET_H

#include "TurtleCodeEditorTextWidget.hpp"
#include <QWidget>

namespace turtle
{

    class TurtleFindReplaceTextWidget : public QWidget
    {
        Q_OBJECT

    public:
        TurtleFindReplaceTextWidget(TurtleCodeEditorTextWidget *ed, QWidget *parent = nullptr);
        bool is_replace_enabled();

    public slots:
        void set_replace_enabled(bool flag);
        void find_next(QString text);
        void find_previous(QString text);
        void replace_next(QString text, QString replace_text);
        void replace_all(QString text, QString replace_text);

    private:
        bool replace_enabled;
        TurtleCodeEditorTextWidget *editor;
        QWidget *replace_widget;
    };
};

#endif // TURTLEFINDREPLACETEXTWIDGET_H
