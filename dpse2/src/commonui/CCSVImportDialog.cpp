#include "CCSVImportDialog.h"
#include "ui_CCSVImportDialog.h"

#include <QFile>
#include <QTextStream>


CCSVImportDialog::CCSVImportDialog(QWidget *parent): 
	QDialog(parent),
	ui(new Ui::CCSVImportDialog)
{
	ui->setupUi(this);
}


CCSVImportDialog::~CCSVImportDialog()
{
}


void CCSVImportDialog::setFileName(const QString & fileName)
{
	m_fileName = fileName;
}


int CCSVImportDialog::exec()
{
	if (!QFile::exists(m_fileName))
	{
		return withError(tr("%1 does not exist").arg(m_fileName));
	}

	QFile f(m_fileName);
	if (!f.open(QFile::ReadOnly))
	{
		return withError(tr("%1 cannot be read").arg(m_fileName));
	}

	QTextStream ts(&f);
	while (!ts.atEnd())
		m_lines << ts.readLine();

	preview();

	return QDialog::exec();
}


void CCSVImportDialog::preview()
{
	ui->PreviewTable->setUpdatesEnabled(false);
	ui->PreviewTable->clear();



	ui->PreviewTable->setUpdatesEnabled(true);
}


void CCSVImportDialog::on_CustomDelim_toggled(bool on)
{
	ui->CustomDelimEdit->setEnabled(on);

	if (on)
		ui->CustomDelimEdit->setFocus();
}


int CCSVImportDialog::withError(const QString &text)
{
	m_lastErrorText = text;
	return Rejected;
}

