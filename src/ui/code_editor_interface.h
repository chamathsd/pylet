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
#include <qsettings.h>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QSettings *s, QWidget *parent = 0);

    void lineNumbersPaintEvent(QPaintEvent *event);
    int lineNumbersWidth();
    int tabSpacing;
    bool tabsEmitSpaces;

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private slots:
    void updateLineNumbersWidth(int newBlockCount);
    void updateLineNumbersArea(const QRect &, int);
    void highlightCurrentLine();

private:
    QWidget *lineNumbers;
    QFont monoFont = QFont("Courier New", 10, QFont::Normal, false);
};

#endif // CODE_EDITOR_INTERFACE_H