#include "CNodePropertiesUI.h"
#include "ui_CNodePropertiesUI.h"

CNodePropertiesUI::CNodePropertiesUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CNodePropertiesUI)
{
    ui->setupUi(this);

    //ui->ColorButton->setStandardColors();
    //ui->ColorButton->setScheme(QSint::namedColors());
	//ui->ColorButton->setPopupMode(QToolButton::DelayedPopup);
	//ui->ColorButton->setColorScheme(QSint::namedColorsOpenOffice());
	ui->ColorButton->setColorScheme(QSint::namedColorsSvg());
	ui->ColorButton->setColor(QColor());

    ui->ShapeCombo->addItem(QIcon(":/Icons/Node-Disc"), tr("Disc"));
    ui->ShapeCombo->addItem(QIcon(":/Icons/Node-Square"), tr("Square"));
    ui->ShapeCombo->addItem(QIcon(":/Icons/Node-Triangle"), tr("Triangle Up"));
    ui->ShapeCombo->addItem(QIcon(":/Icons/Node-Diamond"), tr("Diamond"));
    ui->ShapeCombo->addItem(QIcon(":/Icons/Node-Triangle-Down"), tr("Triangle Down"));
}

CNodePropertiesUI::~CNodePropertiesUI()
{
    delete ui;
}
