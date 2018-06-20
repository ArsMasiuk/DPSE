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


CGraphSimulatorDialog::CGraphSimulatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CGraphSimulatorDialog)
{
    ui->setupUi(this);

	setWindowModality(Qt::WindowModal);

	m_Chart.setAxisX(m_AxisX = new QValueAxis(this));
	m_Chart.setAxisY(m_AxisY = new QValueAxis(this));

	m_ChartView = new QChartView(ui->ChartBox);
	ui->ChartBox->layout()->addWidget(m_ChartView);
	m_ChartView->setChart(&m_Chart);

    m_simuScene = new CNodeEditorScene(this);
	ui->SceneView->setScene(m_simuScene);
	ui->SceneView->setInteractive(false);
}


CGraphSimulatorDialog::~CGraphSimulatorDialog()
{
    delete ui;
}


// reimp

void CGraphSimulatorDialog::setSimulator(CSimulatorBase &simu)
{
    m_simu = &simu;

    m_simu->setLogger(*this);

	connect(m_simu, SIGNAL(prepareOutput(const QStringList&, const QStringList&)),
		this, SLOT(onPrepareOutput(const QStringList&, const QStringList&)));

    connect(m_simu, SIGNAL(stepFinished(double, int, std::vector<double>&)),
    	this, SLOT(onStepFinished(double, int, std::vector<double>&)));

    connect(m_simu, SIGNAL(simulationFinished()), this, SLOT(onSimulationFinished()));
}


bool CGraphSimulatorDialog::run(const CNodeEditorScene& scene)
{
    if (!m_simu || !m_simuScene)
        return false;

	m_simuScene->reset();

	// to do: copy scenes
	QByteArray b;
	QDataStream ds(&b, QIODevice::ReadWrite);
	scene.storeTo(ds, true);
	QDataStream ds2(&b, QIODevice::ReadWrite);
	m_simuScene->restoreFrom(ds2, true);

	m_simuScene->setClassAttributeVisible("edge", "Q");

    // set to Simu
    m_graph.setScene(*m_simuScene);
    m_simu->setGraph(m_graph);


	// table
	ui->StepTable->clear();
	ui->StepTable->setRowCount(1);

    //ui->NetInfo->setText(m_simu.getNetInfo());

	showMaximized();
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
	ui->SimuParams->setEnabled(false);

    ui->SimuTimeLabel->setText("0");
    ui->SimuStepLabel->setText("0");
    ui->ProgressBar->setValue(0);

	m_Chart.removeAllSeries();


    write(tr("Start topology check..."), LOG_OK);
    bool isChecked = m_simu->analyse();
    if (!isChecked)
    {
        write(tr("Topology check failed. Cannot run the simulation."), LOG_ERROR);

		ui->SimuParams->setEnabled(true);
		onSimulationFinished();
		return;
    }


    int simTime = ui->SimuTime->value();
	write(tr("Start simulation (%1 s)").arg(simTime), LOG_OK);

	ui->ProgressBar->setMaximum(simTime * 1000);

	SimuParams params = { simTime, (float) ui->DeltaX->value(), 0 };
	if (!m_simu->run(params))
	{
		write(tr("Simulation failed due to parameter errors. Please fix them and try again."), LOG_ERROR);

		ui->SimuParams->setEnabled(true);
		onSimulationFinished();
		return;
	}

	if (ui->StepTable->columnCount() > 0 && ui->StepTable->rowCount() > 0)
		ui->StepTable->setItemSelected(ui->StepTable->item(0, 0), true);
}


void CGraphSimulatorDialog::on_Stop_clicked()
{
    m_simu->stop();
}


void CGraphSimulatorDialog::on_StepTable_itemSelectionChanged()
{
	auto selectedItems = ui->StepTable->selectedItems();
	
	// show all
	if (selectedItems.isEmpty() || selectedItems.count() == m_series.count())
	{
		for (int i = 0; i < m_series.count(); ++i)
		{
			auto& ts = m_series[i].series;
			m_Chart.addSeries(ts);
			ts->attachAxis(m_AxisX);
			ts->attachAxis(m_AxisY);
		}

		m_AxisY->setRange(m_totalMin, m_totalMax);

		return;
	}

	// remove all at first
	for (int i = 0; i < m_series.count(); ++i)
	{
		m_Chart.removeSeries(m_series[i].series);
	}

	// add only selected
	for (auto& item : selectedItems)
	{
		int index = item->data(Qt::UserRole).toInt();
		auto& ts = m_series[index].series;
		m_Chart.addSeries(ts);
		ts->attachAxis(m_AxisX);
		ts->attachAxis(m_AxisY);
	}

	if (selectedItems.count() > 1)
	{
		m_AxisY->setRange(m_totalMin, m_totalMax);
	}
	else
	{
		int index = selectedItems.first()->data(Qt::UserRole).toInt();
		m_AxisY->setRange(m_series[index].yMin, m_series[index].yMax);
	}
}


void CGraphSimulatorDialog::onPrepareOutput(const QStringList& branchIds, const QStringList& paramIds)
{
	m_branchIds = branchIds;

	ui->StepTable->setColumnCount(branchIds.size());

	auto it = branchIds.constBegin();
	for (int r = 0; r < branchIds.size(); ++r, ++it)
	{
		ui->StepTable->setItem(0, r, new QTableWidgetItem());
		ui->StepTable->item(0, r)->setData(Qt::UserRole, r);

		ui->StepTable->setHorizontalHeaderItem(r, new QTableWidgetItem(*it));
	}

	// temp
	ui->StepTable->setVerticalHeaderItem(0, new QTableWidgetItem(paramIds.first()));


	// charts
	m_Chart.removeAllSeries();	// kills all of them
	m_series.resize(branchIds.size());
	for (int r = 0; r < branchIds.size(); ++r)
	{
		auto &ts = m_series[r].series;
		ts = new QLineSeries();
		ts->setName(m_branchIds[r]);

		m_Chart.addSeries(ts);

		ts->attachAxis(m_AxisX);
		ts->attachAxis(m_AxisY);

		m_series[r].yMax = INT_MIN;
		m_series[r].yMin = INT_MAX;
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

		for (int r = 0; r < qvec.size(); ++r)
		{
			ui->StepTable->item(0, r)->setText(QString::number(qvec[r]));
		}

		ui->StepTable->setUpdatesEnabled(true);


		// chart update
		if (step == 0)
		{
			m_totalMin = INT_MAX;
			m_totalMax = INT_MIN;
		}

		for (int r = 0; r < qvec.size(); ++r)
		{
			m_series[r].series->append(time, qvec[r]);

			m_totalMin = qMin(m_totalMin, qvec[r]);
			m_totalMax = qMax(m_totalMax, qvec[r]);

			m_series[r].yMax = qMax(m_series[r].yMax, qvec[r]);
			m_series[r].yMin = qMin(m_series[r].yMin, qvec[r]);
		}

		m_AxisX->setRange(0, time);

		on_StepTable_itemSelectionChanged();


		// scene update
		for (int r = 0; r < qvec.size(); ++r)
		{
			m_graph.setEdgeAttr(m_branchIds[r], "Q", qvec[r]);
		}

		//m_simuScene->addUndoState();
		m_simuScene->needUpdate();

		qApp->processEvents();
	}
}


void CGraphSimulatorDialog::onSimulationFinished()
{
	ui->SimuParams->setEnabled(true);

	write(tr("Simulation finished"), LOG_OK);
}


// events

void CGraphSimulatorDialog::closeEvent(QCloseEvent *event)
{
    m_simu->stop();

	QDialog::closeEvent(event);
}


void CGraphSimulatorDialog::showEvent(QShowEvent *event)
{
	QDialog::showEvent(event);
}
