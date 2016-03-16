/*
* Copyright (c) 2016 Jake Dharmasiri.
* Licensed under the GNU GPLv3 License. See LICENSE for details.
*/

#include "code_editor_interface.h"
#include "editor_stack.h"
#include <qstandardpaths.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qpainter.h>
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

void EditorStack::fileStream(CodeEditor* c, QFile* saveFile) {
    if (saveFile->open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
        QTextStream stream(saveFile);
        stream << c->toPlainText();
        saveFile->flush();
        saveFile->close();

        c->location = QFileInfo(*saveFile).canonicalFilePath();
        setTabText(indexOf(c), QFileInfo(*saveFile).fileName());
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Unable to write file at the specified location."));

        return;
    }
}

void EditorStack::save() {
    if (CodeEditor* c = qobject_cast<CodeEditor*>(currentWidget())) {
        QFile* saveFile = new QFile(c->location);
        if (c->location != "" && saveFile->exists()) {
            fileStream(c, saveFile);
        } else {
            saveAs();
        }
        delete saveFile;
    } else {
        qDebug() << "Nothing to save - are any files open?";
    }
}

void EditorStack::saveAs() {
    if (CodeEditor* c = qobject_cast<CodeEditor*>(currentWidget())) {
        QString filename = QFileDialog::getSaveFileName(this, tr("Save File As"),
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
            "Python files (*.py *.pyw);;Text files (*.txt);;All files (*.*)");
        if (filename != "") {
            QFile* saveFile = new QFile(filename, c);
            fileStream(c, saveFile);
            delete saveFile;
        }
    } else {
        qDebug() << "Nothing to save - are any files open?";
    }
}

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
