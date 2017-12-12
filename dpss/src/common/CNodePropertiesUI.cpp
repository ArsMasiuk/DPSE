#include "CNodePropertiesUI.h"
#include "ui_CNodePropertiesUI.h"

CNodePropertiesUI::CNodePropertiesUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CNodePropertiesUI)
{
    ui->setupUi(this);
}

CNodePropertiesUI::~CNodePropertiesUI()
{
    delete ui;
}
