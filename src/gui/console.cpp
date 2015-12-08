/*
* Copyright (c) 2015 Jake Schadaway.
* Licensed under the MIT License. See LICENSE.md for details.
*/

#include "console.h"
#include "src/python/buffer.h"

Console::Console(QWidget *parent) : QPlainTextEdit(parent) {
    // QString s = QString::fromStdString(parsePyFile());
    // this->textCursor().insertText(s);
}

void Console::runFile(const QString &filename) {
    std::string pyFile = filename.toStdString();
    QString readOut = QString::fromStdString(parsePyFile(pyFile));
    this->textCursor().insertText(readOut);
    this->ensureCursorVisible();
}
