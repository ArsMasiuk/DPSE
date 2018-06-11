#include "CGraphSimulator.h"

#include <qvge/CNodeEditorScene.h>
#include <qvge/IGraphInterface.h>
#include <qvge/CConnection.h>
#include <qvge/CNode.h>


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

    auto edges = m_scene->getEdges();
    for (auto edge: edges)
    {
        if (m_branchList.contains(edge->getId()))
        {
            auto& edgeInfo = m_branchList[edge->getId()];
            edgeInfo.edge = edge;

            ok = false;
            edgeInfo.isOk = false;

            logError(tr("edge id: %1 has duplicate(s)").arg(edge->getId()));
        }
        else
        {
            auto& edgeInfo = m_branchList[edge->getId()];
            edgeInfo.edge = edge;

            edgeInfo.isOk = true;
            edgeInfo.branch = new CBranch();
        }
    }

    // check nodes as well
    auto nodes = m_scene->getNodes();
    for (auto node: nodes)
    {
        if (m_nodeList.contains(node->getId()))
        {
            auto& nodeInfo = m_nodeList[node->getId()];
            nodeInfo.node = node;

            ok = false;
            nodeInfo.isOk = false;

            logError(tr("node id: %1 has duplicate(s)").arg(node->getId()));
        }
        else
        {
            auto& nodeInfo = m_nodeList[node->getId()];
            nodeInfo.node = node;

            nodeInfo.isOk = true;
        }
    }

    return ok;
}


bool CGraphSimulator::prepare()
{
    bool ok = true;

    // check & init simulation params
    for (auto& edgeInfo: m_branchList.values())
    {
        if (edgeInfo.isOk)
        {
            bool isOk_L = false;
            double L = edgeInfo.edge->getAttribute("L").toDouble(&isOk_L);
            if (L <= 0 || !isOk_L)
            {
                ok = edgeInfo.isOk = false;
                logError(tr("edge id: %1 has invalid L value: %2").arg(edgeInfo.edge->getId()).arg(L));
            }

            bool isOk_S = false;
            double S = edgeInfo.edge->getAttribute("S").toDouble(&isOk_S);
            if (S <= 0 || !isOk_S)
            {
                ok = edgeInfo.isOk = false;
                logError(tr("edge id: %1 has invalid S value: %2").arg(edgeInfo.edge->getId()).arg(S));
            }

            bool isOk_R = false;
            double R = edgeInfo.edge->getAttribute("R").toDouble(&isOk_R);

//            bool isOk_Q = false;
//            double Q = edgeInfo.edge->getAttribute("Q").toDouble(&isOk_Q);

            if (!isOk_R)
            {
                ok = edgeInfo.isOk = false;
                logError(tr("edge id: %1 has invalid R value: %2").arg(edgeInfo.edge->getId()).arg(R));
            }

            if (!edgeInfo.isOk)
                continue;


            double Pbeg = 0, Pend = 0;

            auto n1 = edgeInfo.edge->firstNode();
            auto n2 = edgeInfo.edge->lastNode();
            Q_ASSERT(n1 != nullptr);
            Q_ASSERT(n2 != nullptr);

            Pbeg = n1->getAttribute("H").toDouble();
            Pend = n2->getAttribute("H").toDouble();

            if (!edgeInfo.isOk)
                continue;

            // ok, prepare branch
            edgeInfo.branch->init(L, S, 20);    // 20: to dos
            edgeInfo.branch->setR(R);
            edgeInfo.branch->setP(Pbeg, Pend);
        }
    }

    // init interchnages
    for (auto& edgeInfo: m_branchList.values())
    {
        if (edgeInfo.isOk)
        {
            edgeInfo.branch->m_inBeg.clear();
            edgeInfo.branch->m_outBeg.clear();
            edgeInfo.branch->m_inEnd.clear();
            edgeInfo.branch->m_outEnd.clear();

            auto n1 = edgeInfo.edge->firstNode();
            auto n1_in = n1->getInConnections();
            auto n1_out = n1->getOutConnections();

            for (auto e1: n1_in)
            {
                if (e1 != edgeInfo.edge)
                {
                    auto edgeInfo2 = m_branchList[e1->getId()];
                    if (edgeInfo2.isOk)
                        edgeInfo.branch->m_inBeg << edgeInfo2.branch;
                }
            }

            for (auto e1: n1_out)
            {
                if (e1 != edgeInfo.edge)
                {
                    auto edgeInfo2 = m_branchList[e1->getId()];
                    if (edgeInfo2.isOk)
                        edgeInfo.branch->m_outBeg << edgeInfo2.branch;
                }
            }



            auto n2 = edgeInfo.edge->lastNode();
            auto n2_in = n2->getInConnections();
            auto n2_out = n2->getOutConnections();

            for (auto e2: n2_in)
            {
                if (e2 != edgeInfo.edge)
                {
                    auto edgeInfo2 = m_branchList[e2->getId()];
                    if (edgeInfo2.isOk)
                        edgeInfo.branch->m_inEnd << edgeInfo2.branch;
                }
            }

            for (auto e2: n2_out)
            {
                if (e2 != edgeInfo.edge)
                {
                    auto edgeInfo2 = m_branchList[e2->getId()];
                    if (edgeInfo2.isOk)
                        edgeInfo.branch->m_outEnd << edgeInfo2.branch;
                }
            }

        }
    }

    return ok;
}


bool CGraphSimulator::simulate(int steps)
{
	m_inSimulation = true;

	if (steps <= 0) steps = 10000;	// test

	for (int step = 0; step < steps; ++step)
	{
		for (auto& edgeInfo : m_branchList.values())
		{
			if (edgeInfo.isOk)
			{
				edgeInfo.branch->exchange();
				edgeInfo.branch->stepRK4(0.001);
				edgeInfo.branch->exchange();
			}
		}

		if (!m_inSimulation)
			return false;
	}

	return true;
}


bool CGraphSimulator::run()
{
    prepare();

	simulate();

    emit simulationFinished();
    return true;
}


bool CGraphSimulator::stop()
{
	m_inSimulation = false;

    return true;
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

    m_nodeList.clear();
}
