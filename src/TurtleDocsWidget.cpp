#include "TurtleDocsWidget.hpp"
#include <QVBoxLayout>
#include <QFile>
#include <QIODevice>
#include <QMessageBox>

TurtleDocsWidget::TurtleDocsWidget(QWidget *parent)
: docs_text_display{ new QTextBrowser(this)},
QWidget(parent)
{
	docs_text_display->setMinimumHeight(400);
	docs_text_display->setMinimumWidth(300);

    docs_text_display->setReadOnly(true);
	docs_text_display->setTextInteractionFlags(Qt::TextBrowserInteraction);
	docs_text_display->setOpenExternalLinks(true);
	docs_text_display->setOpenLinks(true);

    QFile file(":/docs/help.html");
	// QFile file(":/lua/learnlua.lua");

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::information(this, tr("Unable to open file"),
								 file.errorString());
	} else {
		QString docs_text = file.readAll();
        docs_text_display->setHtml(docs_text);
	}

   	QVBoxLayout* vb_layout = new QVBoxLayout(this);
	vb_layout->addWidget(docs_text_display);
	setLayout(vb_layout);
}
