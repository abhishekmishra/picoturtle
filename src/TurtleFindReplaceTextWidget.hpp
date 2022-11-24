#pragma once

#ifndef TURTLEFINDREPLACETEXTWIDGET_H
#define TURTLEFINDREPLACETEXTWIDGET_H

#include <QWidget>

namespace turtle
{

    class TurtleFindReplaceTextWidget: public QWidget
    {
        Q_OBJECT

    public:
        TurtleFindReplaceTextWidget(QWidget *parent = nullptr);
    };

};

#endif // TURTLEFINDREPLACETEXTWIDGET_H
