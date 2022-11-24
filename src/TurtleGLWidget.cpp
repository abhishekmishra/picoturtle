#include "TurtleGLWidget.hpp"

#include "core/SkSurface.h"
#include <QPainter>
#include <QTimer>
#include <QPaintEvent>
#include <QImage>
#include <QSize>

#include "Turtle.hpp"

TurtleGLWidget::TurtleGLWidget(QWidget *parent)
	: QOpenGLWidget(parent),
	  turtle{NULL}
{
	elapsed = 0;
	// setFixedSize(GLWIDGET_WIDTH, GLWIDGET_HEIGHT);
	setAutoFillBackground(false);
	background = QBrush(Qt::white);

	// TODO: check if this is needed for resize anymore
	QOpenGLWidget::setUpdateBehavior(QOpenGLWidget::PartialUpdate);

	// QSizePolicy p(QSizePolicy::Preferred, QSizePolicy::Preferred);
	// p.setHeightForWidth(true);
	// setSizePolicy(p);

	// QTimer *timer = new QTimer(this);
	// connect(timer, &QTimer::timeout, this, &TurtleGLWidget::animate);
	// timer->start(50);
}

void TurtleGLWidget::animate()
{
	elapsed = (elapsed + qobject_cast<QTimer *>(sender())->interval()) % 1000;
	update();
}

void TurtleGLWidget::draw_turtle()
{
	update();
}

void TurtleGLWidget::paintEvent(QPaintEvent *event)
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

		// see https://stackoverflow.com/a/48244327/9483968
		// to debug the image color type of the given snapshot
		if (!img.get()->isTextureBacked())
		{
			SkPixmap peek_pixmap;
			img->peekPixels(&peek_pixmap);
			SkColorType colorType = peek_pixmap.colorType();
			SkAlphaType alphaType = peek_pixmap.alphaType();
			qDebug() << "color type = " << colorType << " alpha type = " << alphaType;

			SkImageInfo info = SkImageInfo::MakeN32Premul(img->width(), img->height());
			std::vector<uint32_t> srcPixels;
			const int rowBytes = img->width() * 4;
			srcPixels.resize(img->height() * rowBytes);
			SkPixmap pixmap(info, (const void *)&srcPixels.front(), rowBytes);
			img->readPixels(pixmap, 0, 0);

			QImage qImg;

			switch(colorType)
			{
				case kRGBA_8888_SkColorType:
					qImg = QImage((uchar *)&srcPixels.front(), img->width(), img->height(), QImage::Format_RGBA8888);
					break;

				case kBGRA_8888_SkColorType:
					qImg = QImage((uchar *)&srcPixels.front(), img->width(), img->height(), QImage::Format_ARGB32_Premultiplied);
					break;

				default:
					qDebug() << "Not sure how to deal with this image color type";
					break;

			}
			// qDebug() << "Image size is" << img->width() << "x" << img->height();
			// qDebug() << "GLWidget size is" << size().width() << "x" << size().height();

			painter.drawImage(QRect(0, 0, size().width(), size().height()), qImg);
		}
		else
		{
			qDebug() << "this is a texture on the GPU and can't be determined easily";
		}
	}
	painter.end();
}

void TurtleGLWidget::set_turtle(turtle::PicoTurtle *t)
{
	turtle = t;
}

void TurtleGLWidget::resizeEvent(QResizeEvent *event)
{
	event->accept();
	// qDebug() << "GLWidget size is" << size().width() << "x" << size().height();
	if (event->size().width() > event->size().height())
	{
		QOpenGLWidget::resize(event->size().height(), event->size().height());
	}
	else
	{
		QOpenGLWidget::resize(event->size().width(), event->size().width());
	}

	// NOTE: see documentation note about this method in qt documentation
	QOpenGLWidget::resizeEvent(event);
}
