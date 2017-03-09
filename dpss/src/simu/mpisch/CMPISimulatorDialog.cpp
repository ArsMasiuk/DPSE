#include "CMPISimulatorDialog.h"
#include "ui_CMPISimulatorDialog.h"

#include <QFileDialog>
#include <QDebug>


CMPISimulatorDialog::CMPISimulatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMPISimulatorDialog)
{
    ui->setupUi(this);

    m_simu.setLogger(this);
}

CMPISimulatorDialog::~CMPISimulatorDialog()
{
    delete ui;
}


// reimp

bool CMPISimulatorDialog::run(const CBranchEditorScene& scene)
{
    m_simu.setScene(scene);

    ui->NetInfo->setText(m_simu.getNetInfo());

    QSettings& set = m_simu.getSettings();
    //qDebug() << set.fileName();
    set.sync();
    ui->lePythonPath->setText(set.value("Python/Path").toString());

    ui->Tabs->setCurrentIndex(0);

    exec();

    return true;
}


// log

void CMPISimulatorDialog::write(const QString& text, int state, const QDateTime &timestamp)
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

void CMPISimulatorDialog::on_Start_clicked()
{
    m_simu.run();
}


void CMPISimulatorDialog::on_tbPythonPath_clicked()
{
    QString filePath =  QFileDialog::getOpenFileName(NULL,
                                                     "Path to Python executable",
                                                     "python.exe",
                                                     "Executables (*.exe)");
    if (filePath.isEmpty())
        return;

    ui->lePythonPath->setText(filePath);
    on_lePythonPath_editingFinished();
}


void CMPISimulatorDialog::on_lePythonPath_editingFinished()
{
    QSettings& set = m_simu.getSettings();
    set.setValue("Python/Path", ui->lePythonPath->text());
    set.sync();

    //qDebug() << set.fileName();
}
