#pragma once

#include <QObject>

#include <simu/ISimulator.h>

class IGraphInterface;
class ILogger;


class CSimulatorBase : public QObject, public ISimulator
{
    Q_OBJECT

public:
    explicit CSimulatorBase(QObject *parent = nullptr);

    virtual void setLogger(ILogger& logger);

    // ISimulator
    virtual void setGraph(const IGraphInterface& scene);

signals:
    void simulationFinished();

public slots:

protected:
    void logInfo(const QString& text);
    void logOk(const QString& text);
    void logWarning(const QString& text);
    void logError(const QString& text);

    ILogger *m_logger = nullptr;
    const IGraphInterface *m_scene = nullptr;
};

