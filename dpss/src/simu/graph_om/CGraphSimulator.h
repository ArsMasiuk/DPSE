#pragma once

#include <ISimulator.h>
#include <ILogger.h>

class CGraphSimulator : public ISimulator
{
public:
	CGraphSimulator(); 

	// reimp
	virtual void setScene(const CBranchEditorScene& scene);
	virtual bool run();

private:
	bool createDDSfile();
	int startSimulation();

private:
	const CBranchEditorScene *m_scene;

	QString m_ddsFileName;
};