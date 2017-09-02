#include "CSceneOptionsDialog.h"
#include "ui_CSceneOptionsDialog.h"

CSceneOptionsDialog::CSceneOptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSceneOptionsDialog)
{
    ui->setupUi(this);

	ui->BackgroundColor->setScheme(QSint::openOfficeColors());
}

CSceneOptionsDialog::~CSceneOptionsDialog()
{
    delete ui;
}


int CSceneOptionsDialog::exec(CEditorScene &scene)
{
	ui->BackgroundColor->setColor(scene.backgroundBrush().color());

	if (QDialog::exec() == QDialog::Rejected)
		return QDialog::Rejected;

	scene.setBackgroundBrush(ui->BackgroundColor->color());

	return QDialog::Accepted;
}
