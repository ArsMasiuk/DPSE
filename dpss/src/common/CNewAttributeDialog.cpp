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

QByteArray CNewAttributeDialog::getAttrId() const
{
    return ui->AttrID->text().toLatin1();
}

QString CNewAttributeDialog::getAttrName() const
{
    return ui->AttrName->text();
}

QVariant CNewAttributeDialog::getDefaultValue() const
{
    if (!ui->ValueGB->isChecked())
        return QVariant();

    return ui->AttrValue->text();
}
