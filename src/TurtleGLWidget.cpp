#include "TurtleGLWidget.hpp"

#include "core/SkSurface.h"
#include <QPainter>
#include <QTimer>
#include <QPaintEvent>
#include <QImage>
#include <QSize>

#include "Turtle.hpp"

using namespace turtle;

SkColorType TurtleGLWidget::sk_color_type = SkColorType::kUnknown_SkColorType;
SkAlphaType TurtleGLWidget::sk_alpha_type = SkAlphaType::kUnknown_SkAlphaType;

TurtleGLWidget::TurtleGLWidget(QWidget *parent)
	: QOpenGLWidget(parent),
	  turtle{NULL}
{
	elapsed = 0;
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

	// TODO: set all rendering hints to get a much
	// better quality picture than default.
	// need to check which one is acutally required.
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);
	painter.setRenderHint(QPainter::SmoothPixmapTransform);
	painter.setRenderHint(QPainter::VerticalSubpixelPositioning);
	painter.setRenderHint(QPainter::LosslessImageRendering);

	if (turtle == NULL)
	{
		painter.fillRect(event->rect(), background);
	}
	else
	{
		sk_sp<SkImage> img = turtle->getRasterSurface()->makeImageSnapshot();

		// See change below to make a specific kind of image info
		// the extraction of sk_color_type is perhaps no longer required
		// as it can be hardcoded to be the same value as created in SkiaCanvas
		TurtleGLWidget::extract_sk_color_props(img);

		// 12/01/2024 Create the same kind of image info as created in SkiaCanvas
		// This was a result of changes to Skia API wherein we always create an
		// image with specific image info instead of a premul.
		SkImageInfo info = SkImageInfo::Make(img->width(), img->height(),
        	SkColorType::kRGBA_8888_SkColorType, kPremul_SkAlphaType);
		
		std::vector<uint32_t> srcPixels;
		const int rowBytes = img->width() * 4;
		srcPixels.resize(img->height() * rowBytes);
		SkPixmap pixmap(info, (const void *)&srcPixels.front(), rowBytes);
		img->readPixels(pixmap, 0, 0);

		QImage qImg;

		switch (sk_color_type)
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

void TurtleGLWidget::extract_sk_color_props(sk_sp<SkImage> img)
{
	if (sk_color_type == SkColorType::kUnknown_SkColorType)
	{
		// see https://stackoverflow.com/a/48244327/9483968
		// to debug the image color type of the given snapshot
		if (!img.get()->isTextureBacked())
		{

			SkPixmap peek_pixmap;
			img->peekPixels(&peek_pixmap);
			sk_color_type = peek_pixmap.colorType();
			sk_alpha_type = peek_pixmap.alphaType();
			qDebug() << "Extracted SkImage (skia image) color type = " << sk_color_type << " alpha type = " << sk_alpha_type;
		}
		else
		{
			qDebug() << "The given SkImage is a texture on the GPU and can't be determined easily";
		}
	}
}
