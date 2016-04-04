/*
* Copyright (c) 2016 Jake Dharmasiri.
* Licensed under the GNU GPLv3 License. See LICENSE for details.
*/

#include "editor_stack.h"
#include <qfilesystemwatcher.h>
#include <qtemporaryfile.h>
#include <qstandardpaths.h>
#include <qapplication.h>
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
    connect(qApp, SIGNAL(applicationStateChanged(Qt::ApplicationState)), this, SLOT(manageFocus()));
}

CodeEditor* EditorStack::currentEditor() {
    if (CodeEditor* c = qobject_cast<CodeEditor*>(currentWidget())) {
        return c;
    } else {
        qDebug() << "Cast to CodeEditor failed on EditorStack";
        return nullptr;
    }
}

void EditorStack::fileStream(CodeEditor* c, QFile* saveFile) {
    if (saveFile->open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
        saveQueued = true;

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

void EditorStack::refresh(CodeEditor* c) {
    setCurrentWidget(c);
    QFile* checkFile = new QFile(c->location);
    if (checkFile->exists()) {
        QMessageBox::StandardButton reload;
        reload = QMessageBox::question(this, "Reload", "The following file has been modified by another program:\n\n" +
            QFileInfo(*checkFile).absoluteFilePath() + "\n\nDo you want to reload it?",
            QMessageBox::Yes | QMessageBox::No);
        if (reload == QMessageBox::Yes) {
            if (checkFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream stream(checkFile);
                c->setPlainText(stream.readAll());
                setTabText(indexOf(c), c->filename);
                modificationQueued = true;
            }
            checkFile->close();
        } else {
            save(true);
        }
    } else {
        QMessageBox::StandardButton saveNew;
        saveNew = QMessageBox::question(this, "Save New", "The following file has been deleted or renamed:\n\n" +
            QFileInfo(*checkFile).absoluteFilePath() + "\n\nWould you like to save a new copy?",
            QMessageBox::Yes | QMessageBox::No);
        if (saveNew == QMessageBox::No || saveAs() != 0) {
            c->location = "";
            c->filename = nullptr;
            int fileID = generateUntrackedID();
            setTabText(indexOf(c), "untitled" + QString::number(fileID) + ".py");
            untrackedFiles.insert(fileID, c);
            c->untrackedID = fileID;
        }
    }
    delete checkFile;
}

int EditorStack::generateUntrackedID() {
    int id = 1;
    QMap<int, CodeEditor*>::iterator iter = untrackedFiles.begin();
    while (id == iter.key() && iter != untrackedFiles.end()) {
        id += 1;
        ++iter;
    }
    return id;
}

void EditorStack::manageFocus() {
    if (qApp->applicationState() == Qt::ApplicationActive) {
        for (int index = 0; index < count(); ++index) {
            if (CodeEditor* c = qobject_cast<CodeEditor*>(widget(index))) {
                if (c->pendingRefresh) {
                    refresh(c);
                    c->pendingRefresh = false;
                }
            }
        }
    }
}

void EditorStack::flagAsModified(bool modified) {
    if (CodeEditor* c = qobject_cast<CodeEditor*>(QObject::sender())) {
        if (!modificationQueued) {
            if (!c->filename.isNull()) {
                setTabText(indexOf(c), c->filename + "*");
            }
        } else {
            c->document()->setModified(false);
            modificationQueued = false;
        }
    }
}

void EditorStack::manageExternalModification() {
    qDebug() << "modification triggered";
    if (CodeEditor* c = qobject_cast<CodeEditor*>(QObject::sender()->parent())) {
        if (qApp->applicationState() != Qt::ApplicationInactive) {
            if (!saveQueued) {
                refresh(c);
            } else {
                saveQueued = false;
            }
        } else {
            c->pendingRefresh = true;
        }
    }
}

CodeEditor* EditorStack::insertEditor(const QString &filePath) {
    CodeEditor* codeEditor = new CodeEditor(settingsPtr, this, filePath);

    if (filePath == "") {
        int fileID = generateUntrackedID();
        addTab(codeEditor, "untitled" + QString::number(fileID) + ".py");
        untrackedFiles.insert(fileID, codeEditor);
        codeEditor->untrackedID = fileID;
    } else {
        addTab(codeEditor, "");
    }

    connect(codeEditor, SIGNAL(modificationChanged(bool)),
        this, SLOT(flagAsModified(bool)));

    codeEditor->resetZoom(globalZoom);
    setCurrentWidget(codeEditor);
    return codeEditor;
}

void EditorStack::open() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        "Python files (*.py *.pyw);;Text files (*.txt);;All files (*.*)");
    if (filename == "")
        return;

    CodeEditor* codeEditor = insertEditor(filename);
    QFile* openFile = new QFile(filename, codeEditor);
    
    if (openFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(openFile);
        codeEditor->setPlainText(stream.readAll());
        codeEditor->filename = QFileInfo(*openFile).fileName();
        setTabText(indexOf(codeEditor), codeEditor->filename);
        codeEditor->document()->setModified(false);
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Unable to read the specified file."));
        codeEditor->location = "";
    }
    delete openFile;
}

/*
* Series of slots that re-route global shortcuts to editor windows.
*/

void EditorStack::save(int index, bool forceSave) {
    if (index == -1)
        index = indexOf(currentWidget());
    qDebug() << "Index is:" << index;
    if (CodeEditor* c = qobject_cast<CodeEditor*>(widget(index))) {
        if (!c->document()->isModified() && !forceSave) {
            qDebug() << "Nothing to save - file has not been modified.";
            return;
        }
        qDebug() << "Got here.";
        QFile* saveFile = new QFile(c->location);
        if (c->location != "" && saveFile->exists()) {
            fileStream(c, saveFile);
            setTabText(indexOf(c), c->filename);
        } else {
            setCurrentWidget(c);
            saveAs();
        }
        delete saveFile;
        qDebug() << "File saved.";
    } else {
        qDebug() << "Nothing to save - are any files open?";
    }
}

int EditorStack::saveAs() {
    if (CodeEditor* c = qobject_cast<CodeEditor*>(currentWidget())) {
        QString filename = QFileDialog::getSaveFileName(this, tr("Save File As"),
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
            "/untitled" + QString::number(c->untrackedID) + ".py",
            "Python files (*.py *.pyw);;Text files (*.txt);;All files (*.*)");
        if (filename != "") {
            QFile* saveFile = new QFile(filename, c);
            fileStream(c, saveFile);
            setTabText(indexOf(c), c->filename);
            saveQueued = false;
            delete saveFile;

            untrackedFiles.remove(c->untrackedID);
            c->untrackedID = 0;

            c->watchdog = new QFileSystemWatcher(c);
            if (c->watchdog->addPath(filename)) {
                connect(c->watchdog, SIGNAL(fileChanged(QString)), this, SLOT(manageExternalModification()));
            } else {
                qDebug() << "Unable to set file watchdog at path " << filename;
            }
            currentChanged(indexOf(c));
            qDebug() << "File saved for the first time.";
            return 0;
        } else {
            qDebug() << "Filename is empty.";
            return 1;
        }
    } else {
        qDebug() << "Nothing to save - are any files open?";
        return 2;
    }
}

void EditorStack::saveAll() {
    for (int index = 0; index < count(); ++index) {
        save(index);
    }
}

void EditorStack::closeTab(int index, bool forceClose) {
    if (index == -1)
        index = indexOf(currentWidget());
    if (CodeEditor* c = qobject_cast<CodeEditor*>(widget(index))) {
        if (forceClose) {
            c->deleteLater();
        } else {
            bool isUntracked = c->filename == "";
            if (c->document()->isModified()) {
                setCurrentWidget(c);
                QString filename;
                if (!isUntracked) {
                    filename = c->location;
                } else {
                    filename = "untitled" + QString::number(c->untrackedID) + ".py";
                }
                QMessageBox::StandardButton saveQuery;
                saveQuery = QMessageBox::question(this, "Save", "Save file \"" + filename + "\"?",
                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
                if (saveQuery == QMessageBox::Cancel) {
                    return;
                } else {
                    if (isUntracked)
                        untrackedFiles.remove(c->untrackedID);
                    if (saveQuery == QMessageBox::Yes) {
                        save();
                    } else {
                        c->deleteLater();
                    }
                }
            } else {
                if (isUntracked)
                    untrackedFiles.remove(c->untrackedID);
                c->deleteLater();
            }
        }
    }
    removeTab(index);
    if (count() == 0)
        insertEditor();
}

void EditorStack::closeAll() {
    while (count() != 1) {
        closeTab();
    }
    // Close last tab manually since it will re-insert the default editor.
    closeTab();
}


void EditorStack::run() {
    if (CodeEditor* c = qobject_cast<CodeEditor*>(currentWidget())) {
        if (c->filename != "") {
            save();

            QFile execFile(c->location);
            console->runFile(execFile.fileName());
        } else {
            QTemporaryFile tempFile(QDir::tempPath() + "-pyrun-XXXXXX.py", c);

            tempFile.open();
            QTextStream out(&tempFile);
            out << c->toPlainText() << endl;
            tempFile.close();

            console->runFile(tempFile.fileName());
            tempFile.remove();
        }
    } else {
        console->throwError("No active files to run.");
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
    globalZoom += 2;
}

void EditorStack::zoomOut() {
    for (int index = 0; index < count(); ++index) {
        if (CodeEditor* c = qobject_cast<CodeEditor*>(widget(index))) {
            c->zoomOutSlot();
        }
    }
    globalZoom += 2;
}

void EditorStack::resetZoom() {
    for (int index = 0; index < count(); ++index) {
        if (CodeEditor* c = qobject_cast<CodeEditor*>(widget(index))) {
            c->resetZoom();
        }
    }
    globalZoom = 12;
}
