/*
* Copyright (c) 2015 Jake Schadaway.
* Licensed under the MIT License. See LICENSE.md for details.
*/

#ifndef PYLET_WINDOW_H
#define PYLET_WINDOW_H

#include "code_editor_interface.h"
#include <qmainwindow.h>
#include <qsettings.h>
#include <qevent.h>

class PyletWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PyletWindow(QWidget *parent = 0);
    ~PyletWindow();

private:
    void initWindow();
    void initWidgets();
    void populateMenu();
    CodeEditor *codeEditor;
    QSettings *s;
    QRect screenRect;
};

#endif // PYLET_WINDOW_H