#include "MainWindow.hpp"
#include <QPushButton>
#include <QMessageBox>
#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QDockWidget>
#include "TurtleCanvasWidget.hpp"

MainWindow::MainWindow(QWidget* parent)
	: text_edit(new QTextEdit), QMainWindow{ parent }
{
	setWindowTitle("PicoTurtle");

	createActions();
	createToolBar();
	createMenuBar();

	TurtleCanvasWidget* turtle_canvas = new TurtleCanvasWidget(this);
	// Add a central widget
	setCentralWidget(turtle_canvas);

	QDockWidget* dock = new QDockWidget(tr("TurtleEdit"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dock->setWidget(text_edit);

	addDockWidget(Qt::LeftDockWidgetArea, dock);

	// Set statusbar items
	statusBar()->showMessage("Uploading file ...", 3000);
}

void MainWindow::createActions()
{
	// Define the quit action
	quitAction = new QAction("Quit");

	connect(quitAction, &QAction::triggered, [=]() {
		QApplication::quit();
		});
}

void MainWindow::createToolBar()
{
	FileBar = this->addToolBar(tr("File"));
	FileBar->addAction(quitAction);
}

void MainWindow::createMenuBar()
{
	// see https://stackoverflow.com/a/49921405
	// on macos setting this flag enables the menubar for the window.
	menuBar()->setNativeMenuBar(false);

	// Add menus
	QMenu* fileMenu = menuBar()->addMenu(tr("File"));
	fileMenu->addAction(quitAction);
	menuBar()->addMenu("Edit");
	menuBar()->addMenu("Window");
	menuBar()->addMenu("Settings");
	menuBar()->addMenu("Help");
}


QSize MainWindow::sizeHint() const
{
	return QSize(800, 600);
}
