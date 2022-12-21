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

		/**
		 * accept a lua error code, and print
		 * an appropriate message to the console.
		 */
		bool handle_lua_error(int luaErrorCode);

		/**
		 * run a lua chunk, with the given compilation status
		 * @param status
		 * @return result status
		 */
		int dochunk(int status);

		/**
		 * run a lua file as a chunk in the current lua repl
		 * @param filename
		 * @return status
		 */
		int run_lua_file(const char *filename);

		/**
		 * run a lua script as a chunk in the current lua repl
		 * @param script
		 * @return status
		 */
		int run_lua_script(const char *script);

		/**
		 * run a lua script as a chunk in the current lua repl
		 * with an optional file_path argument.
		 * The parent directory of the file_path (if it is not null) will be added to
		 * lua path.
		 * @param script
		 * @param file_path
		 * @return status
		 */
		int run_lua_script_path(const char *script, const char *file_path = NULL);

		/**
		 * prepend the given lua path fragment to the lua path variable.
		 *
		 * the resultant path will be
		 *
		 * package.path = "<path_fragment>/?.lua;" .. package.path
		 *
		 * @param path_fragment
		 */
		void add_to_lua_path(const char *path_fragment);

	protected:
		/** Lua State */
		lua_State *L;

	signals:
		void prompt_changed(QString prompt);

	public slots:
		void repl_enter_line();

		/**
		 * @brief clear the lua repl console.
		 */
		void clear_console();

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
