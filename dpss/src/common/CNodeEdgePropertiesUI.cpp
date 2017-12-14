#include "CNodeEdgePropertiesUI.h"
#include "ui_CNodeEdgePropertiesUI.h"

#include <qvge/CNodeEditorScene.h>
#include <qvge/CConnection.h>
#include <qvge/CNode.h>


CNodeEdgePropertiesUI::CNodeEdgePropertiesUI(QWidget *parent) :
    QWidget(parent),
    m_scene(NULL),
    m_updateLock(true),
    ui(new Ui::CNodeEdgePropertiesUI)
{
    ui->setupUi(this);

    ui->NodeColor->setColorScheme(QSint::namedColorsOpenOffice());
    ui->NodeColor->setColor(Qt::green);

    ui->NodeShape->addAction(QIcon(":/Icons/Node-Disc"), tr("Disc"), "disc");
    ui->NodeShape->addAction(QIcon(":/Icons/Node-Square"), tr("Square"), "square");
    ui->NodeShape->addAction(QIcon(":/Icons/Node-Triangle"), tr("Triangle Up"), "triangle");
    ui->NodeShape->addAction(QIcon(":/Icons/Node-Diamond"), tr("Diamond"), "diamond");
    ui->NodeShape->addAction(QIcon(":/Icons/Node-Triangle-Down"), tr("Triangle Down"), "triangle2");


    ui->EdgeColor->setColorScheme(QSint::namedColorsOpenOffice());
    ui->EdgeColor->setColor(Qt::red);

    ui->EdgeStyle->setUsedRange(Qt::SolidLine, Qt::DotLine);
}

CNodeEdgePropertiesUI::~CNodeEdgePropertiesUI()
{
    delete ui;
}


void CNodeEdgePropertiesUI::setScene(CNodeEditorScene* scene)
{
    if (m_scene)
        onSceneDetached(m_scene);

    m_scene = scene;

    setEnabled(m_scene);

    if (m_scene)
        onSceneAttached(m_scene);
}


void CNodeEdgePropertiesUI::connectSignals(CEditorScene* scene)
{
    connect(scene, SIGNAL(sceneChanged()), this, SLOT(onSceneChanged()), Qt::QueuedConnection);
    connect(scene, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()), Qt::QueuedConnection);
}

void CNodeEdgePropertiesUI::onSceneAttached(CEditorScene* scene)
{
    connectSignals(scene);

    onSceneChanged();
}

void CNodeEdgePropertiesUI::onSceneDetached(CEditorScene* scene)
{
    scene->disconnect(this);
}

void CNodeEdgePropertiesUI::onSceneChanged()
{
    // update active selections if any
    onSelectionChanged();
}

void CNodeEdgePropertiesUI::onSelectionChanged()
{
    if (m_scene == NULL)
        return;

    m_updateLock = true;

    QList<CConnection*> edges = m_scene->getSelectedEdges();
    QList<CNode*> nodes = m_scene->getSelectedNodes();

    ui->NodesBox->setTitle(tr("Nodes (%1)").arg(nodes.count()));

    if (nodes.count())
    {
        auto node = nodes.first();

        ui->NodeColor->setColor(node->getAttribute("color").value<QColor>());
        ui->NodeShape->selectAction(node->getAttribute("shape"));
        ui->NodeSize->setValue(node->getAttribute("size").toSize().width());
    }
    //ui->NodesBox->setEnabled(nodes.count());

    ui->EdgesBox->setTitle(tr("Edges (%1)").arg(edges.count()));

    if (edges.count())
    {
        auto edge = edges.first();

        ui->EdgeColor->setColor(edge->getAttribute("color").value<QColor>());
        ui->EdgeWeight->setValue(edge->getAttribute("weight").toDouble());
        ui->EdgeStyle->selectAction(edge->getAttribute("style"));
    }
    //ui->EdgesBox->setEnabled(edges.count());

    // allow updates
    m_updateLock = false;
}


void CNodeEdgePropertiesUI::on_NodeColor_activated(const QColor &color)
{
    if (m_updateLock || m_scene == NULL)
        return;

    QList<CNode*> nodes = m_scene->getSelectedNodes();
    if (nodes.isEmpty())
        return;

    for (auto node: nodes)
    {
        node->setAttribute("color", color);
    }

    m_scene->addUndoState();
}


void CNodeEdgePropertiesUI::on_NodeShape_activated(QVariant data)
{
    if (m_updateLock || m_scene == NULL)
        return;

    QList<CNode*> nodes = m_scene->getSelectedNodes();
    if (nodes.isEmpty())
        return;

    for (auto node: nodes)
    {
        node->setAttribute("shape", data);
    }

    m_scene->addUndoState();
}


void CNodeEdgePropertiesUI::on_NodeSize_valueChanged(int value)
{
    if (m_updateLock || m_scene == NULL)
        return;

    QList<CNode*> nodes = m_scene->getSelectedNodes();
    if (nodes.isEmpty())
        return;

    for (auto node: nodes)
    {
        node->setAttribute("size", value);
    }

    m_scene->addUndoState();
}


void CNodeEdgePropertiesUI::on_EdgeColor_activated(const QColor &color)
{
    if (m_updateLock || m_scene == NULL)
        return;

    QList<CConnection*> edges = m_scene->getSelectedEdges();
    if (edges.isEmpty())
        return;

    for (auto edge: edges)
    {
        edge->setAttribute("color", color);
    }

    m_scene->addUndoState();
}


void CNodeEdgePropertiesUI::on_EdgeWeight_valueChanged(double value)
{
    if (m_updateLock || m_scene == NULL)
        return;

    QList<CConnection*> edges = m_scene->getSelectedEdges();
    if (edges.isEmpty())
        return;

    for (auto edge: edges)
    {
        edge->setAttribute("weight", value);
    }

    m_scene->addUndoState();
}


void CNodeEdgePropertiesUI::on_EdgeStyle_activated(QVariant data)
{
    if (m_updateLock || m_scene == NULL)
        return;

    QList<CConnection*> edges = m_scene->getSelectedEdges();
    if (edges.isEmpty())
        return;

    for (auto edge: edges)
    {
        edge->setAttribute("style", data);
    }

    m_scene->addUndoState();
}
