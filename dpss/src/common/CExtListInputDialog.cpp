#include "CExtListInputDialog.h"
#include "ui_CExtListInputDialog.h"

CExtListInputDialog::CExtListInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CExtListInputDialog)
{
    ui->setupUi(this);
}

CExtListInputDialog::~CExtListInputDialog()
{
    delete ui;
}

int CExtListInputDialog::getItemIndex(const QStringList &texts, const QList<QIcon> &icons, int selectedIndex)
{
    static CExtListInputDialog dialog(NULL);

    dialog.ui->comboBox->clear();
    dialog.ui->comboBox->addItems(texts);
    while (dialog.ui->comboBox->count() < icons.size())
        dialog.ui->comboBox->addItem("");

    for (int i = 0; i < icons.size(); i++)
    {
        dialog.ui->comboBox->setItemIcon(i, icons.at(i));
    }

    dialog.ui->comboBox->setCurrentIndex(selectedIndex);

    if (dialog.exec() == QDialog::Rejected)
        return -1;

    return dialog.ui->comboBox->currentIndex();
}
