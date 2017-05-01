#pragma once

#include <QObject>

#include <ISimulator.h>
#include <ISimulatorEngine.h>
#include <ILogger.h>

class Graph;


class CGraphSimulator : public QObject, public ISimulator, public ISimulatorCallback
{
	Q_OBJECT
public:
	CGraphSimulator(); 

	void setLogger(ILogger *logger);

	void setSimulationTime(int sec);
	int getMaxSteps() const;

	// ISimulator
	virtual void setScene(const CBranchEditorScene& scene);
	virtual bool run();
	virtual bool stop();

	// ISimulatorCallback
	virtual void log(const char* text, int status = 0);
	virtual void stepResult(double time, int step, std::vector<double>& qvec);

Q_SIGNALS:
	void stepFinished(double time, int step, std::vector<double>& qvec);
	void simulationFinished();

private:
	void doParseScene();
	bool createDDSfile();
	bool createSceneFile();
	int startSimulation();

private:
	const CBranchEditorScene *m_scene;
	ILogger *m_log;

	Graph *m_graph;
	QString m_ddsFileName;
	QString m_tableFileName;

	int m_simuTime;

	QList<CBranchConnection*> m_branches;
	QList<CBranchNode*> m_nodes;

	QMap<CNode*, int> m_nodeToId;
	QMap<int, CFanNode*> m_idToFan;
};