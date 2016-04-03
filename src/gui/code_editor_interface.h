/*
* Copyright (c) 2016 Jake Dharmasiri.
* Licensed under the GNU GPLv3 License. See LICENSE for details.
* 
* Special thanks to Qt for their code editor reference,
* from which the following code is forked.
*/

#ifndef CODE_EDITOR_INTERFACE_H
#define CODE_EDITOR_INTERFACE_H

#include "code_editor_highlighter.h"
#include <qfilesystemwatcher.h>
#include <qplaintextedit.h>
#include <qsettings.h>

class CodeEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    CodeEditor(QSettings *s, QWidget* parent = 0, const QString &filePath = "");
    QFileSystemWatcher* watchdog = nullptr;
    QString filename;
    QString location;

    void lineNumbersPaintEvent(QPaintEvent *event);
    int lineNumbersWidth();
    int tabSpacing;
    bool tabsEmitSpaces;
    bool pendingRefresh = false;
    int untrackedID = 0;

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    QWidget *lineNumbers;
    QFont monoFont = QFont("Courier New", 12, QFont::Normal, false);
    PythonHighlighter* highlighter;

private slots:
    void updateLineNumbersWidth(int newBlockCount);
    void updateLineNumbersArea(const QRect &, int);
    void highlightCurrentLine();

public slots:
    void zoomInSlot();
    void zoomOutSlot();
    void resetZoom();
};

#endif // CODE_EDITOR_INTERFACE_H
