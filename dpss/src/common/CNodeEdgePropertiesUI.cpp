#include "CNodeEdgePropertiesUI.h"
#include "ui_CNodeEdgePropertiesUI.h"

#include <qvge/CNodeEditorScene.h>
#include <qvge/CConnection.h>
#include <qvge/CNode.h>


CNodeEdgePropertiesUI::CNodeEdgePropertiesUI(QWidget *parent) :
    QWidget(parent),
    m_scene(NULL),
    ui(new Ui::CNodeEdgePropertiesUI)
{
    ui->setupUi(this);

    ui->NodeColor->setColorScheme(QSint::namedColorsOpenOffice());
    ui->NodeColor->setColor(Qt::green);

    QMenu *shapeMenu = new QMenu();
    auto *diskShape = shapeMenu->addAction(QIcon(":/Icons/Node-Disc"), tr("Disc"));
    shapeMenu->addAction(QIcon(":/Icons/Node-Square"), tr("Square"));
    shapeMenu->addAction(QIcon(":/Icons/Node-Triangle"), tr("Triangle Up"));
    shapeMenu->addAction(QIcon(":/Icons/Node-Diamond"), tr("Diamond"));
    shapeMenu->addAction(QIcon(":/Icons/Node-Triangle-Down"), tr("Triangle Down"));
    ui->NodeShapeTB->setMenu(shapeMenu);
    shapeMenu->setDefaultAction(diskShape);
    ui->NodeShapeTB->setDefaultAction(diskShape);


    ui->EdgeColor->setColorScheme(QSint::namedColorsOpenOffice());
    ui->EdgeColor->setColor(Qt::gray);
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

    QList<CConnection*> edges = m_scene->getSelectedEdges();
    QList<CNode*> nodes = m_scene->getSelectedNodes();

    ui->NodesBox->setTitle(tr("Nodes (%1)").arg(nodes.count()));
    //ui->NodesBox->setEnabled(nodes.count());

    ui->EdgesBox->setTitle(tr("Edges (%1)").arg(edges.count()));
    //ui->EdgesBox->setEnabled(edges.count());
}


void CNodeEdgePropertiesUI::on_NodeColor_activated(const QColor &color)
{
    if (m_scene == NULL)
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


void CNodeEdgePropertiesUI::on_EdgeColor_activated(const QColor &color)
{
    if (m_scene == NULL)
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
