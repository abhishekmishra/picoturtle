#pragma once
#ifndef TURTLE_DOCS_WIDGET_H
#define TURTLE_DOCS_WIDGET_H

#include <QWidget>
#include <QTextEdit>

class TurtleDocsWidget : public QWidget
{
    Q_OBJECT

public:
    TurtleDocsWidget(QWidget *parent = nullptr);

private:
    QTextEdit* docs_text_display;
};

#endif
