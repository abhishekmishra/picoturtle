#include <QApplication>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QFileInfo>

#include "TurtleAppWindow.hpp"

/**
 * Creates the Qt main window and runs the main loop for Qt.
 *
 * @param argc program args
 * @param argv program args
 * @return return value from qt mainloop
 */
int gui_window(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	return gui_window(argc, argv);
}

int gui_window(int argc, char *argv[])
{
	QApplication a(argc, argv);

	turtle::TurtleAppWindow *mainWindow = new turtle::TurtleAppWindow();
	mainWindow->show();

	return a.exec();
}
