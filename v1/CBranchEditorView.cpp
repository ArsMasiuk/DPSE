#include "CBranchEditorView.h"


CBranchEditorView::CBranchEditorView(QWidget *parent): CEditorView(parent)
{
    setSelectionMode();
}

void CBranchEditorView::setSelectionMode()
{
	setInteractor(&m_defaultInteractor);

    m_mode = EM_SELECT;
}

void CBranchEditorView::setBranchesMode()
{
	setInteractor(&m_branchInteractor);

	m_mode = EM_ADD_BRANCH;
}

void CBranchEditorView::setFansMode()
{
    m_mode = EM_ADD_FAN;

    //doCleanup();

    //setDragMode(QGraphicsView::NoDrag);
}
