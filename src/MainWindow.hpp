#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QTextEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:


    // QWidget interface
private:
    QSize sizeHint() const;
    void createActions();
    void createToolBar();
    void createMenuBar();

    QTextEdit *text_edit;
    QAction *quitAction;
    QToolBar* FileBar;
};

#endif // MAINWINDOW_H
