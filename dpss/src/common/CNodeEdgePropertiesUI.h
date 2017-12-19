#ifndef CNODEPROPERTIESUI_H
#define CNODEPROPERTIESUI_H

#include <QWidget>
#include <QVariant>

class CEditorScene;
class CNodeEditorScene;


namespace Ui {
class CNodeEdgePropertiesUI;
}

class CNodeEdgePropertiesUI : public QWidget
{
    Q_OBJECT

public:
    explicit CNodeEdgePropertiesUI(QWidget *parent = 0);
    ~CNodeEdgePropertiesUI();

    void setScene(CNodeEditorScene* scene);

protected:
    void connectSignals(CEditorScene* scene);
    void onSceneAttached(CEditorScene* scene);
    void onSceneDetached(CEditorScene* scene);

protected Q_SLOTS:
    void onSceneChanged();
    void onSelectionChanged();

    void on_NodeColor_activated(const QColor &color);
    void on_NodeShape_activated(QVariant data);
    void on_NodeSize_valueChanged(int value);

    void on_EdgeColor_activated(const QColor &color);
    void on_EdgeWeight_valueChanged(double value);
    void on_EdgeStyle_activated(QVariant data);
    void on_EdgeLabelFont_activated(const QFont &font);

private:
    CNodeEditorScene *m_scene;
    bool m_updateLock;

    Ui::CNodeEdgePropertiesUI *ui;
};

#endif // CNODEPROPERTIESUI_H
