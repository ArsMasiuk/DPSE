#include "CSizeInputDialog.h"
#include "ui_CSizeInputDialog.h"

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
