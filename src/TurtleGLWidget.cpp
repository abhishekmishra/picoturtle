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
	setFixedSize(TURTLE_DEFAULT_CANVAS_WIDTH, TURTLE_DEFAULT_CANVAS_HEIGHT);
	setAutoFillBackground(false);
	background = QBrush(QColor(64, 32, 64));
	//textPen = QPen(Qt::white);
	//textFont.setPixelSize(50);

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
		painter.setRenderHint(QPainter::Antialiasing);
		painter.fillRect(event->rect(), background);
		painter.translate(150, 150);

		// TODO: this causes a crash in qtgl glyph cache (only on windows)
		// not sure of the reasons - as none of the vals are managed by this class.
		// commenting out for now since not needed at all.
		// Most probably a qt bug.

		//painter.setPen(textPen);
		//painter.setFont(textFont);
		//painter.drawText(QRect(-50, -50, 100, 100), Qt::AlignCenter, QStringLiteral("Yolo"));
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

#if SK_PMCOLOR_BYTE_ORDER(B,G,R,A)
		QImage qImg = QImage((uchar*)&srcPixels.front(), img->width(), img->height(), QImage::Format_ARGB32);
#elif SK_PMCOLOR_BYTE_ORDER(R,G,B,A)
		QImage qImg = QImage((uchar*)&srcPixels.front(), img->width(), img->height(), QImage::Format_RGBA8888_Premultiplied);
#else
		qDebug() << "Undefined byte order of sk color";
#endif
		painter.drawImage(QPointF(0, 0), qImg);
	}
	painter.end();
}

void TurtleGLWidget::set_turtle(turtle::PicoTurtle* t)
{
	turtle = t;
}
