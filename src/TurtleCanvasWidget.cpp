#include "TurtleCanvasWidget.hpp"
#include <QPushButton>
#include <QDebug>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>

TurtleCanvasWidget::TurtleCanvasWidget(QWidget *parent)
    : QWidget(parent)
{
    QLabel* imgLabel = new QLabel("This is a long label", this);
    imgLabel->move(20, 20);
}

TurtleCanvasWidget::~TurtleCanvasWidget()
{
}

