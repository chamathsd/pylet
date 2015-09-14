/*
* Copyright (c) 2015 Jake Schadaway. All rights reserved.
* Licensed under the MIT License. See LICENSE.md for details.
*/

#include "pylet_window.h"
#include <qapplication.h>
#include <qdesktopwidget.h>

PyletWindow::PyletWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QDesktopWidget *desktop = QApplication::desktop();
    screenRect = desktop->screen()->rect();

    initWindow();
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
