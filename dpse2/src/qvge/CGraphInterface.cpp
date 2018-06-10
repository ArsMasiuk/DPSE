#include "CGraphInterface.h"
#include "CNodeEditorScene.h"
#include "CNode.h"
#include "CDirectConnection.h"


CGraphInterface::CGraphInterface(CNodeEditorScene &scene)
{
    m_scene = &scene;
}


CConnection* CGraphInterface::addEdge(const QString &edgeId, const QString &startNodeId, const QString &endNodeId)
{
    if (m_scene == nullptr)
        return nullptr;

    // look for existing edge
    if (m_scene->getItemsById<CConnection>(edgeId).count())
        return nullptr;

    auto* edge = m_scene->createItemOfType<CDirectConnection>();
    if (!edge)
        return nullptr;

    auto* node1 = getNode(startNodeId, true);
    if (!node1)
        return nullptr;

    auto* node2 = getNode(endNodeId, true);
    if (!node2)
        return nullptr;

    edge->setId(edgeId);
    edge->setFirstNode(node1);
    edge->setLastNode(node2);

	m_scene->addItem(edge);

    return edge;
}


CNode* CGraphInterface::addNode(const QString &nodeId)
{
    if (m_scene == nullptr)
        return nullptr;

    // look for existing node
    if (m_scene->getItemsById<CNode>(nodeId).count())
        return nullptr;

    auto* node = m_scene->createItemOfType<CNode>();
    if (!node)
        return nullptr;

    node->setId(nodeId);

	m_scene->addItem(node);

    return node;
}


CNode* CGraphInterface::getNode(const QString &nodeId, bool autoCreate)
{
    if (m_scene == nullptr)
        return nullptr;

    // look for existing node
    auto nodes = (m_scene->getItemsById<CNode>(nodeId));
    if (nodes.count())
        return nodes.first();

    if (autoCreate)
    {
        auto* node = m_scene->createItemOfType<CNode>();
        if (!node)
            return nullptr;

        node->setId(nodeId);

		m_scene->addItem(node);
		
		return node;
    }

    return nullptr;
}


QList<CConnection*> CGraphInterface::getEdges() const
{
    QList<CConnection*> edges;

    if (m_scene)
    {
        auto items = m_scene->items();
        for (auto item: items)
            if (auto edge = dynamic_cast<CConnection*>(item))
                edges << edge;
    }

    return edges;
}


QList<CNode*> CGraphInterface::getNodes() const
{
    QList<CNode*> nodes;

    if (m_scene)
    {
        auto items = m_scene->items();
        for (auto item: items)
            if (auto node = dynamic_cast<CNode*>(item))
                nodes << node;
    }

    return nodes;
}
