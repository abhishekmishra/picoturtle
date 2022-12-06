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

#define REPL_WIDGET_GLOBAL_LUA_NAME "__lua_repl_widget__"

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

		/**
		 * @brief Prints the given string to the repl
		 */
		void print_to_repl(std::string value);

		/**
		 * @brief Initialize lua state with libraries,
		 * and some global stuff.
		 */
		virtual int init_lua();

		/**
		 * cleanup lua state and other items related to it.
		 */
		virtual void cleanup_lua();

		bool handleLuaError(int luaErrorCode);
		int run_lua_file(const char* filename);
		int run_lua_script(const char* script);

	protected:
		/** Lua State */
		lua_State* L;


	signals:
		void prompt_changed(QString prompt);

	public slots:
		void repl_enter_line();

	private:
		bool multiline;
		QString prompt;
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
