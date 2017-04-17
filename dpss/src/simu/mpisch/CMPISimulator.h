#pragma once

#include <QString>
#include <QList>
#include <QTemporaryFile>
#include <QSettings>
#include <QProcess>

#include <ISimulator.h>
#include <ILogger.h>


struct SimulationStep
{
    enum Status{
        S_NOTRUN,
        S_DONE,
        S_FAILED
    };

    SimulationStep(const QString& text, int id);

    int m_status;
    QString m_text;
    int m_id;
};


class CMPISimulator: public QObject, public ISimulator
{
    Q_OBJECT
public:
    CMPISimulator();
    virtual ~CMPISimulator();

    QString getNetInfo() const;

    QSettings& getSettings() { return *m_settings; }

    void setLogger(ILogger *logger);

    // ISimulator
    virtual void setScene(const CBranchEditorScene& scene);
    virtual bool run();
	virtual bool stop() { return false;  }

protected:
    // mpisch specific
    bool doRunStep(SimulationStep &step);
    void doParseScene();
    bool doPrepareNetwork();
    bool doGenerateEquations();

    // common fn
    void doCleanup();
    bool createTempFile(QTemporaryFile& file, const QString& name = "");
    bool checkFileExists(const QString& fileName);
    bool readAllFile(QByteArray& data, const QString& fileName);

    void logInfo(const QString& text);
    void logOk(const QString& text);
    void logWarning(const QString& text);
    void logError(const QString& text);

protected Q_SLOTS:
    void readStdOutput();

protected:
    const CBranchEditorScene *m_scene;

    QList<SimulationStep> m_steps;

    QList<CBranchConnection*> m_branches;
    QList<CBranchNode*> m_nodes;

    QMap<CNode*, int> m_nodeToId;
    QMap<int, CFanNode*> m_idToFan;

    QStringList m_tempFiles;
    ILogger *m_log;
    QSettings *m_settings;

    QString m_inputTableFileName;

    QProcess process;
};
