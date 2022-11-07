#include "TurtleAppWindow.hpp"
#include <QApplication>
#include <QMenu>
#include <QStatusBar>

namespace turtle {
	TurtleAppWindow::TurtleAppWindow(QWidget* parent)
		: turtle_code_edit(new QTextEdit), QMainWindow{ parent }
	{
		setWindowTitle("PicoTurtle");

		create_actions();
		create_toolbar();
		create_menubar();

		// Add widgets
		create_canvas_widget();
		create_turtle_code_edit_widget();

		// Set statusbar items
		statusBar()->showMessage("Uploading file ...", 3000);
	}

	void TurtleAppWindow::create_actions()
	{
		// Define the quit action
		quit_action = new QAction("Quit");

		connect(quit_action, &QAction::triggered, [=]() {
			QApplication::quit();
			});
	}

	void TurtleAppWindow::create_toolbar()
	{
		file_toolbar = this->addToolBar(tr("File"));
		file_toolbar->addAction(quit_action);
	}

	void TurtleAppWindow::create_menubar()
	{
		// see https://stackoverflow.com/a/49921405
		// on macos setting this flag enables the menubar for the window.
		menuBar()->setNativeMenuBar(false);

		// Add menus
		QMenu* fileMenu = menuBar()->addMenu(tr("File"));
		fileMenu->addAction(quit_action);
		menuBar()->addMenu("Edit");
		menuBar()->addMenu("Window");
		menuBar()->addMenu("Settings");
		menuBar()->addMenu("Help");
	}

	void TurtleAppWindow::create_canvas_widget()
	{
		turtle_canvas = new TurtleCanvasWidget(this);
		setCentralWidget(turtle_canvas);
	}

	void TurtleAppWindow::create_turtle_code_edit_widget()
	{
		turtle_code_edit_dock = new QDockWidget(tr("TurtleEdit"), this);
		turtle_code_edit_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		turtle_code_edit_dock->setWidget(turtle_code_edit);

		addDockWidget(Qt::LeftDockWidgetArea, turtle_code_edit_dock);
	}


	QSize TurtleAppWindow::sizeHint() const
	{
		return QSize(800, 600);
	}
}
