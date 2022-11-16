#include "TurtleCodeEditorParentWidget.hpp"
#include <QVBoxLayout>

using namespace turtle;

TurtleCodeEditorParentWidget::TurtleCodeEditorParentWidget(QWidget *parent)
    : tabs{new QTabWidget(this)},
      editor_widgets{QList<TurtleCodeEditorWidget *>()}
{
    QVBoxLayout *vb_layout = new QVBoxLayout(this);
    vb_layout->addWidget(tabs);
    setLayout(vb_layout);
}

TurtleCodeEditorParentWidget::~TurtleCodeEditorParentWidget()
{
}

void TurtleCodeEditorParentWidget::new_file()
{
    TurtleCodeEditorWidget *code_editor = new TurtleCodeEditorWidget(this);
    code_editor->new_file();
    tabs->addTab(code_editor, code_editor->get_file_name());
    editor_widgets.append(code_editor);
}

int TurtleCodeEditorParentWidget::open_file(const QString &file_path)
{
    return 0;
}
int TurtleCodeEditorParentWidget::save_file()
{
    return 0;
}
bool TurtleCodeEditorParentWidget::set_file_path(const QString &file_path, bool override_current_path)
{
    return false;
}
void TurtleCodeEditorParentWidget::run_file()
{
    return;
}

bool TurtleCodeEditorParentWidget::has_file_path() {
    return false;
}
bool TurtleCodeEditorParentWidget::is_dirty() {
    return true;
}
const QString TurtleCodeEditorParentWidget::get_file_name() {
    return "blah";
}

const QString &TurtleCodeEditorParentWidget::get_file_path() {
    return QString();
}
