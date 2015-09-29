/*
* Copyright (c) 2015 Jake Schadaway.
* Licensed under the MIT License. See LICENSE.md for details.
* 
* Special thanks to Qt for their code editor reference,
* from which the following code is forked.
*/

#ifndef CODE_EDITOR_INTERFACE_H
#define CODE_EDITOR_INTERFACE_H

#include <qplaintextedit.h>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);

    void lineNumbersPaintEvent(QPaintEvent *event);
    int lineNumbersWidth();

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void updateLineNumbersWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumbersArea(const QRect &, int);

private:
    QWidget *lineNumbers;
    QFont monoFont = QFont("Courier New", 10, QFont::Normal, false);
};

#endif // CODE_EDITOR_INTERFACE_H