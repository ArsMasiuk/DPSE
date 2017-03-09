#include "CBranchInteractor.h"
#include "CNode.h"
#include "CConnection.h"
#include "CEditorView.h"
#include "CEditorScene.h"

#include <QMouseEvent>


CBranchInteractor::CBranchInteractor()
:	m_startRealNode(false),
	m_startNode(NULL),
	m_endNode(NULL),
	m_connection(NULL) 
{
}

CBranchInteractor::~CBranchInteractor()
{
}


// callbacks

void CBranchInteractor::onStarted(CEditorView * view, CEditorScene * scene)
{
	Super::onStarted(view, scene);

	doCleanup(scene);
}

void CBranchInteractor::onFinished(CEditorView * view, CEditorScene * scene)
{
	doCleanup(scene);

	Super::onFinished(view, scene);
}

bool CBranchInteractor::onMousePressed(CEditorView * view, CEditorScene * scene, QMouseEvent * event)
{
	doCleanup(scene);

	if (!Super::onMousePressed(view, scene, event))
		return false;

	QPoint viewPos = event->pos();
	QPoint scenePos = view->mapToScene(viewPos).toPoint();

	if (m_dragNode)   // clicked on a real item
	{
		m_startNode = m_dragNode;
		m_startRealNode = true;
	}
	else    // clicked on empty scene
	{
		m_startRealNode = false;
		m_startNode = new CRoundNode();
		m_startNode->moveTo(scenePos);

		scene->addItem(dynamic_cast<QGraphicsItem*>(m_startNode));
	}

	m_endNode = new CRoundNode();
	m_endNode->moveTo(scenePos);

	m_connection = new CConnection();
	m_connection->setFirstNode(m_startNode);
	m_connection->setLastNode(m_endNode);

	scene->addItem(dynamic_cast<QGraphicsItem*>(m_endNode));
	scene->addItem(dynamic_cast<QGraphicsItem*>(m_connection));

	return false;
}

bool CBranchInteractor::onMouseMoved(CEditorView * view, CEditorScene * scene, QMouseEvent * event)
{
	Super::onMouseMoved(view, scene, event);

	QPoint viewPos = event->pos();
	QPoint scenePos = view->mapToScene(viewPos).toPoint();

	if (m_endNode)
	{
		m_endNode->moveTo(scenePos);
	}

	return false;
}

bool CBranchInteractor::onMouseReleased(CEditorView * view, CEditorScene * scene, QMouseEvent * event)
{
	Super::onMouseReleased(view, scene, event);

	QPoint viewPos = event->pos();
	QPoint scenePos = view->mapToScene(viewPos).toPoint();

	if (m_endNode)
	{
		QList<QGraphicsItem*> itemsList = view->items(viewPos);

		if (itemsList.contains(dynamic_cast<QGraphicsItem*>(m_startNode)))
		{
			// clicked on the start - bail out
			doCleanup(scene);
			return false;
		}

		itemsList.removeAll(dynamic_cast<QGraphicsItem*>(m_endNode));
		itemsList.removeAll(dynamic_cast<QGraphicsItem*>(m_connection));

		if (itemsList.isEmpty())    // empty field: finished adding branch
		{
			m_startNode = m_endNode = NULL;
			m_connection = NULL;
			m_startRealNode = false;
			return false;
		}

		foreach(QGraphicsItem* item, itemsList)
		{
			CNode *node = dynamic_cast<CNode*>(item);
			if (!node)
				continue;

			// merge end with the node and exit
			m_connection->setLastNode(node);

			scene->removeItem(dynamic_cast<QGraphicsItem*>(m_endNode));
			delete m_endNode;

			m_startNode = m_endNode = NULL;
			m_connection = NULL;
			m_startRealNode = false;
			return false;
		}

		// some crap hovered: cleanup & exit
		doCleanup(scene);
	}

	return false;
}


void CBranchInteractor::doCleanup(CEditorScene *scene)
{
	if (!m_startRealNode && m_startNode)
	{
		scene->removeItem(dynamic_cast<QGraphicsItem*>(m_startNode));
		delete m_startNode;
	}

	if (m_endNode)
	{
		scene->removeItem(dynamic_cast<QGraphicsItem*>(m_endNode));
		delete m_endNode;
	}

	if (m_connection)
	{
		scene->removeItem(dynamic_cast<QGraphicsItem*>(m_connection));
		delete m_connection;
	}

	m_startRealNode = false;
	m_startNode = NULL;
	m_endNode = NULL;
	m_connection = NULL;
}

