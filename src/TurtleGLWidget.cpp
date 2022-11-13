#include "TurtleGLWidget.hpp"

#include "core/SkSurface.h"
#include <QPainter>
#include <QTimer>
#include <QPaintEvent>
#include <QImage>

#include "Turtle.hpp"

TurtleGLWidget::TurtleGLWidget(QWidget* parent)
	: QOpenGLWidget(parent),
	turtle{ NULL }
{
	elapsed = 0;
	setFixedSize(GLWIDGET_WIDTH, GLWIDGET_HEIGHT);
	setAutoFillBackground(false);
	background = QBrush(Qt::white);

	//QTimer *timer = new QTimer(this);
	//connect(timer, &QTimer::timeout, this, &TurtleGLWidget::animate);
	//timer->start(50);
}

void TurtleGLWidget::animate()
{
	elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
	update();
}

void TurtleGLWidget::draw_turtle()
{
	update();
}

void TurtleGLWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter;
	painter.begin(this);

	if (turtle == NULL)
	{
		painter.fillRect(event->rect(), background);
	}
	else
	{
		sk_sp<SkImage> img = turtle->getRasterSurface()->makeImageSnapshot();

		SkImageInfo info = SkImageInfo::MakeN32Premul(img->width(), img->height());
		std::vector<uint32_t> srcPixels;
		const int rowBytes = img->width() * 4;
		srcPixels.resize(img->height() * rowBytes);
		SkPixmap pixmap(info, (const void*)&srcPixels.front(), rowBytes);
		img->readPixels(pixmap, 0, 0);

		qDebug() << "Image size is" << img->width() << "x" << img->height();

#if SK_PMCOLOR_BYTE_ORDER(B,G,R,A)
		QImage qImg = QImage((uchar*)&srcPixels.front(), img->width(), img->height(), QImage::Format_ARGB32);
#elif SK_PMCOLOR_BYTE_ORDER(R,G,B,A)
		//TODO: learn which one to use rgba8888 or rgba32premul
		QImage qImg = QImage((uchar*)&srcPixels.front(), img->width(), img->height(), QImage::Format_RGBA8888);
#else
		qDebug() << "Undefined byte order of sk color";
#endif
		painter.drawImage(QRect(0, 0, GLWIDGET_WIDTH, GLWIDGET_HEIGHT), qImg);
	}
	painter.end();
}

void TurtleGLWidget::set_turtle(turtle::PicoTurtle* t)
{
	turtle = t;
}
