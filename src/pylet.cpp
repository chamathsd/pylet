/*
* Copyright (c) 2015 Jake Schadaway.
* Licensed under the MIT License. See LICENSE.md for details.
*/

#include "ui/pylet_window.h"
#include <qapplication.h>
#include <qsettings.h>

/* Entry point initialization - mostly runtime QSettings */
static void g_initSettings(const QApplication &application);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    g_initSettings(app);

    PyletWindow w;

    return app.exec();
}

static void g_initSettings(const QApplication &application)
{
    QString configFile = "config.ini";
    QSettings config(configFile, QSettings::IniFormat);

    config.setPath(QSettings::IniFormat, QSettings::UserScope, application.applicationDirPath());

    if (/* !QFile(configFile).exists() && */ config.isWritable())
    {
        config.beginGroup("Editor");

        config.setValue("iTabSpacing", 4);
        config.setValue("bTabsEmitSpaces", true);
        config.endGroup();

        config.beginGroup("Shortcuts");

        config.setValue("Undo", QKeySequence(Qt::CTRL + Qt::Key_Z));
        config.setValue("Redo", QKeySequence(Qt::CTRL + Qt::Key_Y));
        config.setValue("Cut", QKeySequence(Qt::CTRL + Qt::Key_X));
        config.setValue("Copy", QKeySequence(Qt::CTRL + Qt::Key_C));
        config.setValue("Paste", QKeySequence(Qt::CTRL + Qt::Key_V));
        config.setValue("Select All", QKeySequence(Qt::CTRL + Qt::Key_A));
        config.setValue("Zoom In", QKeySequence(Qt::CTRL + Qt::Key_Plus));
        config.setValue("Zoom In Alt", QKeySequence(Qt::CTRL + Qt::KeypadModifier + Qt::Key_Plus));
        config.setValue("Zoom In Alt2", QKeySequence(Qt::CTRL + Qt::Key_Equal));
        config.setValue("Zoom Out", QKeySequence(Qt::CTRL + Qt::Key_Minus));
        config.setValue("Zoom Out Alt", QKeySequence(Qt::CTRL + Qt::KeypadModifier + Qt::Key_Minus));
        config.setValue("Reset Zoom", QKeySequence(Qt::CTRL + Qt::Key_Slash));
        config.setValue("Reset Zoom Alt", QKeySequence(Qt::CTRL + Qt::KeypadModifier + Qt::Key_Slash));
        config.endGroup();
    }
}
