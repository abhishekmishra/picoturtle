#include "TurtleCodeEditorWidget.hpp"
#include "TurtleController.hpp"
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QIODevice>

TurtleCodeEditorWidget::TurtleCodeEditorWidget(QWidget* parent)
	: noname_file_count{ 0 }
{
	turtle_code_edit = new QTextEdit(this);
	turtle_code_edit->setMinimumHeight(500);
	turtle_code_edit->setMinimumWidth(400);

	// Set the default monospace font for now
	// TODO: perhaps include a decent open source font
	QFont font("Courier");
	font.setPointSize(12);
	QFontInfo info(font);
	qDebug() << font << info.family() << info.fixedPitch();
	turtle_code_edit->setFont(font);

	QPalette p = turtle_code_edit->palette();
	
	// set background colour
	p.setColor(QPalette::Active, QPalette::Base, Qt::black);
	p.setColor(QPalette::Inactive, QPalette::Base, Qt::black);

	// set foreground colour
	p.setColor(QPalette::Active, QPalette::Text, Qt::green);
	p.setColor(QPalette::Inactive, QPalette::Text, Qt::green);

	turtle_code_edit->setPalette(p);

	QVBoxLayout* vb_layout = new QVBoxLayout(this);
	vb_layout->addWidget(turtle_code_edit);
	setLayout(vb_layout);

	new_file();
}

TurtleCodeEditorWidget::~TurtleCodeEditorWidget()
{
}

void TurtleCodeEditorWidget::run_file()
{
	int res = turtle::TurtleController::run_lua_script(turtle_code_edit->toPlainText().toLocal8Bit().data());
	emit turtle_run_complete(res);
}

void TurtleCodeEditorWidget::new_file()
{
	// TODO: call save if the editor is dirty (not saved)
	turtle_code_edit->clear();
	// TODO: create this text from a resource

	turtle_code_edit->setText(
		"-- turtle lua program\n"
		"local picoturtle = require 'picoturtle'\n"
		"local t = t or picoturtle.new()\n"
		"t:penup()\n"
		"t:back(100)\n"
		"t:pendown()\n"
		"t:pencolour(255, 0, 0)\n"
		"t:forward(100)\n"
		"t:pencolour(0, 255, 0)\n"
		"t:forward(100)\n"
		"t:pencolour(0, 0, 255)\n"
		"t:forward(100)\n"
		"print('Turtle done.')\n"
	);
	filename = "noname" + QString::number(noname_file_count);
	noname_file_count += 1;
	emit new_file_created(filename);
}

int TurtleCodeEditorWidget::open_file(const QString& file_path)
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
        return -1;
	}

    QTextStream in(&file);
	QString text = in.readAll();
	turtle_code_edit->clear();
	turtle_code_edit->setText(text);

	emit file_opened(file_path);
	return 0;
}

