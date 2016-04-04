/*
* Copyright (c) 2016 Jake Dharmasiri.
* Licensed under the GNU GPLv3 License. See LICENSE for details.
*/

#ifndef CONSOLE_H
#define CONSOLE_H

#include "info_box.h"
#include <qplaintextedit.h>

class Console : public QPlainTextEdit {
    Q_OBJECT

public:
    Console(InfoBox* infoBox, QWidget *parent = 0);
    void runFile(const QString &filename);
    void parseString(const QString &inputString);
    void interpretError(const QString &checkString);
    void throwError(const QString &message);

private:
    QFont monoFont = QFont("Courier New", 12, QFont::Normal, false);
    QString generateRestartString();
    InfoBox* i;
};

#endif // CONSOLE_H
