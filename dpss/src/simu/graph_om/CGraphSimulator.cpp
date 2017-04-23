#include <stdio.h>

#include <QApplication>
#include <QFileInfo>

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


// ISimulator

void CGraphSimulator::setScene(const CBranchEditorScene& scene)
{
	m_scene = &scene;
}


bool CGraphSimulator::run()
{
	if (!createDDSfile())
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

  result = m_graph->InitFromFile_DDS(m_ddsFileName.toLocal8Bit().data());
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
