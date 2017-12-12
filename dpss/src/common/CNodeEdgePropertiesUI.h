#ifndef CNODEPROPERTIESUI_H
#define CNODEPROPERTIESUI_H

#include <QWidget>

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

private:
    Ui::CNodeEdgePropertiesUI *ui;

    CNodeEditorScene *m_scene;
};

#endif // CNODEPROPERTIESUI_H
