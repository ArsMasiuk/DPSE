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
    ui->NodeColor->setColor(QColor());

    ui->NodeShape->addItem(QIcon(":/Icons/Node-Disc"), tr("Disc"));
    ui->NodeShape->addItem(QIcon(":/Icons/Node-Square"), tr("Square"));
    ui->NodeShape->addItem(QIcon(":/Icons/Node-Triangle"), tr("Triangle Up"));
    ui->NodeShape->addItem(QIcon(":/Icons/Node-Diamond"), tr("Diamond"));
    ui->NodeShape->addItem(QIcon(":/Icons/Node-Triangle-Down"), tr("Triangle Down"));
    ui->NodeShape->setCurrentIndex(-1);

    ui->NodeSize->setValue(-1);


    ui->EdgeColor->setColorScheme(QSint::namedColorsOpenOffice());
    ui->EdgeColor->setColor(QColor());

    ui->EdgeWeight->setValue(-1);

    ui->EdgePen->setCurrentIndex(-1);

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
    QList<CConnection*> edges = m_scene->getSelectedItems<CConnection>();
    QList<CNode*> nodes = m_scene->getSelectedItems<CNode>();

    ui->NodesBox->setTitle(tr("Nodes (%1)").arg(nodes.count()));
    ui->NodesBox->setEnabled(nodes.count());

    ui->EdgesBox->setTitle(tr("Edges (%1)").arg(edges.count()));
    ui->EdgesBox->setEnabled(edges.count());
}
