#include "TurtleGLWidget.hpp"

#include <QPainter>
#include <QTimer>
#include <QPaintEvent>

TurtleGLWidget::TurtleGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      turtle{NULL}
{
    elapsed = 0;
    setFixedSize(300, 300);
    setAutoFillBackground(false);
    background = QBrush(QColor(64, 32, 64));

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TurtleGLWidget::animate);
    timer->start(50);
}

void TurtleGLWidget::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer *>(sender())->interval()) % 1000;
    update();
}

void TurtleGLWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // helper->paint(&painter, event, elapsed);

    painter.fillRect(event->rect(), background);
    painter.translate(150, 150);

    painter.setPen(textPen);
    painter.setFont(textFont);
    if (turtle == NULL) 
    {
        painter.drawText(QRect(-50, -50, 100, 100), Qt::AlignCenter, QStringLiteral("Yolo"));
    }
    else
    {
        QString out_text = QString("Turtle -> ") + QString::fromStdString(turtle->getName());
        painter.drawText(QRect(-100, -100, 150, 150), Qt::AlignCenter, out_text);
    }
    painter.end();
}

void TurtleGLWidget::set_turtle(turtle::PicoTurtle* t)
{
    turtle = t;
}