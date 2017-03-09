#include "CEditorScene.h"

#include <QPainter>


CEditorScene::CEditorScene(QObject *parent): QGraphicsScene(parent)
{
    m_gridSize = 25;
    m_gridEnabled = true;
    m_gridSnap = true;
    m_gridPen = QPen(Qt::gray, 0, Qt::DotLine);

    setSceneRect(0,0, 500,500);
}

void CEditorScene::setGridSize(int newSize)
{
    if (newSize <= 0)
        return;

    m_gridSize = newSize;

    update();
}

void CEditorScene::enableGrid(bool on)
{
    m_gridEnabled = on;

    update();
}

void CEditorScene::enableGridSnap(bool on)
{
    m_gridSnap = on;
}

void CEditorScene::setGridPen(const QPen &gridPen)
{
    m_gridPen = gridPen;

    update();
}

void CEditorScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->setPen(QPen(Qt::darkGray, 2, Qt::SolidLine));
    QColor fillColor("lightsteelblue");
    fillColor.setAlpha(50);
    painter->setBrush(fillColor);
    painter->drawRect(sceneRect());

    if (m_gridSize < 0 || !m_gridEnabled)
        return;

    painter->setPen(m_gridPen);

    qreal left = int(rect.left()) - (int(rect.left()) % m_gridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % m_gridSize);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right(); x += m_gridSize)
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += m_gridSize)
        lines.append(QLineF(rect.left(), y, rect.right(), y));

    //qDebug() << lines.size();

    painter->drawLines(lines.data(), lines.size());
}
