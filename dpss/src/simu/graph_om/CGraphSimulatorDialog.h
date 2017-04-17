#pragma once

#include <QDialog>

#include <ILogger.h>


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

private:
    Ui::CGraphSimulatorDialog *ui;

    //CMPISimulator m_simu;
};