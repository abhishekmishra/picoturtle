#pragma once
#ifndef TURTLE_CODE_EDITOR_WIDGET_H
#define TURTLE_CODE_EDITOR_WIDGET_H

#include <QWidget>
#include <QTextEdit>

class TurtleCodeEditorWidget : public QWidget
{
    Q_OBJECT

public:
    TurtleCodeEditorWidget(QWidget* parent = nullptr);
    ~TurtleCodeEditorWidget();

signals:
    void new_file_created();
    void file_opened();
    void file_saved();
    void turtle_run_complete(const int error_code);
    void file_path_changed();
    void file_changed();

public slots:
    void new_file();
    int open_file(const QString& file_path);
    int save_file();
    bool set_file_path(const QString& file_path, bool override_current_path = false);
    void run_file();

public:
    bool has_file_path();
    void isDirty();
    const QString& get_file_name();
    const QString& get_file_path();

private:
    void set_dirty();

private:
    QTextEdit* turtle_code_edit;
    QString file_path;
    int noname_file_count;
    bool dirty;
};
#endif // TURTLE_CODE_EDITOR_WIDGET_H
