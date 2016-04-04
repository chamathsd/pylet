/*
* Copyright (c) 2016 Jake Dharmasiri.
* Licensed under the GNU GPLv3 License. See LICENSE for details.
*/

#include "gui/pylet_window.h"
#include <qapplication.h>
#include <qmessagebox.h>
#include <qsettings.h>
#include <sstream>
#include <iostream>

/* Entry point initialization - mostly runtime QSettings */
static void g_initSettings(const QApplication &application);

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    g_initSettings(app);
    app.setCursorFlashTime(800);

    PyletWindow w;

    return app.exec();
}

static void g_initFonts(const QApplication &application) {
    application.font().setStyleStrategy(QFont::PreferAntialias);
    application.setFont(application.font());

    // Dynamically load fonts
    QStringList list;
    list << "SourceCodePro-Black.ttf" <<
        "SourceCodePro-Bold.ttf" <<
        "SourceCodePro-ExtraLight.ttf" <<
        "SourceCodePro-Light.ttf" <<
        "SourceCodePro-Medium.ttf" <<
        "SourceCodePro-Regular.ttf" <<
        "SourceCodePro-Semibold.ttf" <<
        "SourceSansPro-BlackItalic.ttf" <<
        "SourceSansPro-Black.ttf" <<
        "SourceSansPro-BoldItalic.ttf" <<
        "SourceSansPro-Bold.ttf" <<
        "SourceSansPro-ExtraLightItalic.ttf" <<
        "SourceSansPro-ExtraLight.ttf" <<
        "SourceSansPro-Italic.ttf" <<
        "SourceSansPro-LightItalic.ttf" <<
        "SourceSansPro-Light.ttf" <<
        "SourceSansPro-Regular.ttf" <<
        "SourceSansPro-SemiboldItalic.ttf" <<
        "SourceSansPro-Semibold.ttf" <<
        "MotivaSans-Medium.ttf";

    int fontID(-1);
    bool fontWarningShown(false);
    for (auto font : list) {
        QFile res(":/typeface/" + font);
        if (!res.open(QIODevice::ReadOnly)) {
            if (!fontWarningShown) {
                QMessageBox::warning(0, "Application", (QString) "Warning: Unable to load font " + QChar(0x00AB) + font + QChar(0x00BB) + ".");
                fontWarningShown = true;
            }
        } else {
            fontID = QFontDatabase::addApplicationFontFromData(res.readAll());
            if (fontID == -1 && !fontWarningShown) {
                QMessageBox::warning(0, "Application", (QString) "Warning: Unable to load font " + QChar(0x00AB) + font + QChar(0x00BB) + ".");
                fontWarningShown = true;
            }
        }
    }
}

static void g_initSettings(const QApplication &application) {
    QString configFile = "config.ini";
    QSettings config(configFile, QSettings::IniFormat);

    config.setPath(QSettings::IniFormat, QSettings::UserScope, application.applicationDirPath());

    if (/* !QFile(configFile).exists() && */ config.isWritable()) {
        config.beginGroup("Editor");

        config.setValue("iTabSpacing", 4);
        config.setValue("bTabsEmitSpaces", true);
        config.endGroup();

        config.beginGroup("Shortcuts");

        config.setValue("New", QKeySequence(Qt::CTRL + Qt::Key_N));
        config.setValue("Save", QKeySequence(Qt::CTRL + Qt::Key_S));
        config.setValue("Save As...", QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_S));
        config.setValue("Save All", QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
        config.setValue("Close", QKeySequence(Qt::CTRL + Qt::Key_W));
        config.setValue("Undo", QKeySequence(Qt::CTRL + Qt::Key_Z));
        config.setValue("Redo", QKeySequence(Qt::CTRL + Qt::Key_Y));
        config.setValue("Cut", QKeySequence(Qt::CTRL + Qt::Key_X));
        config.setValue("Copy", QKeySequence(Qt::CTRL + Qt::Key_C));
        config.setValue("Paste", QKeySequence(Qt::CTRL + Qt::Key_V));
        config.setValue("Select All", QKeySequence(Qt::CTRL + Qt::Key_A));
        config.setValue("Run", QKeySequence(Qt::CTRL + Qt::Key_R));
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
