/*
* Copyright (c) 2016 Jake Dharmasiri.
* Licensed under the GNU GPLv3 License. See LICENSE for details.
*/

#ifndef EDITOR_STACK_H
#define EDITOR_STACK_H

#include "code_editor_interface.h"
#include <qtabwidget.h>

class EditorStack : public QTabWidget {
    Q_OBJECT

public:
    EditorStack(QSettings* s, QWidget *parent);
    CodeEditor* currentEditor();
    void insertEditor(const QString &filePath = "");

private:
    void fileStream(CodeEditor* c, QFile* saveFile);
    QSettings* settingsPtr;
    QMap<int, CodeEditor*> untrackedFiles;

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
    void flagAsModified(bool);
};


#endif // EDITOR_STACK_H