#ifndef CNODE_H
#define CNODE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QApplication>
#include <QPen>
#include <QSet>

#include "CEditorScene.h"


class CConnection;


class CNode
{
public:
	virtual ~CNode();

    void setNodeId(const QString& id)
    {
        m_id = id;
    }

    virtual QPointF getPos() const = 0;
    virtual void moveTo(const QPointF &pos) = 0;

    // callbacks
	virtual void onConnectionAttach(CConnection *conn);
	virtual void onConnectionDetach(CConnection *conn);

	virtual bool onMousePressed();	// return true if dragging allowed

protected:
    void onPositionChanged();
	void onMouseOver(QGraphicsSceneHoverEvent *event, QGraphicsItem *sourceItem);

protected:
    QString m_id;

    QSet<CConnection*> m_connections;
};


template<class ShapeItem>
class TNode : public ShapeItem, public CNode
{
public:
    TNode(QGraphicsItem *parent = Q_NULLPTR): ShapeItem(parent)
    {
        setZValue(0);

        setPen(QPen(Qt::blue));
        setBrush(Qt::yellow);

        setFlags(QGraphicsItem::ItemIsMovable |
                 QGraphicsItem::ItemIsSelectable |
                 QGraphicsItem::ItemSendsGeometryChanges);

    }

    virtual QPointF getPos() const
    {
        return ShapeItem::pos();
    }

    virtual void moveTo(const QPointF &pos)
    {
        ShapeItem::setPos(pos);
    }

protected:
    virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
    {
        if (change == QGraphicsItem::ItemPositionChange && QApplication::mouseButtons() == Qt::LeftButton)
        {
            CEditorScene* customScene = dynamic_cast<CEditorScene*>(scene());

            if (customScene)
            {
                if (customScene->gridEnabled() && customScene->gridSnapEnabled())
                {
                    int gridSize = customScene->getGridSize();
                    QPointF newPos = value.toPointF();

                    qreal xV = round(newPos.x()/gridSize)*gridSize;
                    qreal yV = round(newPos.y()/gridSize)*gridSize;

                    return QPointF(xV, yV);
                }
            }
        }
        else
        if (change == QGraphicsItem::ItemPositionHasChanged)
        {
            onPositionChanged();
        }

        // default call
        return ShapeItem::itemChange(change, value);
    }
};


class CRoundNode : public TNode<QGraphicsEllipseItem>
{
public:
    CRoundNode(QGraphicsItem *parent = Q_NULLPTR);

	virtual bool onMousePressed()
	{
		return true;
	}
};


class CRectNode : public TNode<QGraphicsRectItem>
{
public:
    CRectNode(QGraphicsItem *parent = Q_NULLPTR);

    using QGraphicsRectItem::boundingRect;
};


#endif // CNODE_H

