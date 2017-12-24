#pragma once

#include "qsplitbutton.h"

#include <QPen>


class QPenButton : public QSplitButton
{
    Q_OBJECT

public:
    QPenButton(QWidget *parent = Q_NULLPTR);

    void setUsedRange(Qt::PenStyle start, Qt::PenStyle end);

private:
    virtual void init();
};