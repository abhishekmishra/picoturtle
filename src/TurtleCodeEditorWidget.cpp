#include "TurtleCodeEditorWidget.hpp"
#include "TurtleController.hpp"
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QMessageBox>
#include <QIODevice>
#include <QFileInfo>

TurtleCodeEditorWidget::TurtleCodeEditorWidget(QWidget *parent)
	: noname_file_count{0},
	  file_path{QString()}
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

	QVBoxLayout *vb_layout = new QVBoxLayout(this);
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
		"print('Turtle done.')\n");

	noname_file_count += 1;
	emit new_file_created();
}

int TurtleCodeEditorWidget::open_file(const QString &file_path)
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

	emit file_opened();
	return 0;
}

int TurtleCodeEditorWidget::save_file()
{
	if (!has_file_path())
	{
		return -1;
	}
	QFile file(file_path);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::information(this, tr("Unable to open file"),
								 file.errorString());
		return file.error();
	}

	QTextStream out(&file);
	out << turtle_code_edit->toPlainText();
	file.close();

	dirty = false;
	emit file_saved();
}

bool TurtleCodeEditorWidget::has_file_path()
{
	return !(file_path.isNull());
}

bool TurtleCodeEditorWidget::set_file_path(const QString &file_path, bool override_current_path)
{
	bool has_fp = has_file_path();
	if ((has_fp && override_current_path) || (!has_fp))
	{
		this->file_path = file_path;

		set_dirty();
		emit file_path_changed();
		return true;
	}
	return false;
}

void TurtleCodeEditorWidget::set_dirty()
{
	dirty = true;
	emit file_changed();
}

const QString &TurtleCodeEditorWidget::get_file_name()
{
	if (!has_file_path())
	{
		return "noname" + QString::number(noname_file_count);
	}
	return QFileInfo(file_path).fileName();
}

const QString &TurtleCodeEditorWidget::get_file_path()
{
	return file_path;
}
