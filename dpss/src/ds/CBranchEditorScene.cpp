#include "CBranchEditorScene.h"

#include "qvge/CNode.h"
#include "qvge/CConnection.h"

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QStyleOptionGraphicsItem>


/// CBranchEditorScene

CBranchEditorScene::CBranchEditorScene(QObject *parent)
	: Super(parent)
{
	// default factories
	registerItemFactory<CBranchNode>();
	registerItemFactory<CFanNode>();
	registerItemFactory<CBranchConnection>();

	setBranchesMode();

	activateConnectionFactory("CBranchConnection");
}

CBranchEditorScene::~CBranchEditorScene()
{
}

void CBranchEditorScene::initialize()
{
	Super::initialize();

	// default fan attr
    CAttribute fanAttr("fan", "color", "Color", QColor(Qt::green));
	setClassAttribute(fanAttr);

    // default branch attributes
    setClassAttribute(CAttribute("edge", "MODEL", "Model", ""));
    setClassAttribute(CAttribute("edge", "L", "L", 0));
    setClassAttribute(CAttribute("edge", "S", "S", 0));
    setClassAttribute(CAttribute("edge", "R", "R", 0));
    setClassAttribute(CAttribute("edge", "H", "H", 0));
    setClassAttribute(CAttribute("edge", "Q", "Q", 0));
}


void CBranchEditorScene::setBranchesMode()
{
	m_mode = EM_ADD_BRANCH;

	activateItemFactory("CBranchNode");
}

void CBranchEditorScene::setFansMode()
{
	m_mode = EM_ADD_FAN;

	activateItemFactory("CFanNode");
}

// reimp

bool CBranchEditorScene::populateMenu(QMenu& menu, QGraphicsItem* item, const QList<QGraphicsItem*>& selectedItems)
{
	if (!Super::populateMenu(menu, item, selectedItems))
		return false;

	return true;
}

void CBranchEditorScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (m_mode == EM_ADD_FAN)
	{
		if (itemAt(mouseEvent->scenePos(), QTransform()) == NULL)
		{
            auto point = mouseEvent->scenePos();

            /*CFanNode* node = */createItemOfType<CFanNode>(&point);

			addUndoState();

			return;
		}
	}

	Super::mouseDoubleClickEvent(mouseEvent);
}

void CBranchEditorScene::keyPressEvent(QKeyEvent *keyEvent)
{
	Super::keyPressEvent(keyEvent);
}


// privates

CBranchNode::CBranchNode(QGraphicsItem *parent) : Super(parent)
{
	setBrush(Qt::cyan);

	//setNodeFlag(NF_OrphanAllowed);
}

QString CBranchNode::createNewId() const
{
	static int count = 0;

	return QString("U%1").arg(++count);
}


// fans

CFanNode::CFanNode(QGraphicsItem *parent) : Super(parent)
{
	setBrush(QBrush(Qt::yellow));
	setPen(QPen(Qt::darkGreen, 5));
	
	setRect(-8, -8, 17, 17);

	setNodeFlag(NF_OrphanAllowed);
}

QString CFanNode::createNewId() const
{
	static int count = 0;

	return QString("F%1").arg(++count);
}

QList<CNode*> CFanNode::unlink()
{
	QSet<CConnection*> temp = m_connections;

	qDeleteAll(temp);	// m_connections cleared by callbacks

	return QList<CNode*>() << this;
}

void CFanNode::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Super::paint(painter, option, widget);

	painter->setPen(Qt::black);
	
	painter->save();
	
	for (int i = 0; i < 360; i += 45)
	{
		painter->drawLine(0, 0, 0, rect().height());
		painter->rotate(45);
	}
	
	painter->restore();
}


// links

CBranchConnection::CBranchConnection(QGraphicsItem *parent): Super(parent)
{
	setItemFlag(CF_End_Arrow);
}

void CBranchConnection::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Super::paint(painter, option, widget);

	QString modelType = getAttribute("MODEL").toString();
	if (modelType.isEmpty())
		return;

	if (modelType == "rrv")
	{
		painter->setPen(Qt::black);
		painter->setBrush(Qt::blue);
		painter->drawEllipse(m_controlPos, 5, 5);
		return;
	}

	if (modelType == "grrv")
	{
		painter->setPen(Qt::black);
		painter->setBrush(Qt::blue);
		painter->drawEllipse(m_controlPos, 9, 9);
		painter->setBrush(Qt::magenta);
		painter->drawEllipse(m_controlPos, 5, 5);
		return;
	}
}