#ifndef CEDITORSCENE_H
#define CEDITORSCENE_H

#include <QGraphicsScene>


class CEditorScene : public QGraphicsScene
{
    Q_OBJECT

public:
    CEditorScene(QObject *parent);

    void setGridSize(int newSize);
    int getGridSize() const
        { return m_gridSize; }

    bool gridEnabled() const
        { return m_gridEnabled; }

    bool gridSnapEnabled() const
        { return m_gridSnap; }

    void setGridPen(const QPen& gridPen);
    const QPen& getGridPen() const
        { return m_gridPen; }

public Q_SLOTS:
    void enableGrid(bool on = true);
    void enableGridSnap(bool on = true);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

protected:
    int m_gridSize;
    bool m_gridEnabled;
    bool m_gridSnap;
    QPen m_gridPen;
};

#endif // CEDITORSCENE_H
