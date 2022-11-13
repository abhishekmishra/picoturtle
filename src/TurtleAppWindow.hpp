#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QDockWidget>
#include "TurtleCanvasWidget.hpp"
#include "TurtleCodeEditorWidget.hpp"
#include "TurtleConsoleWidget.hpp"
#include "TurtleController.hpp"

namespace turtle {

	/**
	* This class is the main window of the Turtle Application.
	* It has menubar, toolbars, statusbar, a central canvas widget,
	* and editor and console widgets docked
	*/
	class TurtleAppWindow : public QMainWindow
	{
		Q_OBJECT
	public:
		explicit TurtleAppWindow(QWidget* parent = nullptr);
		~TurtleAppWindow();
	signals:

	private:
		QSize sizeHint() const;

		// helper functions
		void create_actions();
		void create_toolbar();
		void create_menubar();
		void create_canvas_widget();
		void create_turtle_code_edit_widget();
		void create_turtle_console_widget();
		void update_title();

		// Turtle Controller Instance
		TurtleController* turtle_controller;

		// Widgets
		TurtleCanvasWidget* turtle_canvas;
		TurtleCodeEditorWidget* turtle_code_editor;
		TurtleConsoleWidget* turtle_console;
		QDockWidget* turtle_code_edit_dock;
		QDockWidget* turtle_console_dock;

		// Menu actions and toolbar
		QAction* quit_action;
		QAction* new_action;
		QAction* open_action;
		QAction* save_action;
		QAction* save_as_action;

		QAction* cut_action;
		QAction* copy_action;
		QAction* paste_action;
		QAction* undo_action;
		QAction* redo_action;

		QAction* run_action;

		QAction* about_action;

		QToolBar* file_toolbar;
		QToolBar* edit_toolbar;
		QToolBar* turtle_toolbar;
		QToolBar* extras_toolbar;

	public slots:
		void show_status_message(const QString& message);
		void write_to_console(const QString& input) const;
	    void set_turtle(turtle::PicoTurtle* t);
	};
}

#endif // MAINWINDOW_H
