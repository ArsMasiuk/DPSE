/*
This file is a part of
DPSE - Distributed Parallel Simulation Environment

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#ifndef ILOGGER_H
#define ILOGGER_H

#include <QString>
#include <QDateTime>

class ILogger
{
public:
    enum State{
        LOG_INFO,
        LOG_OK,
        LOG_WARN,
        LOG_ERROR
    };

    virtual void write(const QString& text, int state = LOG_INFO, const QDateTime& timestamp = QDateTime::currentDateTime()) = 0;
};

#endif // ILOGGER_H
