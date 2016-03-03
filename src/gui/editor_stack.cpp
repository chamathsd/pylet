/*
* Copyright (c) 2016 Jake Dharmasiri.
* Licensed under the GNU GPLv3 License. See LICENSE for details.
*/

#include "code_editor_interface.h"
#include "editor_stack.h"

#include <qdebug.h>

EditorStack::EditorStack(QWidget *parent) : QTabWidget(parent) {
    setStyleSheet("QTabBar::tab { height: 30px; }");
    setTabsClosable(true);
    setMovable(true);

    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

void EditorStack::closeTab(int index) {
    removeTab(index);
}

/*
* Series of slots that re-route global shortcuts to editor windows.
*/

void EditorStack::undo() {
    if (CodeEditor* c = qobject_cast<CodeEditor*>(currentWidget())) {
        c->undo();
    } else {
        qDebug() << "Cast to CodeEditor failed in EditorStack slot undo()";
    }
}

void EditorStack::redo() {
    if (CodeEditor* c = qobject_cast<CodeEditor*>(currentWidget())) {
        c->redo();
    } else {
        qDebug() << "Cast to CodeEditor failed in EditorStack slot undo()";
    }
}

void EditorStack::cut() {
    if (CodeEditor* c = qobject_cast<CodeEditor*>(currentWidget())) {
        c->cut();
    } else {
        qDebug() << "Cast to CodeEditor failed in EditorStack slot cut()";
    }
}

void EditorStack::copy() {
    if (CodeEditor* c = qobject_cast<CodeEditor*>(currentWidget())) {
        c->copy();
    } else {
        qDebug() << "Cast to CodeEditor failed in EditorStack slot copy()";
    }
}

void EditorStack::paste() {
    if (CodeEditor* c = qobject_cast<CodeEditor*>(currentWidget())) {
        c->paste();
    } else {
        qDebug() << "Cast to CodeEditor failed in EditorStack slot paste()";
    }
}

void EditorStack::selectAll() {
    if (CodeEditor* c = qobject_cast<CodeEditor*>(currentWidget())) {
        c->selectAll();
    } else {
        qDebug() << "Cast to CodeEditor failed in EditorStack slot selectAll()";
    }
}

void EditorStack::zoomIn() {
    for (int index = 0; index < count(); ++index) {
        if (CodeEditor* c = qobject_cast<CodeEditor*>(widget(index))) {
            c->zoomInSlot();
        }
    }
}

void EditorStack::zoomOut() {
    for (int index = 0; index < count(); ++index) {
        if (CodeEditor* c = qobject_cast<CodeEditor*>(widget(index))) {
            c->zoomOutSlot();
        }
    }
}

void EditorStack::resetZoom() {
    for (int index = 0; index < count(); ++index) {
        if (CodeEditor* c = qobject_cast<CodeEditor*>(widget(index))) {
            c->resetZoom();
        }
    }
}
