/*
* Copyright (c) 2015 Jake Schadaway.
* Licensed under the MIT License. See LICENSE.md for details.
*/

#include "pylet_window.h"
#include "code_editor_interface.h"
#include <qapplication.h>
#include <qdesktopwidget.h>
#include <qmenubar.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qsplitter.h>

PyletWindow::PyletWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QDesktopWidget *desktop = QApplication::desktop();
    screenRect = desktop->screen()->rect();

    initWindow();
    initWidgets();
    showMaximized();
}

void PyletWindow::initWindow()
{
    setMinimumSize(600, 400);
    int screenWidth = screenRect.width();
    int screenHeight = screenRect.height();
    resize(screenWidth - 250, screenHeight - 250);

    setWindowTitle("Pylet (Editor)"); 
}

void PyletWindow::initWidgets()
{
    populateMenu();
    addToolBar("Action Bar");
    statusBar();

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

    CodeEditor* codeEditor = new CodeEditor(coreWidget);
    codeEditor->setMinimumWidth(280);
    coreWidget->insertWidget(1, codeEditor);

    QLabel* shell = new QLabel(coreWidget);
    shell->setStyleSheet("background-color: orange; color: white; font-size: 40px;");
    shell->setAlignment(Qt::AlignCenter);
    shell->setText("Shell / Tracer");
    shell->setMinimumWidth(280);
    coreWidget->insertWidget(2, shell);

    coreWidget->setStretchFactor(0, 1);
    coreWidget->setStretchFactor(1, 5);
    coreWidget->setStretchFactor(2, 3);

    coreWidget->setMidLineWidth(8);
}

void PyletWindow::populateMenu()
{
    menuBar()->addMenu("File");
    menuBar()->addMenu("Edit");
    menuBar()->addMenu("Search");
    menuBar()->addMenu("Run");
    menuBar()->addMenu("View");
    menuBar()->addMenu("Settings");
    menuBar()->addMenu("Help");
}
