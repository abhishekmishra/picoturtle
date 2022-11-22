#include "TurtleCodeEditorParentWidget.hpp"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>

using namespace turtle;

TurtleCodeEditorParentWidget::TurtleCodeEditorParentWidget(QWidget *parent)
	: tabs{new QTabWidget(this)}
{
	QVBoxLayout *vb_layout = new QVBoxLayout(this);
	vb_layout->addWidget(tabs);
	setLayout(vb_layout);

	tabs->setTabsClosable(true);
	tabs->setDocumentMode(true);

	connect(tabs, &QTabWidget::tabCloseRequested, [=](int idx)
			{
		// get the handle to the editor widget
		// then remove the tab
		// and then delete the widget object.
		TurtleCodeEditorWidget* editor_widget = (TurtleCodeEditorWidget*)tabs->widget(idx);
		if(editor_widget->is_dirty())
		{
			QMessageBox::StandardButton btn = QMessageBox::question(
				this, 
				"File has changes!", 
				"File " + editor_widget->get_file_name() 
					+ "has unsaved changed. Save before closing tab?", 
				QMessageBox::Cancel 
				| QMessageBox::No 
				| QMessageBox::Yes,
				QMessageBox::Yes);
			switch(btn)
			{
				case QMessageBox::Yes:
					qDebug() << "you chose to save file and close tab";
					editor_widget->save_file();
					delete_editor_and_tab_at_idx(idx);
					break;
				case QMessageBox::No:
					qDebug() << "you chose to close tab without saving the file";
					delete_editor_and_tab_at_idx(idx);
					break;
				case QMessageBox::Cancel:
					qDebug() << "you chose to cancel close tab";
					break;
				case QMessageBox::Escape:
					qDebug() << "you chose to cancel close tab";
					break;
				default:
					qWarning() << "This is an unexpected response from the dialog!";
					break;
			}
		} });

	connect(tabs, &QTabWidget::currentChanged, [=](int idx)
			{ emit current_tab_changed(idx); });

	new_file();
}

TurtleCodeEditorParentWidget::~TurtleCodeEditorParentWidget()
{
}

void TurtleCodeEditorParentWidget::delete_editor_and_tab_at_idx(int idx)
{
	TurtleCodeEditorWidget *editor_widget = (TurtleCodeEditorWidget *)tabs->widget(idx);
	tabs->removeTab(idx);
	delete editor_widget;
}

void TurtleCodeEditorParentWidget::new_file()
{
	TurtleCodeEditorWidget *code_editor = new_code_editor();
	code_editor->new_file();
}

int TurtleCodeEditorParentWidget::open_file(const QString &file_path)
{
	TurtleCodeEditorWidget *code_editor = new_code_editor();
	return code_editor->open_file(file_path);
}

int TurtleCodeEditorParentWidget::save_file()
{
	TurtleCodeEditorWidget *code_editor = get_current_editor_widget();
	if (code_editor != NULL)
	{
		return code_editor->save_file();
	}
	return -1;
}

bool TurtleCodeEditorParentWidget::set_file_path(const QString &file_path, bool override_current_path)
{
	TurtleCodeEditorWidget *code_editor = get_current_editor_widget();
	if (code_editor != NULL)
	{
		return code_editor->set_file_path(file_path, override_current_path);
	}
	return -1;
}

void TurtleCodeEditorParentWidget::run_file()
{
	TurtleCodeEditorWidget *code_editor = get_current_editor_widget();
	if (code_editor != NULL)
	{
		return code_editor->run_file();
	}
}

bool TurtleCodeEditorParentWidget::has_file_path()
{
	TurtleCodeEditorWidget *code_editor = get_current_editor_widget();
	if (code_editor != NULL)
	{
		return code_editor->has_file_path();
	}
	return false;
}

bool TurtleCodeEditorParentWidget::is_dirty()
{
	TurtleCodeEditorWidget *code_editor = get_current_editor_widget();
	if (code_editor != NULL)
	{
		return code_editor->is_dirty();
	}
	return false;
}
const QString TurtleCodeEditorParentWidget::get_file_name()
{
	TurtleCodeEditorWidget *code_editor = get_current_editor_widget();
	if (code_editor != NULL)
	{
		return code_editor->get_file_name();
	}
	return QString();
}

const QString &TurtleCodeEditorParentWidget::get_file_path()
{
	TurtleCodeEditorWidget *code_editor = get_current_editor_widget();
	if (code_editor != NULL)
	{
		return code_editor->get_file_path();
	}
	// TODO: is this the correct way to return an empty string in cpp/qt
	return *(new QString());
}

TurtleCodeEditorWidget *TurtleCodeEditorParentWidget::get_current_editor_widget()
{
	// qDebug() << "current index is ->" << tabs->currentIndex();
	return (TurtleCodeEditorWidget *)tabs->currentWidget();
}

/**
 * Creates a new code editor widget, and adds it to the tabs widget.
 * The newly created widget is also made the current tab.
 * All of the code editor widget's signals are connected to this widget.
 */
TurtleCodeEditorWidget *TurtleCodeEditorParentWidget::new_code_editor()
{
	TurtleCodeEditorWidget *code_editor = new TurtleCodeEditorWidget(this);
	tabs->addTab(code_editor, code_editor->get_file_name());
	tabs->setCurrentWidget(code_editor);
	code_editor->layout()->setContentsMargins(0, 0, 0, 0);

	connect(code_editor, &TurtleCodeEditorWidget::new_file_created, [=]()
			{ emit new_file_created(); });
	connect(code_editor, &TurtleCodeEditorWidget::file_opened, [=]()
			{ emit file_opened(); });
	connect(code_editor, &TurtleCodeEditorWidget::file_saved, [=]()
			{ emit file_saved(); });
	connect(code_editor, &TurtleCodeEditorWidget::turtle_run_complete, [=](const int error_code)
			{ emit turtle_run_complete(error_code); });
	connect(code_editor, &TurtleCodeEditorWidget::file_path_changed, [=]()
			{
			int idx = tabs->indexOf(code_editor);
	QString tab_name = code_editor->get_file_name();
	if (code_editor->is_dirty()) {
		tab_name += "*";
	}
	tabs->setTabText(idx, tab_name);
	emit file_path_changed(); });

	connect(code_editor, &TurtleCodeEditorWidget::file_modified_changed, [=](bool modified)
			{
		int idx = tabs->indexOf(code_editor);
	QString tab_name = code_editor->get_file_name();
	if (code_editor->is_dirty()) {
		tab_name += "*";
	}
	tabs->setTabText(idx, tab_name);
	emit file_modified_changed(modified); });

	return code_editor;
}
