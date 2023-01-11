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
#include <QFont>
#include <QFontDatabase>
#include <QFontMetrics>
#include <QFileDialog>
#include <QVBoxLayout>

using namespace turtle;

int TurtleCodeEditorWidget::noname_file_count = 0;

TurtleCodeEditorWidget::TurtleCodeEditorWidget(TurtleLuaReplWidget* repl, QWidget* parent)
	: file_path{ QString() }, lua_repl{ repl }
{
	turtle_code_edit = new TurtleCodeEditorTextWidget();
	turtle_code_edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	find_replace = new TurtleFindReplaceTextWidget(turtle_code_edit);
	find_replace->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	// create and set highlighter
	lua_highlighter = new LuaSyntaxHighlighter(turtle_code_edit->document());

	QVBoxLayout* vb_layout = new QVBoxLayout(this);
	vb_layout->addWidget(turtle_code_edit);
	vb_layout->addWidget(find_replace);
	find_replace->hide();
	vb_layout->setSpacing(0);
	setLayout(vb_layout);

	connect(turtle_code_edit, &QPlainTextEdit::modificationChanged, [=](bool flag)
		{ emit file_modified_changed(flag); });

	QFile file(":/lua/turtle/basic_turtle.lua");
	// QFile file(":/lua/learnlua.lua");

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::information(this, tr("Unable to open file"),
			file.errorString());
	}
	else
	{
		basic_turtle_text = file.readAll();
	}

	// new_file();
}

TurtleCodeEditorWidget::~TurtleCodeEditorWidget()
{
	// cleanup the lua state
	lua_repl->cleanup_lua();

	qDebug() << "Editor destroyed.";
}

void TurtleCodeEditorWidget::run_file()
{
	// init a new lua state
	lua_repl->init_lua();

	// execute the script and get the result code
	int res = lua_repl->run_lua_script_path(
		turtle_code_edit->toPlainText().toLocal8Bit().data(), 
		file_path.isEmpty()? NULL : file_path.toLocal8Bit().data()
	);

	lua_repl->run_lua_script("t:drawme()");

	// emit the signal with the res code.
	emit turtle_run_complete(res);
}

void TurtleCodeEditorWidget::new_file()
{
	// TODO: call save if the editor is dirty (not saved)
	turtle_code_edit->clear();
	// TODO: create this text from a resource

	turtle_code_edit->setPlainText(basic_turtle_text);
	noname_fname = "noname" + QString::number(noname_file_count);
	noname_file_count += 1;

	turtle_code_edit->document()->setModified(true);
	emit new_file_created();
}

int TurtleCodeEditorWidget::open_file(const QString& file_path)
{
	QFile file(file_path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return -1;
	}

	set_file_path(file_path, true);
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
		QString fileName = QFileDialog::getSaveFileName(this,
			tr("Save Turtle Lua File"), "",
			tr("Turtle/Lua Files (*.lua)"));
		set_file_path(fileName);
	}
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
	// qDebug() << file_path << "has file path -> " << !(file_path.isNull());
	return !(file_path.isNull());
}

bool TurtleCodeEditorWidget::set_file_path(const QString& file_path, bool override_current_path)
{
	bool has_fp = has_file_path();
	if ((has_fp && override_current_path) || (!has_fp))
	{
		this->file_path = file_path;

		// turtle_code_edit->document()->setModified(true);
		emit file_path_changed();
		return true;
	}
	return false;
}

const QString TurtleCodeEditorWidget::get_file_name()
{
	if (!has_file_path())
	{
		return noname_fname;
	}
	return QString(QFileInfo(file_path).fileName());
}

const QString& TurtleCodeEditorWidget::get_file_path()
{
	return file_path;
}

bool TurtleCodeEditorWidget::is_dirty()
{
	return turtle_code_edit->document()->isModified();
}

TurtleCodeEditorTextWidget* TurtleCodeEditorWidget::get_editor()
{
	return turtle_code_edit;
}

void TurtleCodeEditorWidget::toggle_find()
{
	find_replace->set_replace_enabled(false);
	if (find_replace->isVisible())
	{
		find_replace->hide();
	}
	else 
	{
		find_replace->show();
		find_replace->setFocus(Qt::MouseFocusReason);
	}
}

void TurtleCodeEditorWidget::toggle_find_replace() 
{
	find_replace->set_replace_enabled(true);
	if (find_replace->isVisible())
	{
		find_replace->hide();
	}
	else
	{
		find_replace->show();
		find_replace->setFocus(Qt::MouseFocusReason);
	}
}
