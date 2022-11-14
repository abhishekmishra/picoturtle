#include <QVBoxLayout>
#include <QDateTime>
#include "TurtleConsoleWidget.hpp"

TurtleConsoleWidget::TurtleConsoleWidget(QWidget* parent)
{
	turtle_console_edit = new QPlainTextEdit(this);

	// Set the default monospace font for now
	// TODO: perhaps include a decent open source font
	QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
	font.setPointSize(12);
	QFontInfo info(font);
	// qDebug() << font << info.family() << info.fixedPitch();
	turtle_console_edit->setFont(font);

	QPalette p = turtle_console_edit->palette();

	// set background colour
	p.setColor(QPalette::Active, QPalette::Base, Qt::darkBlue);
	p.setColor(QPalette::Inactive, QPalette::Base, Qt::darkBlue);

	// set foreground colour
	p.setColor(QPalette::Active, QPalette::Text, Qt::white);
	p.setColor(QPalette::Inactive, QPalette::Text, Qt::white);

	turtle_console_edit->setPalette(p);

	turtle_console_edit->setMinimumHeight(150);

	QVBoxLayout* vb_layout = new QVBoxLayout(this);
	vb_layout->addWidget(turtle_console_edit);
	setLayout(vb_layout);
}

TurtleConsoleWidget::~TurtleConsoleWidget()
{
}

void TurtleConsoleWidget::info(const QString& msg)
{
	QDateTime dt = QDateTime::currentDateTime();
	QString formattedTime = dt.toString("dd.MM.yyyy hh:mm:ss");

	turtle_console_edit->appendPlainText(formattedTime + ": info: " + msg);
}

void TurtleConsoleWidget::warning(const QString& msg)
{
}

void TurtleConsoleWidget::error(const QString& msg)
{
}

void TurtleConsoleWidget::fatal(const QString& msg)
{
}

