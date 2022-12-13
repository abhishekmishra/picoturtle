#include "TurtleAboutDialog.hpp"
#include <QLabel>
#include <QDialogButtonBox>
#include <QVBoxLayout>

using namespace turtle;


void enable_hyperlinks(QLabel* label)
{
	label->setTextFormat(Qt::RichText);
	label->setTextInteractionFlags(Qt::TextBrowserInteraction);
	label->setOpenExternalLinks(true);
}

TurtleAboutDialog::TurtleAboutDialog(QWidget* parent)
{
	setWindowTitle(tr("About PicoTurtle"));

	// TODO: get actual version from cmake build.
	QLabel* program_name = new QLabel("<a href='https://github.com/abhishekmishra/picoturtle'>PicoTurtle</a> v0.1.0-alpha is a Turtle programming environment.");
	enable_hyperlinks(program_name);

	QLabel* author_name = new QLabel("by Abhishek Mishra (<a href='https://github.com/abhishekmishra'>github.com/abhishekmishra</a>)");
	enable_hyperlinks(author_name);

	QLabel* license_brief = new QLabel("PicoTurtle license - <a href='https://raw.githubusercontent.com/abhishekmishra/picoturtle/main/LICENSE'>GNU General Public License v3.0 only</a>.");
	enable_hyperlinks(license_brief);
	QLabel* license_brief_part2 = new QLabel("For full license notices including those of dependencies see the source code.");
	QLabel* copyright_brief = new QLabel("PicoTurtle  Copyright (C) 2018-2022  Abhishek Mishra");

	QDialogButtonBox* button_box = new QDialogButtonBox(QDialogButtonBox::Ok);

	QVBoxLayout *vb_layout = new QVBoxLayout(this);

	vb_layout->addWidget(program_name);
	vb_layout->addWidget(author_name);
	vb_layout->addWidget(license_brief);
	vb_layout->addWidget(license_brief_part2);
	vb_layout->addWidget(copyright_brief);
	vb_layout->addWidget(button_box);

	setLayout(vb_layout);

	connect(button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
};
