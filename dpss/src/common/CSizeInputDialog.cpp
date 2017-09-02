#include "CSizeInputDialog.h"
#include "ui_CSizeInputDialog.h"

#include <limits>


CSizeInputDialog::CSizeInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSizeInputDialog)
{
    ui->setupUi(this);
}

CSizeInputDialog::~CSizeInputDialog()
{
    delete ui;
}


QSize CSizeInputDialog::getSize(const QString &title, const QString &label,
                                const QSize &defaultSize, const QSize &minSize, const QSize &maxSize,
                                const QString &unit)
{
	QSizeF szf = getSizeF(title, label, QSizeF(defaultSize), QSizeF(minSize), QSizeF(maxSize), unit, 0);
	if (szf.isValid())
		return szf.toSize();
	else
		return QSize();
}


QSizeF CSizeInputDialog::getSizeF(const QString & title, const QString & label, 
	const QSizeF & defaultSize, const QSizeF & minSize, const QSizeF & maxSize, 
	const QString & unit, int decimals)
{
	static CSizeInputDialog* dialog = new CSizeInputDialog(NULL);

	dialog->setWindowTitle(title);
	dialog->ui->label->setText(label);

	dialog->ui->unit->setText(unit);
	dialog->ui->unit->setVisible(unit.length());

	if (minSize.isValid()) {
		dialog->ui->widthBox->setMinimum(minSize.width());
		dialog->ui->heightBox->setMinimum(minSize.height());
	}
	else {
		dialog->ui->widthBox->setMinimum(0);
		dialog->ui->heightBox->setMinimum(0);
	}

	if (maxSize.isValid()) {
		dialog->ui->widthBox->setMaximum(maxSize.width());
		dialog->ui->heightBox->setMaximum(maxSize.height());
	}
	else {
        dialog->ui->widthBox->setMaximum(std::numeric_limits<double>::max());
        dialog->ui->heightBox->setMaximum(std::numeric_limits<double>::max());
	}

	if (defaultSize.isValid()) {
		dialog->ui->widthBox->setValue(defaultSize.width());
		dialog->ui->heightBox->setValue(defaultSize.height());
	}

	dialog->ui->widthBox->setDecimals(decimals);
	dialog->ui->heightBox->setDecimals(decimals);

	dialog->ui->widthBox->selectAll();
	dialog->ui->widthBox->setFocus();

	dialog->ui->cbHeightAsWidth->setChecked(defaultSize.width() == defaultSize.height());

	if (dialog->exec() == QDialog::Rejected)
		return QSizeF();

	if (dialog->ui->cbHeightAsWidth->isChecked())
		return QSizeF(dialog->ui->widthBox->value(), dialog->ui->widthBox->value());

	return QSizeF(dialog->ui->widthBox->value(), dialog->ui->heightBox->value());
}


// slots

void CSizeInputDialog::on_cbHeightAsWidth_toggled(bool on)
{
	ui->heightBox->setDisabled(on);

	ui->widthBox->setFocus();
}
