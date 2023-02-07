#include <QApplication>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QFileInfo>
#include <QSettings>

#include <PicoTurtleVersion.hpp>
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

	// set the settings configuration before initializing
	// any settings such that it is not repeated all over
	// the application.
	QCoreApplication::setOrganizationName("neolateral");
	QCoreApplication::setOrganizationDomain("neolateral.in");
	QCoreApplication::setApplicationName("PicoTurtle");
	QCoreApplication::setApplicationVersion(PICOTURTLE_VERSION_STR);
	QSettings::setDefaultFormat(QSettings::IniFormat);

	turtle::TurtleAppWindow *mainWindow = new turtle::TurtleAppWindow();
	mainWindow->show();

	return a.exec();
}
