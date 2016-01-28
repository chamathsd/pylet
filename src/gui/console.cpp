/*
* Copyright (c) 2015 Jake Schadaway.
* Licensed under the MIT License. See LICENSE.md for details.
*/

#include "console.h"
#include "src/python/buffer.h"

Console::Console(QWidget *parent) : QPlainTextEdit(parent) {
    setFont(monoFont);
    setWordWrapMode(QTextOption::WrapAnywhere);
}

void Console::runFile(const QString &filename) {
    moveCursor(QTextCursor::End);
    textCursor().insertText(generateRestartString());
    std::string pyFile = filename.toStdString();
    QString readOut = QString::fromStdString(parsePyFile(pyFile));
    textCursor().insertText(readOut);
    ensureCursorVisible();
}

QString Console::generateRestartString() {
    int fontWidth = fontMetrics().width('=');
    QString spacer(((width() - 140) / fontWidth) / 2, '=');
    QString restartString = "\n" + spacer + " RESTART " + spacer + "\n";
    return restartString;
}
