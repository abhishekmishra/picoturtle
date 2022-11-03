#include "MainWindow.hpp"
#include <QPushButton>
#include <QMessageBox>
#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QDockWidget>
#include "TurtleCanvasWidget.hpp"

MainWindow::MainWindow(QWidget* parent)
	: text_edit(new QTextEdit), QMainWindow{ parent }
{
	setWindowTitle("PicoTurtle");

	// Add a central widget
	setCentralWidget(text_edit);

	TurtleCanvasWidget* turtle_canvas = new TurtleCanvasWidget(this);

	QDockWidget* dock = new QDockWidget(tr("TurtleCanvas"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dock->setWidget(turtle_canvas);

	addDockWidget(Qt::RightDockWidgetArea, dock);

	// Define the quit action
	quitAction = new QAction("Quit");

	connect(quitAction, &QAction::triggered, [=]() {
		QApplication::quit();
		});

	// see https://stackoverflow.com/a/49921405
	// on macos setting this flag enables the menubar for the window.
	menuBar()->setNativeMenuBar(false);

	// Add menus
	QMenu* fileMenu = menuBar()->addMenu("File");
	fileMenu->addAction(quitAction);
	menuBar()->addMenu("Edit");
	menuBar()->addMenu("Window");
	menuBar()->addMenu("Settings");
	menuBar()->addMenu("Help");

	// Set statusbar items
	statusBar()->showMessage("Uploading file ...", 3000);
}


QSize MainWindow::sizeHint() const
{
	return QSize(800, 600);
}
