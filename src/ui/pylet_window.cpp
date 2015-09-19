/*
* Copyright (c) 2015 Jake Schadaway. All rights reserved.
* Licensed under the MIT License. See LICENSE.md for details.
*/

#include "pylet_window.h"
#include <qapplication.h>
#include <qdesktopwidget.h>
#include <qmenubar.h>
#include <qlayout.h>

PyletWindow::PyletWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QDesktopWidget *desktop = QApplication::desktop();
    screenRect = desktop->screen()->rect();

    initWindow();
    initWidgets();
    show();
}

void PyletWindow::initWindow()
{
    setMinimumSize(400, 400);
    int screenWidth = screenRect.width();
    int screenHeight = screenRect.height();
    resize(screenWidth - 250, screenHeight - 250);
    showMaximized();

    setWindowTitle("Pylet (Editor)"); 
}

void PyletWindow::initWidgets()
{
    populateMenu();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    mainLayout->addStretch();

    statusBar();
}

void PyletWindow::populateMenu()
{
    menuBar()->addMenu("File");
    menuBar()->addMenu("Edit");
    menuBar()->addMenu("Search");
    menuBar()->addMenu("Run");
    menuBar()->addMenu("View");
    menuBar()->addMenu("Settings");
    menuBar()->addMenu("Help");
}
