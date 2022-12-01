#include <QBoxLayout>
#include "LuaReplWidget.hpp"
#include <QTextBlock>

/* mark in error messages for incomplete statements */
#define EOFMARK "<eof>"
#define marklen (sizeof(EOFMARK) / sizeof(char) - 1)

/*
** Check whether 'status' signals a syntax error and the error
** message at the top of the stack ends with the above mark for
** incomplete statements.
*/
static int incomplete(lua_State* L, int status)
{
	if (status == LUA_ERRSYNTAX)
	{
		size_t lmsg;
		const char* msg = lua_tolstring(L, -1, &lmsg);
		if (lmsg >= marklen && strcmp(msg + lmsg - marklen, EOFMARK) == 0)
		{
			lua_pop(L, 1);
			return 1;
		}
	}
	return 0; /* else... */
}

static void stackDump(lua_State* L)
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
** Try to compile line on the stack as 'return <line>;'; on return, stack
** has either compiled chunk or original line (if compilation failed).
*/
static int try_addreturn(lua_State* L)
{
	const char* line = lua_tostring(L, 1); /* original line */
	// qDebug() << "input line is " << line;

	const char* retline = lua_pushfstring(L, "return %s;", line);
	// qDebug() << "input line is " << retline;
	int status = luaL_loadbuffer(L, retline, strlen(retline), "=repl");
	lua_remove(L, -2); /* remove modified line */

	// if status is ok there will be no message
	// so push an empty string
	if (status == LUA_OK)
	{
		lua_pushstring(L, "");
	}

	lua_pushinteger(L, status);
	return 2;
}

static int try_command(lua_State* L)
{
	const char* line = lua_tostring(L, 1); /* original line */
	// qDebug() << "input line is " << line;

	lua_pushstring(L, line);
	int status = luaL_loadbuffer(L, line, strlen(line), "=repl");
	lua_remove(L, -2); /* line */

	// if status is ok there will be no message
	// so push an empty string
	if (status == LUA_OK)
	{
		lua_pushstring(L, "");
	}
	lua_pushboolean(L, incomplete(L, status));
	lua_pushinteger(L, status);
	return 3;
}

using namespace turtle;

const std::string LuaReplWidget::LUA_REPL_PROMPT = "lua";

LuaReplWidget::LuaReplWidget(QWidget* parent) : L{ NULL }
{
	init_lua();

	create_repl_display();

	create_repl_entry();

	layout_widgets();

	// actions on the line entry
	connect(repl_entry, &QLineEdit::returnPressed, this, &LuaReplWidget::repl_enter_line);

	connect(this, &LuaReplWidget::prompt_changed, [=](QString prompt) {
		repl_prompt->setText(prompt);
		});

	// set multiline mode to false,
	// this will also init the proper prompt.
	set_multiline(false);
};

void LuaReplWidget::layout_widgets()
{
	QHBoxLayout* lua_line_hbox = new QHBoxLayout(this);
	lua_line_hbox->addWidget(repl_prompt);
	lua_line_hbox->addWidget(repl_entry);
	QWidget* lua_line_widget = new QWidget();
	lua_line_widget->setLayout(lua_line_hbox);
	lua_line_hbox->setContentsMargins(2, 2, 0, 0);

	QVBoxLayout* vb_layout = new QVBoxLayout(this);
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
	repl_display->appendHtml("<b>############# Lua Session #############</b>");

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
	int status;
	L = luaL_newstate(); // new lua state
	if (L == NULL)
	{
		return EXIT_FAILURE;
	}
	luaL_openlibs(L);			// open std libraries
	lua_gc(L, LUA_GCGEN, 0, 0); // gc in generational mode
	return EXIT_SUCCESS;
}

bool LuaReplWidget::singleline_return_syntax_check()
{
	// call try_addreturn in protected mode.
	// send text as parameter
	// call method
	// get result as boolean
	int status, result;
	std::string text_str = current_line.toUtf8().constData();
	const char* text_cstr = text_str.c_str();

	lua_pushcfunction(L, &try_addreturn);
	lua_pushstring(L, text_cstr);
	status = lua_pcall(L, 1, 2, 0);
	stackDump(L);
	result = lua_tointeger(L, -1);
	const char* msg = lua_tostring(L, -2);
	lua_settop(L, 0); // clear the stack

	qDebug() << "status of try_addreturn is -> " << status << " result is " << result << " msg is" << msg;

	if (result == 0)
	{
		return true;
	}
	else {
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

	if (!multiline) {
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
		const char* text_cstr = text_str.c_str();
		
		// try as command
		lua_pushcfunction(L, &try_command);
		lua_pushstring(L, text_cstr);
		status = lua_pcall(L, 1, 3, 0);
		stackDump(L);
		result = lua_tointeger(L, -1);
		bool incomplete = lua_toboolean(L, -2);
		const char* err_mesg = lua_tostring(L, -3);
		lua_settop(L, 0); // clear the stack

		if (incomplete)
		{
			set_multiline(true);
			previous_lines = all_lines;
		}
		else
		{
			if (result != 0)
			{
				repl_display->appendHtml(QString("<font color=\"magenta\"><i>") + err_mesg + "</i></font>");
			}
			else
			{
				code_to_run = all_lines;
				multi_line_good = true;

				// end multiline mode
				set_multiline(false);
			}
		}
	}

	if (single_line_good || multi_line_good)
	{
		qDebug() << "I will run this code";
		qDebug() << code_to_run;
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
