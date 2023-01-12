#pragma once
#ifndef TURTLE_CODE_EDITOR_WIDGET_H
#define TURTLE_CODE_EDITOR_WIDGET_H

#include <QWidget>
#include <QPlainTextEdit>
#include "LuaSyntaxHighlighter.hpp"
#include "TurtleCodeEditorTextWidget.hpp"
#include "TurtleLuaReplWidget.hpp"
#include "TurtleFindReplaceTextWidget.hpp"

namespace turtle
{

    class TurtleCodeEditorWidget : public QWidget
    {
        Q_OBJECT

    public:
        TurtleCodeEditorWidget(TurtleLuaReplWidget* repl, QWidget *parent = nullptr);
        ~TurtleCodeEditorWidget();

    signals:
        void new_file_created();
        void file_opened();
        void file_saved();
        void turtle_run_complete(const int error_code);
        void file_path_changed();
        void file_modified_changed(bool modified);

    public slots:
        void new_file();
        int open_file(const QString &file_path);
        int save_file();
        bool set_file_path(const QString &file_path, bool override_current_path = false);
        void run_file();
        void toggle_find();
        void toggle_find_replace();
        void indent_line_or_selection();

    public:
        bool has_file_path();
        bool is_dirty();
        const QString get_file_name();
        const QString &get_file_path();
        TurtleCodeEditorTextWidget* get_editor();

    private:
        TurtleCodeEditorTextWidget *turtle_code_edit;
        QString file_path;
        static int noname_file_count;
        QString noname_fname;
        QString basic_turtle_text;
        LuaSyntaxHighlighter *lua_highlighter;
        TurtleLuaReplWidget* lua_repl;
        TurtleFindReplaceTextWidget* find_replace;
    };
}
#endif // TURTLE_CODE_EDITOR_WIDGET_H
