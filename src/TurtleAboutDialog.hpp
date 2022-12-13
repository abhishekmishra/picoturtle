#pragma once

#ifndef TURTLEABOUTWIDGET_H
#define TURTLEABOUTWIDGET_H

#include <QDialog>

namespace turtle
{

	class TurtleAboutDialog: public QDialog
	{
		Q_OBJECT

	public:
		TurtleAboutDialog(QWidget* parent = nullptr);

	};

};


#endif // TURTLEABOUTWIDGET_H
