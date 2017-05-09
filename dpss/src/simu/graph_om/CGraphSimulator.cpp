/*
This file is a part of
DPSE - Distributed Parallel Simulation Environment

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include <stdio.h>

#include <QApplication>
#include <QFileInfo>
#include <QTextStream>

#include "Graph.h"

#include "CGraphSimulator.h"

using namespace std;


// CGraphSimulator

CGraphSimulator::CGraphSimulator(): m_log(NULL)
{
	m_scene = NULL;
	m_graph = new Graph();

	m_simuTime = 60;
}


void CGraphSimulator::setLogger(ILogger *logger)
{
	m_log = logger;
}


void CGraphSimulator::setSimulationTime(int sec)
{
	m_simuTime = sec > 0 ? sec : INT_MAX;
}


int CGraphSimulator::getMaxSteps() const
{
	// temp
	return m_simuTime * 1000;
}


bool CGraphSimulator::createDDSfile()
{
	// TEST
	QString dataDir = QApplication::applicationDirPath() + "/../data/test/";
	m_ddsFileName = QFileInfo(dataDir + "Graph-117_DDS.txt").canonicalFilePath();

	if (m_log) m_log->write(QString("DDS file: %1").arg(m_ddsFileName));

	return true;
}


void CGraphSimulator::doParseScene()
{
}


bool CGraphSimulator::createSceneFile()
{
	//	NUj  KUj  Qi  Ri     Qist  H  Fi  Pi  αi  Li  Ki

	QString inputTable;
	QTextStream ts(&inputTable);

	auto branches = m_scene->getItems<CBranchConnection>();

	// sort by ids
	QMap<QString, CBranchConnection*> branchMap;
	for (auto branch : branches)
	{
		branchMap[branch->getId()] = branch;
	}

	int Qi = 0;	// Q number
	int ni = 1;	// N number (0 = ATM)

	QMap<CNode*, int> nodeMap;	// node to N

	for (auto branch : branchMap.values())
	{
		Qi++;	// branch is 1-based

		auto startNode = branch->firstNode();
		auto endNode = branch->lastNode();

		float H = 0;

		// start node
		int NUi = ni;
		auto fan1 = dynamic_cast<CFanNode*>(startNode);
		if (fan1)
		{
			H = fan1->getAttribute("H").toFloat();
			NUi = nodeMap[startNode] = 0;	// connect to ATM
		}
		else
			if (nodeMap.contains(startNode))
				NUi = nodeMap[startNode];
			else
				NUi = nodeMap[startNode] = ni++;

		// end node
		int KUi = ni;
		auto fan2 = dynamic_cast<CFanNode*>(endNode);
		if (fan2)
		{
			H = fan2->getAttribute("H").toFloat();
			KUi = nodeMap[endNode] = 0;	// connect to ATM
		}
		else
			if (nodeMap.contains(endNode))
				KUi = nodeMap[endNode];
			else 
				KUi = nodeMap[endNode] = ni++;

		// warn: two fans should not be?
		// if (fan1 && fan2)

		// output line
		ts << NUi << "\t " << KUi << "\t " << Qi << "\t ";
		ts << branch->getAttribute("R").toFloat() << "\t ";
		ts << "0" << "\t ";
		ts << H << "\t ";
		ts << branch->getAttribute("S").toFloat() << "\t ";
		ts << "0" << "\t ";
		ts << "0" << "\t ";
		ts << branch->getAttribute("L").toFloat() << "\t ";
		ts << branch->getAttribute("L").toFloat() << "\t ";	// TEMP: must be K here
		//ts << "0" << " ";

		ts << "\n";
	}

	ts.flush();


	QString dataDir = QApplication::applicationDirPath() + "/../data/test/";
	m_tableFileName = QFileInfo(dataDir + "Graph.txt").absoluteFilePath();

	if (m_log) 
		m_log->write(QString("Table file: %1").arg(m_tableFileName));

	QFile ft(m_tableFileName);
	if (!ft.open(QFile::WriteOnly))
		return false;

	ft.write(inputTable.toLatin1());

	return true;
}


// ISimulator

void CGraphSimulator::setScene(const CBranchEditorScene& scene)
{
	m_scene = &scene;

	//doParseScene();
}


bool CGraphSimulator::run()
{
	//if (!createDDSfile())
	//	return false;

	if (!createSceneFile())
		return false;

	if (startSimulation() < 0)
		return false;

	Q_EMIT simulationFinished();

	return true;
}


bool CGraphSimulator::stop()
{
	if (!m_graph)
		return false;

	m_graph->stopSimulation(*this);

	return true;
}


// simulation

int CGraphSimulator::startSimulation()
{  
	m_graph = new Graph();

  int result = 0;

  result = m_graph->InitFromFile(m_tableFileName.toLocal8Bit().data());
  //result = m_graph->InitFromFile_DDS(m_ddsFileName.toLocal8Bit().data());
            if (result)
	           {
		           fprintf(stderr,"g.InitFromFile() error");
		           return -1;
	           }

  result = m_graph->MSmBuildFromArchs();
             if (result == -1)
             {
               fprintf(stderr, "g.MSmBuildFromArchs() error");
               return -1;
             }
   
  result = m_graph->MSmCheckForLonelyVertices();
             if (result)
             {
               fprintf(stderr, "Graph has %d lonely vertices. Please, check input file...\n", result);
               return -1;
             }
  
  //g.MSmPrint();



			 m_graph->BuildTree_Prim();
  //g.BuildTree_TO_DELETE();



			 m_graph->BuildCycles();

  //g.PrintAllCycles();

  result = m_graph->MInBuildFromArchs();
             if (result == -1)
             {
               fprintf(stderr, "g.MInBuildFromArchs() error");
               return -1;
             }

  //g.MInPrint();
  
  result = m_graph->MInConvertToEigenFormat();
  if (result)
            {
              fprintf(stderr, "Error during converting MIn to sparse format\n");
              return -1;
            }

  result = m_graph->MKonBuildFromArchs();
             if (result == -1)
             {
               fprintf(stderr, "g.MKonBuildFromArchs() error");
               return -1;
             }

  //g.MKonPrint();
  
  //g.PrintKonturs();

  result = m_graph->MKonConvertToEigenFormat();
            if (result)
            {
              fprintf(stderr, "Error during converting MIn to sparse format\n");
              return -1;
            }

  //g.Calculate();
  //g.EigenCalculate();

	ISimulatorEngine::SimulationParams params;
	params.duration = m_simuTime;
	m_graph->runSimulation(NULL, params, *this);
   
 // fprintf_s(stdout, "Press Enter...\n");
	//_getchar_nolock();
	return 0;
}


// ISimulatorCallback

void CGraphSimulator::log(const char* text, int status)
{
	if (m_log)
		m_log->write(QString(text), status);
}


void CGraphSimulator::stepResult(double time, int step, std::vector<double>& qvec)
{
	Q_EMIT stepFinished(time, step, qvec);
}
