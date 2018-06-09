#ifndef CGRAPHSIMULATOR_H
#define CGRAPHSIMULATOR_H

#include <QObject>

#include <simu/CSimulatorBase.h>

#include "CBranch.h"


class CGraphSimulator : public CSimulatorBase
{
    Q_OBJECT

public:
    explicit CGraphSimulator(QObject *parent = nullptr);
    virtual ~CGraphSimulator();

    // ISimulator
    //virtual void setGraph(const IGraphInterface& scene);
    virtual bool analyse();
    virtual bool run();
    virtual bool stop();

public slots:


private:
    void cleanup();

    struct BranchInfo
    {
        CBranch *branch = nullptr;
        bool isOk = false;
    };

    QMap<QString, BranchInfo> m_branchList;
};


#endif // CGRAPHSIMULATOR_H
