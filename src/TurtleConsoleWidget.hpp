#pragma once
#ifndef TURTLE_CONSOLE_WIDGET_H
#define TURTLE_CONSOLE_WIDGET_H

#include <QWidget>
#include <QPlainTextEdit>

class TurtleConsoleWidget : public QWidget
{
    Q_OBJECT

public:
    TurtleConsoleWidget(QWidget* parent = nullptr);
    ~TurtleConsoleWidget();

signals:
    void console_has_error();

public slots:
    void info(const QString& msg);
    void warning(const QString& msg);
    void error(const QString& msg);
    void fatal(const QString& msg);

private:
    QPlainTextEdit* turtle_console_edit;
};
#endif // TURTLE_CONSOLE_WIDGET_H
