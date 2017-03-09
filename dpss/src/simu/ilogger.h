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
