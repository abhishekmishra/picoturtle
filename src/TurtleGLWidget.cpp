#include "TurtleGLWidget.hpp"

#include "core/SkSurface.h"
#include <QPainter>
#include <QTimer>
#include <QPaintEvent>
#include <QImage>

#include "Turtle.hpp"

TurtleGLWidget::TurtleGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      turtle{NULL}
{
    elapsed = 0;
    setFixedSize(TURTLE_DEFAULT_CANVAS_WIDTH, TURTLE_DEFAULT_CANVAS_HEIGHT);
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
    if (turtle == NULL) 
    {
        painter.translate(150, 150);

        painter.setPen(textPen);
        painter.setFont(textFont);
        painter.drawText(QRect(-50, -50, 100, 100), Qt::AlignCenter, QStringLiteral("Yolo"));
    }
    else
    {
        // QString out_text = QString("Turtle -> ") + QString::fromStdString(turtle->getName());
        // painter.drawText(QRect(-100, -100, 150, 150), Qt::AlignCenter, out_text);
        sk_sp<SkImage> img = turtle->getRasterSurface()->makeImageSnapshot();

        SkImageInfo info = SkImageInfo::MakeN32Premul(img->width(), img->height());
        std::vector<uint32_t> srcPixels;
        const int rowBytes = img->width() * 4;
        srcPixels.resize(img->height() * rowBytes);
        SkPixmap pixmap(info, (const void *)&srcPixels.front(), rowBytes);
        img->readPixels(pixmap, 0, 0);

        QImage qImg = QImage((uchar*)&srcPixels.front(),img->width(), img->height(), QImage::Format_RGBA8888_Premultiplied);

        painter.drawImage(QPointF(0, 0), qImg);
    }
    painter.end();
}

void TurtleGLWidget::set_turtle(turtle::PicoTurtle* t)
{
    turtle = t;
}