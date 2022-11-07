#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QDockWidget>
#include "TurtleCanvasWidget.hpp"

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

		// Widgets
		TurtleCanvasWidget* turtle_canvas;
		QDockWidget* turtle_code_edit_dock;
		QTextEdit* turtle_code_edit;

		// Menu actions and toolbar
		QAction* quit_action;
		QAction* new_action;
		QAction* open_action;
		QAction* save_action;
		QAction* save_as_action;
		QToolBar* file_toolbar;
	};
}

#endif // MAINWINDOW_H
