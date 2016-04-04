/*
* Copyright (c) 2016 Jake Dharmasiri.
* Licensed under the GNU GPLv3 License. See LICENSE for details.
*/

#include "console.h"
#include "src/python/buffer.h"
#include <qdebug.h>

Console::Console(QWidget *parent) : QPlainTextEdit(parent) {
    setFont(monoFont);
    setWordWrapMode(QTextOption::WrapAnywhere);
    setTextInteractionFlags(Qt::TextSelectableByMouse);
}

void Console::runFile(const QString &filename) {
    moveCursor(QTextCursor::End);
    textCursor().insertText(generateRestartString());
    std::string pyFile = filename.toStdString();
    QString readOut = QString::fromStdString(parsePyFile(pyFile));
    textCursor().insertText(readOut);
    ensureCursorVisible();
}

void Console::parseString(const QString &inputString) {
    moveCursor(QTextCursor::End);
    std::string pyString = inputString.toStdString();
    QString readOut = QString::fromStdString(parsePyString(pyString));
    textCursor().insertText(readOut);
    ensureCursorVisible();
}

void Console::throwError(const QString &message) {
    QString htmlString = "<font color = \"red\">[Pylet] " + message + "</font>";
    appendHtml(htmlString);
}

QString Console::generateRestartString() {
    int fontWidth = fontMetrics().width('=');
    QString spacer(((width() - 140) / fontWidth) / 2, '=');
    QString restartString = "\n" + spacer + " RESTART " + spacer + "\n";
    return restartString;
}
