/*
* Copyright (c) 2015 Jake Schadaway.
* Licensed under the MIT License. See LICENSE.md for details.
*/

#ifndef PYLET_WINDOW_H
#define PYLET_WINDOW_H

#include <qmainwindow.h>
#include <qevent.h>

class PyletWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PyletWindow(QWidget *parent = 0);

private:
    void initWindow();
    void initWidgets();
    void populateMenu();
    QRect screenRect;
};

#endif // PYLET_WINDOW_H