#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QDockWidget>
#include "TurtleCanvasWidget.hpp"
#include "TurtleCodeEditorWidget.hpp"

namespace turtle {
	class TurtleAppWindow : public QMainWindow
	{
		Q_OBJECT
	public:
		explicit TurtleAppWindow(QWidget* parent = nullptr);

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

		// Widgets
		TurtleCanvasWidget* turtle_canvas;
		TurtleCodeEditorWidget* turtle_code_editor;
		QDockWidget* turtle_code_edit_dock;
		QDockWidget* turtle_console_dock;
		QTextEdit* turtle_console;

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

	public slots:
		void show_status_message(const QString& message);
	};
}

#endif // MAINWINDOW_H
