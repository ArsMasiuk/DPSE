/*
This file is a part of
DPSE - Distributed Parallel Simulation Environment

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "CMPISimulator.h"
#include "CMPISimulatorDialog.h"

#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QApplication>
#include <QProcess>


// SimulationStep

SimulationStep::SimulationStep(const QString& text, int id):
    m_status(S_NOTRUN),
    m_text(text),
    m_id(id)
{
}


enum Steps{
    Step_Prepare,
    Step_Generate,
    Step_Compile,
    Step_Simulate,
    Step_Results,
    Step_Visualize
};


// CMPISimulator

CMPISimulator::CMPISimulator():
    m_scene(NULL)
{
    m_steps << SimulationStep("Prepare network", Step_Prepare);
    m_steps << SimulationStep("Generate equations", Step_Generate);
    m_steps << SimulationStep("Compile equations", Step_Compile);
    m_steps << SimulationStep("Run simulation", Step_Simulate);
    m_steps << SimulationStep("Get results", Step_Results);
    m_steps << SimulationStep("Visualise results", Step_Visualize);

    m_settings = new QSettings(QSettings::IniFormat, QSettings::SystemScope, "DPSS", "MPISchSimulator");
}


CMPISimulator::~CMPISimulator()
{
    doCleanup();

    delete m_settings;
}


QString CMPISimulator::getNetInfo() const
{
    QString info = QString("Edges: %1\nNodes: %2\n(Fans: %3)")
            .arg(m_branches.count())
            .arg(m_nodes.count())
            .arg(m_idToFan.count());

    return info;
}

void CMPISimulator::setLogger(ILogger *logger)
{
    m_log = logger;
}


void CMPISimulator::setScene(const CBranchEditorScene& scene)
{
    m_scene = &scene;

    doParseScene();
}


bool CMPISimulator::run()
{
    // clear steps
    for (SimulationStep& step: m_steps)
        step.m_status = SimulationStep::S_NOTRUN;

    // run steps
    for (SimulationStep& step: m_steps)
    {
        if (step.m_status != SimulationStep::S_DONE)
        {
            logInfo(QString("Starting: <b>%1</b>...").arg(step.m_text));

            bool isOk = doRunStep(step);

            step.m_status = isOk ? SimulationStep::S_DONE : SimulationStep::S_FAILED;
            if (!isOk)
            {
                logError(QString("<b>%1 FAILED</b>").arg(step.m_text));

                return false;
            }

            logOk(QString("<b>%1 OK</b>").arg(step.m_text));
        }
    }

    return true;
}


// steps of simulation

bool CMPISimulator::doRunStep(SimulationStep &step)
{
    switch (step.m_id)
    {
    case Step_Prepare:
        return doPrepareNetwork();

    case Step_Generate:
        return doGenerateEquations();

    default:;
    }

    return false;
}


void CMPISimulator::doParseScene()
{
    m_branches.clear();
    m_nodes.clear();
    m_nodeToId.clear();
    m_idToFan.clear();

    // enumerated lists of items
    for (auto item: m_scene->items())
    {
        if (auto branch = dynamic_cast<CBranchConnection*>(item))
        {
            m_branches.append(branch);
            continue;
        }

        if (auto fan = dynamic_cast<CFanNode*>(item))
        {
            int id = m_nodes.count();

            m_nodeToId[fan] = id;
            m_nodes.append(fan);    // fan is node!

            m_idToFan[id] = fan;
            continue;
        }

        if (auto node = dynamic_cast<CBranchNode*>(item))
        {
            m_nodeToId[node] = m_nodes.count();
            m_nodes.append(node);
            continue;
        }
    }
}


bool CMPISimulator::doPrepareNetwork()
{
    // ATM x
    // NUj KUj Qi Ri Fi Li H Hpos

    QString inputTable = "ATM ";

    QTextStream ts(&inputTable);
    for (auto fanId : m_idToFan.keys())
    {
        ts << fanId << " ";
    }

    ts << "\nNUj KUj Qi Ri Fi Li H Hpos\n";

    for (int Qi = 0; Qi < m_branches.size(); Qi++)
    {
        auto branch = m_branches[Qi];

        int NUj = m_nodeToId[branch->firstNode()];
        int KUj = m_nodeToId[branch->lastNode()];

        ts << NUj << " " << KUj << " " << Qi << " ";
        ts << branch->getAttribute("LINK_R").toFloat() << " ";
        ts << branch->getAttribute("LINK_F1").toFloat() << " ";
        ts << branch->getAttribute("LINK_L").toFloat() << " ";

        // fan at start
        if (m_idToFan.contains(NUj))
        {
            auto fan = m_idToFan[NUj];
            ts << fan->getAttribute("LINK_H").toFloat() << " ";
            ts << "1" << " ";
        }
        else
            ts << "0" << " ";

        // fan at end
        if (m_idToFan.contains(KUj))
        {
            auto fan = m_idToFan[KUj];
            ts << fan->getAttribute("LINK_H").toFloat() << " ";
            ts << "-1" << " ";
        }
        else
            ts << "0" << " ";

        ts << "\n";
    }

    ts.flush();

    //qDebug() << inputTable;

    QTemporaryFile tableFile;
    createTempFile(tableFile, "net_XXXXXX.tab");
    tableFile.write(inputTable.toLatin1());

    logInfo(QString("Input table generated: %1").arg(tableFile.fileName()));

    m_inputTableFileName = tableFile.fileName();

    return true;
}


bool CMPISimulator::doGenerateEquations()
{
    // look for templates
    QString dataDir = QApplication::applicationDirPath() + "/../data/simu/mpisch/";

    // temp: rk only
    QString pyTemplatePath = dataDir + "RK_GSL_template.py";
    QString cTemplatePath = dataDir + "RK_GSL_template.c";

    if (!checkFileExists(pyTemplatePath))
        return false;

    if (!checkFileExists(cTemplatePath))
        return false;

    if (!checkFileExists(m_inputTableFileName))
        return false;

    // prepare templates
    QByteArray pyTemplate;
    if (!readAllFile(pyTemplate, pyTemplatePath))
        return false;

    QByteArray cTemplate;
    if (!readAllFile(cTemplate, cTemplatePath))
        return false;

    QTemporaryFile cTempFile;
    createTempFile(cTempFile, "template_XXXXXX.c");
    cTempFile.write(cTemplate);

    QTemporaryFile cOutFile;
    createTempFile(cOutFile, "output_XXXXXX.c");

    pyTemplate = pyTemplate.replace("$INPUT_TAB_FILE$", m_inputTableFileName.toLocal8Bit());
    pyTemplate = pyTemplate.replace("$INPUT_C_FILE$", cTempFile.fileName().toLocal8Bit());
    pyTemplate = pyTemplate.replace("$OUTPUT_C_FILE$", cOutFile.fileName().toLocal8Bit());

    QTemporaryFile pyFile;
    createTempFile(pyFile, "graph_XXXXXX.py");
    pyFile.write(pyTemplate);

    // look for python
    QString pythonPath = m_settings->value("Python/Path").toString();
    pythonPath = QDir::toNativeSeparators(pythonPath);
    if (!checkFileExists(pythonPath))
    {
        logError("Cannot find Python executable, please set the correct path in the Toolchain settings.");
        return false;
    }


    //QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    //process.setProgram(pythonPath);
    QStringList args;
    //args << "/c" << pythonPath;
    args << "-i" << "-u";
    //args << "--version";
    args << QDir::toNativeSeparators(pyFile.fileName());
    //process.setArguments(args);

    logInfo(pythonPath + " " + args.join(" "));

    //connect(&process, SIGNAL(readyRead()), this, SLOT(readStdOutput()));

    process.start(pythonPath, args);
    //process.start("cmd", args);

    if (!process.waitForStarted())
    {
        logError("Cannot execute Python process.");
        return false;
    }

    process.closeWriteChannel();

    if (process.waitForFinished(-1))
    {
        process.waitForReadyRead();

        QString result(process.readAll());
//                qDebug() << result;

        int code = process.exitCode();
        if (code >= 0)
        {
            logInfo(result);
            return true;
        }

        logError(result);
        return false;
    }
    else
    {
        logError(process.errorString());
        logError("Cannot execute Python: timeout occured.");
    }

    return false;
}


// common

void CMPISimulator::doCleanup()
{
    for (auto name: m_tempFiles)
    {
        QFile::remove(name);
    }

    m_tempFiles.clear();
}


bool CMPISimulator::createTempFile(QTemporaryFile& file, const QString& name)
{
    QString tempDirPath = QDir::tempPath() + "/dpss/";
    QDir tempDir(tempDirPath);

    if (!tempDir.exists())
    {
        if (!tempDir.mkpath(tempDirPath))
            return false;
    }

    file.setFileTemplate(tempDirPath + name);
    file.setAutoRemove(false);
    file.open();
    m_tempFiles << file.fileName();

    return true;
}


bool CMPISimulator::checkFileExists(const QString& fileName)
{
    if (QFile::exists(fileName))
        return true;

    logWarning(QString("Cannot find file: %1").arg(QFileInfo(fileName).absoluteFilePath()));

    return false;
}


bool CMPISimulator::readAllFile(QByteArray& data, const QString& fileName)
{
    QFile f(fileName);
    if (!f.open(QFile::ReadOnly))
    {
        logWarning(QString("Cannot read file: %1").arg(QFileInfo(fileName).absoluteFilePath()));
        return false;
    }

    data = f.readAll();
    f.close();
    return true;
}


// log

void CMPISimulator::logInfo(const QString &text)
{
    if (m_log)
        m_log->write(text);
}

void CMPISimulator::logOk(const QString &text)
{
    if (m_log)
        m_log->write(text, ILogger::LOG_OK);
}

void CMPISimulator::logWarning(const QString &text)
{
    if (m_log)
        m_log->write(text, ILogger::LOG_WARN);
}

void CMPISimulator::logError(const QString &text)
{
    if (m_log)
        m_log->write(text, ILogger::LOG_ERROR);
}


void CMPISimulator::readStdOutput()
{
    logInfo(QString(process.readAll()));
}
