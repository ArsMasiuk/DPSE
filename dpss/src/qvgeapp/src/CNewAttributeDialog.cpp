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

QString CNewAttributeDialog::getId() const
{
    return ui->Id->text();
}

int CNewAttributeDialog::getType() const
{
    switch (ui->Type->currentIndex())
    {
    case 0:     return QVariant::Int;
    case 1:     return QVariant::Double;
    case 2:     return QVariant::Bool;
    default:    return QVariant::String;
    }
}

QVariant CNewAttributeDialog::getValue() const
{
    switch (ui->Type->currentIndex())
    {
    case 0:     return int(0);
    case 1:     return double(0.0);
    case 2:     return bool(true);
    default:    return QString();
    }
}
