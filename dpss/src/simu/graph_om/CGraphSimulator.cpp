#include <stdio.h>
#include "Graph.h"

using namespace std;

int main_simu(Graph& g, const char* fileName)
{  
  int result = 0;

  result = g.InitFromFile_DDS(fileName);
            if (result)
	           {
		           fprintf(stderr,"g.InitFromFile() error");
		           return -1;
	           }

  result = g.MSmBuildFromArchs();
             if (result == -1)
             {
               fprintf(stderr, "g.MSmBuildFromArchs() error");
               return -1;
             }
   
  result = g.MSmCheckForLonelyVertices();
             if (result)
             {
               fprintf(stderr, "Graph has %d lonely vertices. Please, check input file...\n", result);
               return -1;
             }
  
  g.MSmPrint();





  g.BuildTree_Prim();
  //g.BuildTree_TO_DELETE();





  g.BuildCycles();

  g.PrintAllCycles();

  result = g.MInBuildFromArchs();
             if (result == -1)
             {
               fprintf(stderr, "g.MInBuildFromArchs() error");
               return -1;
             }

  g.MInPrint();
  
  result = g.MInConvertToEigenFormat();
  if (result)
            {
              fprintf(stderr, "Error during converting MIn to sparse format\n");
              return -1;
            }

  result = g.MKonBuildFromArchs();
             if (result == -1)
             {
               fprintf(stderr, "g.MKonBuildFromArchs() error");
               return -1;
             }

  g.MKonPrint();
  
  g.PrintKonturs();

  result = g.MKonConvertToEigenFormat();
            if (result)
            {
              fprintf(stderr, "Error during converting MIn to sparse format\n");
              return -1;
            }

  //g.Calculate();
  g.EigenCalculate();

   
  fprintf_s(stdout, "Press Enter...\n");
	_getchar_nolock();
	return 0;
}

int main2(int argc, char **argv)
{
  Graph g;
  std::vector < std::vector < int > > M1, M2;


  //      | 3  1  1  0  1 |
  //      | 1 -1  0  1  0 |  
  //  M = | 1  0  0  2  1 |
  //      | 0  1  2  7  1 |
  //      | 1  0  1  1  4 |  

  M1.resize(5);
  M1[0].resize(4);
  M1[1].resize(4);
  M1[2].resize(4);
  M1[3].resize(4);
  M1[4].resize(4);

  M1[0][0] = 3;
  M1[0][1] = 1;
  M1[0][2] = 1;
  M1[0][3] = 0;
  M1[1][0] = 1;
  M1[1][1] = -1;
  M1[1][2] = 0;
  M1[1][3] = 1;
  M1[2][0] = 1;
  M1[2][1] = 0;
  M1[2][2] = 0;
  M1[2][3] = 2;
  M1[3][0] = 0;
  M1[3][1] = 1;
  M1[3][2] = 2;
  M1[3][3] = 7;
  M1[4][0] = 1;
  M1[4][1] = 0;
  M1[4][2] = 1;
  M1[4][3] = 1;
  


  M2.resize(4);
  M2[0].resize(4);
  M2[1].resize(4);
  M2[2].resize(4);
  M2[3].resize(4);

  M2[0][0] = 2;
  M2[0][1] = 0;
  M2[0][2] = 4;
  M2[0][3] = 0;
  M2[1][0] = 6;
  M2[1][1] = 7;
  M2[1][2] = 0;
  M2[1][3] = 1;
  M2[2][0] = -1;
  M2[2][1] = 2;
  M2[2][2] = -4;
  M2[2][3] = 4;
  M2[3][0] = 0;
  M2[3][1] = 1;
  M2[3][2] = 0;
  M2[3][3] = 1;
  



  SparseMatrixType m1, m2, AddSEM;

  int result;
  result = g.ConvertMatrixToSparseFormat(M1, &m1);
  result = g.ConvertMatrixToSparseFormat(M2, &m2);

  //g.SparseMatricesAdd(m1, m2, &AddSEM);
  //g.SparseMatricesSub(m1, m2, &AddSEM);
  //g.SparseMatriceLineMulConst(&m1, 1, 0);
  //g.SparseMatriceLinesSwap(&m1, 1, 4);

  //g.SparseMatricesMul(m1, m2, &AddSEM);
  //g.SparseMatricesMul(m2, m1, &AddSEM);
  g.SparseMatrixReverse(m2, &AddSEM);

  // altr  = { 3, 1, 1, 1,  1,-1, 1,  1, 2, 1,  1, 2, 7, 1,  1, 1, 1, 4 }
  // jptr  = { 0, 1, 2, 4,  0, 1, 3,  0, 3, 4,  1, 2, 3, 4,  0, 2, 3, 4 }
  // iptr  = { 0,           4,        7,       10,          14,          18}

  // altr  = { 3, 1, 1, 1,  1, 1, 1, 4,  1, 2, 1,  1, 2, 7, 1,  1,-1, 1 }
  // jptr  = { 0, 1, 2, 4,  0, 2, 3, 4,  0, 3, 4,  1, 2, 3, 4,  0, 1, 3 }
  // iptr  = { 0,           4,           8,       11,          15,       18}

  int i = 0;
  
  return 0;
}