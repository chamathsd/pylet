/*
* Copyright (c) 2016 Jake Dharmasiri.
* Licensed under the GNU GPLv3 License. See LICENSE for details.
*/

#include "editor_stack.h"
#include <qstandardpaths.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qpainter.h>
#include <qdebug.h>

EditorStack::EditorStack(QSettings* s, QWidget *parent) : 
    settingsPtr(s), 
    QTabWidget(parent) {

    setStyleSheet("QTabBar::tab { height: 30px; }");
    setTabsClosable(true);
    setMovable(true);

    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

void EditorStack::insertEditor(const QString &filePath) {
    CodeEditor* codeEditor = new CodeEditor(settingsPtr, this);

    int fileID = 1;
    QMap<int, CodeEditor*>::iterator iter = untrackedFiles.begin();
    while (fileID == iter.key() && iter != untrackedFiles.end()) {
        fileID += 1;
        ++iter;
    }
    addTab(codeEditor, "untitled" + QString::number(fileID) + ".py");
    untrackedFiles.insert(fileID, codeEditor);

    connect(codeEditor, SIGNAL(modificationChanged(bool)),
        this, SLOT(flagAsModified(bool)));
}

void EditorStack::closeTab(int index) {
    removeTab(index);
}

void EditorStack::flagAsModified(bool modified) {
    if (CodeEditor* c = qobject_cast<CodeEditor*>(QObject::sender())) {
        if (!c->filename.isNull()) {
            setTabText(indexOf(c), c->filename + "*");
        }
    }
}

CodeEditor* EditorStack::currentEditor() {
    if (CodeEditor* c = qobject_cast<CodeEditor*>(currentWidget())) {
        return c;
    } else {
        qDebug() << "Cast to CodeEditor failed on EditorStack";
        return nullptr;
    }
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
        c->filename = QFileInfo(*saveFile).fileName();
        c->document()->setModified(false);
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Unable to write file at the specified location."));
    }
}

void EditorStack::save() {
    if (CodeEditor* c = qobject_cast<CodeEditor*>(currentWidget())) {
        QFile* saveFile = new QFile(c->location);
        if (c->location != "" && saveFile->exists()) {
            fileStream(c, saveFile);
            setTabText(indexOf(c), c->filename);
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
            setTabText(indexOf(c), c->filename);
            delete saveFile;
        }
    } else {
        qDebug() << "Nothing to save - are any files open?";
    }
}

void EditorStack::undo() {
    currentEditor()->undo();
}

void EditorStack::redo() {
    currentEditor()->redo();
}

void EditorStack::cut() {
    currentEditor()->cut();
}

void EditorStack::copy() {
    currentEditor()->copy();
}

void EditorStack::paste() {
    currentEditor()->paste();
}

void EditorStack::selectAll() {
    currentEditor()->selectAll();
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
