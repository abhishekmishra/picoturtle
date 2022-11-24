#include "TurtleCanvasWidget.hpp"
#include <QDebug>
#include <QVBoxLayout>

using namespace turtle;
using super = QWidget;

TurtleCanvasWidget::TurtleCanvasWidget(QWidget *parent)
	: gl_widget{new TurtleGLWidget(this)},
	  QWidget(parent)
{
	// QLabel* imgLabel = new QLabel("This is a long label", this);
	// imgLabel->move(20, 20);

	// QVBoxLayout* vb_layout = new QVBoxLayout(this);
	// vb_layout->addWidget(gl_widget);
	// setLayout(vb_layout);
	// vb_layout->setContentsMargins(0, 0, 0, 0);
	setMinimumHeight(400);
	setMinimumWidth(400);
}

TurtleCanvasWidget::~TurtleCanvasWidget()
{
}

void TurtleCanvasWidget::set_turtle(turtle::PicoTurtle *t)
{
	gl_widget->set_turtle(t);
}

void TurtleCanvasWidget::draw_turtle()
{
	gl_widget->draw_turtle();
}

// see https://stackoverflow.com/a/43306929/9483968
// TODO: the following code centers the only child widget of this widget
// TODO: this helps keep the widget square, but the child does not become
// smaller than a particular size if only width is resized.
// Might need to put a layout widget between the canvas and parent or
// between the gl and canvas.

QSize TurtleCanvasWidget::sizeHint() const
{
	return (gl_widget ? gl_widget->sizeHint() : super::sizeHint());
}

void TurtleCanvasWidget::resizeEvent(QResizeEvent *event)
{
	super::resizeEvent(event);
	fixup();
}

void TurtleCanvasWidget::fixup()
{
	if (gl_widget)
	{
		QRect r(QPoint(), QSize(height(), height()));
		r.moveCenter(rect().center());
		gl_widget->setGeometry(r);
	}
}
