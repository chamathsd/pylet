/*
* Copyright (c) 2016 Jake Dharmasiri.
* Licensed under the GNU GPLv3 License. See LICENSE.md for details.
*/

#ifndef PYLET_WINDOW_H
#define PYLET_WINDOW_H

#include "code_editor_interface.h"
#include "console.h"
#include <qmainwindow.h>
#include <qsettings.h>
#include <qevent.h>

class PyletWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit PyletWindow(QWidget *parent = 0);
    ~PyletWindow();

private:
    void initWindow();
    void initWidgets();
    void populateMenu();
    CodeEditor *codeEditor;
    Console *console;
    QSettings *s;
    QRect screenRect;

private slots:
    void run();
};

#endif // PYLET_WINDOW_H
