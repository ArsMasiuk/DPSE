/*
This file is a part of
DPSE - Distributed Parallel Simulation Environment

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#ifndef CMPISIMULATORDIALOG_H
#define CMPISIMULATORDIALOG_H

#include <QDialog>

#include "CMPISimulator.h"

#include <ILogger.h>


namespace Ui {
class CMPISimulatorDialog;
}

class CMPISimulatorDialog : public QDialog, public ILogger
{
    Q_OBJECT

public:
    explicit CMPISimulatorDialog(QWidget *parent = 0);
    ~CMPISimulatorDialog();

    // reimp
    virtual bool run(const CBranchEditorScene& scene);

    // logger
    virtual void write(const QString& text, int state = LOG_INFO, const QDateTime& timestamp = QDateTime::currentDateTime());

private Q_SLOTS:
    void on_Start_clicked();

    void on_tbPythonPath_clicked();
    void on_lePythonPath_editingFinished();

private:
    Ui::CMPISimulatorDialog *ui;

    CMPISimulator m_simu;
};

#endif // CMPISIMULATORDIALOG_H
