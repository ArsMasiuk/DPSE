#include "CQuickHelpUI.h"
#include "ui_CQuickHelpUI.h"


CQuickHelpUI::CQuickHelpUI(QWidget *parent): 
	QWidget(parent),
	ui(new Ui::CQuickHelpUI)
{
	ui->setupUi(this);
}


CQuickHelpUI::~CQuickHelpUI()
{
}

