#include "CAttributesEditorUI.h"
#include "ui_CAttributesEditorUI.h"

CAttributesEditorUI::CAttributesEditorUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CAttributesEditorUI)
{
    ui->setupUi(this);
}

CAttributesEditorUI::~CAttributesEditorUI()
{
    delete ui;
}
