#include <QBoxLayout>
#include "LuaReplWidget.hpp"

using namespace turtle;


LuaReplWidget::LuaReplWidget(QWidget* parent)
{
	lua_repl_edit = new QPlainTextEdit(this);

	// Set the default monospace font for now
	// TODO: perhaps include a decent open source font
	QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
	font.setPointSize(12);
	QFontInfo info(font);
	// qDebug() << font << info.family() << info.fixedPitch();
	lua_repl_edit->setFont(font);

	QPalette p = lua_repl_edit->palette();

	// set background colour
	p.setColor(QPalette::Active, QPalette::Base, Qt::darkBlue);
	p.setColor(QPalette::Inactive, QPalette::Base, Qt::darkBlue);

	// set foreground colour
	p.setColor(QPalette::Active, QPalette::Text, Qt::white);
	p.setColor(QPalette::Inactive, QPalette::Text, Qt::white);

	lua_repl_edit->setPalette(p);

	lua_repl_edit->setMinimumHeight(150);

	QVBoxLayout* vb_layout = new QVBoxLayout(this);
	vb_layout->addWidget(lua_repl_edit);
	setLayout(vb_layout);
};
