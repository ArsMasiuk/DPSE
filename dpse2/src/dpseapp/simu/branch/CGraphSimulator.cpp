#include "CGraphSimulator.h"

#include <qvge/CNodeEditorScene.h>


CGraphSimulator::CGraphSimulator(QObject *parent) : QObject(parent)
{

}


// ISimulator

void CGraphSimulator::setScene(const CNodeEditorScene& scene)
{
    m_scene = &scene;
}


bool CGraphSimulator::run()
{
    return false;
}


bool CGraphSimulator::stop()
{
    return false;
}

