#pragma once
#ifndef TURTLE_CODE_EDITOR_PARENT_WIDGET_H
#define TURTLE_CODE_EDITOR_PARENT_WIDGET_H

#include "TurtleCodeEditorWidget.hpp"
#include <QWidget>
#include <QList>
#include <QTabWidget>

namespace turtle
{
    class TurtleCodeEditorParentWidget : public QWidget
    {
        Q_OBJECT

    public:
        TurtleCodeEditorParentWidget(QWidget *parent = nullptr);
        ~TurtleCodeEditorParentWidget();

    signals:
        void new_file_created();
        void file_opened();
        void file_saved();
        void turtle_run_complete(const int error_code);
        void file_path_changed();
        void file_modified_changed(bool modified);
        void current_tab_changed(int idx);

    public slots:
        void new_file();
        int open_file(const QString &file_path);
        int save_file();
        bool set_file_path(const QString &file_path, bool override_current_path = false);
        void run_file();

    public:
        bool has_file_path();
        bool is_dirty();
        const QString get_file_name();
        const QString &get_file_path();
        TurtleCodeEditorWidget* get_current_editor_widget();

    private:
        QTabWidget *tabs;
        QList<TurtleCodeEditorWidget *> editor_widgets;
        TurtleCodeEditorWidget* new_code_editor();
    };
}

#endif // TURTLE_CODE_EDITOR_PARENT_WIDGET_H