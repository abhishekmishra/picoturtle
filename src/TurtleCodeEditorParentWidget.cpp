#include "TurtleCodeEditorParentWidget.hpp"
#include <QVBoxLayout>

using namespace turtle;

TurtleCodeEditorParentWidget::TurtleCodeEditorParentWidget(QWidget* parent)
	: tabs{ new QTabWidget(this) },
	editor_widgets{ QList<TurtleCodeEditorWidget*>() }
{
	QVBoxLayout* vb_layout = new QVBoxLayout(this);
	vb_layout->addWidget(tabs);
	setLayout(vb_layout);

	tabs->setTabsClosable(true);
	tabs->setDocumentMode(true);

	connect(tabs, &QTabWidget::tabCloseRequested, [=](int idx) {
		editor_widgets.removeOne(tabs->widget(idx));
		delete tabs->widget(idx);
		tabs->removeTab(idx);
		});

	connect(tabs, &QTabWidget::currentChanged, [=](int idx) {
		emit current_tab_changed(idx);
		});

	new_file();
}

TurtleCodeEditorParentWidget::~TurtleCodeEditorParentWidget()
{
}

void TurtleCodeEditorParentWidget::new_file()
{
	TurtleCodeEditorWidget* code_editor = new_code_editor();
	code_editor->new_file();
}

int TurtleCodeEditorParentWidget::open_file(const QString& file_path)
{
	TurtleCodeEditorWidget* code_editor = new_code_editor();
	return code_editor->open_file(file_path);
}

int TurtleCodeEditorParentWidget::save_file()
{
	TurtleCodeEditorWidget* code_editor = get_current_editor_widget();
	if (code_editor != NULL)
	{
		return code_editor->save_file();
	}
	return -1;
}

bool TurtleCodeEditorParentWidget::set_file_path(const QString& file_path, bool override_current_path)
{
	TurtleCodeEditorWidget* code_editor = get_current_editor_widget();
	if (code_editor != NULL)
	{
		return code_editor->set_file_path(file_path, override_current_path);
	}
	return -1;
}

void TurtleCodeEditorParentWidget::run_file()
{
	TurtleCodeEditorWidget* code_editor = get_current_editor_widget();
	if (code_editor != NULL)
	{
		return code_editor->run_file();
	}
}

bool TurtleCodeEditorParentWidget::has_file_path()
{
	TurtleCodeEditorWidget* code_editor = get_current_editor_widget();
	if (code_editor != NULL)
	{
		return code_editor->has_file_path();
	}
	return false;
}

bool TurtleCodeEditorParentWidget::is_dirty()
{
	TurtleCodeEditorWidget* code_editor = get_current_editor_widget();
	if (code_editor != NULL)
	{
		return code_editor->is_dirty();
	}
	return false;
}
const QString TurtleCodeEditorParentWidget::get_file_name()
{
	TurtleCodeEditorWidget* code_editor = get_current_editor_widget();
	if (code_editor != NULL)
	{
		return code_editor->get_file_name();
	}
	return QString();
}

const QString& TurtleCodeEditorParentWidget::get_file_path()
{
	TurtleCodeEditorWidget* code_editor = get_current_editor_widget();
	if (code_editor != NULL)
	{
		return code_editor->get_file_path();
	}
	return QString();
}

TurtleCodeEditorWidget* TurtleCodeEditorParentWidget::get_current_editor_widget()
{
	// qDebug() << "current index is ->" << tabs->currentIndex();
	return (TurtleCodeEditorWidget*)tabs->currentWidget();
}

/**
* Creates a new code editor widget, and adds it to the tabs widget.
* The newly created widget is also made the current tab.
* All of the code editor widget's signals are connected to this widget.
*/
TurtleCodeEditorWidget* TurtleCodeEditorParentWidget::new_code_editor()
{
	TurtleCodeEditorWidget* code_editor = new TurtleCodeEditorWidget(this);
	tabs->addTab(code_editor, code_editor->get_file_name());
	editor_widgets.append(code_editor);
	tabs->setCurrentWidget(code_editor);
    code_editor->layout()->setContentsMargins(0, 0, 0, 0);

	connect(code_editor, &TurtleCodeEditorWidget::new_file_created, [=]() {
		emit new_file_created();
		});
	connect(code_editor, &TurtleCodeEditorWidget::file_opened, [=]() {
		emit file_opened();
		});
	connect(code_editor, &TurtleCodeEditorWidget::file_saved, [=]() {
		emit file_saved();
		});
	connect(code_editor, &TurtleCodeEditorWidget::turtle_run_complete, [=](const int error_code) {
		emit turtle_run_complete(error_code);
		});
	connect(code_editor, &TurtleCodeEditorWidget::file_path_changed, [=]()
		{
			int idx = tabs->indexOf(code_editor);
	QString tab_name = code_editor->get_file_name();
	if (code_editor->is_dirty()) {
		tab_name += "*";
	}
	tabs->setTabText(idx, tab_name);
	emit file_path_changed();
		});

	connect(code_editor, &TurtleCodeEditorWidget::file_modified_changed, [=](bool modified) {
		int idx = tabs->indexOf(code_editor);
	QString tab_name = code_editor->get_file_name();
	if (code_editor->is_dirty()) {
		tab_name += "*";
	}
	tabs->setTabText(idx, tab_name);
	emit file_modified_changed(modified);
		});

	return code_editor;
}
