/*
* Copyright (c) 2016 Jake Dharmasiri.
* Licensed under the GNU GPLv3 License. See LICENSE for details.
*/

#ifndef EDITOR_STACK_H
#define EDITOR_STACK_H

#include <qtabwidget.h>

class EditorStack : public QTabWidget {
    Q_OBJECT

public:
    EditorStack(QWidget *parent);

public slots:
    void save();
    void saveAs();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void selectAll();
    void zoomIn();
    void zoomOut();
    void resetZoom();

private slots:
    void closeTab(int);
};


#endif // EDITOR_STACK_H