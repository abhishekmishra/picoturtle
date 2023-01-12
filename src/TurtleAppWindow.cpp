#include "TurtleAppWindow.hpp"
#include <QApplication>
#include <QMenu>
#include <QStatusBar>
#include <QFileDialog>
#include <QDir>
#include <QIcon>
#include <QPixmap>
#include <QPalette>
#include <QLayout>
#include <QLabel>

using namespace turtle;
TurtleAppWindow::TurtleAppWindow(QWidget *parent)
	: lua_repl{new TurtleLuaReplWidget(this)},
	  turtle_about{new TurtleAboutDialog(this)},
	  QMainWindow{parent}
{
	// Add widgets
	create_canvas_widget();
	create_turtle_code_edit_widget();
	create_turtle_docs_widget();

	create_actions();
	create_toolbar();
	create_menubar();

	tabifyDockWidget(turtle_docs_dock, turtle_canvas_dock);

	create_lua_repl_widget();

	// toggle the docs view once to hide it
	turtle_docs_dock->hide();

	// Set statusbar items
	create_statusbar_items();

	// update the title
	update_title();

	// connect signal to editor cursor changed.
	connect(code_editor_parent, &TurtleCodeEditorParentWidget::cursor_position_changed, [=](int ln, int col)
			{ set_editor_status_text(ln, col, false); });

	code_editor_parent->get_current_editor_widget()->get_editor()->setFocus();
	code_editor_parent->get_current_editor_widget()->get_editor()->move_cursor_to_end();

	// Connect to file actions.

	// *** file created
	connect(code_editor_parent, &TurtleCodeEditorParentWidget::new_file_created, [=]()
			{ show_status_message("New file created: " + code_editor_parent->get_file_name()); });

	// *** file opened
	connect(code_editor_parent, &TurtleCodeEditorParentWidget::file_opened, [=]()
			{
				show_status_message("File opened: " + code_editor_parent->get_file_path());
		update_title(); });

	// *** file saved
	connect(code_editor_parent, &TurtleCodeEditorParentWidget::file_saved, [=]()
			{
				show_status_message("File saved: " + code_editor_parent->get_file_path());
		update_title(); });

	// *** file path changed
	connect(code_editor_parent, &TurtleCodeEditorParentWidget::file_path_changed, [=]()
			{
				show_status_message("File saved: " + code_editor_parent->get_file_path());
		update_title(); });

	// *** file modified changed
	connect(code_editor_parent, &TurtleCodeEditorParentWidget::file_modified_changed, [=](bool flag)
			{
				show_status_message("File dirty flag: " + QString::number(code_editor_parent->is_dirty()));
		update_title();
		if (code_editor_parent->is_dirty())
		{
			save_action->setDisabled(false);
			save_as_action->setDisabled(false);
		}
		else
		{
			save_action->setDisabled(true);
			save_as_action->setDisabled(true);
		} });

	connect(code_editor_parent, &TurtleCodeEditorParentWidget::turtle_run_complete, [=](const int error_code)
			{
				if (error_code == 0)
				{
					show_status_message("Turtle run complete!");
				}
				else
				{
					show_status_message("Err: Turtle run completed with errors!");
				} });

	connect(code_editor_parent, &TurtleCodeEditorParentWidget::turtle_run_complete, turtle_canvas, &TurtleCanvasWidget::draw_turtle);

	connect(code_editor_parent, &TurtleCodeEditorParentWidget::current_tab_changed, [=]()
			{
				update_title();
		if (code_editor_parent->is_dirty())
		{
			save_action->setDisabled(false);
			save_as_action->setDisabled(false);
		}
		else
		{
			save_action->setDisabled(true);
			save_as_action->setDisabled(true);
		} });

	// qDebug() << "We are in dark theme?" << QString::number(in_dark_theme());
}

TurtleAppWindow::~TurtleAppWindow()
{
}

void TurtleAppWindow::create_actions()
{
	// Define the actions
	new_action = new QAction(tr("New"));
	new_action->setIcon(get_icon("insert_drive_file"));
	new_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));

	open_action = new QAction(tr("Open"));
	open_action->setIcon(get_icon("file_open"));
	open_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));

	save_action = new QAction(tr("Save"));
	save_action->setIcon(get_icon("save"));
	save_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
	// save_action->setDisabled(true);

	save_as_action = new QAction(tr("Save As"));
	save_as_action->setIcon(get_icon("save_as"));
	// save_as_action->setDisabled(true);

	quit_action = new QAction(tr("Quit"));
	quit_action->setIcon(get_icon("exit_to_app"));
	quit_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));

	cut_action = new QAction(tr("Cut"));
	cut_action->setIcon(get_icon("content_cut"));
	cut_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_X));

	copy_action = new QAction(tr("Copy"));
	copy_action->setIcon(get_icon("content_copy"));
	copy_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_C));

	paste_action = new QAction(tr("Paste"));
	paste_action->setIcon(get_icon("content_paste"));
	paste_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_V));

	undo_action = new QAction(tr("Undo"));
	undo_action->setIcon(get_icon("undo"));
	undo_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z));

	redo_action = new QAction(tr("Redo"));
	redo_action->setIcon(get_icon("redo"));
	redo_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y));

	find_action = new QAction(tr("Find"));
	find_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));

	find_replace_action = new QAction(tr("Find/Replace"));

#ifdef Q_OS_MAC
	find_replace_action->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_F));
#else
	find_replace_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_H));
#endif

	indent_action = new QAction(tr("Indent Code"));
	indent_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_BracketRight));

	deindent_action = new QAction(tr("De-Indent Code"));
	deindent_action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_BracketLeft));

	run_action = new QAction(tr("Run"));
	run_action->setIcon(get_icon("slideshow"));
	run_action->setShortcut(QKeySequence(Qt::Key_F5));

	clear_console_action = new QAction(tr("Clear Console"));
	clear_console_action->setIcon(get_icon("clear_all"));

	turtle_docs_action = new QAction(tr("Turtle API Docs"));
	turtle_docs_action->setIcon(get_icon("help_outline"));
	turtle_docs_action->setShortcut(QKeySequence(Qt::Key_F1));

	about_action = new QAction(tr("About"));

	// connect file actions
	// *** New File
	connect(new_action, &QAction::triggered, code_editor_parent, &TurtleCodeEditorParentWidget::new_file);

	// TODO:: open file path (should be user home or application path?)
	// note - samples are in application_dir/lua

	// *** Open File
	QString start_path = qApp->applicationDirPath(); // QDir::homePath()
	connect(open_action, &QAction::triggered, [=]()
			{
				QString fileName = QFileDialog::getOpenFileName(this,
				tr("Open Turtle Lua File"), start_path, tr("Turtle/Lua Files (*.lua)"));
		qDebug() << fileName;
		if (!fileName.isEmpty())
		{
			//TODO: handle return value to show appropriate error.
			code_editor_parent->open_file(fileName);
		} });

	// *** Save File
	connect(save_action, &QAction::triggered, [=]() { // TODO: handler return value to show appropriate error.
		code_editor_parent->save_file();
	});

	// *** Save-as File
	connect(save_as_action, &QAction::triggered, [=]()
			{
				QString fileName = QFileDialog::getSaveFileName(this,
				tr("Save As Turtle Lua File"), "",
				tr("Turtle/Lua Files (*.lua)"));
		// TODO: handler return value to show appropriate error.
		code_editor_parent->set_file_path(fileName, true);

		// TODO: handler return value to show appropriate error.
		code_editor_parent->save_file(); });

	// connect turtle actions
	// *** Run Turtle Program
	connect(run_action, &QAction::triggered, code_editor_parent,
			&TurtleCodeEditorParentWidget::run_file);
	connect(run_action, &QAction::triggered, [=]()
			{ turtle_canvas_dock->raise(); });
	connect(clear_console_action, &QAction::triggered, lua_repl,
			&TurtleLuaReplWidget::clear_console);

	// connect help menu items
	// *** Open Turtle API Docs
	connect(turtle_docs_action, &QAction::triggered, [=]()
			{ turtle_docs_dock->show(); turtle_docs_dock->raise(); });

	// connect quit
	// *** Quit PicoTurtle
	connect(quit_action, &QAction::triggered, [=]()
			{ QApplication::quit(); });

	connect(cut_action, &QAction::triggered, [=]()
			{ code_editor_parent->get_current_editor_widget()->get_editor()->cut(); });
	connect(copy_action, &QAction::triggered, [=]()
			{ code_editor_parent->get_current_editor_widget()->get_editor()->copy(); });
	connect(paste_action, &QAction::triggered, [=]()
			{ code_editor_parent->get_current_editor_widget()->get_editor()->paste(); });
	connect(undo_action, &QAction::triggered, [=]()
			{ code_editor_parent->get_current_editor_widget()->get_editor()->undo(); });
	connect(redo_action, &QAction::triggered, [=]()
			{ code_editor_parent->get_current_editor_widget()->get_editor()->redo(); });
	connect(find_action, &QAction::triggered, [=]()
			{ code_editor_parent->get_current_editor_widget()->toggle_find(); });
	connect(find_replace_action, &QAction::triggered, [=]()
			{ code_editor_parent->get_current_editor_widget()->toggle_find_replace(); });

	connect(indent_action, &QAction::triggered, [=]()
			{ code_editor_parent->get_current_editor_widget()->indent_line_or_selection(); });

	connect(deindent_action, &QAction::triggered, [=]()
		{ code_editor_parent->get_current_editor_widget()->deindent_line_or_selection(); });

	// connect about
	connect(about_action, &QAction::triggered, turtle_about, &TurtleAboutDialog::exec);
}

void TurtleAppWindow::create_toolbar()
{
	file_toolbar = this->addToolBar(tr("File"));
	file_toolbar->addAction(new_action);
	file_toolbar->addAction(open_action);
	file_toolbar->addAction(save_action);

	// TODO: decide if fixed size of toolbars needed.
	//  file_toolbar->setIconSize(QSize(20, 20));
	//  file_toolbar->setFixedHeight(36);

	edit_toolbar = this->addToolBar(tr("Edit"));
	edit_toolbar->addAction(cut_action);
	edit_toolbar->addAction(copy_action);
	edit_toolbar->addAction(paste_action);
	edit_toolbar->addAction(undo_action);
	edit_toolbar->addAction(redo_action);

	turtle_toolbar = this->addToolBar(tr("Turtle"));
	turtle_toolbar->addAction(run_action);
	turtle_toolbar->addAction(clear_console_action);

	extras_toolbar = this->addToolBar(tr("Extras"));
	extras_toolbar->addAction(turtle_docs_action);
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
	edit_menu->addAction(find_action);
	edit_menu->addAction(find_replace_action);
	edit_menu->addAction(indent_action);
	edit_menu->addAction(deindent_action);

	QMenu *turtle_menu = menuBar()->addMenu(tr("Turtle"));
	turtle_menu->addAction(run_action);
	turtle_menu->addAction(clear_console_action);

	QMenu *settings_menu = menuBar()->addMenu(tr("Settings"));

	QMenu *help_menu = menuBar()->addMenu(tr("Help"));
	help_menu->addAction(turtle_docs_action);
	help_menu->addAction(about_action);
}

void TurtleAppWindow::create_canvas_widget()
{
	turtle_canvas = new TurtleCanvasWidget(this);

	turtle_canvas_dock = new QDockWidget(tr("Turtle Canvas"), this);
	turtle_canvas_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	turtle_canvas_dock->setWidget(turtle_canvas);
	// turtle_canvas_dock->widget()->layout()->setContentsMargins(0, 0, 0, 0);

	// setCentralWidget(turtle_canvas);

	addDockWidget(Qt::RightDockWidgetArea, turtle_canvas_dock);
}

void TurtleAppWindow::create_turtle_code_edit_widget()
{
	code_editor_parent = new TurtleCodeEditorParentWidget(lua_repl, this);

	turtle_code_edit_dock = new QDockWidget(tr("Turtle Editor"), this);
	turtle_code_edit_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	turtle_code_edit_dock->setWidget(code_editor_parent);
	turtle_code_edit_dock->widget()->layout()->setContentsMargins(0, 0, 0, 0);

	addDockWidget(Qt::LeftDockWidgetArea, turtle_code_edit_dock);
}

void TurtleAppWindow::create_lua_repl_widget()
{
	std::function<void(turtle::PicoTurtle * t)> notify_turtle_fn = [=](turtle::PicoTurtle *t)
	{
		this->set_turtle(t);
	};

	std::function<void(turtle::PicoTurtle * t)> handle_turtle_update_fn = [=](turtle::PicoTurtle *t)
	{
		this->handle_turtle_update(t);
	};

	std::function<void(turtle::PicoTurtle * t)> handle_turtle_paint_fn = [=](turtle::PicoTurtle *t)
	{
		this->handle_turtle_paint(t);
	};

	std::function<void(turtle::PicoTurtle * t, int tm)> turtle_delay_fn = [=](turtle::PicoTurtle *t, int tm)
	{
		turtle::TurtleAppWindow::delay(tm);
	};

	turtle::TurtleLuaReplWidget::set_notify_turtle_created_fn(notify_turtle_fn);
	turtle::TurtleLuaReplWidget::set_notify_turtle_update_fn(handle_turtle_update_fn);
	turtle::TurtleLuaReplWidget::set_notify_turtle_paint_fn(handle_turtle_paint_fn);
	turtle::TurtleLuaReplWidget::set_turtle_delay_fn(turtle_delay_fn);

	lua_repl_dock = new QDockWidget(tr("Lua"), this);
	lua_repl_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
	lua_repl_dock->setWidget(lua_repl);
	lua_repl_dock->widget()->layout()->setContentsMargins(0, 0, 0, 0);

	addDockWidget(Qt::RightDockWidgetArea, lua_repl_dock);
}

void TurtleAppWindow::create_turtle_docs_widget()
{
	turtle_docs = new TurtleDocsWidget(this);
	turtle_docs_dock = new QDockWidget(tr("Turtle Docs"), this);
	turtle_docs_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
	turtle_docs_dock->setWidget(turtle_docs);
	turtle_docs_dock->widget()->layout()->setContentsMargins(0, 0, 0, 0);

	addDockWidget(Qt::RightDockWidgetArea, turtle_docs_dock);
}

void TurtleAppWindow::show_status_message(const QString &message)
{
	statusBar()->showMessage(message);
}

void TurtleAppWindow::update_title()
{
	QString title = "PicoTurtle";
	TurtleCodeEditorWidget *code_edit = code_editor_parent->get_current_editor_widget();

	if (code_edit)
	{
		title += " ";
		if (code_edit->get_file_path().isNull())
		{
			title += code_edit->get_file_name();
		}
		else
		{
			title += code_edit->get_file_path();
		}

		title += " [*]";
	}
	setWindowTitle(title);
	if (code_edit && code_edit->is_dirty())
	{
		setWindowModified(true);
	}
	else
	{
		setWindowModified(false);
	}
	// qDebug() << code_edit->get_file_name() << "dirty flag" << code_edit->is_dirty();
	// qDebug() << "Title is" << title;
}

void TurtleAppWindow::write_to_console(const QString &input) const
{
	lua_repl->print_to_repl(input.toStdString());
}

QSize TurtleAppWindow::sizeHint() const
{
	return QSize(800, 600);
}

void TurtleAppWindow::set_turtle(turtle::PicoTurtle *t)
{
	turtle_canvas->set_turtle(t);
}

// see https://stackoverflow.com/a/11487434/9483968
void TurtleAppWindow::delay(int tm)
{
	// qDebug() << "Delay called for" << tm << "ms";
	QTime dieTime = QTime::currentTime().addMSecs(tm);
	while (QTime::currentTime() < dieTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, tm);
}

void TurtleAppWindow::handle_turtle_update(turtle::PicoTurtle *t)
{
	turtle_canvas->draw_turtle();
	// delay();
	// qDebug() << "update called.";
}

void TurtleAppWindow::handle_turtle_paint(turtle::PicoTurtle *t)
{
	turtle_canvas->draw_turtle();
}

bool TurtleAppWindow::in_dark_theme()
{
	int text_hsv_val = this->palette().color(QPalette::WindowText).value();
	int bg_hsv_val = this->palette().color(QPalette::Window).value();
	return text_hsv_val > bg_hsv_val;
}

QIcon TurtleAppWindow::get_icon(QString name)
{
	QString icon_colour = in_dark_theme() ? "white" : "black";
	return QIcon(":/images/outline_" + name + "_" + icon_colour + "_24dp.png");
}

void TurtleAppWindow::create_statusbar_items()
{
	turtle_status = new QLabel("");
	statusBar()->addPermanentWidget(turtle_status, 0);
	editor_status = new QLabel("");
	statusBar()->addPermanentWidget(editor_status, 0);
}

void TurtleAppWindow::set_editor_status_text(int ln, int col, bool changed)
{
	// TODO: use fixed width formatting if possible.
	editor_status->setText(QString("Line:") + QString::number(ln) + QString(", Col:") + QString::number(col));
}
