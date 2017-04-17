
#include <set>
#include <list>
#include <vector>
#include <iostream>

#include <Eigen/SparseCore>
#include <Eigen/SparseLU>
#include <Eigen/LU>

#define EPS 1e-5
#define SIMULATION_TIME 60
#define TIME_STEP_SIZE 0.001

typedef struct arch
{
  int StartVertex = 0;
  int EndVertex = 0;
  double R = 0.0;
  double Q = 0.0;
  double H = 0.0;
  double F = 0.0;
  double P = 0.0;
  double alpha = 0.0;
  double L = 0.0;
  double K = 0.0;
  int tree = 0;
} ArchType;

typedef struct archlight
{
  int v1 = 0;
  int v2 = 0;
  double weight = 0.0;
} ArchLight;

typedef struct SparseMatrixStruct
{
  int rows;
  int cols;
  double *altr = NULL;
  int *jptr = NULL;
  int *iptr = NULL;
} SparseMatrixType;


class Graph
{
public:
  Graph();
  ~Graph();
  int InitFromFile(char *FileName);
  int InitFromFile_DDS(char *FileName);
  int MSmBuildFromArchs();
  void MSmPrint();
  int MInBuildFromArchs();
  int MInPrint();
  int MInXPrint();
  int MInYPrint();
  int MInSaveToFile();
  int MKonBuildFromArchs();
  int MKonPrint();
  int MKonXPrint();
  int MKonYPrint();
  int MKonSaveToFile();
  int PrintKonturs();

  int AddVertex(int name);
  int GetVertexNameByID(int id);
  int GetVertexIDByName(int name);
  int BuildCycles();
  int MSmCheckForLonelyVertices();
  void PrintAllCycles();
  void PrintAllArchs();

  int BuildTree();
  int BuildTree_Prim();
  int BuildTree_TO_DELETE();
  int iNumTreeVariables;
  int iNumATreeVariables;

  int ConvertMatrixToSparseFormat(std::vector < std::vector < int > > M, SparseMatrixType *SEM);
  int SparseMatricesAdd(SparseMatrixType m1, SparseMatrixType m2, SparseMatrixType *result);
  int SparseMatricesSub(SparseMatrixType m1, SparseMatrixType m2, SparseMatrixType *result);
  int SparseMatricesMul(SparseMatrixType m1, SparseMatrixType m2, SparseMatrixType *result);
  int SparseMatrixLineMulConst(SparseMatrixType *m, int row, double value);
  int SparseMatrixMulConst(SparseMatrixType *m, double value);
  int SparseMatrixLineErase(SparseMatrixType *m, int row);
  int SparseMatrixLinesSwap(SparseMatrixType *m, int r1, int r2);
  int SparseMatrixReverse_old(SparseMatrixType m, SparseMatrixType *result);
  int SparseMatrixReverse(SparseMatrixType m, SparseMatrixType *result);
  double SparseMatrixMinElement(SparseMatrixType m);
  double SparseMatrixMaxElement(SparseMatrixType m);
  double EigenVectorMaxElement(Eigen::VectorXd v);

  int SparseMatrixSaveToFile(SparseMatrixType m, char *FileName);

  int MInConvertToSparseFormat();
  int MKonConvertToSparseFormat();

  int MInConvertToEigenFormat();
  int MKonConvertToEigenFormat();

  int SaveStepValuesToFile(FILE *f);
  int SaveStepValuesToFile_Eigen(FILE *f);

  int InitializeR();
  int CalculateZ();
  int InitializeH();
  int InitializeY();
  int CalculateX();

  void InitializeEigenY();
  void InitializeEigenR();
  void InitializeEigenK();
  void InitializeEigenH();
  void CalculateEigenZ();
  int  CalculateEigenW();
  int  CalculateEigenSp();


  int Calculate();
  int EigenCalculate();

private:

  int gGraphDebug = 1;

  // Массив вершин графа 
  std::vector <int> Vertices;

  // Матрица смежности
  std::vector < std::vector < int > > MSm;
  SparseMatrixType MSmSparse;

  // Матрица инцидентности
  std::vector < std::vector < int > > MIn;
  SparseMatrixType Ax, Ay;
  Eigen::SparseMatrix<double> EAx, EAy;

  // Матрица контуров
  std::vector < std::vector < int > > MKon;
  SparseMatrixType S, Sx, Sy;
  Eigen::SparseMatrix<double> ES, ESx, ESy;

  // Массив, содержащий номера вершин в порядке обхода цикла
  std::vector < int > VertexCycle;

  // Массив циклов графа по номерам вершин
  std::vector < std::vector < int > > VertexCycles;

  // Массив длин циклов графа по номерам вершин
  std::vector < int > VertexCyclesLens;

  // Массив, содержащий номера дуг в порядке обхода цикла
  std::vector < int > ArchCycle;

  // Массив циклов графа по номерам дуг
  std::vector < std::vector < int > > ArchCycles;
  
  // Массив индексов соответствия циклов из дуг и циклов из вершин графа
  std::vector < int > ArchCycleToVertexCycle;

  // Векторы индексов ветвей дерева и антидерева
  std::vector < int > iTreeInd, iATreeInd;

  // Массив структур для дуг графа
  //int iNumArchs;
  std::vector < ArchType > Archs;

  SparseMatrixType R, Kx, Ky, Z, H, X, Y;
  SparseMatrixType Ax_rev, W;

  Eigen::VectorXd EY, EX, EH, EZ;
  Eigen::SparseMatrix<double> ER, EKx, EKy;
  Eigen::SparseMatrix < double > EW, ESp;


  void VerticesSort();
  void FindVertexCycle(int from, int to, std::set < int > nnotUsedV);
  int GetPlaceInCycle(std::vector < int > VertexCycle, int id);
  int AreCyclesDifferent(std::vector < int > cycle1, std::vector < int > cycle2);
  void PrintCycle(int iCycleIndex);
  void PrintKontur(int iCycleIndex);
  void PrintArch(ArchType a);
  int GetMaxCycleLength();
  int GetArchIndexByVertices(int StartVertex, int EndVertex, int StartSearchPosition);
  int GetNumATreeArchsInCycle(int iArchCycleIndex, int *iLastATreeArchIndex);
  bool IsATreeArch(int iArchIndex);
  int GetMKonValue(int iArchCycleIndex, int iNumArch);

  int PrintDoubleMatrixToFile(char * FileName, double *m, int c, int r);

  int getmin(int a, int b);
  int getmax(int a, int b);
};