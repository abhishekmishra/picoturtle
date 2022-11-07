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
		create_turtle_console_widget();

		// Set statusbar items
		statusBar()->showMessage("Uploading file ...", 3000);
	}

	void TurtleAppWindow::create_actions()
	{
		// Define the actions
		new_action = new QAction(tr("New"));
		open_action = new QAction(tr("Open"));
		save_action = new QAction(tr("Save"));
		save_as_action = new QAction(tr("Save As")); 
		quit_action = new QAction(tr("Quit"));		

		cut_action = new QAction(tr("Cut"));
		copy_action = new QAction(tr("Copy"));
		paste_action = new QAction(tr("Paste"));		
		undo_action = new QAction(tr("Undo"));		
		redo_action = new QAction(tr("Redo"));

		about_action = new QAction(tr("About"));

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
		QMenu* file_menu = menuBar()->addMenu(tr("File"));
		file_menu->addAction(new_action);
		file_menu->addAction(open_action);
		file_menu->addAction(save_action);
		file_menu->addAction(save_as_action);
		file_menu->addAction(quit_action);
		
		QMenu* edit_menu = menuBar()->addMenu(tr("Edit"));
		edit_menu->addAction(cut_action);
		edit_menu->addAction(copy_action);
		edit_menu->addAction(paste_action);
		edit_menu->addAction(undo_action);
		edit_menu->addAction(redo_action);

		QMenu* turtle_menu = menuBar()->addMenu(tr("Turtle"));
		QMenu* settings_menu = menuBar()->addMenu(tr("Settings"));
		
		QMenu* help_menu = menuBar()->addMenu(tr("Help"));
		help_menu->addAction(about_action);
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

	void TurtleAppWindow::create_turtle_console_widget()
	{
		turtle_console_dock = new QDockWidget(tr("Turtle Console"), this);
		turtle_console_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
		turtle_console_dock->setWidget(turtle_code_edit);

		addDockWidget(Qt::BottomDockWidgetArea, turtle_console_dock);
	}

	QSize TurtleAppWindow::sizeHint() const
	{
		return QSize(800, 600);
	}
}
