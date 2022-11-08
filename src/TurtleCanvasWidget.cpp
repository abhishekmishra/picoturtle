#include "TurtleCanvasWidget.hpp"
#include <QDebug>
#include <QVBoxLayout>

TurtleCanvasWidget::TurtleCanvasWidget(QWidget* parent)
	: sdl_widget{ new TurtleSDLWidget(this) },
	QWidget(parent)
{
	//QLabel* imgLabel = new QLabel("This is a long label", this);
	//imgLabel->move(20, 20);

	QVBoxLayout* vb_layout = new QVBoxLayout(this);
	vb_layout->addWidget(sdl_widget);
	setLayout(vb_layout);
}

TurtleCanvasWidget::~TurtleCanvasWidget()
{
}

