#ifndef CGRAPHSIMULATOR_H
#define CGRAPHSIMULATOR_H

#include <QObject>

#include <simu/CSimulatorBase.h>

#include "CBranch.h"

class CConnection;
class CNode;


class CGraphSimulator : public CSimulatorBase
{
    Q_OBJECT

public:
    explicit CGraphSimulator(QObject *parent = nullptr);
    virtual ~CGraphSimulator();

    // ISimulator
    //virtual void setGraph(const IGraphInterface& scene);
    virtual bool analyse();
    virtual bool run(const SimuParams& params);
    virtual bool stop();

Q_SIGNALS:
	void prepareOutput(const QStringList& branchIds, const QStringList& paramIds);
	void stepFinished(double time, int step, std::vector<double>& qvec);

private:
    void cleanup();
    bool prepare();
	bool simulate(int steps = 0);


    struct BranchInfo
    {
        CConnection *edge = nullptr;
        CBranch *branch = nullptr;
        bool isOk = false;
    };

    QMap<QString, BranchInfo> m_branchList;

    struct NodeInfo
    {
        CNode *node = nullptr;
        bool isOk = false;
    };

    QMap<QString, NodeInfo> m_nodeList;


	SimuParams m_params;

	bool m_inSimulation = false;

	std::vector<double> m_qvec;
};


#endif // CGRAPHSIMULATOR_H
