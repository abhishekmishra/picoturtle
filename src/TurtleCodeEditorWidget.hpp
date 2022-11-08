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
    void new_file_created(const QString& file_name);
    void file_opened(const QString& file_path);
    void turtle_run_complete(const int error_code);

public slots:
    void new_file();
    int open_file(const QString& file_path);
    void run_file();

private:
    QTextEdit* turtle_code_edit;
    QString filename;
    int noname_file_count;
};
#endif // TURTLE_CODE_EDITOR_WIDGET_H
