#include "TurtleCodeEditorWidget.hpp"
#include "TurtleController.hpp"
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QMessageBox>
#include <QIODevice>
#include <QFileInfo>
#include <QDebug>

TurtleCodeEditorWidget::TurtleCodeEditorWidget(QWidget *parent)
	: noname_file_count{0},
	  file_path{QString()}
{
	turtle_code_edit = new QPlainTextEdit(this);
	turtle_code_edit->setMinimumHeight(500);
	turtle_code_edit->setMinimumWidth(400);

	// Set the default monospace font for now
	// TODO: perhaps include a decent open source font
	// QFont font("monospace");
	QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
	font.setPointSize(12);
	QFontInfo info(font);
	qDebug() << font << info.family() << info.fixedPitch();
	turtle_code_edit->setFont(font);

	QPalette p = turtle_code_edit->palette();

	// set background colour
	p.setColor(QPalette::Active, QPalette::Base, Qt::darkBlue);
	p.setColor(QPalette::Inactive, QPalette::Base, Qt::darkBlue);

	// set foreground colour
	p.setColor(QPalette::Active, QPalette::Text, Qt::yellow);
	p.setColor(QPalette::Inactive, QPalette::Text, Qt::yellow);

	turtle_code_edit->setPalette(p);

	// set minimum width of code_edit
	turtle_code_edit->setMinimumWidth(512);

	// create and set highlighter
	lua_highlighter = new LuaSyntaxHighlighter(turtle_code_edit->document());

	QVBoxLayout *vb_layout = new QVBoxLayout(this);
	vb_layout->addWidget(turtle_code_edit);
	setLayout(vb_layout);

	connect(turtle_code_edit, &QPlainTextEdit::modificationChanged, [=](bool flag){
		emit file_modified_changed(flag);
	});

	QFile file(":/lua/turtle/basic_turtle.lua");
	// QFile file(":/lua/learnlua.lua");

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::information(this, tr("Unable to open file"),
								 file.errorString());
	} else {
		basic_turtle_text = file.readAll();
	}

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

	turtle_code_edit->setPlainText(basic_turtle_text);

	noname_file_count += 1;

	turtle_code_edit->document()->setModified(true);
	emit new_file_created();
}

int TurtleCodeEditorWidget::open_file(const QString &file_path)
{
	QFile file(file_path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return -1;
	}

	set_file_path(file_path);
	turtle_code_edit->document()->setModified(false);
	QTextStream in(&file);
	QString text = in.readAll();
	turtle_code_edit->clear();
	turtle_code_edit->setPlainText(text);

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

	turtle_code_edit->document()->setModified(false);
	emit file_saved();
	return 0;
}

bool TurtleCodeEditorWidget::has_file_path()
{
	qDebug() << file_path << "has file path -> " << !(file_path.isNull());
	return !(file_path.isNull());
}

bool TurtleCodeEditorWidget::set_file_path(const QString &file_path, bool override_current_path)
{
	bool has_fp = has_file_path();
	if ((has_fp && override_current_path) || (!has_fp))
	{
		this->file_path = file_path;

		turtle_code_edit->document()->setModified(true);
		emit file_path_changed();
		return true;
	}
	return false;
}

const QString TurtleCodeEditorWidget::get_file_name()
{
	if (!has_file_path())
	{
		return "noname" + QString::number(noname_file_count);
	}
	return QString(QFileInfo(file_path).fileName());
}

const QString &TurtleCodeEditorWidget::get_file_path()
{
	return file_path;
}

bool TurtleCodeEditorWidget::is_dirty()
{
	return turtle_code_edit->document()->isModified();
}