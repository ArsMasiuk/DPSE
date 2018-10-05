#ifndef DPSEUICONTROLLER_H
#define DPSEUICONTROLLER_H

#include <QWidget>

#include <commonui/CNodeEditorUIController.h>

class CNodeEditorScene;


class dpseUIController : public CNodeEditorUIController
{
    Q_OBJECT

public:
	friend class dpseMainWindow;

    typedef CNodeEditorUIController Super;

    dpseUIController(CMainWindow *parent);

private Q_SLOTS:
    void doNetSimulation();

	void doContextMenu(QMenu &menu);
	void setSimpleNodeModel();
	void setAtmNodeModel();
	void setMainFanNodeModel();
	void setLocalFanNodeModel();
};


#endif // DPSEUICONTROLLER_H
