#include "CGraphSimulator.h"

#include <qvge/CNodeEditorScene.h>
#include <qvge/IGraphInterface.h>


CGraphSimulator::CGraphSimulator(QObject *parent) : CSimulatorBase(parent)
{
}


CGraphSimulator::~CGraphSimulator()
{
    cleanup();
}


// ISimulator

bool CGraphSimulator::analyse()
{
    if (!m_scene)
        return false;

    cleanup();

    // create edge array
    bool ok = true;

    auto edgeIds = m_scene->getEdgeIds();
    for (auto edgeId: edgeIds)
    {
        if (m_branchList.contains(edgeId))
        {
            ok = false;
            auto& edge = m_branchList[edgeId];
            edge.isOk = false;

            logError(tr("edge id: %1 has duplicate(s)").arg(edgeId));
        }
        else
        {
            auto& edge = m_branchList[edgeId];
            edge.isOk = true;
            edge.branch = new CBranch();
        }
    }

    return true;
}


bool CGraphSimulator::run()
{

    emit simulationFinished();
    return false;
}


bool CGraphSimulator::stop()
{
    emit simulationFinished();
    return false;
}


// privates

void CGraphSimulator::cleanup()
{
    for (auto it = m_branchList.begin(); it != m_branchList.end(); ++it)
    {
        if (it.value().branch)
        {
            delete it.value().branch;
        }
    }

    m_branchList.clear();
}
