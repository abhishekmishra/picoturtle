#include "TurtleCanvasWidget.hpp"
#include <QDebug>
#include <QVBoxLayout>

TurtleCanvasWidget::TurtleCanvasWidget(QWidget* parent)
	: gl_widget{ new TurtleGLWidget(this) },
	QWidget(parent)
{
	//QLabel* imgLabel = new QLabel("This is a long label", this);
	//imgLabel->move(20, 20);

	QVBoxLayout* vb_layout = new QVBoxLayout(this);
	vb_layout->addWidget(gl_widget);
	setLayout(vb_layout);
}

TurtleCanvasWidget::~TurtleCanvasWidget()
{
}

void TurtleCanvasWidget::set_turtle(turtle::PicoTurtle* t)
{
	gl_widget->set_turtle(t);
}