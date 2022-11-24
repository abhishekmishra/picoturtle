#include "TurtleLineNumberAreaWidget.hpp"

using namespace turtle;

TurtleLineNumberAreaWidget::TurtleLineNumberAreaWidget(TurtleCodeEditorTextWidget *ce) : QWidget(ce), code_editor{ce}
{
}

QSize TurtleLineNumberAreaWidget::sizeHint() const
{
    return QSize(code_editor->lineNumberAreaWidth(), 0);
}

void TurtleLineNumberAreaWidget::paintEvent(QPaintEvent *event)
{
    code_editor->lineNumberAreaPaintEvent(event);
}
