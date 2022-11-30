#include <QBoxLayout>
#include <QHboxLayout>
#include "LuaReplWidget.hpp"
#include <QTextBlock>

using namespace turtle;

const std::string LuaReplWidget::LUA_REPL_PROMPT = "lua";

LuaReplWidget::LuaReplWidget(QWidget *parent)
{
	lua_repl_display = new QPlainTextEdit(this);
	lua_repl_display->setReadOnly(true);
	lua_repl_display->appendPlainText("############# Lua Session #############");

	// Set the default monospace font for now
	// TODO: perhaps include a decent open source font
	QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
	font.setPointSize(12);
	QFontInfo info(font);
	// qDebug() << font << info.family() << info.fixedPitch();
	lua_repl_display->setFont(font);

	QPalette p = lua_repl_display->palette();

	// set background colour
	p.setColor(QPalette::Active, QPalette::Base, Qt::darkBlue);
	p.setColor(QPalette::Inactive, QPalette::Base, Qt::darkBlue);

	// set foreground colour
	p.setColor(QPalette::Active, QPalette::Text, Qt::white);
	p.setColor(QPalette::Inactive, QPalette::Text, Qt::white);

	lua_repl_display->setPalette(p);

	lua_repl_display->setMinimumHeight(150);

	// create line entry widgets
	lua_entry = new QLineEdit(this);
	lua_prompt = new QLabel(QString::fromStdString(LUA_REPL_PROMPT) + " >", this);

	QHBoxLayout *lua_line_hbox = new QHBoxLayout(this);
	lua_line_hbox->addWidget(lua_prompt);
	lua_line_hbox->addWidget(lua_entry);
	QWidget *lua_line_widget = new QWidget();
	lua_line_widget->setLayout(lua_line_hbox);
	lua_line_hbox->setContentsMargins(2, 2, 0, 0);

	QVBoxLayout *vb_layout = new QVBoxLayout(this);
	vb_layout->addWidget(lua_repl_display);
	vb_layout->addWidget(lua_line_widget);
	setLayout(vb_layout);

	// actions on the line entry
	connect(lua_entry, &QLineEdit::returnPressed, [=]()
			{ auto text = lua_entry->text();
			lua_repl_display->appendPlainText(text);
			lua_entry->setText(""); });
};

/*
** Check whether 'status' signals a syntax error and the error
** message at the top of the stack ends with the above mark for
** incomplete statements.
*/
int LuaReplWidget::incomplete(lua_State *L, int status)
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
