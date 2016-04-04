/*
* Copyright (c) 2016 Jake Dharmasiri.
* Licensed under the GNU GPLv3 License. See LICENSE for details.
*/

#include <qwidget.h>
#include <qlabel.h>

#ifndef INFO_BOX_H
#define INFO_BOX_H

class InfoBox : public QWidget {
    Q_OBJECT

public:
    InfoBox(QWidget *parent = 0);
    QLabel* errorLabel;
    QLabel* errorDesc;
};

#endif // INFO_BOX_H