#include "TurtleAppWindow.hpp"
#include <QApplication>
#include <QMenu>
#include <QStatusBar>
#include <QFileDialog>
#include <QDir>
#include <QIcon>
#include <QPixmap>

namespace turtle
{
	TurtleAppWindow::TurtleAppWindow(QWidget *parent)
		: turtle_code_editor{new TurtleCodeEditorWidget(this)},
		  turtle_console{new TurtleConsoleWidget(this)},
		  QMainWindow{parent}
	{
		create_actions();
		create_toolbar();
		create_menubar();

		// Add widgets
		create_canvas_widget();
		create_turtle_code_edit_widget();
		create_turtle_console_widget();

		// Set statusbar items
		statusBar()->showMessage("Starting PicoTurtle ...");

		update_title();

		// Connect to file actions.

		// *** file created
		connect(turtle_code_editor, &TurtleCodeEditorWidget::new_file_created, [=]()
				{ show_status_message("New file created: " + turtle_code_editor->get_file_name()); });

		// *** file opened
		connect(turtle_code_editor, &TurtleCodeEditorWidget::file_opened, [=]()
				{ 
					show_status_message("File opened: " + turtle_code_editor->get_file_path()); 
					update_title(); });

		// *** file saved
		connect(turtle_code_editor, &TurtleCodeEditorWidget::file_saved, [=]()
				{ 
					show_status_message("File saved: " + turtle_code_editor->get_file_path()); 
					update_title(); });

		// *** file path changed
		connect(turtle_code_editor, &TurtleCodeEditorWidget::file_path_changed, [=]()
				{ 
					show_status_message("File saved: " + turtle_code_editor->get_file_path()); 
					update_title(); });

		// *** file modified changed
		connect(turtle_code_editor, &TurtleCodeEditorWidget::file_modified_changed, [=](bool flag)
				{ 
					show_status_message("File dirty flag: " + QString::number(turtle_code_editor->is_dirty())); 
					update_title(); });

		connect(turtle_code_editor, &TurtleCodeEditorWidget::turtle_run_complete, [=](const int error_code)
				{
				if (error_code == 0)
				{
					show_status_message("Turtle run complete!");
				}
				else
				{
					show_status_message("Err: Turtle run completed with errors!");
				} });

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
		new_action->setIcon(QIcon(":/images/outline_insert_drive_file_black_24dp.png"));
		new_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));

		open_action = new QAction(tr("Open"));
		open_action->setIcon(QIcon(":/images/outline_file_open_black_24dp.png"));
		open_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));

		save_action = new QAction(tr("Save"));
		save_action->setIcon(QIcon(":/images/outline_save_black_24dp.png"));
		save_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));

		save_as_action = new QAction(tr("Save As"));
		save_as_action->setIcon(QIcon(":/images/outline_save_as_black_24dp.png"));

		quit_action = new QAction(tr("Quit"));
		quit_action->setIcon(QIcon(":/images/outline_exit_to_app_black_24dp.png"));
		quit_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));

		cut_action = new QAction(tr("Cut"));
		cut_action->setIcon(QIcon(":/images/outline_content_cut_black_24dp.png"));
		cut_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_X));

		copy_action = new QAction(tr("Copy"));
		copy_action->setIcon(QIcon(":/images/outline_content_copy_black_24dp.png"));
		copy_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_C));

		paste_action = new QAction(tr("Paste"));
		paste_action->setIcon(QIcon(":/images/outline_content_paste_black_24dp.png"));
		paste_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_V));

		undo_action = new QAction(tr("Undo"));
		undo_action->setIcon(QIcon(":/images/outline_undo_black_24dp.png"));
		undo_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z));

		redo_action = new QAction(tr("Redo"));
		redo_action->setIcon(QIcon(":/images/outline_redo_black_24dp.png"));
		redo_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y));

		run_action = new QAction(tr("Run"));
		run_action->setIcon(QIcon(":/images/outline_slideshow_black_24dp.png"));
		run_action->setShortcut(QKeySequence(Qt::Key_F5));

		about_action = new QAction(tr("About"));

		// connect file actions
		// *** New File
		connect(new_action, &QAction::triggered, turtle_code_editor, &TurtleCodeEditorWidget::new_file);

		// TODO:: open file path (should be user home or application path?)
		// note - samples are in application_dir/lua

		// *** Open File
		QString start_path = qApp->applicationDirPath(); // QDir::homePath()
		connect(open_action, &QAction::triggered, [=]()
				{
				QString fileName = QFileDialog::getOpenFileName(this,
					tr("Open Turtle Lua File"), start_path, tr("Turtle/Lua Files (*.lua)"));
				qDebug() << fileName;
				//TODO: handle return value to show appropriate error.
				turtle_code_editor->open_file(fileName); });

		// *** Save File
		connect(save_action, &QAction::triggered, [=]()
				{ 
					if (!turtle_code_editor->has_file_path()) {
						QString fileName = QFileDialog::getSaveFileName(this,
																	tr("Save Turtle Lua File"), "",
																	tr("Turtle/Lua Files (*.lua)")); 
						// TODO: handler return value to show appropriate error.
						turtle_code_editor->set_file_path(fileName);
					}
					// TODO: handler return value to show appropriate error.
					turtle_code_editor->save_file(); });

		// *** Save-as File
		connect(save_as_action, &QAction::triggered, [=]()
				{
					QString fileName = QFileDialog::getSaveFileName(this,
																tr("Save As Turtle Lua File"), "",
																tr("Turtle/Lua Files (*.lua)")); 
					// TODO: handler return value to show appropriate error.
					turtle_code_editor->set_file_path(fileName, true);
					
					// TODO: handler return value to show appropriate error.
					turtle_code_editor->save_file(); });

		// connect turtle actions
		// *** Run Turtle Program
		connect(run_action, &QAction::triggered, turtle_code_editor,
				&TurtleCodeEditorWidget::run_file);

		// connect quit
		// *** Quit PicoTurtle
		connect(quit_action, &QAction::triggered, [=]()
				{ QApplication::quit(); });
	}

	void TurtleAppWindow::create_toolbar()
	{
		file_toolbar = this->addToolBar(tr("File"));
		file_toolbar->addAction(new_action);
		file_toolbar->addAction(open_action);
		file_toolbar->addAction(quit_action);

		turtle_toolbar = this->addToolBar(tr("Turtle"));
		turtle_toolbar->addAction(run_action);
	}

	void TurtleAppWindow::create_menubar()
	{
		// see https://stackoverflow.com/a/49921405
		// on macos setting this flag enables the menubar for the window.
		menuBar()->setNativeMenuBar(false);

		// Add menus
		QMenu *file_menu = menuBar()->addMenu(tr("File"));
		file_menu->addAction(new_action);
		file_menu->addAction(open_action);
		file_menu->addAction(save_action);
		file_menu->addAction(save_as_action);
		file_menu->addAction(quit_action);

		QMenu *edit_menu = menuBar()->addMenu(tr("Edit"));
		edit_menu->addAction(cut_action);
		edit_menu->addAction(copy_action);
		edit_menu->addAction(paste_action);
		edit_menu->addAction(undo_action);
		edit_menu->addAction(redo_action);

		QMenu *turtle_menu = menuBar()->addMenu(tr("Turtle"));
		turtle_menu->addAction(run_action);

		QMenu *settings_menu = menuBar()->addMenu(tr("Settings"));

		QMenu *help_menu = menuBar()->addMenu(tr("Help"));
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

	void TurtleAppWindow::show_status_message(const QString &message)
	{
		statusBar()->showMessage(message);
	}

	void TurtleAppWindow::update_title()
	{
		QString title = "PicoTurtle";
		if (turtle_code_editor)
		{
			title += " ";
			title += turtle_code_editor->get_file_name();
			title += " [*]";
			qDebug() << turtle_code_editor->get_file_name() << "dirty flag" << turtle_code_editor->is_dirty();
			if (turtle_code_editor->is_dirty())
			{
				setWindowModified(true);
			}
			qDebug() << "Title is" << title;
		}
		setWindowTitle(title);
		if (turtle_code_editor && turtle_code_editor->is_dirty())
		{
			setWindowModified(true);
		}
	}

	void TurtleAppWindow::write_to_console(const QString &input) const
	{
		turtle_console->info(input);
	}

	QSize TurtleAppWindow::sizeHint() const
	{
		return QSize(800, 600);
	}

	void TurtleAppWindow::set_turtle(turtle::PicoTurtle *t)
	{
		turtle_canvas->set_turtle(t);
	}

}
