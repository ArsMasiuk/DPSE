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

int CExtListInputDialog::getItemIndex(const QString& title, const QString& label,
                                      const QStringList &texts, const QList<QIcon> &icons, int selectedIndex)
{
    static CExtListInputDialog* dialog = new CExtListInputDialog(NULL);

    dialog->setWindowTitle(title);
    dialog->ui->label->setText(label);

    dialog->ui->comboBox->clear();
    dialog->ui->comboBox->addItems(texts);
    while (dialog->ui->comboBox->count() < icons.size())
        dialog->ui->comboBox->addItem("");

    for (int i = 0; i < icons.size(); i++)
    {
        dialog->ui->comboBox->setItemIcon(i, icons.at(i));
    }

	dialog->ui->comboBox->setIconSize(QSize(24, 24));
    dialog->ui->comboBox->setCurrentIndex(selectedIndex);

    if (dialog->exec() == QDialog::Rejected)
        return -1;

    return dialog->ui->comboBox->currentIndex();
}
