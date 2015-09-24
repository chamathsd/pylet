/*
* Copyright (c) 2015 Jake Schadaway.
* Licensed under the MIT License. See LICENSE.md for details.
*/

#include "ui/pylet_window.h"
#include <qapplication.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PyletWindow w;

    return a.exec();
}
