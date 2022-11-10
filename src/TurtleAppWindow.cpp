#include "TurtleAppWindow.hpp"
#include <QApplication>
#include <QMenu>
#include <QStatusBar>
#include <QFileDialog>
#include <QDir>

namespace turtle
{
	TurtleAppWindow::TurtleAppWindow(QWidget* parent)
		: turtle_code_editor{ new TurtleCodeEditorWidget(this) },
		turtle_console{ new TurtleConsoleWidget(this) },
		QMainWindow{ parent }
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
		statusBar()->showMessage("Starting PicoTurtle ...", 3000);

		connect(turtle_code_editor, &TurtleCodeEditorWidget::new_file_created, [=](const QString& file_name)
			{ show_status_message("New file created: " + file_name); });

		connect(turtle_code_editor, &TurtleCodeEditorWidget::file_opened, [=](const QString& file_name)
			{ show_status_message("File opened: " + file_name); });

		connect(turtle_code_editor, &TurtleCodeEditorWidget::turtle_run_complete, [=](const int error_code)
			{
				if (error_code == 0)
				{
					show_status_message("Turtle run complete!");
				}
				else
				{
					show_status_message("Err: Turtle run completed with errors!");
				}
			});
		connect(turtle_code_editor, &TurtleCodeEditorWidget::turtle_run_complete, turtle_canvas, &TurtleCanvasWidget::draw_turtle);
	}

	TurtleAppWindow::~TurtleAppWindow()
	{
		// cleanup the turtle lua binding
		TurtleController::cleanup_turtle_lua_binding();
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

		run_action = new QAction(tr("Run"));

		about_action = new QAction(tr("About"));

		// connect file actions
		connect(new_action, &QAction::triggered, turtle_code_editor, &TurtleCodeEditorWidget::new_file);
		connect(open_action, &QAction::triggered, [=]()
			{ QString fileName = QFileDialog::getOpenFileName(this,
				tr("Open Turtle Lua File"), QDir::homePath(), tr("Turtle/Lua Files (*.lua)"));
		qDebug() << fileName;
		//TODO: handle return value to show appropriate error.
		turtle_code_editor->open_file(fileName); });

		// connect turtle actions
		connect(run_action, &QAction::triggered, turtle_code_editor,
			&TurtleCodeEditorWidget::run_file);

		// connect quit
		connect(quit_action, &QAction::triggered, [=]()
			{ QApplication::quit(); });
	}

	void TurtleAppWindow::create_toolbar()
	{
		file_toolbar = this->addToolBar(tr("File"));
		file_toolbar->addAction(new_action);
		file_toolbar->addAction(open_action);
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
		turtle_menu->addAction(run_action);

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
		turtle_code_edit_dock->setWidget(turtle_code_editor);

		addDockWidget(Qt::LeftDockWidgetArea, turtle_code_edit_dock);
	}

	void TurtleAppWindow::create_turtle_console_widget()
	{
		turtle_console_dock = new QDockWidget(tr("Turtle Console"), this);
		turtle_console_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
		turtle_console_dock->setWidget(turtle_console);

		addDockWidget(Qt::BottomDockWidgetArea, turtle_console_dock);
	}

	void TurtleAppWindow::show_status_message(const QString& message)
	{
		statusBar()->showMessage(message);
	}

	void TurtleAppWindow::write_to_console(const QString& input) const
	{
		turtle_console->info(input);
	}

	QSize TurtleAppWindow::sizeHint() const
	{
		return QSize(800, 600);
	}

    void TurtleAppWindow::set_turtle(turtle::PicoTurtle* t)
	{
		turtle_canvas->set_turtle(t);
	}

}
