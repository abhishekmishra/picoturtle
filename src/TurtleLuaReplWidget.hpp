#pragma once

#ifndef TURTLELUAREPLWIDGET_H
#define TURTLELUAREPLWIDGET_H

#include "LuaReplWidget.hpp"

namespace turtle
{

	
class TurtleLuaReplWidget: public LuaReplWidget
{

public:
    TurtleLuaReplWidget(QWidget* parent = nullptr);
    ~TurtleLuaReplWidget();

};


};

	
#endif // TURTLELUAREPLWIDGET_H
