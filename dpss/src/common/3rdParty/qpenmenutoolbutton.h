#ifndef QPENMENUTOOLBUTTON_H
#define QPENMENUTOOLBUTTON_H

#include "qmenutoolbutton.h"

#include <QPen>


class QPenMenuToolButton : public QMenuToolButton
{
    Q_OBJECT

public:
    QPenMenuToolButton(QWidget *parent = Q_NULLPTR);

    void setUsedRange(Qt::PenStyle start, Qt::PenStyle end);
};

#endif // QPENMENUTOOLBUTTON_H
