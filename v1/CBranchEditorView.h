#ifndef CBRANCHEDITORVIEW_H
#define CBRANCHEDITORVIEW_H

#include "CEditorView.h"
#include "CDefaultInteractor.h"
#include "CBranchInteractor.h"


enum BranchEditorMode
{
    EM_SELECT,
    EM_ADD_BRANCH,
    EM_ADD_FAN
};


class CBranchEditorView : public CEditorView
{
    Q_OBJECT
public:
    CBranchEditorView(QWidget *parent);

public Q_SLOTS:
    void setSelectionMode();
	void setBranchesMode();
    void setFansMode();

private:
    BranchEditorMode m_mode;
	CDefaultInteractor m_defaultInteractor;
	CBranchInteractor m_branchInteractor;
};


#endif // CBRANCHEDITORVIEW_H
