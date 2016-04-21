/*
* Copyright (c) 2016 Jake Dharmasiri.
* Licensed under the GNU GPLv3 License. See LICENSE for details.
*/

#ifndef PYLET_WINDOW_H
#define PYLET_WINDOW_H

#include "code_editor_interface.h"
#include "editor_stack.h"
#include "console.h"
#include <qfilesystemmodel.h>
#include <qmainwindow.h>
#include <qtreeview.h>
#include <qtoolbar.h>
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
    CodeEditor* codeEditor;
    EditorStack* editorStack;
    Console* console;
    QLineEdit* shell;
    QFileSystemModel* model;
    QFileSystemModel* blank = new QFileSystemModel(this);
    QTreeView* fileTree;
    QList<QAction*> actions;
    QToolBar *toolBar;
    QSettings *s;
    QRect screenRect;

private Q_SLOTS:
    void updateWindowTitle(int index = -1);
    void updateFileTree();
    void openFromFileTree(const QModelIndex&);
    void parseConsoleString();

public Q_SLOTS:
    void finalizeRuntime();
};

#endif // PYLET_WINDOW_H
