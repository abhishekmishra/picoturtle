#pragma once

#ifndef LUAREPLWIDGET_H
#define LUAREPLWIDGET_H

#include <string>
#include <QWidget>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QLabel>

extern "C"
{

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

/*
** {==================================================================
** Read-Eval-Print Loop (REPL)
** ===================================================================
*/

#if !defined(LUA_PROMPT)
#define LUA_PROMPT "> "
#define LUA_PROMPT2 ">> "
#endif

#if !defined(LUA_MAXINPUT)
#define LUA_MAXINPUT 512
#endif

/* mark in error messages for incomplete statements */
#define EOFMARK "<eof>"
#define marklen (sizeof(EOFMARK) / sizeof(char) - 1)

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

		static const std::string LUA_REPL_PROMPT;

	public:
		LuaReplWidget(QWidget *parent = nullptr);

	private:
		QPlainTextEdit *lua_repl_display;
		QLineEdit *lua_entry;
		QLabel *lua_prompt;
		int incomplete(lua_State *L, int status);
	};
};

#endif // LUAREPLWIDGET_H
