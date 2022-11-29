#pragma once

#ifndef LUAREPLWIDGET_H
#define LUAREPLWIDGET_H

#include <QWidget>
#include <QPlainTextEdit>


namespace turtle
{
	/**
	 * @brief Implements a Lua REPL for use as a docked widget
	 * or a standalone widget if required later.
	 * 
	 * The code in this implementation is heavily based on lua.c
	 * in the Lua 5.4 source code.
	 */
	class LuaReplWidget : public QWidget
	{
		Q_OBJECT

	public:
		LuaReplWidget(QWidget* parent = nullptr);

	private:
		QPlainTextEdit* lua_repl_edit;

	};

};


#endif // LUAREPLWIDGET_H
