/*
* Copyright (c) 2016 Jake Dharmasiri.
* Licensed under the GNU GPLv3 License. See LICENSE.md for details.
*/

#include "pylet_window.h"
#include <qapplication.h>
#include <qdesktopwidget.h>
#include <qtemporaryfile.h>
#include <qmenubar.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qsplitter.h>
#include <qdebug.h>
#include <qdir.h>

PyletWindow::PyletWindow(QWidget *parent) :
    QMainWindow(parent) {

    QDesktopWidget *desktop = QApplication::desktop();
    screenRect = desktop->screen()->rect();

    s = new QSettings("config.ini", QSettings::IniFormat);

    initWindow();
    initWidgets();
    showMaximized();
}

PyletWindow::~PyletWindow() {
    /* Clean up QSettings that were passed around. */
    delete s;
}


void PyletWindow::initWindow() {
    setMinimumSize(600, 400);
    int screenWidth = screenRect.width();
    int screenHeight = screenRect.height();
    resize(screenWidth - 250, screenHeight - 250);

    setWindowTitle("Pylet (Editor)"); 
}

void PyletWindow::initWidgets() {
    QSplitter* coreWidget = new QSplitter(Qt::Horizontal);
    setCentralWidget(coreWidget);

    QWidget* navigator = new QWidget(coreWidget);
    QVBoxLayout* navLayout = new QVBoxLayout(navigator);
    navLayout->setMargin(0);
    navLayout->setSpacing(0);
    navigator->setLayout(navLayout);
    navigator->setMaximumWidth(280);
    coreWidget->insertWidget(0, navigator);

    QLabel* crawler = new QLabel(navigator);
    crawler->setStyleSheet("background-color: red; color: white; font-size: 40px;");
    crawler->setAlignment(Qt::AlignCenter);
    crawler->setText("File Crawler");
    navLayout->addWidget(crawler, 3);

    QLabel* infoBox = new QLabel(navigator);
    infoBox->setStyleSheet("background-color: green; color: white; font-size: 40px;");
    infoBox->setAlignment(Qt::AlignCenter);
    infoBox->setText("Info Box");
    navLayout->addWidget(infoBox, 1);

    codeEditor = new CodeEditor(s, coreWidget);
    codeEditor->setMinimumWidth(280);
    coreWidget->insertWidget(1, codeEditor);

    // QLabel* shell = new QLabel(coreWidget);
    // shell->setStyleSheet("background-color: orange; color: white; font-size: 40px;");
    // shell->setAlignment(Qt::AlignCenter);
    // shell->setText("Shell / Tracer");
    // shell->setMinimumWidth(280);
    console = new Console(coreWidget);
    console->setMinimumWidth(280);
    coreWidget->insertWidget(2, console);

    coreWidget->setStretchFactor(0, 1);
    coreWidget->setStretchFactor(1, 5);
    coreWidget->setStretchFactor(2, 3);

    coreWidget->setMidLineWidth(8);

    populateMenu();
    addToolBar("Action Bar");
    statusBar();
}

void PyletWindow::run() {
    QTemporaryFile tempFile(QDir::tempPath() + "-pyrun-XXXXXX.py", this->codeEditor);

    tempFile.open();
    QTextStream out(&tempFile);
    out << this->codeEditor->toPlainText() << endl;
    tempFile.close();

    console->runFile(tempFile.fileName());
    tempFile.remove();
}

void PyletWindow::populateMenu() {
    QList<QAction*> actions;

    QAction* undo = new QAction("Undo", this); actions << undo;
    connect(undo, SIGNAL(triggered()), codeEditor, SLOT(undo()));

    QAction* redo = new QAction("Redo", this); actions << redo;
    connect(redo, SIGNAL(triggered()), codeEditor, SLOT(redo()));

    QAction* cut = new QAction("Cut", this); actions << cut;
    connect(cut, SIGNAL(triggered()), codeEditor, SLOT(cut()));

    QAction* copy = new QAction("Copy", this); actions << copy;
    connect(copy, SIGNAL(triggered()), codeEditor, SLOT(copy()));

    QAction* paste = new QAction("Paste", this); actions << paste;
    connect(paste, SIGNAL(triggered()), codeEditor, SLOT(paste()));

    QAction* selectAll = new QAction("Select All", this); actions << selectAll;
    connect(selectAll, SIGNAL(triggered()), codeEditor, SLOT(selectAll()));

    QAction* run = new QAction("Run", this); actions << run;
    connect(run, SIGNAL(triggered()), this, SLOT(run()));

    QAction* zoomIn = new QAction("Zoom In", this); actions << zoomIn;
    connect(zoomIn, SIGNAL(triggered()), codeEditor, SLOT(zoomIn()));

    QAction* zoomOut = new QAction("Zoom Out", this); actions << zoomOut;
    connect(zoomOut, SIGNAL(triggered()), codeEditor, SLOT(zoomOut()));

    QAction *resetZoom = new QAction("Reset Zoom", this); actions << resetZoom;
    connect(resetZoom, SIGNAL(triggered()), codeEditor, SLOT(resetZoom()));

    for (int i = 0; i < actions.size(); ++i) {
        QAction *a = actions.at(i);
        if (s->value("Shortcuts/" + a->text() + " Alt").isValid()) {
            QList<QKeySequence> keylist;
            keylist.append(QKeySequence(s->value("Shortcuts/" + a->text()).toString()));
            keylist.append(QKeySequence(s->value("Shortcuts/" + a->text() + " Alt").toString()));
            if (s->value("Shortcuts/" + a->text() + " Alt2").isValid()) {
                keylist.append(QKeySequence(s->value("Shortcuts/" + a->text() + " Alt2").toString()));
            }
            a->setShortcuts(keylist);
        } else {
            a->setShortcut(QKeySequence(s->value("Shortcuts/" + a->text()).toString()));
        }
    }

    QMenu *fileMenu = menuBar()->addMenu("File");
    QMenu *editMenu = menuBar()->addMenu("Edit");
        editMenu->addAction(undo);
        editMenu->addAction(redo);
        editMenu->addSeparator();
        editMenu->addAction(cut);
        editMenu->addAction(copy);
        editMenu->addAction(paste);
        editMenu->addAction(selectAll);
    QMenu *searchMenu = menuBar()->addMenu("Search");
    QMenu *runMenu = menuBar()->addMenu("Run");
        runMenu->addAction(run);
    QMenu *viewMenu = menuBar()->addMenu("View");
        QMenu *zoomMenu = viewMenu->addMenu("Zoom");
            zoomMenu->addAction(zoomIn);
            zoomMenu->addAction(zoomOut);
            zoomMenu->addAction(resetZoom);
    QMenu *settingsMenu = menuBar()->addMenu("Settings");
    QMenu *helpMenu = menuBar()->addMenu("Help");
}
