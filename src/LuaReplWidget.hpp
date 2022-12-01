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

		int init_lua();

	signals:
		void prompt_changed(QString prompt);

	public slots:
		void repl_enter_line();

	private:
		bool multiline;
		QString prompt;
		lua_State *L;
		QPlainTextEdit *repl_display;
		QLineEdit *repl_entry;
		QLabel *repl_prompt;
		QString current_line;
		QString previous_lines;

		void create_repl_display();
		void create_repl_entry();
		void layout_widgets();

		bool singleline_return_syntax_check();

		void set_multiline(bool flag);

		/**
		 * @brief reads all the available values on the
		 * lua stack and writes their string representations
		 * to the console
		 */
		void print_values_on_stack();
	};
};

#endif // LUAREPLWIDGET_H
