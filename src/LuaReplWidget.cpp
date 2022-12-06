#include <QBoxLayout>
#include "LuaReplWidget.hpp"
#include <QTextBlock>

using namespace turtle;

const std::string LuaReplWidget::LUA_REPL_PROMPT = "lua";

int print(lua_State *L)
{
	int nargs = lua_gettop(L);
	int widget_type = lua_getglobal(L, REPL_WIDGET_GLOBAL_LUA_NAME);
	LuaReplWidget *repl_widget = (LuaReplWidget *)lua_touserdata(L, -1);
	if (repl_widget != NULL)
	{
		std::string input = "";
		for (int i = 1; i <= nargs; i++)
		{
			if (i > 1)
			{
				input += " ";
			}

			// luaL_tolstring should ideally call the __tostring function
			// if it is set in the metatable of the value at i
			input += luaL_tolstring(L, i, NULL);
		}

		repl_widget->print_to_repl(input.c_str());
	}
	return 0;
}

/*
** Message handler used to run all chunks
*/
static int msghandler(lua_State *L)
{
	const char *msg = lua_tostring(L, 1);
	if (msg == NULL)
	{											 /* is error object not a string? */
		if (luaL_callmeta(L, 1, "__tostring") && /* does it have a metamethod */
			lua_type(L, -1) == LUA_TSTRING)		 /* that produces a string? */
			return 1;							 /* that is the message */
		else
			msg = lua_pushfstring(L, "(error object is a %s value)",
								  luaL_typename(L, 1));
	}
	luaL_traceback(L, L, msg, 1); /* append a standard traceback */
	return 1;					  /* return the traceback */
}

/*
** Interface to 'lua_pcall', which sets appropriate message function
** and C-signal handler. Used to run all chunks.
*/
static int docall(lua_State *L, int narg, int nres)
{
	int status;
	int base = lua_gettop(L) - narg;  /* function index */
	lua_pushcfunction(L, msghandler); /* push message handler */
	lua_insert(L, base);			  /* put it under function and args */
	status = lua_pcall(L, narg, nres, base);
	lua_remove(L, base); /* remove message handler from the stack */
	return status;
}

/* mark in error messages for incomplete statements */
#define EOFMARK "<eof>"
#define marklen (sizeof(EOFMARK) / sizeof(char) - 1)

/*
** Check whether 'status' signals a syntax error and the error
** message at the top of the stack ends with the above mark for
** incomplete statements.
*/
static int incomplete(lua_State *L, int status)
{
	if (status == LUA_ERRSYNTAX)
	{
		size_t lmsg;
		const char *msg = lua_tolstring(L, -1, &lmsg);
		if (lmsg >= marklen && strcmp(msg + lmsg - marklen, EOFMARK) == 0)
		{
			lua_pop(L, 1);
			return 1;
		}
	}
	return 0; /* else... */
}

static void stackDump(lua_State *L)
{
	qDebug() << "----------lua stack begins----------";
	int i;
	int top = lua_gettop(L); /* depth of the stack */
	for (i = 1; i <= top; i++)
	{ /* repeat for each level */
		int t = lua_type(L, i);
		switch (t)
		{
		case LUA_TSTRING:
		{ /* strings */
			qDebug() << lua_tostring(L, i);
			break;
		}
		case LUA_TBOOLEAN:
		{ /* Booleans */
			qDebug() << (lua_toboolean(L, i) ? "true" : "false");
			break;
		}
		case LUA_TNUMBER:
		{ /* numbers */
			qDebug() << lua_tonumber(L, i);
			break;
		}
		default:
		{ /* other values */
			qDebug() << lua_typename(L, t);
			break;
		}
		}
	}
	qDebug() << "----------lua stack ends----------";
}

/*
** Try to compile line on the stack as 'return <line>;'; 
* on return the stack has either the compiled chunk
* or the error message as the second value.
* the first return value is the status of lua load.
*/
static int try_addreturn(lua_State *L)
{
	const char *line = lua_tostring(L, 1); /* original line */
	// qDebug() << "input line is " << line;

	const char *retline = lua_pushfstring(L, "return %s;", line);
	// qDebug() << "input line is " << retline;
	int status = luaL_loadbuffer(L, retline, strlen(retline), "=repl");
	stackDump(L);

	// remove the two input lines
	lua_remove(L, -3);
	lua_remove(L, -2);

	// now we have just the error message OR compiled chunk
	// in the stack
	stackDump(L);

	lua_pushinteger(L, status);
	stackDump(L);
	return 2;
}

static int try_command(lua_State *L)
{
	const char *line = lua_tostring(L, 1); /* original line */
	// qDebug() << "input line is " << line;

	lua_pushstring(L, line);
	int status = luaL_loadbuffer(L, line, strlen(line), "=repl");
	stackDump(L);

	// remove the input line
	lua_remove(L, -2);

	// now we have just the error message OR compiled chunk
	// in the stack
	stackDump(L);

	lua_pushboolean(L, incomplete(L, status));
	lua_pushinteger(L, status);
	return 3;
}

LuaReplWidget::LuaReplWidget(QWidget *parent) : L{NULL}
{
	init_lua();

	create_repl_display();

	create_repl_entry();

	layout_widgets();

	// actions on the line entry
	connect(repl_entry, &QLineEdit::returnPressed, this, &LuaReplWidget::repl_enter_line);

	connect(this, &LuaReplWidget::prompt_changed, [=](QString prompt)
			{ repl_prompt->setText(prompt); });

	// set multiline mode to false,
	// this will also init the proper prompt.
	set_multiline(false);
};

void LuaReplWidget::layout_widgets()
{
	QHBoxLayout *lua_line_hbox = new QHBoxLayout(this);
	lua_line_hbox->addWidget(repl_prompt);
	lua_line_hbox->addWidget(repl_entry);
	QWidget *lua_line_widget = new QWidget();
	lua_line_widget->setLayout(lua_line_hbox);
	lua_line_hbox->setContentsMargins(2, 2, 0, 0);

	QVBoxLayout *vb_layout = new QVBoxLayout(this);
	vb_layout->addWidget(repl_display);
	vb_layout->addWidget(lua_line_widget);
	setLayout(vb_layout);
}

void LuaReplWidget::create_repl_entry()
{
	// create line entry widgets
	repl_entry = new QLineEdit(this);
	repl_prompt = new QLabel(QString::fromStdString(LUA_REPL_PROMPT) + " >", this);
}

void LuaReplWidget::create_repl_display()
{
	repl_display = new QPlainTextEdit(this);
	repl_display->setReadOnly(true);
	repl_display->appendHtml("<b>----------- Lua Session Created -----------</b>");

	// Set the default monospace font for now
	// TODO: perhaps include a decent open source font
	QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
	font.setPointSize(12);
	QFontInfo info(font);
	// qDebug() << font << info.family() << info.fixedPitch();
	repl_display->setFont(font);

	QPalette p = repl_display->palette();

	// set background colour
	p.setColor(QPalette::Active, QPalette::Base, Qt::darkBlue);
	p.setColor(QPalette::Inactive, QPalette::Base, Qt::darkBlue);

	// set foreground colour
	p.setColor(QPalette::Active, QPalette::Text, Qt::white);
	p.setColor(QPalette::Inactive, QPalette::Text, Qt::white);

	repl_display->setPalette(p);

	repl_display->setMinimumHeight(150);
}

// TODO: proper message about failure
// trigger error signal if failure.
int LuaReplWidget::init_lua()
{
	// cleanup lua if already initialized
	cleanup_lua();

	int status;
	L = luaL_newstate(); // new lua state
	if (L == NULL)
	{
		return EXIT_FAILURE;
	}
	luaL_openlibs(L);			// open std libraries
	lua_gc(L, LUA_GCGEN, 0, 0); // gc in generational mode

	// push the repl widget object as a light userdata
	lua_pushlightuserdata(L, this);
	// set it as global value REPL_WIDGET_GLOBAL_LUA_NAME
	lua_setglobal(L, REPL_WIDGET_GLOBAL_LUA_NAME);

	lua_pushcfunction(L, print);
	lua_setglobal(L, "print");

	return EXIT_SUCCESS;
}

void LuaReplWidget::cleanup_lua()
{
	if (L != NULL) {
		lua_close(L);
	}
}

bool LuaReplWidget::singleline_return_syntax_check()
{
	// call try_addreturn in protected mode.
	// send text as parameter
	// call method
	// get result as boolean
	int status, result;
	std::string text_str = current_line.toUtf8().constData();
	const char *text_cstr = text_str.c_str();

	lua_pushcfunction(L, &try_addreturn);
	lua_pushstring(L, text_cstr);
	status = lua_pcall(L, 1, 2, 0);
	stackDump(L);
	result = lua_tointeger(L, -1);
	const char *msg = lua_tostring(L, -2);

	qDebug() << "status of try_addreturn is -> " << status << " result is " << result << " msg is" << msg;

	if (result == 0)
	{
		lua_pop(L, 1);
		return true;
	}
	else
	{
		lua_settop(L, 0); // clear the stack
		return false;
	}
}

void LuaReplWidget::repl_enter_line()
{
	QString code_to_run;

	current_line = repl_entry->text();
	repl_display->appendHtml(prompt + " " + current_line);
	repl_entry->setText("");

	bool single_line_good = false;
	bool multi_line_good = false;

	if (!multiline)
	{
		single_line_good = singleline_return_syntax_check();
		if (single_line_good)
		{
			code_to_run = current_line;
		}
	}

	// do a multiline syntax check
	if (!single_line_good)
	{
		int status, result;
		QString all_lines = current_line;

		if (previous_lines != "")
		{
			all_lines = previous_lines + "\n" + current_line;
		}

		std::string text_str = all_lines.toUtf8().constData();
		const char *text_cstr = text_str.c_str();

		// try as command
		lua_pushcfunction(L, &try_command);
		lua_pushstring(L, text_cstr);
		status = lua_pcall(L, 1, 3, 0);
		stackDump(L);
		result = lua_tointeger(L, -1);
		bool incomplete = lua_toboolean(L, -2);
		const char *err_mesg = lua_tostring(L, -3);

		if (incomplete)
		{
			lua_settop(L, 0); // clear the stack
			set_multiline(true);
			previous_lines = all_lines;
		}
		else
		{
			if (result != 0)
			{
				lua_settop(L, 0); // clear the stack
				repl_display->appendHtml(QString("<font color=\"magenta\"><i>") + err_mesg + "</i></font>");
			}
			else
			{
				lua_pop(L, 2);
				code_to_run = all_lines;
				multi_line_good = true;
			}
			// end multiline mode
			set_multiline(false);
		}
	}

	if (single_line_good || multi_line_good)
	{
		qDebug() << "I will run this code";
		qDebug() << code_to_run;
		stackDump(L);
		int res = docall(L, 0, LUA_MULTRET);
		qDebug() << "ran the code";
		stackDump(L);
		print_values_on_stack();
		lua_settop(L, 0); // clear the stack
	}
	current_line = "";
}

void LuaReplWidget::set_multiline(bool flag)
{
	if (flag)
	{
		multiline = true;
	}
	else
	{
		multiline = false;
		previous_lines = "";
	}

	prompt = QString::fromStdString(LUA_REPL_PROMPT) + ">";
	if (multiline)
	{
		prompt += ">";
	}

	emit prompt_changed(prompt);
}

void LuaReplWidget::print_values_on_stack()
{
	int i;
	int top = lua_gettop(L); /* depth of the stack */
	for (i = 1; i <= top; i++)
	{ /* repeat for each level */
		int t = lua_type(L, i);
		switch (t)
		{
		case LUA_TSTRING:
		{ /* strings */
			repl_display->appendPlainText(lua_tostring(L, i));
			break;
		}
		case LUA_TBOOLEAN:
		{ /* Booleans */
			repl_display->appendPlainText(lua_toboolean(L, i) ? "true" : "false");
			break;
		}
		case LUA_TNUMBER:
		{ /* numbers */
			repl_display->appendPlainText(QString::number(lua_tonumber(L, i)));
			break;
		}
		default:
		{ /* other values */
			repl_display->appendPlainText(luaL_tolstring(L, i, NULL));
			break;
		}
		}
	}
	// repl_display->appendPlainText("\n");
}

void LuaReplWidget::print_to_repl(std::string value)
{
	repl_display->appendPlainText(QString::fromStdString(value));
}


bool LuaReplWidget::handleLuaError(int luaErrorCode)
{
	if (luaErrorCode == LUA_OK)
	{
		return true;
	}
	else
	{
		const char* err_msg = lua_tostring(L, -1);
		qDebug() << err_msg;
		print_to_repl(err_msg);
		return false;
	}
}

int LuaReplWidget::run_lua_file(const char* filename)
{
	if (filename != NULL && strlen(filename) > 0)
	{
		if (handleLuaError(luaL_dofile(L, filename)))
		{
			// TurtleLuaReplWidget::turtle_message("app", QString("File execution complete -> ") + filename);
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

int LuaReplWidget::run_lua_script(const char* script)
{
	// TurtleLuaReplWidget::turtle_message("app", QString("running lua script."));
	if (script != NULL && strlen(script) > 0)
	{
		if (handleLuaError(luaL_dostring(L, script)))
		{
			// uncomment for debug only.
			// printf("Script execution complete.\n");
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return -1;
	}
}
