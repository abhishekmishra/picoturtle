#include "TurtleCodeEditorWidget.hpp"
#include "TurtleController.hpp"
#include <QVBoxLayout>

TurtleCodeEditorWidget::TurtleCodeEditorWidget(QWidget* parent)
	: noname_file_count{ 0 }
{
	turtle_code_edit = new QTextEdit(this);
	turtle_code_edit->setMinimumHeight(500);
	turtle_code_edit->setMinimumWidth(400);
	turtle_code_edit->setText("blah");
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
	turtle::TurtleController::run_lua_script(turtle_code_edit->toPlainText().toLocal8Bit().data());
	emit turtle_run_complete();
}

void TurtleCodeEditorWidget::new_file()
{
	// TODO: call save if the editor is dirty (not saved)
	turtle_code_edit->clear();
	turtle_code_edit->setText("--empty turtle lua program");
	filename = "noname" + QString::number(noname_file_count);
	noname_file_count += 1;
	emit new_file_created(filename);
}
