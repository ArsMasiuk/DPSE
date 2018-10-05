#include "dpseUIController.h"

#include <simu/branch/CGraphSimulatorDialog.h>
#include <simu/branch/CGraphSimulator.h>

#include <simu/ISimulator.h>

#include <QMenu>
#include <QMenuBar>

#include <appbase/CMainWindow.h>

#include <commonui/CSceneMenuUIController.h>

#include <qvge/CNode.h>
#include <qvge/CNodeEditorScene.h>


dpseUIController::dpseUIController(CMainWindow *parent): Super(parent)
{
    // add simulator menu
    QMenu *simuMenu = new QMenu(tr("&Simulation"));
    parent->menuBar()->insertMenu(parent->getWindowMenuAction(), simuMenu);

    QAction *simuAction = simuMenu->addAction(tr("SISD Net Simulation"), this, SLOT(doNetSimulation()));
    simuAction->setStatusTip(tr("Single process simulation of whole network"));

	// connect popup menu
	auto *menuController = dynamic_cast<CSceneMenuUIController*>(scene()->getContextMenuController());
	connect(menuController, &CSceneMenuUIController::onContextMenu, this, &dpseUIController::doContextMenu);
}


// slots

void dpseUIController::doNetSimulation()
{
    if (!scene())
        return;

    // that might be moved away
    CGraphSimulator simu;

    CGraphSimulatorDialog simuDialog;
    simuDialog.setSimulator(simu);

    simuDialog.run(*scene());
}


void dpseUIController::doContextMenu(QMenu &menu)
{
	int nodesCount = scene()->getSelectedNodes().size();
	if (nodesCount > 0)
	{
		menu.addSeparator();
		QMenu* modelMenu = menu.addMenu(tr("Node(s) Model"));
		modelMenu->addAction(tr("Simple Node"), this, SLOT(setSimpleNodeModel()));
		modelMenu->addAction(tr("Athmosphere"), this, SLOT(setAtmNodeModel()));
		modelMenu->addAction(tr("Main Airing"), this, SLOT(setMainFanNodeModel()));
		modelMenu->addAction(tr("Local Airing"), this, SLOT(setLocalFanNodeModel()));
	}
}


void dpseUIController::setSimpleNodeModel()
{
	auto nodes = scene()->getSelectedNodes();

	for (auto* node : nodes)
	{
		node->removeAttribute("model");
		node->removeAttribute("shape");
		node->removeAttribute("P");
		node->removeAttribute("color");
		node->removeAttribute("stroke.color");
	}

	scene()->addUndoState();
}


void dpseUIController::setAtmNodeModel()
{
	auto nodes = scene()->getSelectedNodes();

	for (auto* node : nodes)
	{
		node->setAttribute("model", "ATM");
		node->setAttribute("P", 0);
		node->setAttribute("shape", "triangle");
		node->setAttribute("color", QColor(Qt::cyan));
		node->setAttribute("stroke.color", QColor(Qt::darkCyan));
	}

	scene()->addUndoState();
}


void dpseUIController::setMainFanNodeModel()
{
	auto nodes = scene()->getSelectedNodes();

	for (auto* node : nodes)
	{
		node->setAttribute("model", "VGP");
		node->setAttribute("P", 1000);
		node->setAttribute("shape", "hexagon");
		node->setAttribute("color", QColor(Qt::magenta));
		node->setAttribute("stroke.color", QColor(Qt::darkMagenta));
	}

	scene()->addUndoState();
}


void dpseUIController::setLocalFanNodeModel()
{
	auto nodes = scene()->getSelectedNodes();

	for (auto* node : nodes)
	{
		node->setAttribute("model", "VMP");
		node->setAttribute("P", 100);
		node->setAttribute("shape", "diamond");
		node->setAttribute("color", QColor(Qt::green));
		node->setAttribute("stroke.color", QColor(Qt::darkGreen));
	}

	scene()->addUndoState();
}


