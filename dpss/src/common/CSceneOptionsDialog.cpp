#include "CSceneOptionsDialog.h"
#include "ui_CSceneOptionsDialog.h"

CSceneOptionsDialog::CSceneOptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSceneOptionsDialog)
{
    ui->setupUi(this);

    ui->BackgroundColor->setColorScheme(QSint::namedColorsOpenOffice());
    ui->GridColor->setColorScheme(QSint::namedColorsOpenOffice());
}

CSceneOptionsDialog::~CSceneOptionsDialog()
{
    delete ui;
}


int CSceneOptionsDialog::exec(CEditorScene &scene)
{
	ui->BackgroundColor->setColor(scene.backgroundBrush().color());

	QPen gridPen = scene.getGridPen();
	ui->GridColor->setColor(gridPen.color());

	ui->GridSize->setValue(scene.getGridSize());
	ui->GridVisible->setChecked(scene.gridEnabled());
	ui->GridSnap->setChecked(scene.gridSnapEnabled());


	if (QDialog::exec() == QDialog::Rejected)
		return QDialog::Rejected;


	scene.setBackgroundBrush(ui->BackgroundColor->color());

	gridPen.setColor(ui->GridColor->color());
	scene.setGridPen(gridPen);

	scene.setGridSize(ui->GridSize->value());
	scene.enableGrid(ui->GridVisible->isChecked());
	scene.enableGridSnap(ui->GridSnap->isChecked());

	return QDialog::Accepted;
}
