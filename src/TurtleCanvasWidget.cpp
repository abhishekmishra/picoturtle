#include "TurtleCanvasWidget.hpp"
#include <QPushButton>
#include <QDebug>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QLabel>

TurtleCanvasWidget::TurtleCanvasWidget(QWidget *parent)
    : QScrollArea(parent)
{
    setBackgroundRole(QPalette::Dark);
    QLabel* imgLabel = new QLabel("Yo", this);
    setWidget(imgLabel);
}

TurtleCanvasWidget::~TurtleCanvasWidget()
{
}

