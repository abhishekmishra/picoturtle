#include "MainWindow.hpp"
#include <QPushButton>
#include <QMessageBox>
#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
{
    // Add a central widget
    QPushButton *button = new QPushButton("Hello", this);
    setCentralWidget(button);

    // connect a slot to the button
    connect(button, &QPushButton::clicked, [=]() {
        QMessageBox::information(this, "Some info", "Blah blah blah... button click kyon kiya?");
    });

    // Define the quit action
    quitAction = new QAction("Quit");

    connect(quitAction, &QAction::triggered, [=](){
       QApplication::quit();
    });

    // see https://stackoverflow.com/a/49921405
    // on macos setting this flag enables the menubar for the window.
    menuBar()->setNativeMenuBar(false);

    // Add menus
    QMenu *fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction(quitAction);
    menuBar()->addMenu("Edit");
    menuBar()->addMenu("Window");
    menuBar()->addMenu("Settings");
    menuBar()->addMenu("Help");

    // Set statusbar items
    statusBar()->showMessage("Uploading file ...", 3000);
}


QSize MainWindow::sizeHint() const
{
    return QSize(800, 600);
}
