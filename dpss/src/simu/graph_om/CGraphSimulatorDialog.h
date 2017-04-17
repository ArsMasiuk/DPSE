#pragma once

#include <QDialog>

#include <CBranchEditorScene.h>

#include <ILogger.h>

#include "CGraphSimulator.h"


namespace Ui {
class CGraphSimulatorDialog;
}

class CGraphSimulatorDialog : public QDialog, public ILogger
{
    Q_OBJECT

public:
    explicit CGraphSimulatorDialog(QWidget *parent = 0);
    ~CGraphSimulatorDialog();

    // reimp
    virtual bool run(const CBranchEditorScene& scene);

    // logger
    virtual void write(const QString& text, int state = LOG_INFO, const QDateTime& timestamp = QDateTime::currentDateTime());

private Q_SLOTS:
    void on_Start_clicked();
	void on_Stop_clicked();

	void onStepFinished(double time, int step, std::vector<double>& qvec);
	void onSimulationFinished();

protected:
	virtual void closeEvent(QCloseEvent *event);

private:
    Ui::CGraphSimulatorDialog *ui;

	CGraphSimulator m_simu;
};