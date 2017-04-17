#include "CGraphSimulatorDialog.h"
#include "ui_CGraphSimulatorDialog.h"

#include <QFileDialog>
#include <QDebug>


CGraphSimulatorDialog::CGraphSimulatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CGraphSimulatorDialog)
{
    ui->setupUi(this);

    //m_simu.setLogger(this);
}

CGraphSimulatorDialog::~CGraphSimulatorDialog()
{
    delete ui;
}


// reimp

bool CGraphSimulatorDialog::run(const CBranchEditorScene& scene)
{
    //m_simu.setScene(scene);

    //ui->NetInfo->setText(m_simu.getNetInfo());

    //QSettings& set = m_simu.getSettings();
    ////qDebug() << set.fileName();
    //set.sync();
    //ui->lePythonPath->setText(set.value("Python/Path").toString());

    //ui->Tabs->setCurrentIndex(0);

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
    //m_simu.run();
}

