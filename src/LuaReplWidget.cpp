#include <QBoxLayout>
#include "LuaReplWidget.hpp"
#include <QTextBlock>

/* mark in error messages for incomplete statements */
#define EOFMARK         "<eof>"
#define marklen         (sizeof(EOFMARK)/sizeof(char) - 1)


/*
** Check whether 'status' signals a syntax error and the error
** message at the top of the stack ends with the above mark for
** incomplete statements.
*/
static int incomplete(lua_State* L, int status) {
	if (status == LUA_ERRSYNTAX) {
		size_t lmsg;
		const char* msg = lua_tolstring(L, -1, &lmsg);
		if (lmsg >= marklen && strcmp(msg + lmsg - marklen, EOFMARK) == 0) {
			lua_pop(L, 1);
			return 1;
		}
	}
	return 0;  /* else... */
}

static void stackDump(lua_State* L)
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
	qDebug() << ""; /* end the listing */
}

/*
** Try to compile line on the stack as 'return <line>;'; on return, stack
** has either compiled chunk or original line (if compilation failed).
*/
static int addreturn(lua_State* L) {
	stackDump(L);
	const char* line = lua_tostring(L, 1);  /* original line */
	//qDebug() << "input line is " << line;

	const char* retline = lua_pushfstring(L, "return %s;", line);
	//qDebug() << "input line is " << retline;
	int status = luaL_loadbuffer(L, retline, strlen(retline), "t");
	if (status == LUA_OK) {
		lua_remove(L, -2);  /* remove modified line */
	}
	else
	{
		lua_pop(L, 2);  /* pop result from 'luaL_loadbuffer' and modified line */
	}
	return status;
}


using namespace turtle;

const std::string LuaReplWidget::LUA_REPL_PROMPT = "lua";

LuaReplWidget::LuaReplWidget(QWidget* parent) :L{ NULL }
{
	init_lua();

	create_repl_display();

	create_repl_entry();

	layout_widgets();

	// actions on the line entry
	connect(repl_entry, &QLineEdit::returnPressed, this, &LuaReplWidget::repl_enter_line);
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
	repl_display->appendPlainText("############# Lua Session #############");

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
	luaL_openlibs(L); // open std libraries
	lua_gc(L, LUA_GCGEN, 0, 0); // gc in generational mode
	return EXIT_SUCCESS;
}

void LuaReplWidget::repl_enter_line()
{
	auto text = repl_entry->text();
	repl_display->appendPlainText(text);
	repl_entry->setText("");

	std::string text_str = text.toUtf8().constData();
	const char* text_cstr = text_str.c_str();

	qDebug() << "input string" << text_cstr;

	// call addreturn in protected mode.
	// send text as parameter
	// call method
	// get result as boolean
	int status, result;
	lua_pushcfunction(L, &addreturn);
	lua_pushstring(L, text_cstr);
	status = lua_pcall(L, 1, 1, 0);
	result = lua_toboolean(L, -1);

	qDebug() << "status of addreturn is -> " << result;
	//report(L, status);
}

int LuaReplWidget::incomplete(int status)
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
