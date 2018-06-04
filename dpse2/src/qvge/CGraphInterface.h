#ifndef CGRAPHINTERFACE_H
#define CGRAPHINTERFACE_H

#include <QString>

class CEditorScene;
class CConnection;
class CNode;


class CGraphInterface
{
public:
    explicit CGraphInterface(CEditorScene& scene);

    // interface
    CConnection* addEdge(const QString& edgeId, const QString& startNodeId, const QString& endNodeId);
    CNode* addNode(const QString& nodeId);
    CNode* getNode(const QString& nodeId, bool autoCreate = false);

private:
    CEditorScene* m_scene;
};

#endif // CGRAPHINTERFACE_H
