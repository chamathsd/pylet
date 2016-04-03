/*
* Copyright (c) 2016 Jake Dharmasiri.
* Licensed under the GNU GPLv3 License. See LICENSE for details.
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

    setWindowTitle("Pylet");
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

    editorStack = new EditorStack(s, coreWidget);
    editorStack->setMinimumWidth(280);
    coreWidget->insertWidget(1, editorStack);

    editorStack->insertEditor();
    editorStack->insertEditor();
    // codeEditor->setMinimumWidth(280);

    console = new Console(coreWidget);
    console->setMinimumWidth(280);
    coreWidget->insertWidget(2, console);

    coreWidget->setStretchFactor(0, 1);
    coreWidget->setStretchFactor(1, 5);
    coreWidget->setStretchFactor(2, 3);

    coreWidget->setMidLineWidth(8);

    connect(editorStack, SIGNAL(currentChanged(int)), this, SLOT(updateWindowTitle(int)));

    /* Do action population and fill out menus correspondingly */
    QAction* save = new QAction("Save", this); actions << save;
    connect(save, SIGNAL(triggered()), editorStack, SLOT(save()));

    QAction* saveAs = new QAction("Save As...", this); actions << saveAs;
    connect(saveAs, SIGNAL(triggered()), editorStack, SLOT(saveAs()));

    QAction* undo = new QAction("Undo", this); actions << undo;
    connect(undo, SIGNAL(triggered()), editorStack, SLOT(undo()));

    QAction* redo = new QAction("Redo", this); actions << redo;
    connect(redo, SIGNAL(triggered()), editorStack, SLOT(redo()));

    QAction* cut = new QAction("Cut", this); actions << cut;
    connect(cut, SIGNAL(triggered()), editorStack, SLOT(cut()));

    QAction* copy = new QAction("Copy", this); actions << copy;
    connect(copy, SIGNAL(triggered()), editorStack, SLOT(copy()));

    QAction* paste = new QAction("Paste", this); actions << paste;
    connect(paste, SIGNAL(triggered()), editorStack, SLOT(paste()));

    QAction* selectAll = new QAction("Select All", this); actions << selectAll;
    connect(selectAll, SIGNAL(triggered()), editorStack, SLOT(selectAll()));

    QAction* run = new QAction("Run", this); actions << run;
    connect(run, SIGNAL(triggered()), this, SLOT(run()));

    QAction* zoomIn = new QAction("Zoom In", this); actions << zoomIn;
    connect(zoomIn, SIGNAL(triggered()), editorStack, SLOT(zoomIn()));

    QAction* zoomOut = new QAction("Zoom Out", this); actions << zoomOut;
    connect(zoomOut, SIGNAL(triggered()), editorStack, SLOT(zoomOut()));

    QAction *resetZoom = new QAction("Reset Zoom", this); actions << resetZoom;
    connect(resetZoom, SIGNAL(triggered()), editorStack, SLOT(resetZoom()));

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

    /* QMenu population */
    QMenu *fileMenu = menuBar()->addMenu("File");
        fileMenu->addAction(save);
        fileMenu->addAction(saveAs);
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

    /* QToolBar population */
    toolBar = addToolBar("Action Bar");
    
    QPixmap runIcon(":/pylet_icons/icons/run.png"),
            cutIcon(":/pylet_icons/icons/cut.png"),
            copyIcon(":/pylet_icons/icons/copy.png"),
            pasteIcon(":/pylet_icons/icons/paste.png"),
            undoIcon(":/pylet_icons/icons/undo.png"),
            redoIcon(":/pylet_icons/icons/redo.png"),
            zoomInIcon(":/pylet_icons/icons/zoom-in.png"),
            zoomOutIcon(":/pylet_icons/icons/zoom-out.png"),
            zoomResetIcon(":/pylet_icons/icons/zoom-fit.png");
    toolBar->addAction(QIcon(runIcon), "Run File", this, SLOT(run()));
    toolBar->addSeparator();
    toolBar->addAction(QIcon(cutIcon), "Cut", editorStack, SLOT(cut()));
    toolBar->addAction(QIcon(copyIcon), "Copy", editorStack, SLOT(copy()));
    toolBar->addAction(QIcon(pasteIcon), "Paste", editorStack, SLOT(paste()));
    toolBar->addSeparator();
    toolBar->addAction(QIcon(undoIcon), "Undo", editorStack, SLOT(undo()));
    toolBar->addAction(QIcon(redoIcon), "Redo", editorStack, SLOT(redo()));
    toolBar->addSeparator();
    toolBar->addAction(QIcon(zoomInIcon), "Zoom In", editorStack, SLOT(zoomIn()));
    toolBar->addAction(QIcon(zoomOutIcon), "Zoom Out", editorStack, SLOT(zoomOut()));
    toolBar->addAction(QIcon(zoomResetIcon), "Reset Zoom", editorStack, SLOT(resetZoom()));

    statusBar();
}

void PyletWindow::run() {
    if (CodeEditor* c = qobject_cast<CodeEditor*>(editorStack->currentWidget())) {
        QTemporaryFile tempFile(QDir::tempPath() + "-pyrun-XXXXXX.py", c);

        tempFile.open();
        QTextStream out(&tempFile);
        out << c->toPlainText() << endl;
        tempFile.close();

        console->runFile(tempFile.fileName());
        tempFile.remove();
    } else {
        console->throwError("No active files to run.");
    }
}

void PyletWindow::updateWindowTitle(int index) {
    qDebug() << "Got window update signal.";
    qDebug() << "Index is:" << index;
    if (index == -1) {
        setWindowTitle("Pylet");
    } else {
        if (CodeEditor* c = qobject_cast<CodeEditor*>(editorStack->widget(index))) {
            if (c->filename != nullptr) {
                setWindowTitle(c->location + " - Pylet");
            } else {
                setWindowTitle(editorStack->tabText(index) + " - Pylet");
            }
        } else {
            setWindowTitle("Pylet");
        }
    }
}
