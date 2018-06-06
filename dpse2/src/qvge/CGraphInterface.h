#ifndef CGRAPHINTERFACE_H
#define CGRAPHINTERFACE_H

#include <QString>

class CNodeEditorScene;
class CConnection;
class CNode;


class CGraphInterface
{
public:
    explicit CGraphInterface(CNodeEditorScene& scene);

    // interface
    CConnection* addEdge(const QString& edgeId, const QString& startNodeId, const QString& endNodeId);
    CNode* addNode(const QString& nodeId);
    CNode* getNode(const QString& nodeId, bool autoCreate = false);

private:
    CNodeEditorScene *m_scene = nullptr;
};

#endif // CGRAPHINTERFACE_H
