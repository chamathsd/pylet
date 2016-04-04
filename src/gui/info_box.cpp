/*
* Copyright (c) 2016 Jake Dharmasiri.
* Licensed under the GNU GPLv3 License. See LICENSE for details.
*/

#include "info_box.h"
#include <qlayout.h>

InfoBox::InfoBox(QWidget *parent) : QWidget(parent) {
    setStyleSheet("background-color: #ffffcc;");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    errorLabel = new QLabel(this);
    errorLabel->setMinimumHeight(40);
    errorLabel->setMaximumHeight(40);
    errorLabel->setAlignment(Qt::AlignHCenter);
    errorLabel->setStyleSheet("font-weight: bold;");
    QFont monoFont = QFont("Courier New", 12, QFont::Normal, false);
    errorLabel->setFont(monoFont);
    layout->addWidget(errorLabel);

    errorDesc = new QLabel(this);
    errorDesc->setWordWrap(true);
    errorDesc->setAlignment(Qt::AlignTop);
    errorDesc->setStyleSheet("padding: 15px;");
    layout->addWidget(errorDesc);

    setLayout(layout);
}