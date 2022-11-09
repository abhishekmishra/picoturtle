#include "TurtleGLWidget.hpp"

#include <QPainter>
#include <QTimer>
#include <QPaintEvent>

TurtleGLWidget::TurtleGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    elapsed = 0;
    setFixedSize(200, 200);
    setAutoFillBackground(false);
    background = QBrush(QColor(64, 32, 64));
}

void TurtleGLWidget::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    update();
}

void TurtleGLWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // helper->paint(&painter, event, elapsed);

    painter.fillRect(event->rect(), background);
    painter.translate(100, 100);

    painter.setPen(textPen);
    painter.setFont(textFont);
    painter.drawText(QRect(-50, -50, 100, 100), Qt::AlignCenter, QStringLiteral("Yolo"));

    painter.end();
}