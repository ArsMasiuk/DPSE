#include "CSimulatorBase.h"
#include "ILogger.h"

#include <qvge/CNodeEditorScene.h>


CSimulatorBase::CSimulatorBase(QObject *parent) : QObject(parent)
{

}


void CSimulatorBase::setLogger(ILogger &logger)
{
    m_logger = &logger;
}


void CSimulatorBase::logInfo(const QString& text)
{
    if (m_logger)
        m_logger->write(text, ILogger::LOG_INFO);
}


void CSimulatorBase::logOk(const QString& text)
{
    if (m_logger)
        m_logger->write(text, ILogger::LOG_OK);
}


void CSimulatorBase::logWarning(const QString& text)
{
    if (m_logger)
        m_logger->write(text, ILogger::LOG_WARN);
}


void CSimulatorBase::logError(const QString& text)
{
    if (m_logger)
        m_logger->write(text, ILogger::LOG_ERROR);
}


// ISimulator

void CSimulatorBase::setGraph(const IGraphInterface& scene)
{
    m_scene = &scene;
}



