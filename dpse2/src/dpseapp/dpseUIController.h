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

private:
};


#endif // DPSEUICONTROLLER_H
