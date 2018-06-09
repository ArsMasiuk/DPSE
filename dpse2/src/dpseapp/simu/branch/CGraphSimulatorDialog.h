/*
This file is a part of
DPSE - Distributed Parallel Simulation Environment

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

#include <QDialog>
#include <QtCharts>

#include <qvge/CNodeEditorScene.h>
#include <qvge/CEditorView.h>
#include <qvge/CGraphInterface.h>

#include <simu/ILogger.h>
#include <simu/ISimulator.h>
#include <simu/CSimulatorBase.h>


namespace Ui {
class CGraphSimulatorDialog;
}

using namespace QtCharts;


class CGraphSimulatorDialog : public QDialog, public ILogger
{
    Q_OBJECT

public:
    explicit CGraphSimulatorDialog(QWidget *parent = nullptr);
    ~CGraphSimulatorDialog();

    void setSimulator(CSimulatorBase& simu);
    bool run(const CNodeEditorScene& scene);

    // ILogger
    virtual void write(const QString& text, int state = LOG_INFO, const QDateTime& timestamp = QDateTime::currentDateTime());

private Q_SLOTS:
    void on_Start_clicked();
	void on_Stop_clicked();

	void on_StepTable_itemSelectionChanged();

	void onStepFinished(double time, int step, std::vector<double>& qvec);
	void onSimulationFinished();

protected:
	virtual void closeEvent(QCloseEvent *event);
	virtual void showEvent(QShowEvent *event);

private:
    Ui::CGraphSimulatorDialog *ui;
	QChartView *m_ChartView;

	QChart m_Chart;
	QMap<int, QList<QPointF>> m_testPoints;

    CSimulatorBase *m_simu = nullptr;
    CNodeEditorScene *m_simuScene = nullptr;
    CGraphInterface m_graph;
};
