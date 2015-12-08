/*
* Copyright (c) 2015 Jake Schadaway.
* Licensed under the MIT License. See LICENSE.md for details.
*/

#ifndef CONSOLE_H
#define CONSOLE_H

#include <qplaintextedit.h>

class Console : public QPlainTextEdit {
    Q_OBJECT

public:
    Console(QWidget *parent = 0);
    void runFile(const QString &filename);

private:
    QFont monoFont = QFont("Courier New", 12, QFont::Normal, false);
    QString generateRestartString();
};

#endif // CONSOLE_H
