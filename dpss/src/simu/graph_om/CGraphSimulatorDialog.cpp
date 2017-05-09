/*
This file is a part of
DPSE - Distributed Parallel Simulation Environment

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "CGraphSimulatorDialog.h"
#include "ui_CGraphSimulatorDialog.h"

#include <QFileDialog>
#include <QDebug>

#include "graph.h"


CGraphSimulatorDialog::CGraphSimulatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CGraphSimulatorDialog)
{
    ui->setupUi(this);

	m_ChartView = new QChartView(ui->ChartBox);
	ui->ChartBox->layout()->addWidget(m_ChartView);
	m_ChartView->setChart(&m_Chart);

    m_simu.setLogger(this);

	connect(&m_simu, SIGNAL(stepFinished(double, int, std::vector<double>&)),
		this, SLOT(onStepFinished(double, int, std::vector<double>&)));

	connect(&m_simu, SIGNAL(simulationFinished()), this, SLOT(onSimulationFinished()));
}

CGraphSimulatorDialog::~CGraphSimulatorDialog()
{
    delete ui;
}


// reimp

bool CGraphSimulatorDialog::run(const CBranchEditorScene& scene)
{
    m_simu.setScene(scene);

    //ui->NetInfo->setText(m_simu.getNetInfo());

    //QSettings& set = m_simu.getSettings();
    ////qDebug() << set.fileName();
    //set.sync();
    //ui->lePythonPath->setText(set.value("Python/Path").toString());

    ui->Tabs->setCurrentIndex(0);

    exec();

    return true;
}


// log

void CGraphSimulatorDialog::write(const QString& text, int state, const QDateTime &timestamp)
{
    switch (state)
    {
    case LOG_OK:
        ui->Console->append(QString("<font color=green><b>[%1]</b> %2</font>").arg(timestamp.toString()).arg(text));
        break;

    case LOG_WARN:
        ui->Console->append(QString("<font color=orange><b>[%1]</b> %2</font>").arg(timestamp.toString()).arg(text));
        break;

    case LOG_ERROR:
        ui->Console->append(QString("<font color=red><b>[%1]</b> %2</font>").arg(timestamp.toString()).arg(text));
        break;

    default:
        ui->Console->append(QString("<b>[%1]</b> %2").arg(timestamp.toString()).arg(text));
        break;
    }
}


// slots

void CGraphSimulatorDialog::on_Start_clicked()
{
	int simTime = ui->SimuTime->value();
	write(tr("Start simulation (%1 s)").arg(simTime), LOG_OK);

	m_simu.setSimulationTime(simTime);

	// temp
	ui->Tabs->setTabEnabled(0, false);
	ui->Tabs->setTabEnabled(1, false); 
	ui->Tabs->setCurrentIndex(2);

	ui->SimuTimeLabel->setText("0");
	ui->SimuStepLabel->setText("0");
	ui->ProgressBar->setValue(0);
	ui->ProgressBar->setMaximum(m_simu.getMaxSteps());

	m_testPoints.clear();

    m_simu.run();
}


void CGraphSimulatorDialog::on_Stop_clicked()
{
	m_simu.stop();
}

void CGraphSimulatorDialog::on_StepTable_itemSelectionChanged()
{
	// chart 
	m_Chart.removeAllSeries();


	auto selectedItems = ui->StepTable->selectedItems();
	for (auto item : selectedItems)
	{
		int index = item->data(Qt::UserRole).toInt();
		QLineSeries *ts = new QLineSeries();
		ts->append(m_testPoints[index]);
		m_Chart.addSeries(ts);
	}
}


void CGraphSimulatorDialog::onStepFinished(double time, int step, std::vector<double>& qvec)
{
	if (step % 100 == 0)
	{
		ui->SimuTimeLabel->setText(QString("%1 s").arg(time));
		ui->SimuStepLabel->setNum(step);

		ui->ProgressBar->setValue(step);

		// table
		ui->StepTable->setUpdatesEnabled(false);
		ui->StepTable->setRowCount(1);

		int oldRows = ui->StepTable->columnCount();
		ui->StepTable->setColumnCount(qvec.size());
		for (int r = oldRows; r < qvec.size(); ++r) 
		{
			ui->StepTable->setItem(0, r, new QTableWidgetItem());
			ui->StepTable->item(0, r)->setData(Qt::UserRole, r);
		}

		for (int r = 0; r < qvec.size(); ++r)
			ui->StepTable->item(0, r)->setText(QString::number(qvec[r]));
		
		ui->StepTable->setUpdatesEnabled(true);

		// chart update
		for (int r = 0; r < qvec.size(); ++r)
		{
			m_testPoints[r] << QPointF(step, qvec[r]);
		}

		on_StepTable_itemSelectionChanged();

		qApp->processEvents();
	}

}


void CGraphSimulatorDialog::onSimulationFinished()
{
	ui->Tabs->setTabEnabled(0, true);
	ui->Tabs->setTabEnabled(1, true);
	ui->Tabs->setCurrentIndex(2);

	write(tr("Simulation finished"), LOG_OK);
}


// events

void CGraphSimulatorDialog::closeEvent(QCloseEvent *event)
{
	m_simu.stop();

	QDialog::closeEvent(event);
}
