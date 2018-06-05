#include "dpseUIController.h"
#include "simu/branch/CGraphSimulatorDialog.h"

#include <QMenu>
#include <QMenuBar>

#include <appbase/CMainWindow.h>

#include <qvge/CNodeEditorScene.h>


dpseUIController::dpseUIController(CMainWindow *parent): Super(parent)
{
    // add simulator menu
    QMenu *simuMenu = new QMenu(tr("&Simulation"));
    parent->menuBar()->insertMenu(parent->getWindowMenuAction(), simuMenu);

    QAction *simuAction = simuMenu->addAction(tr("SISD Net Simulation"), this, SLOT(doNetSimulation()));
    simuAction->setStatusTip(tr("Single process simulation of whole network"));
}


// slots

void dpseUIController::doNetSimulation()
{
    if (!scene())
        return;

    CGraphSimulatorDialog simuDialog;
    simuDialog.run(*scene());
}
