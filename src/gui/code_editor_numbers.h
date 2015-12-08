/*
* Copyright (c) 2015 Jake Schadaway.
* Licensed under the MIT License. See LICENSE.md for details.
*
* Special thanks to Qt for their code editor reference,
* from which the following code is forked.
*/

#ifndef CODE_EDITOR_NUMBERS_H
#define CODE_EDITOR_NUMBERS_H

#include "code_editor_interface.h"
#include <qwidget.h>

class LineNumberWidget : public QWidget {
    Q_OBJECT

public:
    LineNumberWidget(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }
    QSize sizeHint() const Q_DECL_OVERRIDE {
        return QSize(codeEditor->lineNumbersWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE {
        codeEditor->lineNumbersPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};

#endif // CODE_EDITOR_NUMBERS_H