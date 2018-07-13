#include "CNodePortEditorDialog.h"
#include "ui_CNodePortEditorDialog.h"

#include <qvge/CNode.h>
#include <qvge/CNodePort.h>


CNodePortEditorDialog::CNodePortEditorDialog(): ui(new Ui::CNodePortEditorDialog)
{
	ui->setupUi(this);

	ui->Anchor->addItem(tr("Left"), Qt::AlignLeft + Qt::AlignVCenter);
	ui->Anchor->addItem(tr("Top"), Qt::AlignTop + Qt::AlignHCenter);
	ui->Anchor->addItem(tr("Right"), Qt::AlignRight + Qt::AlignVCenter);
	ui->Anchor->addItem(tr("Bottom"), Qt::AlignBottom + Qt::AlignHCenter);
	ui->Anchor->addItem(tr("Top-Left"), Qt::AlignTop + Qt::AlignLeft);
	ui->Anchor->addItem(tr("Top-Right"), Qt::AlignTop + Qt::AlignRight);
	ui->Anchor->addItem(tr("Bottom-Left"), Qt::AlignBottom + Qt::AlignLeft);
	ui->Anchor->addItem(tr("Bottom-Right"), Qt::AlignBottom + Qt::AlignRight);
	ui->Anchor->addItem(tr("Center"), Qt::AlignCenter);
}


CNodePortEditorDialog::~CNodePortEditorDialog()
{
}


int CNodePortEditorDialog::exec(CNodePort &port)
{
	m_port = &port;

	ui->PortId->setText(port.getId());

	int align = port.getAlign();
	int alignIndex = ui->Anchor->findData(align);
	if (alignIndex < 0) alignIndex = 0;
	ui->Anchor->setCurrentIndex(alignIndex);

	ui->OffsetX->setValue(port.getX());
	ui->OffsetY->setValue(port.getY());

	return QDialog::exec();
}


void CNodePortEditorDialog::doUpdate()
{
	if (m_port)
	{
		if (CNode *node = dynamic_cast<CNode*>(m_port->parentItem()))
		{
			int align = ui->Anchor->currentData().toInt();
			int xv = ui->OffsetX->value();
			int yv = ui->OffsetY->value();
			node->movePort(m_port->getId(), align, xv, yv);
		}
	}
}


void CNodePortEditorDialog::on_Anchor_currentIndexChanged(int)
{
	doUpdate();
}


void CNodePortEditorDialog::on_OffsetX_valueChanged(int)
{
	doUpdate();
}


void CNodePortEditorDialog::on_OffsetY_valueChanged(int)
{
	doUpdate();
}

