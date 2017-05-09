#include "CNewAttributeDialog.h"
#include "ui_CNewAttributeDialog.h"

CNewAttributeDialog::CNewAttributeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CNewAttributeDialog)
{
    ui->setupUi(this);
}

CNewAttributeDialog::~CNewAttributeDialog()
{
    delete ui;
}
