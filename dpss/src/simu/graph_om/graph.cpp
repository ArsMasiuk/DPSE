// Модуль работы с графом сетевого объекта.
// Инициализация из файла, поиск циклов, выделение дерева и антидерева.
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <algorithm>    // std::sort
#include <queue>
#include <map>

#ifndef MSC_VER
// Linux equivalent of sprintf_s
#define sprintf_s snprintf
// Linux equivalent of fscanf_s
#define fscanf_s fscanf
#endif

#include "Graph.h"

char MATRIX_INTSID_FILE_NAME[] = "MIn.txt";
char MATRIX_KONTUR_FILE_NAME[] = "MKon.txt";

// Конструктор
Graph::Graph()
{
	m_forceStop = false;
}

// Деструктор
Graph::~Graph()
{
}

// Инициализация графа из файла.
int Graph::InitFromFile(char *FileName)
//   Файл содержит описание всех дуг графа с указанием начальной и конечной вершин,
//   которые соединяются дугой, а также все параметры дуги.
//	NUj  KUj  Qi  Ri     Qist  H  Fi  Pi  αi  Li  Ki
//	 1    3    2  0.072	  29   0   0   0   0   0   0
//	NUj	 - начальный узел
//	KUj	 - конечный узел
//	Qi	 - номер ветви
//	Ri	 - удельное сопротивление ветви
//	Qist - 
//	H	 - 
//	Fi	 - 
//	Pi	 - 
//	αi	 - 
//	Li	 - 
//	Ki	 - 
//  На выходе имеем массив Archs размерности iNumArchs
{
  FILE *f = NULL;
  errno_t err;
  ArchType a;
  int NumReadBytes;
  int Debug = 1;
  int StartVertexName, EndVertexName, id;
  
  if ((err = fopen_s(&f, FileName, "r")) != 0)
  {
    // File could not be opened. filepoint was set to NULL
    // error code is returned in err.
    // error message can be retrieved with strerror(err);
    char buf[250];
    errno_t e = strerror_s((char *)&buf, 248, err);
    fprintf(stderr, "Cannot open file '%s': %s\n", FileName, buf);
  }
  else
  {
    // File was opened, filepoint can be used to read the stream.
    if (Debug || gGraphDebug)
    {
      fprintf(stdout, "Attempt to open file '%s' for reading\n", FileName);
      fprintf(stdout, "Open input file for reading ..................... OK\n");
    }
    
    while (true)
    {
      NumReadBytes = fscanf_s(f, "%d %d %d %lf %lf %lf %lf %lf %lf %lf %lf",
        &StartVertexName, &EndVertexName, &id, &(a.R), &(a.Q), &(a.H), &(a.F), &(a.P), &(a.alpha), &(a.L), &(a.K));

      if (NumReadBytes == -1)
        break;
      else if (NumReadBytes != 11)
      {
        fprintf_s(stderr, "Input file is corrupted (Record #%d)\n", int(Archs.size()));
        fclose(f);
        return -1;
      }
      else
      {        
        a.StartVertex = StartVertexName;
        AddVertex(StartVertexName);
        a.EndVertex = EndVertexName;
        AddVertex(EndVertexName);

        Archs.push_back(a);
        if (int(Archs.size()) == int(Archs.max_size()))
        {
          // Reallocate memory for Archs array:
          Archs.resize(Archs.size()+100);
        }
      }
    }
    if (Debug || gGraphDebug)
      fprintf(stdout, "Input file reading complete. %d records have been read\n", int(Archs.size()));
    fclose(f);

    VerticesSort();

    for (int i = 0; i < int(Archs.size()); i++)
    {
      Archs[i].StartVertex = GetVertexIDByName(Archs[i].StartVertex);
      Archs[i].EndVertex = GetVertexIDByName(Archs[i].EndVertex);
    }

  }
  return 0;
}

int Graph::InitFromFile_DDS(char * FileName)
{
  FILE *f = NULL;
  errno_t err;
  ArchType a;
  int NumReadBytes;
  int Debug = 1;
  int StartVertexName, EndVertexName, id;

  if ((err = fopen_s(&f, FileName, "r")) != 0)
  {
    // File could not be opened. filepoint was set to NULL
    // error code is returned in err.
    // error message can be retrieved with strerror(err);
    char buf[250];
    errno_t e = strerror_s((char *)&buf, 248, err);
    fprintf(stderr, "Cannot open file '%s': %s\n", FileName, buf);
  }
  else
  {
    // File was opened, filepoint can be used to read the stream.
    if (Debug || gGraphDebug)
    {
      fprintf(stdout, "Attempt to open file '%s' for reading\n", FileName);
      fprintf(stdout, "Open input file for reading ..................... OK\n");
    }

    while (true)
    {
      NumReadBytes = fscanf_s(f, "%d %d %d %lf %lf %lf",
        &StartVertexName, &EndVertexName, &id, &(a.K), &(a.R), &(a.H));

      if (NumReadBytes == -1)
        break;
      else if (NumReadBytes != 6)
      {
        fprintf_s(stderr, "Input file is corrupted (Record #%d)\n", int(Archs.size()));
        fclose(f);
        return -1;
      }
      else
      {
        a.StartVertex = StartVertexName;
        AddVertex(StartVertexName);
        a.EndVertex = EndVertexName;
        AddVertex(EndVertexName);

        Archs.push_back(a);
        if (int(Archs.size()) == int(Archs.max_size()))
        {
          // Reallocate memory for Archs array:
          Archs.resize(Archs.size() + 100);
        }
      }
    }
    if (Debug || gGraphDebug)
      fprintf(stdout, "Input file reading complete. %d records have been read\n", int(Archs.size()));
    fclose(f);

    VerticesSort();

    for (int i = 0; i < int(Archs.size()); i++)
    {
      Archs[i].StartVertex = GetVertexIDByName(Archs[i].StartVertex);
      Archs[i].EndVertex = GetVertexIDByName(Archs[i].EndVertex);
    }

  }
  return 0;
}

// Формирование матрицы смежности по массиву дуг графа
int Graph::MSmBuildFromArchs()
{
	/* Построение матрицы смежности по массиву дуг
       На входе - массив структур, каждая из которых описывает дугу в формате:
	   typedef struct arch
	   {
		   int StartVertex;
		   int EndVertex;
		   double R;
		   double Q;
		   double H;
		   double F;
		   double P;
		   double alpha;
		   double L;
		   double K;
	   } ArchType;
     На выходе - квадратная матрица размерности iNumVertices, где
     элементы MSm[i][j] и MSm[j][i] содержат количество дуг между вершинами i и j соответственно
  */

  // Переменная для вывода отладочных сообщений
	int MSmBuildFromArchsDebug = 0;

	if (MSmBuildFromArchsDebug || gGraphDebug)
		fprintf(stdout, "Building MSm matrix from Archs array ............ ");
	
  // Если количество дуг равно 0, граф не был инициализирован, вывод сообщения об ошибке
	if (!int(Archs.size()))
	{
    if (MSmBuildFromArchsDebug || gGraphDebug)
      fprintf(stdout, "FAILED\n");
		fprintf(stderr, "Matrix MSm can not be built. Archs are not defined.\n");
		return -1;
	}

  // Выделение памяти под матрицу размерности (iNumVertices x iNumVertices)
  // Инициализация всех элементов матрицы нулевыми значениями
  MSm.resize(int(Vertices.size()));
  for (int i = 0; i < int(MSm.size()); i++)
  {
    MSm[i].resize(int(Vertices.size()));
    for (int j = 0; j < int(MSm[i].size()); j++)
      MSm[i][j] = 0;
  }

  // Заполнение матрицы смежности
	for (int i = 0; i < int(Archs.size()); i++)
		MSm[Archs[i].StartVertex][Archs[i].EndVertex] = MSm[Archs[i].EndVertex][Archs[i].StartVertex] += 1;

  if (MSmBuildFromArchsDebug || gGraphDebug)
    fprintf(stdout, "OK\n");

	return 0;
}

// Вывод на печать матрицы смежности
void Graph::MSmPrint()
{
  int i, j;
  
  fprintf(stdout, "=============================================================\n Matritsa smezhnosti\n         ");
  
  
  for (std::vector<int>::iterator it = Vertices.begin(); it != Vertices.end(); ++it)
    fprintf(stdout, "%5d ", *it);
  fprintf(stdout, "\n");

  fprintf(stdout, "      +--");
  for (i = 0; i < int(Vertices.size()); i++)
    fprintf(stdout, "------");
  fprintf(stdout, "\n");

  for (i = 0; i < int(Vertices.size()); i++)
	{
    fprintf(stdout, "%5d |  ", Vertices[i]);
		for (j = 0; j < int(Vertices.size()); j++)
			fprintf(stdout, "%5d ", MSm[i][j]);
		fprintf(stdout, "\n");
	}
  fprintf(stdout, "\n");
	return;
}

// Формирование матрицы инцидентности по массиву дуг графа
int Graph::MInBuildFromArchs()
// На входе имеем вектор Archs дуг графа количества Archs.size() такого формата, который содержит поля
// StartVertex  - из какой вершины выходит дуга;
// EndVertex    - в какую вершины входит дуга;
// tree         - признак принадлежности дереву графа:
//          +1  - ветвь дерева;
//          -1  - ветвь антидерева.
// Задача: построить матрицу инцидентности, которая отражает связь между первыми N-1 вершинами графа
// (по количеству независимых переменных, где N - количество вершин графа) и всеми дугами.
// Размерность матрицы инцидентности (int(Vertices.size())-1 x int(Archs.size()))
// Значение элемента матрицы инцидентности:
//            | +1, если дуга j входит в вершину i
// A[i][j] = <  -1, если дуга j выходит из вершины i
//            |  0, если дуга j к вершине i не имеет отношения
//
{
  // Переменная для вывода отладочных сообщений
  int MInBuildFromArchsDebug = 0;

  if (MInBuildFromArchsDebug || gGraphDebug)
    fprintf(stdout, "Building MIn matrix from Archs array ............ ");

  // Если количество дуг равно 0, граф не был инициализирован, вывод сообщения об ошибке
  if (!int(Archs.size()))
  {
    if (MInBuildFromArchsDebug || gGraphDebug)
      fprintf(stdout, "FAILED\n");
    fprintf(stderr, "Matrix MIn can not be built. Archs are not defined.\n");
    return -1;
  }

  // Задание размеров для матрицы инцидентности
  // Определение количества строк, равного (int(Vertices.size()) - 1)
  MIn.resize(int(Vertices.size()) - 1);

  // Определение количества элементов в кажной строке, равного (int(Archs.size()))
  for (int i = 0; i < int(MIn.size()); i++)
    MIn[i].resize(int(Archs.size()));
  

  // Заполнение матрицы инцидентности происходит с выделением подматриц X и Y.
  // Подматрица X размещается слева и содержит iNumTreeVariables столбцов, соответствующих ветвям дерева
  // Подматрица Y размещается справа и содержит iNumATreeVariables столбцов, соответствующих ветвям антидерева
  
  // Индекс столбца матрицы инцидентности
  int col = 0; 
  
  // Сначала включаем в матрицу все ветви, которые принадлежат дереву
  for (int i = 0; i < iNumTreeVariables; i++)
  {
    // Поскольку для формирования независимых уравнений необходимо (int(Vertices.size()) - 1) строк,
    // то строка, соответвующая последней вершине, в матрицу не включается
    if (Archs[iTreeInd[i]].StartVertex < int(Vertices.size()) - 1)
      MIn[Archs[iTreeInd[i]].StartVertex][i] = -1;
    if (Archs[iTreeInd[i]].EndVertex < int(Vertices.size()) - 1)
      MIn[Archs[iTreeInd[i]].EndVertex][i] = 1;
  }
  
  // Затем включаем в матрицу все ветви, которые принадлежат антидереву
  for (int i = 0; i < iNumATreeVariables; i++)
  {
    // Поскольку для формирования независимых уравнений необходимо (int(Vertices.size()) - 1) строк,
    // то строка, соответвующая последней вершине, в матрицу не включается
    if (Archs[iATreeInd[i]].StartVertex < int(Vertices.size()) - 1)
      MIn[Archs[iATreeInd[i]].StartVertex][i + iNumTreeVariables] = -1;
    if (Archs[iATreeInd[i]].EndVertex < int(Vertices.size()) - 1)
      MIn[Archs[iATreeInd[i]].EndVertex][i + iNumTreeVariables] = 1;
  }

  if (MInBuildFromArchsDebug || gGraphDebug)
  {
    fprintf(stdout, "OK\n");
  }
  return 0;
}

// Вывод на печать матрицы инцидентности
int Graph::MInPrint()
{
  int i;
  fprintf(stdout, "=============================================================\n Matritsa intsidentnosti\n         ");
  //for (i = 0; i < iNumTreeVariables; i++)
  //  fprintf(stdout, "%5d ", iTreeInd[i]);
  //for (i = 0; i < iNumATreeVariables; i++)
  //  fprintf(stdout, "%5d ", iATreeInd[i]);
  //fprintf(stdout, "\n");

  for (i = 0; i < iNumTreeVariables; i++)
    fprintf(stdout, "%5d ", Archs[iTreeInd[i]].StartVertex);
  for (i = 0; i < iNumATreeVariables; i++)
    fprintf(stdout, "%5d ", Archs[iATreeInd[i]].StartVertex);
  fprintf(stdout, "\n         ");
  for (i = 0; i < iNumTreeVariables; i++)
    fprintf(stdout, "%5d ", Archs[iTreeInd[i]].EndVertex);
  for (i = 0; i < iNumATreeVariables; i++)
    fprintf(stdout, "%5d ", Archs[iATreeInd[i]].EndVertex);
  fprintf(stdout, "\n");


  fprintf(stdout, "      +--");
  for (i = 0; i < iNumTreeVariables + iNumATreeVariables; i++)
    fprintf(stdout, "------");
  fprintf(stdout, "\n");

  for (int i = 0; i < int(Vertices.size()) - 1; i++)
  {
    fprintf(stdout, "%5d |  ", Vertices[i]);
    for (int j = 0; j < int(Archs.size()); j++)
      fprintf(stdout, "%5d ", MIn[i][j]);
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\n");
  return 0;
}

// Вывод на печать подматрицы инцидентности X
int Graph::MInXPrint()
{
  // Вывод на печать матрицы инцидентности X
  int i;
  fprintf(stdout, "=============================================================\n Podmatritsa intsidentnosti X\n         ");
  for (i = 0; i < iNumTreeVariables; i++)
    fprintf(stdout, "%5d ", iTreeInd[i]);
  fprintf(stdout, "\n");

  fprintf(stdout, "      +--");
  for (i = 0; i < iNumTreeVariables; i++)
    fprintf(stdout, "------");
  fprintf(stdout, "\n");

  for (int i = 0; i < int(Vertices.size()) - 1; i++)
  {
    fprintf(stdout, "%5d |  ", Vertices[i]);
    for (int j = 0; j < iNumTreeVariables; j++)
      fprintf(stdout, "%5d ", MIn[i][j]);
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\n");
  return 0; 
  
  for (int i = 0; i < int(Vertices.size()) - 1; i++)
  {
    for (int j = 0; j < iNumTreeVariables; j++)
      fprintf(stdout, "%5d ", MIn[i][j]);
    fprintf(stdout, "\n");
  }
  return 0;
}

//Вывод на печать подматрицы инцидентности Y
int Graph::MInYPrint()
{
  // Вывод на печать матрицы инцидентности Y
  int i;
  fprintf(stdout, "=============================================================\n Podmatritsa intsidentnosti Y\n         ");
  for (i = 0; i < iNumATreeVariables; i++)
    fprintf(stdout, "%5d ", iATreeInd[i]);
  fprintf(stdout, "\n");

  fprintf(stdout, "      +--");
  for (i = 0; i < iNumATreeVariables; i++)
    fprintf(stdout, "------");
  fprintf(stdout, "\n");

  for (int i = 0; i < int(Vertices.size()) - 1; i++)
  {
    fprintf(stdout, "%5d |  ", Vertices[i]);
    for (int j = iNumTreeVariables; j < iNumTreeVariables + iNumATreeVariables; j++)
      fprintf(stdout, "%5d ", MIn[i][j]);
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\n");
  return 0; 
}

// Вывод в файл матрицы инцидентности
int Graph::MInSaveToFile()
{
  SparseMatrixSaveToFile(Ax, "Ax.txt");
  SparseMatrixSaveToFile(Ay, "Ay.txt");
  return 0;
}

// Формирование матрицы контуров по массиву дуг графа
int Graph::MKonBuildFromArchs()
// На входе имеем вектор Archs дуг графа количества Archs.size() такого формата, который содержит поля
// StartVertex  - из какой вершины выходит дуга;
// EndVertex    - в какую вершины входит дуга;
// tree         - признак принадлежности дереву графа:
//          +1  - ветвь дерева;
//          -1  - ветвь антидерева.
// Задача: построить матрицу контуров, которая отражает состав iNumATreeVeriables контуров
// (по количеству независимых переменных в ветвях антидерева).
// Размерность матрицы контуров (iNumATreeVariables x int(Archs.size()))
// Значение элемента матрицы контуров:
//            | +1, если дуга j входит в контур i и имеет то же направление, что и обход контура
// S[i][j] = <  -1, если дуга j входит в контур i и имеет встречное направление относительно направления обхода контура
//            |  0, если дуга j не входит в контур i
//
{
  // Переменная для вывода отладочных сообщений
  int MKonBuildFromArchsDebug = 0;

  if (MKonBuildFromArchsDebug || gGraphDebug)
    fprintf(stdout, "Building MKon matrix from Archs array ............ ");

  // Если количество дуг равно 0, граф не был инициализирован, вывод сообщения об ошибке
  if (!int(Archs.size()))
  {
    if (MKonBuildFromArchsDebug || gGraphDebug)
      fprintf(stdout, "FAILED\n");
    fprintf(stderr, "Matrix MKon can not be built. Archs are not defined.\n");
    return -1;
  }

  // Задание размеров для матрицы контуров
  // Определение количества строк, равного (iNumATreeVariables)
  MKon.resize(iNumATreeVariables);

  // Определение количества элементов в кажной строке, равного (int(Archs.size()))
  for (int i = 0; i < int(MKon.size()); i++)
    MKon[i].resize(int(Archs.size()));


  // Заполнение матрицы контуров
  // Поиск всех контуров, которые содержат по одной ветви антидерева
  int i;
  // Перебор всех циклов по дугам графа. Их количество iNumArchCycles
  for (int iArchCycleIndex = 0; iArchCycleIndex < int(ArchCycles.size()); iArchCycleIndex++)
  {
    // Здесь имеем цикл ArchCycles[iArchCycleIndex] из VertexCyclesLens[iArchCycleIndex] дуг
    // Проходим сначала по ветвям дерева iTreeInd в количестве iNumTreeVariables, затем по ветвям антидерева iATreeInd в количестве iNumATreeVariables
    // с целью определения, входит ли очередная дуга в имеющийся цикл. Если входит сонаправлено обходу цикла (обход по порядку составляющих его вершин),
    // соответствущий элемент матрицы контуров равен 1; если направлен встречно, то -1; и если не относится к циклу, то 0.
        
    // Определение строки подматрицы контуров X
    for (i = 0; i < iNumTreeVariables; i++)
    {
      MKon[iArchCycleIndex][i] = GetMKonValue(iArchCycleIndex, iTreeInd[i]);
    }
  
    // Определение строки подматрицы контуров Y
    for (i = 0; i < iNumATreeVariables; i++)
    {
      MKon[iArchCycleIndex][iNumTreeVariables + i] = GetMKonValue(iArchCycleIndex, iATreeInd[i]);
    }
  }
  
  if (MKonBuildFromArchsDebug || gGraphDebug)
  {
    fprintf(stdout, "OK\n");
  }
  return 0;
}

// Вывод на печать матрицы контуров
int Graph::MKonPrint()
{
  int i;
  fprintf(stdout, "=============================================================\n Matritsa konturov\n         ");
  for (i = 0; i < iNumTreeVariables; i++)
    fprintf(stdout, "%5d ", iTreeInd[i]);
  for (i = 0; i < iNumATreeVariables; i++)
    fprintf(stdout, "%5d ", iATreeInd[i]);
  fprintf(stdout, "\n");

  fprintf(stdout, "      +--");
  for (i = 0; i < iNumTreeVariables + iNumATreeVariables; i++)
    fprintf(stdout, "------");
  fprintf(stdout, "\n");

  for (int i = 0; i < iNumATreeVariables; i++)
  {
    fprintf(stdout, "%5d |  ", i);
    for (int j = 0; j < int(Archs.size()); j++)
      fprintf(stdout, "%5d ", MKon[i][j]);
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\n");
  return 0;
}

// Вывод на печать подматрицы контуров X
int Graph::MKonXPrint()
{
  int i;
  fprintf(stdout, "=============================================================\n Podmatritsa konturov X\n         ");
  for (i = 0; i < iNumTreeVariables; i++)
    fprintf(stdout, "%5d ", iTreeInd[i]);
  fprintf(stdout, "\n");

  fprintf(stdout, "      +--");
  for (i = 0; i < iNumTreeVariables; i++)
    fprintf(stdout, "------");
  fprintf(stdout, "\n");

  for (int i = 0; i < iNumATreeVariables; i++)
  {
    fprintf(stdout, "%5d |  ", i);
    for (int j = 0; j < iNumTreeVariables; j++)
      fprintf(stdout, "%5d ", MKon[i][j]);
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\n");
  return 0;
} 

// Вывод на печать подматрицы контуров Y
int Graph::MKonYPrint()
{
  int i;
  fprintf(stdout, "=============================================================\n Podmatritsa konturov Y\n         ");
  for (i = 0; i < iNumATreeVariables; i++)
    fprintf(stdout, "%5d ", iATreeInd[i]);
  fprintf(stdout, "\n");

  fprintf(stdout, "      +--");
  for (i = 0; i < iNumATreeVariables; i++)
    fprintf(stdout, "------");
  fprintf(stdout, "\n");

  for (int i = 0; i < iNumATreeVariables; i++)
  {
    fprintf(stdout, "%5d |  ", i);
    for (int j = 0; j < iNumATreeVariables; j++)
      fprintf(stdout, "%5d ", MKon[i][j + iNumTreeVariables]);
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\n");
  return 0;
}

int Graph::MKonSaveToFile()
{
  SparseMatrixSaveToFile(Sx, "Sx.txt");
  SparseMatrixSaveToFile(Sy, "Sy.txt");
  return 0;
}


// Вывод на печать всех контуров графа
int Graph::PrintKonturs()
{
  fprintf(stdout, "=============================================================\n Tsikly grafa\n Nomer tsikla) Vershina-istochnik -(Duga)-> Vershina-priemnik -> ...\n");
  for (int i = 0; i < int(ArchCycles.size()); i++)
    PrintKontur(i);
  fprintf(stdout, "\n");
  return 0;
}

// Формирование массива вершин с присвоением каждой id
int Graph::AddVertex(int name)
{
	// Если имя вершины уже есть в массиве, возвращаем ее id
	for (int i = 0; i < int(Vertices.size()); i++)
		if (Vertices[i] == name)
			return i;

	// Если в массиве вершины с именем name не оказалось, добавляем ее в массив
	Vertices.push_back(name);

	return int(Vertices.size())-1;
}

// Получение имени вершины по её id
int Graph::GetVertexNameByID(int id)
{
  return Vertices[id];
}

// Получение id вершины по её индексу
int Graph::GetVertexIDByName(int name)
{
  for (int i = 0; i < int(Vertices.size()); i++)
    if (Vertices[i] == name)
      return i;
  return -1;
}

// Построение контуров в графе
int Graph::BuildCycles()
{
  // Поиск контуров в графе.
  // На входе - квадратная матрица смежности MSm размерности int(Vertices.size()),
  // элементами которой являются количества дуг между соответствующими вершинами графа
  // На выходе - массивы VertexCycles, VertexCyclesLens и ArchCycles, содержащие циклы, количества вершин и дуги в циклах соответственно
  // Например, VertexCycles[0] = {0, 1, 3, 4, 2}, VertexCyclesLens[0] = 5, ArchCycles[0] =  {0, 2, 6, 5, 1}
  // кодирует цикл из следующих вершин: 0  -(0)->  1  -(2)->  3  -(6)->  4  -(-5)->  2  -(-1)->  0

  VertexCycle.clear();
  ArchCycle.clear();

  std::set < int > notUsedV;
  for (int i = 0; i < int(Vertices.size()); i++)
    notUsedV.insert(i);

  int from, to;
  // Подготовка текущего цикла, который содержит единственную вершину и рекурсивный вызов процедуры поиска цикла
  
  for (int iATreeArchIndex = 0; iATreeArchIndex < iNumATreeVariables; iATreeArchIndex++)
  {
    from = Archs[iATreeInd[iATreeArchIndex]].StartVertex;
    to   = Archs[iATreeInd[iATreeArchIndex]].EndVertex;
    notUsedV.erase(to);
    
    ArchCycle.push_back(iATreeInd[iATreeArchIndex]);
    VertexCycle.push_back(Archs[iATreeInd[iATreeArchIndex]].StartVertex);
    VertexCycle.push_back(Archs[iATreeInd[iATreeArchIndex]].EndVertex);
    
    FindVertexCycle(from, to, notUsedV);
    ArchCycle.clear();
    VertexCycle.clear();
    notUsedV.insert(from);
    notUsedV.insert(to);
  }

  return 0;
}

// Проверка на наличие "висящих" вершин
int Graph::MSmCheckForLonelyVertices()
// Метод определяет количество вершин графа, которые имеют одно или не имеют ни одного соединения с соседними вершинами графа
// Возвращается количество таких вершин
{
  int NumLonelyVertices = 0;
  int VertexConnections;
  for (int i = 0; i < int(Vertices.size()); i++)
  {
    VertexConnections = 0;
    for (int j = 0; j < int(Vertices.size()); j++)
      VertexConnections += MSm[i][j];
    NumLonelyVertices += (VertexConnections < 2) ? 1 : 0;
  }
  return NumLonelyVertices;
}

// Печать всех циклов.
void Graph::PrintAllCycles()
{
  fprintf(stdout, "=============================================================\n Tsikly grafa\n Nomer tsikla) Vershina-istochnik -(Duga)-> Vershina-priemnik -> ...\n");
  for (int i = 0; i < int(ArchCycles.size()); i++)
    PrintCycle(i);
  fprintf(stdout, "\n");
  return;
}

// Вывод на печать всех дуг графа
void Graph::PrintAllArchs()
{
  for (int i = 0; i < int(Archs.size()); i++)
  {
    fprintf(stdout, "%d) ", i);
    PrintArch(Archs[i]);
  }
  return;
}

// Построение дерева и антидерева графа
int Graph::BuildTree()
/*
   Выделим дерево в имеющемся графе.
   Процесс делится на три этапа:
   1. Все кратные ветви относятся к антидереву
   2. Все ветви с источниками (вентиляторами) относятся к дереву
   3. Последовательно вычеркиваем произвольные дуги до тех пор, пока не останется ни одного цикла
*/
{

  // stage 1
  for (int i = 0; i < int(Vertices.size()) - 1; i++)
    for (int j = i + 1; j < int(Vertices.size()); j++)
      while (MSm[i][j] > 1)
        for (int k = 0; k < int(Archs.size()); k++)
        {
          if (Archs[k].tree)
            continue;
          if (((Archs[k].StartVertex == i) && (Archs[k].EndVertex == j)) ||
              ((Archs[k].StartVertex == j) && (Archs[k].EndVertex == i)))
          {
            if (Archs[k].H > 0.0)
              continue;
            Archs[k].tree = -1;
            MSm[i][j]--;
            break;
          }
        }
  
  // stage 2
  for (int k = 0; k < int(Archs.size()); k++)
  {
    if (Archs[k].tree)
      continue;
    if (Archs[k].H > 0.0)
      Archs[k].tree = 1;
  }

  // stage 3
  // Отправлять дуги графа в ветви антидерева до тех пор, пока ни одного цикла не останется.
  // Признак - количество вершин в цикле (длина цикла) будет равняться нулю.
  while (GetMaxCycleLength() > 0)
  {
    int flag = 0;
    // Проходим по всем циклам в поиске такого, чья длина еще не нулевая (цикл все еще замкнут)
    for (int iNumLongestCycle = 0; iNumLongestCycle < int(VertexCycles.size()); iNumLongestCycle++)
    {
      if (flag)
        break;
      // Если длина цикла = 0, он уже был разорван путем выделения ветви антидерева
      if (VertexCyclesLens[iNumLongestCycle] < 0)
        continue;
      // Стараемся разрывать циклы максимальной длины
      if (VertexCyclesLens[iNumLongestCycle] == GetMaxCycleLength())
      {
        // Проходим по массиву дуг в поисках той, которая еще не распределена и является первой в разрываемом цикле 
        for (int iDeletedArch = 0; iDeletedArch < int(Archs.size()); iDeletedArch++)
        {
          if (Archs[iDeletedArch].tree)
            continue;
          if (((Archs[iDeletedArch].StartVertex == VertexCycles[iNumLongestCycle][0]) && (Archs[iDeletedArch].EndVertex == VertexCycles[iNumLongestCycle][1])) ||
            ((Archs[iDeletedArch].StartVertex == VertexCycles[iNumLongestCycle][1]) && (Archs[iDeletedArch].EndVertex == VertexCycles[iNumLongestCycle][0])))
          {
            Archs[iDeletedArch].tree = -1;
            VertexCyclesLens[iNumLongestCycle] = -VertexCyclesLens[iNumLongestCycle];

            // Необходимо обнулисть длины тех циклов, которые содержат найденную дугу
            for (int iDeletedCycle = 0; iDeletedCycle < int(VertexCycles.size()); iDeletedCycle++)
            {
              if (VertexCyclesLens[iDeletedCycle] < 0)
                continue;
              for (int iNextArchInCycle = 0; iNextArchInCycle < VertexCyclesLens[iDeletedCycle] - 1; iNextArchInCycle++)
              {
                if (((Archs[iDeletedArch].StartVertex == VertexCycles[iDeletedCycle][iNextArchInCycle]) && (Archs[iDeletedArch].EndVertex == VertexCycles[iDeletedCycle][iNextArchInCycle + 1])) ||
                  ((Archs[iDeletedArch].StartVertex == VertexCycles[iDeletedCycle][iNextArchInCycle + 1]) && (Archs[iDeletedArch].EndVertex == VertexCycles[iDeletedCycle][iNextArchInCycle])))
                {
                  VertexCyclesLens[iDeletedCycle] = -VertexCyclesLens[iDeletedCycle];
                  break;
                }
              }
            }
            flag = 1;
            break;
          }
        }
      }
    }
  }

  // stage 4
  // Остальные дуги должны быть ветвями дерева
  for (int j = 0; j < int(Archs.size()); j++)
  {
    if (!Archs[j].tree)
      Archs[j].tree = 1;
  }

  // Определение количества неизвестных в ветвях дерева и антидерева
  iNumTreeVariables = int(Vertices.size()) - 1;
  iNumATreeVariables = int(Archs.size()) - iNumTreeVariables;

  for (int i = 0; i < int(VertexCycles.size()); i++)
    VertexCyclesLens[i] = -VertexCyclesLens[i];

  // Формирование массивов индексов дуг, которые формируют дерево и антидерево
  int iT = 0, iAT = 0;
  for (int i = 0; i < int(Archs.size()); i++)
  {
    if (Archs[i].tree == 1)
      iTreeInd.push_back(i);
    else if (Archs[i].tree == -1)
      iATreeInd.push_back(i);
  }
  return 0;
}

// Построение дерева графа по методу Прима
int Graph::BuildTree_Prim()
// На первом гаше алгоритма определеяет ребро с наименьшим весом.
// Найденно ребро и две вершины, которые оно связывает, являются основой дерева.
// Далее в дерево последоватиельно добавляются ребра, инцидентные тем вершинам, которые уже есть в графе.
// Если очередное ребро связывает с уже входящей в дерево вершиной, найден цикл, и данное ребро не рассматривается.
// Не входе алгоритма количетсво вершин int(Vertices.size()), список дуг Archs и вектор ветвей дерева iTreeInd;
{
  std::vector < ArchLight > notUsedE;
  std::vector < ArchLight > ETree;

  ArchLight e;
  std::set < int > notUsedV;
  std::set < int > UsedV;

  for (int i = 0; i < int(Vertices.size()); i++)
    notUsedV.insert(i);

  for (std::vector < ArchType > ::iterator it = Archs.begin(); it != Archs.end(); it++)
  {
    e.v1 = (*it).StartVertex;
    e.v2 = (*it).EndVertex;
    e.weight = (*it).H;
    notUsedE.push_back(e);
  }

  UsedV.insert(0);
  notUsedV.erase(0);

  while (!notUsedV.empty())
  {
    int minE = -1; //номер наименьшего ребра
                   //поиск наименьшего ребра
    for (int i = 0; i < int(notUsedE.size()); i++)
    {
      if ((UsedV.find(notUsedE[i].v1) != UsedV.end()) && (notUsedV.find(notUsedE[i].v2) != notUsedV.end()) ||
          (UsedV.find(notUsedE[i].v2) != UsedV.end()) && (notUsedV.find(notUsedE[i].v1) != notUsedV.end()))
      {
        if (minE != -1)
        {
          if (notUsedE[i].weight < notUsedE[minE].weight)
            minE = i;
        }
        else
          minE = i;
      }
    }
    //заносим новую вершину в список использованных и удаляем ее из списка неиспользованных
    if (UsedV.find(notUsedE[minE].v1) != UsedV.end())
    {
      UsedV.insert(notUsedE[minE].v2);
      notUsedV.erase(notUsedV.find(notUsedE[minE].v2));
    }
    else
    {
      UsedV.insert(notUsedE[minE].v1);
      notUsedV.erase(notUsedV.find(notUsedE[minE].v1));
    }
    //заносим новое ребро в дерево и удаляем его из списка неиспользованных
    ETree.push_back(notUsedE[minE]);
    //notUsedE.RemoveAt(minE);
  }

  iNumTreeVariables = 0;
  iNumATreeVariables = 0;

  for (std::vector < ArchLight > ::iterator it = ETree.begin(); it != ETree.end(); it++)
  {
    int ind = GetArchIndexByVertices((*it).v1, (*it).v2, 0);
    Archs[ind].tree = 1;
    iTreeInd.push_back(ind);
    iNumTreeVariables++;
  }

  for (int i = 0; i < int(Archs.size()); i++)
  {
    if (Archs[i].tree != 1)
    {
      Archs[i].tree = -1;
      iATreeInd.push_back(i);
      iNumATreeVariables++;
    }
  }

  int i = 0;
  return 0;
}

int Graph::BuildTree_TO_DELETE()
{
  //int n = 0;
  //iNumTreeVariables = 0;

  //n =   1; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =   2; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =   3; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =   4; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =   5; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =   7; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =   8; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =   9; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  10; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  13; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  14; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  15; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  17; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  19; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  20; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  21; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  22; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  23; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  24; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  25; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  27; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  29; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  36; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  37; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  39; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  40; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  46; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  47; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  48; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  50; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  51; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  54; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  56; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  57; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  58; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  61; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  62; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  63; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  72; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  75; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  76; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  77; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  78; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  79; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  80; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  81; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  82; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  83; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  88; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  93; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n =  98; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n = 100; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n = 102; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n = 103; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n = 105; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n = 108; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n = 110; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n = 113; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n = 116; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;
  //n = 117; iTreeInd.push_back(n-1); iNumTreeVariables++; Archs[n-1].tree = 1;

  //iNumATreeVariables = 0;

  //n =   6; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  11; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  12; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  16; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  18; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  26; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  28; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  30; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  31; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  32; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  33; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  34; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  35; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  38; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  41; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  42; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  43; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  44; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  45; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  49; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  52; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  53; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  55; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  59; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  60; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  64; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  65; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  66; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  67; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  68; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  69; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  70; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  71; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  73; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  74; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  84; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  85; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  86; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  87; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  89; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  90; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  91; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  92; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  94; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  95; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  96; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  97; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n =  99; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n = 101; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n = 104; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n = 106; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n = 107; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n = 109; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n = 111; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n = 112; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n = 114; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;
  //n = 115; iATreeInd.push_back(n-1); iNumATreeVariables++; Archs[n-1].tree = -1;


  int n = 0;
  iNumTreeVariables = 0;

  n = 1; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 102; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 117; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 103; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 2; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 10; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 9; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 3; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 36; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 14; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 13; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 82; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 4; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 5; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 72; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 37; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 98; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 17; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 21; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 83; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 15; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 88; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 7; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 51; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 113; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 47; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 39; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 56; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 50; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 75; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 22; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 76; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 23; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 19; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 77; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 78; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 8; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 54; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 63; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 48; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 110; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 40; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 79; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 57; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 100; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 116; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 80; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 25; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 105; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 27; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 24; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 20; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 58; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 62; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 93; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 46; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 29; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 108; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 61; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;
  n = 81; iTreeInd.push_back(n - 1); iNumTreeVariables++; Archs[n - 1].tree = 1;

  iNumATreeVariables = 0;

  n = 6; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 32; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 33; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 34; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 73; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 52; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 64; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 65; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 31; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 89; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 109; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 53; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 38; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 114; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 115; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 97; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 49; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 55; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 99; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 60; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 41; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 42; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 43; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 96; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 95; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 59; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 44; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 45; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 92; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 94; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 91; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 112; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 111; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 90; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 71; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 26; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 28; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 35; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 66; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 67; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 16; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 84; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 85; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 18; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 74; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 104; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 106; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 87; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 107; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 30; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 86; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 12; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 11; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 101; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 68; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 69; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;
  n = 70; iATreeInd.push_back(n - 1); iNumATreeVariables++; Archs[n - 1].tree = -1;


  return 0;
}

// Сортировка имен вершин графа
void Graph::VerticesSort()
{
  std::sort(Vertices.begin(), Vertices.end());
}

// Поиск контура в графе по двум вершинам, которые замыкаются ветвью антидерева
void Graph::FindVertexCycle(int from, int to, std::set < int > notUsedV)
// Рекурсивный поиск контуров графа
// На входе - индексы двух вершин, которые являются крайними в текущей цепочке.
// Если при попытке найти следующую вершину, контур замыкается, вносим его в вектор контуров ка кпо вершинам, так и по дугам
// Используется также множество незадействованных вершин notUsedV, которые определяют потенциальные вершины для продолжения контура
{
  // Проходим по множеству неиспользованных вершин
  // Пытаемся определить, есть ли дуга, которая соединяет последнюю вершину цепочки (to) с вершиной-претендентом
  int nextTo;
  for (std::set < int > ::iterator it = notUsedV.begin(); it != notUsedV.end(); it++)
  {
    int iNextParallArch = GetArchIndexByVertices(to, (*it), 0);
    // При наличии такой вершины возможны два варианта:
    if (iNextParallArch != -1)
    {
      if (Archs[iNextParallArch].tree == -1)
        continue;
      std::set < int > nextNotUsedV = notUsedV;

      // Вершина равна исходной (from), с которой мы пришли. Тогда найден контур, сохраняем его в векторах.
      if ((*it) == from)
      {
        int k = 0;
        // Вставить проверку на наличие эквивалентного цикла ??
        ArchCycle.push_back(iNextParallArch); 
        ArchCycles.push_back(ArchCycle);
        VertexCycles.push_back(VertexCycle);
        VertexCyclesLens.push_back(int(VertexCycle.size()));
      }
      // Вершина не равна исходной, исключаем ее из массива неиспользованных, продолжаем поиск рекурсивно.
      else
      {
        nextTo = (*it);
        nextNotUsedV.erase(nextTo);
        ArchCycle.push_back(iNextParallArch);
        VertexCycle.push_back(nextTo);
        FindVertexCycle(from, nextTo, nextNotUsedV);
        VertexCycle.pop_back();
        ArchCycle.pop_back();
        nextNotUsedV.insert(nextTo);
      }
    }
  }
}

// Проверка на наличие вершины в текущем цикле
int Graph::GetPlaceInCycle(std::vector < int > VertexCycle, int id)
{
  for (int i = 0; i < int(VertexCycle.size()); i++)
    if (VertexCycle[i] == id)
      return i;
  return -1;
}

// Сравнение двух циклов на эквивалентность
int Graph::AreCyclesDifferent(std::vector < int > cycle1, std::vector < int > cycle2)
{
  int len1 = int(cycle1.size());
  int AreDifferent = 0;
  // Для всех вершин цикла 2

  for (int cycle2place = 0; cycle2place < len1; cycle2place++)
  {
    // Определяем позицию текущей вершины в цикле 1
    int cycle1place = GetPlaceInCycle(cycle1, cycle2[cycle2place]);
    // Если вершины в цикл 1 не входит, циклы не эквивалентны, можно выходить
    if (cycle1place == -1)
      return -1;

    // Служебные переменные, означающие следующее:
    // AreNormalNeighboursEqual  = 1, если левый сосед в цикле 1 равен левому в цикле 2 и правый в цикле 1 равен правому в цикле 2
    // AreReverseNeighboursEqual = 1, если левый сосед в цикле 1 равен правому в цикле 2 и правый в цикле 1 равен левому в цикле 2
    int AreNormalNeighboursEqual;
    int AreReverseNeighboursEqual;
    
    if ((cycle1[(cycle1place - 1 + len1) % len1] == cycle2[(cycle2place - 1 + len1) % len1]) && (cycle1[(cycle1place + 1 + len1) % len1] == cycle2[(cycle2place + 1 + len1) % len1]))
      AreNormalNeighboursEqual = 1;
    else
      AreNormalNeighboursEqual = 0;
    
    if ((cycle1[(cycle1place - 1 + len1) % len1] == cycle2[(cycle2place + 1 + len1) % len1]) && (cycle1[(cycle1place + 1 + len1) % len1] == cycle2[(cycle2place - 1 + len1) % len1]))
      AreReverseNeighboursEqual = 1;
    else
      AreReverseNeighboursEqual = 0;
    
    // Если ни прямо, ни обратно соседи не равны, циклы не эквивалентны
    if (AreNormalNeighboursEqual + AreReverseNeighboursEqual == 0)
      return -1;
  }
  // Если дошли сюда, то для всех вершин цикла 1 соседние вершины равны (в прямом или обратном порядке) таковым для вершин цмкла 2
  // Циклы эквивалентны
  return 0;
}

// Вывод на печать цикла графа.
void Graph::PrintCycle(int iCycleIndex)
{ 
  for (int i = 0; i < int(ArchCycles[iCycleIndex].size()); i++)
    if (i == 0)
      fprintf_s(stdout, "%3d) %d", iCycleIndex, VertexCycles[iCycleIndex][i]);
    else
      fprintf_s(stdout, " -(%d)-> %d", ArchCycles[iCycleIndex][i-1], VertexCycles[iCycleIndex][i]);
  fprintf_s(stdout, " -(%d)-> %d\n", ArchCycles[iCycleIndex][int(ArchCycles[iCycleIndex].size()) - 1], VertexCycles[iCycleIndex][0]);
  return;
}

// Вывод на печать контур графа.
void Graph::PrintKontur(int iCycleIndex)
{
  for (int i = 0; i < int(ArchCycles[iCycleIndex].size()); i++)
    if (i == 0)
      fprintf_s(stdout, "%3d) %d", iCycleIndex, VertexCycles[iCycleIndex][i]);
    else
      fprintf_s(stdout, " -(%d)-> %d", ArchCycles[iCycleIndex][i - 1], VertexCycles[iCycleIndex][i]);
  fprintf_s(stdout, " -(%d)-> %d\n", ArchCycles[iCycleIndex][int(ArchCycles[iCycleIndex].size()) - 1], VertexCycles[iCycleIndex][0]);
  return;
}

// Вывод на печать дуги графа
void Graph::PrintArch(ArchType a)
{
  fprintf(stdout, "%4d --> %4d:  %8.3lf  %8.3lf  %8.3lf  %8.3lf  %8.3lf  %8.3lf  %8.3lf  %8.3lf %2d\n",
    a.StartVertex, a.EndVertex, a.R, (a.Q), (a.H), (a.F), (a.P), (a.alpha), (a.L), (a.K), a.tree);
  return;
}

// Определение максимальной из длин циклов
int Graph::GetMaxCycleLength()
{
  int iMax = -1;
  for (int i = 0; i < int(VertexCycles.size()); i++)
    iMax = (iMax > VertexCyclesLens[i]) ? iMax : VertexCyclesLens[i];
  return iMax;
}

// Поиск всех дуг, соединяющей две заданные вершины
int Graph::GetArchIndexByVertices(int StartVertex, int EndVertex, int StartSearchPosition)
{
  for (int i = StartSearchPosition; i < int(Archs.size()); i++)
  {
    if ((Archs[i].StartVertex == StartVertex) && (Archs[i].EndVertex == EndVertex))
      return i;
    if ((Archs[i].StartVertex == EndVertex) && (Archs[i].EndVertex == StartVertex))
      return i;
  }
  return -1;
}

// Получение количества ветвей антидерева в цикле и номера последней из них
int Graph::GetNumATreeArchsInCycle(int iArchCycleIndex, int * iLastATreeArchIndex)
{
  int num = 0;
  int iLastIndex;
  for (int i = 0; i < VertexCyclesLens[iArchCycleIndex]; i++)
  {
    if (IsATreeArch(ArchCycles[iArchCycleIndex][i]))
    {
      num++;
      iLastIndex = i;
    }
  }
  *iLastATreeArchIndex = iLastIndex;
  return num;
}

// Проверка принадлежности ветви к дереву графа
bool Graph::IsATreeArch(int iArchIndex)
{
  for (int i = 0; i < iNumATreeVariables; i++)
    if (iArchIndex == iATreeInd[i])
      return true;
  return false;
}

// Определение вхождения дуги графа в контур
int Graph::GetMKonValue(int iArchCycleIndex, int iNumArch)
// На входе имеем:
// iArchCycleIndex - индекс цикла ArchCycles из дуг, который замыкается очередной ветвью антидерева
// iNumArch - индекс цикла Archs, которая является очередной ветвью антидерева, замыкающей контур
// Например, имеем цикл
// 0) 0 -(0)-> 1 -(2)-> 3 -(6)-> 4 -(7)-> 0,
// где iArchCycleIndex = 0,
//     ArchCycles[iArchCycleIndex] = {0, 2, 6, 7};
//     VertexCycles[iArchCycleIndex] = {0, 1, 3, 4};
//     VertexCyclesLens[iArchCycleIndex] = 4 - длина циклов
//            ArchCycles[iArchCycleIndex] и VertexCycles[iArchCycleIndex];
//     iNumArch = 2;
//     Archs[iNumArch].StartVertex = 1;
//     Archs[iNumArch].EndVertex = 3;
// Поскольку Archs[iNumArch].StartVertex встретился раньше в VertexCycles[iArchCycleIndex],
// то искомое значение = 1.
// Если Archs[iNumArch].EndVertex встретится раньше в VertexCycles[iArchCycleIndex],
// то искомое значение будет равно -1.

{
  int from = Archs[iNumArch].StartVertex;
  int to = Archs[iNumArch].EndVertex;
  int value = 0;
  // Порядок вершин текущего цикла графа
  for (int i = 0; i < VertexCyclesLens[iArchCycleIndex]; i++)
  {
    //if (Archs[iNumArch] != ArchCycles[iArchCycleIndex][i])
    //  break;
    if ((VertexCycles[iArchCycleIndex][i] == from) && (VertexCycles[iArchCycleIndex][(i + 1) % VertexCyclesLens[iArchCycleIndex]] == to))
    {
      if (ArchCycles[iArchCycleIndex][i] == iNumArch)
      {
        value = 1;
        break;
      }
    }
    if ((VertexCycles[iArchCycleIndex][(i+1) % VertexCyclesLens[iArchCycleIndex]] == from) && (VertexCycles[iArchCycleIndex][i] == to))
    {
      if (ArchCycles[iArchCycleIndex][i] == iNumArch)
      {
        value = -1;
        break;
      }
    }
  }
  return value;
}

int Graph::PrintDoubleMatrixToFile(char * FileName, double *m, int c, int r)
{
  FILE *stream;
  if (fopen_s(&stream, FileName, "w") != 0)
  {
    printf("The file %s was not opened for writing\n", FileName);
    return -1;
  }

  for (int i = 0; i < r; i++)
  {
    for (int j = 0; j < c; j++)
      fprintf(stream, "%15.10lf ", m[i * c + j]);
    fprintf(stream, "\n");
  }
  fprintf(stream, "\n");

  _fcloseall();
  return 0;
}













// Файл содержит процедуры обработки матриц, представленных в формате Sparse
// Формат Sparse целесообразно использовать для обработки разреженных матриц
// Пример представления матрицы в формате Sparse
//      | 3  1  1  0  1 |
//      | 1 -1  0  1  0 | 
//  M = | 1  0  0  2  1 |
//      | 0  1  2  7  1 |
//      | 1  0  1  1  4 |
//
//  typedef struct SparseMatrixStruct
//  {
//  int rows;   - количество строк матрицы
//  int cols;   - количество столбцов матрицы
//  int *altr;  - ненулевые элементы матрицы слева-направо сверху-вниз без диагональных
//  int *jptr;  - номера столбцов ненулевых элементов матрицы
//  int *iptr;  - индексы в массиве jptr первых ненулевых элементов каждой строки. 
//                Элемент iptr[cols] сожердит общее количество ненулевых недиагональных элементов матрицы.
//  } SparseMatrixType;
// 
// rows  = 5;
// cols  = 5;
// altr  = { 3, 1, 1, 1,  1,-1, 1,  1, 2, 1,  1, 2, 7, 1,  1, 1, 1, 4 }
// jptr  = { 0, 1, 2, 4,  0, 1, 3,  0, 3, 4,  1, 2, 3, 4,  0, 2, 3, 4 }
// iptr  = { 0,           4,        7,       10,          14,          18}
// 

// Процедура перевода произвольной матрицы в Sparse формат
int Graph::ConvertMatrixToSparseFormat(std::vector < std::vector < int > > M, SparseMatrixType *SEM)
// Процедура преобразует прямоугольную матрицу в Sparse формат
// Матрица описана как вектор векторов типа int. На выходе - структура, соответствующая преобразованной матрице.
// Выделение памяти (переопределение размера) реализовано автоматически внутри текущей процедуры
{
  // Входящая матрица пустая, преобразовывать нечего. Код ошибки 1.
  if (!int(M.size()))
    return 1;
  else
    SEM->rows = int(M.size());

  // Матрица содержит пустые вектора, преобразовывать нечего. Код ошибки 2.
  if (!int(M[0].size()))
    return 2;
  else
    SEM->cols = int(M[0].size());

  // Подсчитаем количество ненулевых элементов для выделения памяти для Sparse-матрицы
  int iNumNonZeroElements = 0;
  for (int i = 0; i < SEM->rows; i++)
    for (int j = 0; j < SEM->cols; j++)    
      iNumNonZeroElements += int(M[i][j] != 0);

  SEM->altr = new double[iNumNonZeroElements];
  if (SEM->altr == NULL)
  {
    fprintf(stderr, "Error. Memory for SEM.altr was not allocated\n");
    return 3;
  }

  SEM->jptr = new int[iNumNonZeroElements];
  if (SEM->jptr == NULL)
  {
    fprintf(stderr, "Error. Memory for SEM.jptr was not allocated\n");
    return 3;
  }

  SEM->iptr = new int[SEM->rows + 1];
  if (SEM->iptr == NULL)
  {
    fprintf(stderr, "Error. Memory for SEM.iptr was not allocated\n");
    return 3;
  }

  int i_altr = 0, i_autr = 0, i_jptr = 0;

  SEM->iptr[0] = 0;

  for (int i = 0; i < SEM->rows; i++)
  {
    SEM->iptr[i + 1] = SEM->iptr[i];
    for (int j = 0; j < SEM->cols; j++)
      if (M[i][j])
      {        
        SEM->altr[i_altr++] = M[i][j];
        SEM->jptr[i_jptr++] = j;
        (SEM->iptr[i + 1])++;
      }
  }
  return 0;
} // SparseMatrixType Graph::MSmConvertToSparseFormat(std::vector < std::vector < int > > M, int *error)


// Сложение разреженных матриц, представленных в формате Sparse
int Graph::SparseMatricesAdd(SparseMatrixType m1, SparseMatrixType m2, SparseMatrixType *result)
// Для успешного выполнение сложения матрицы должны иметь одинаковые ненулевые размерности
// Далее размер результирующей матрицы устанавливается равным размерам входных матриц
// Затем каждый элемент нововй матрицы определяется по формуле
// result[i][j] = m1[i][j] + m2[i][j];
// Сложность заключается в том, что матрицы представлены в формате Sparse (описание в начале текущего файла)
{
  std::vector < double > altr;
  std::vector < int > jptr;
  std::vector < int > iptr;

  // Если входные матрицы некорректной или неодинаковой размерности, складывать нечего
  if (m1.rows <= 0)
    return -1;
  if (m1.cols <= 0)
    return -1;
  if (m1.rows != m2.rows)
    return -1;
  if (m1.cols != m2.cols)
    return -1;

  // Приравниваем размеры новой матрицы к размерам существующих
  int c = m1.cols;
  int r = m1.rows;

  int iNumResultNonZeroElements = 0;
  
  // индексы в массиве индексов ненулевых элементов
  int im1, im2; 
  
  // количества ненулевых элементов текущей строки в матрицах
  int num1, num2;

  // Сам процесс сложения может дать от m1.cols до нуля результатов
  // в зависимости от значений элементов матрицы и их взаимного расположения
  // Проходим последовательно по каждой строке матриц
  for (int i = 0; i < r; i++)
  {
    // Сохраним первое значение результирующего массива, iptr[0]
    // m3.iptr[i] = iNumResultNonZeroElements;

    // Определяем индексы в массивах jptr, которые являются номерами столбцов
    // ненулевых элементов текущей строки
    im1 = m1.iptr[i];
    im2 = m2.iptr[i];

    // Определяем количества ненулевых элементов в строках матриц
    num1 = m1.iptr[i + 1] - m1.iptr[i];
    num2 = m2.iptr[i + 1] - m2.iptr[i];

    while (num1 || num2)
    {
      if (num1 && !num2)
      {
        altr.push_back(m1.altr[im1]);
        jptr.push_back(m1.jptr[im1++]);
        iNumResultNonZeroElements++;
        num1--;
      }
      if (!num1 && num2)
      {
        altr.push_back(m2.altr[im2]);
        jptr.push_back(m2.jptr[im2++]);
        iNumResultNonZeroElements++;
        num2--;
      }
      if (num1 && num2)
      {
        if (m1.jptr[im1] == m2.jptr[im2])
        {
          if (abs(m1.altr[im1] + m2.altr[im2]) > EPS)
          {
            altr.push_back(m1.altr[im1] + m2.altr[im2]);
            jptr.push_back(m1.jptr[im1]);
            iNumResultNonZeroElements++;
          }
          im1++;
          im2++;
          num1--;
          num2--;
        }
        else if (m1.jptr[im1] < m2.jptr[im2])
        {
          altr.push_back(m1.altr[im1]);
          jptr.push_back(m1.jptr[im1++]);
          iNumResultNonZeroElements++;
          num1--;
        }
        else
        {
          altr.push_back(m2.altr[im2]);
          jptr.push_back(m2.jptr[im2++]);
          iNumResultNonZeroElements++;
          num2--;
        }
      }
    }
    iptr.push_back(iNumResultNonZeroElements);
  }

  if (result->altr != NULL)
  {
    delete[] result->altr;
    result->altr = NULL;
  }
  result->altr = new double[iNumResultNonZeroElements];
  if (result->altr == NULL)
  {
    fprintf(stderr, "Error. Memory was not allocated\n");
    return -1;
  }

  if (result->jptr != NULL)
  {
    delete[] result->jptr;
    result->jptr = NULL;
  }
  result->jptr = new int[iNumResultNonZeroElements];
  if (result->jptr == NULL)
  {
    fprintf(stderr, "Error. Memory was not allocated\n");
    return -1;
  }

  if (result->iptr != NULL)
  {
    delete[] result->iptr;
    result->iptr = NULL;
  }
  result->iptr = new int[r + 1];
  if (result->iptr == NULL)
  {
    fprintf(stderr, "Error. Memory was not allocated\n");
    return -1;
  }

  for (int i = iNumResultNonZeroElements; i > 0 ; i--)
  {
    result->altr[i - 1] = altr.back();
    altr.pop_back();
    result->jptr[i - 1] = jptr.back();
    jptr.pop_back();
  }

  for (int i = r; i > 0; i--)
  {
    result->iptr[i] = iptr.back();
    iptr.pop_back();
  }

  // Сохраним последнее значение результирующего массива, iptr[result.rows]
  result->iptr[0] = 0;

  result->cols = c;
  result->rows = r;

  return 0;
}

// Вычитание разреженных матриц, представленных в формате Sparse
int Graph::SparseMatricesSub(SparseMatrixType m1, SparseMatrixType m2, SparseMatrixType * result)
// Для успешного выполнение вычитания матрицы должны иметь одинаковые ненулевые размерности
// Далее размер результирующей матрицы устанавливается равным размерам входных матриц
// Затем каждый элемент нововй матрицы определяется по формуле
// result[i][j] = m1[i][j] - m2[i][j];
// Сложность заключается в том, что матрицы представлены в формате Sparse (описание в начале текущего файла)
{
  std::vector < double > altr;
  std::vector < int > jptr;
  std::vector < int > iptr;

  // Если входные матрицы некорректной или неодинаковой размерности, складывать нечего
  if (m1.rows <= 0)
    return -1;
  if (m1.cols <= 0)
    return -1;
  if (m1.rows != m2.rows)
    return -1;
  if (m1.cols != m2.cols)
    return -1;

  // Приравниваем размеры новой матрицы к размерам существующих
  int c = m1.cols;
  int r = m1.rows;

  int iNumResultNonZeroElements = 0;

  // индексы в массиве индексов ненулевых элементов
  int im1, im2;

  // количества ненулевых элементов текущей строки в матрицах
  int num1, num2;

  // Сам процесс вычитания может дать раное количество результатов
  // в зависимости от значений элементов матрицы и их взаимного расположения
  // Проходим последовательно по каждой строке матриц
  for (int i = 0; i < r; i++)
  {
    // Определяем индексы в массивах jptr, которые являются номерами столбцов
    // ненулевых элементов текущей строки
    im1 = m1.iptr[i];
    im2 = m2.iptr[i];

    // Определяем количества ненулевых элементов в строках матриц
    num1 = m1.iptr[i + 1] - m1.iptr[i];
    num2 = m2.iptr[i + 1] - m2.iptr[i];

    while (num1 || num2)
    {
      if (num1 && !num2)
      {
        altr.push_back(m1.altr[im1]);
        jptr.push_back(m1.jptr[im1++]);
        iNumResultNonZeroElements++;
        num1--;
      }
      if (!num1 && num2)
      {
        altr.push_back(-1.0 * m2.altr[im2]);
        jptr.push_back(m2.jptr[im2++]);
        iNumResultNonZeroElements++;
        num2--;
      }
      if (num1 && num2)
      {
        if (m1.jptr[im1] == m2.jptr[im2])
        {
          if (abs(m1.altr[im1] - m2.altr[im2]) > EPS)
          {
            altr.push_back(m1.altr[im1] - m2.altr[im2]);
            jptr.push_back(m1.jptr[im1]);
            iNumResultNonZeroElements++;
          }
          im1++;
          im2++;
          num1--;
          num2--;
        }
        else if (m1.jptr[im1] < m2.jptr[im2])
        {
          altr.push_back(m1.altr[im1]);
          jptr.push_back(m1.jptr[im1++]);
          iNumResultNonZeroElements++;
          num1--;
        }
        else
        {
          altr.push_back(-1.0 * m2.altr[im2]);
          jptr.push_back(m2.jptr[im2++]);
          iNumResultNonZeroElements++;
          num2--;
        }
      }
    }
    iptr.push_back(iNumResultNonZeroElements);
  }

  if (result->altr != NULL)
  {
    delete[] result->altr;
    result->altr = NULL;
  }
  result->altr = new double[iNumResultNonZeroElements];
  if (result->altr == NULL)
  {
    fprintf(stderr, "Error. Memory was not allocated\n");
    return -1;
  }

  if (result->jptr != NULL)
  {
    delete[] result->jptr;
    result->jptr = NULL;
  }
  result->jptr = new int[iNumResultNonZeroElements];
  if (result->jptr == NULL)
  {
    fprintf(stderr, "Error. Memory was not allocated\n");
    return -1;
  }

  if (result->iptr != NULL)
  {
    delete[] result->iptr;
    result->iptr = NULL;
  }
  result->iptr = new int[r + 1];
  if (result->iptr == NULL)
  {
    fprintf(stderr, "Error. Memory was not allocated\n");
    return -1;
  }

  for (int i = iNumResultNonZeroElements; i > 0; i--)
  {
    result->altr[i - 1] = altr.back();
    altr.pop_back();
    result->jptr[i - 1] = jptr.back();
    jptr.pop_back();
  }

  for (int i = r; i > 0; i--)
  {
    result->iptr[i] = iptr.back();
    iptr.pop_back();
  }
  result->iptr[0] = 0;

  result->cols = c;
  result->rows = r;

  return 0;
}


// Умножение разреженных матриц, представленных в формате Sparse
int Graph::SparseMatricesMul(SparseMatrixType m1, SparseMatrixType m2, SparseMatrixType * result)
// Для успешного выполнение усножения матрицы должны иметь соответствующие ненулевые размерности
// Далее размер результирующей матрицы устанавливается согласно размерам входных матриц
// Затем каждый элемент нововй матрицы определяется по формуле
// result[i][k] = sum(j)<m1[i][j] * m2[j][k]>;
// Сложность заключается в том, что матрицы представлены в формате Sparse
{
  if (m1.cols != m2.rows)
    return -1;
  int r = m1.rows;
  int c = m2.cols;

  // Переменная для накопления частичной суммы
  double temp;
  int iNumNonZeroElements = 0;

  std::queue < double > q_val;
  std::queue < int > q_i, q_j;

  // Последовательно проходим все m1.rows строк первой матрицы
  for (int i = 0; i < m1.rows; i++)
  {
    // Далее перебираем все m2.cols столбцов второй матрицы
    for (int j = 0; j < m2.cols; j++)
    {
      temp = 0;

      for (int k = m1.iptr[i]; k < m1.iptr[i + 1]; k++)
      {
        // Следующий ненулевой элемент строки первой матрицы
        int m1_nonzero_j = m1.jptr[k];

        int m2_nonzero_j = m2.iptr[m1_nonzero_j];

          while ((m2.jptr[m2_nonzero_j] < j) && (m2_nonzero_j < m2.iptr[m1_nonzero_j + 1]))
          {
            m2_nonzero_j++;
          }
          if ((m2.jptr[m2_nonzero_j] == j) && (m2_nonzero_j < m2.iptr[m1_nonzero_j + 1]))
            temp += m1.altr[k] * m2.altr[m2_nonzero_j];

      }
      if (abs(temp) > EPS)
      {
        q_val.push(temp);
        q_j.push(j);
        iNumNonZeroElements++;
      }
    }  
    q_i.push(iNumNonZeroElements);
  }
  
  if (result->altr != NULL)
  {
    delete[] result->altr;
    result->altr = NULL;
  }
  result->altr = new double[iNumNonZeroElements];
  if (result->altr == NULL)
  {
    fprintf(stderr, "Error. Memory was not allocated\n");
    return -1;
  }

  if (result->jptr != NULL)
  {
    delete[] result->jptr;
    result->jptr = NULL;
  }
  result->jptr = new int[iNumNonZeroElements];
  if (result->jptr == NULL)
  {
    fprintf(stderr, "Error. Memory was not allocated\n");
    return -1;
  }

  if (result->iptr != NULL)
  {
    delete[] result->iptr;
    result->iptr = NULL;
  }
  result->iptr = new int[r + 1];
  if (result->iptr == NULL)
  {
    fprintf(stderr, "Error. Memory was not allocated\n");
    return -1;
  }

  for (int i = 0; i < iNumNonZeroElements; i++)
  {
    result->altr[i] = q_val.front(); q_val.pop();
    result->jptr[i] = q_j.front(); q_j.pop();
  }

  result->iptr[0] = 0;
  for (int i = 1; i <= r; i++)
  {
    result->iptr[i] = q_i.front();
    q_i.pop();
  }
  
  result->cols = c;
  result->rows = r;

  return 0;
}

// Умножение строки матрицы, представленной в формате Sparse, на константу
int Graph::SparseMatrixLineMulConst(SparseMatrixType *m, int row, double value)
// Входные параметры
// m     - исходная матрица в Sparse-формате
// row   - индекс строки, которая умножается на константу
// value - значение константы, на которую умножается строка матрицы
//
// Для успешного выполнение умножения количество строк матрицы должны быть не меньше значения row
// Затем каждый элемент указанной строки матрицы определяется по формуле
// m[row][j] = value * m[row][j];
// Сложность заключается в том, что матрицы представлены в формате Sparse (описание в начале текущего файла)
{
  // При передаче коэффициента, равного нулю, необходимо удалить ненулевые элементы указанной строки из матрицы 
  if (!value)
  {
    SparseMatrixLineErase(m, row);
  }
  else
  {
    // Сам процесс вычитания может дать от m1.cols до нуля результатов
    // в зависимости от значений элементов матрицы и их взаимного расположения
    // Проходим последовательно по каждой строке матриц
    for (int j = m->iptr[row]; j < m->iptr[row + 1]; j++)
    {
      // Умножаем каждый ненулевой элемент на значение value
      m->altr[j] *= value;
    }
  }
  return 0;
}

int Graph::SparseMatrixMulConst(SparseMatrixType *m, double value)
{
  for (int i = 0; i < m->rows; i++)
    SparseMatrixLineMulConst(m, i, value);
  return 0;
}

// Обнуление строки матрицы, представленной в формате Sparse
int Graph::SparseMatrixLineErase(SparseMatrixType *m, int row)
// При необходимости обнуления всех элементов строки матрицы, необходимо осуществить следующие изменения:
// Предположим, необходимо обнулить элементы строки с индексом row = 1
//      | 3  1  1  0  1 |
//   -> | 1 -1  0  1  0 | <- 
//  M = | 1  0  0  2  1 |
//      | 0  1  2  7  1 |
//      | 1  0  1  1  4 |
// 
// До преобразования:
// rows  = 5;
// cols  = 5;
// altr  = { 3, 1, 1, 1,  1,-1, 1,  1, 2, 1,  1, 2, 7, 1,  1, 1, 1, 4 }
// jptr  = { 0, 1, 2, 4,  0, 1, 3,  0, 3, 4,  1, 2, 3, 4,  0, 2, 3, 4 }
// iptr  = { 0,           4,        7,       10,          14,          18}
// 
// 
// После преобразования:
// rows  = 5;
// cols  = 5;
// altr  = { 3, 1, 1, 1,  1, 2, 1,  1, 2, 7, 1,  1, 1, 1, 4 }
// jptr  = { 0, 1, 2, 4,  0, 3, 4,  1, 2, 3, 4,  0, 2, 3, 4 }
// iptr  = { 0,           4,        7,          11,          14}
// 
// Удаляются элементы:
//        altr[iptr[row]]..altr[iptr[row+1]-1] 
//        jptr[iptr[row]]..altr[iptr[row+1]-1] 
// значения элементов iptr[row+1]..iptr[cols] уменьшаются на iptr[row+1]-iptr[row]
{
  int leftindex = m->iptr[row];
  int num = m->iptr[row + 1] - m->iptr[row];
  for (int i = leftindex; i < m->iptr[m->cols] - num; i++)
  {
    m->altr[i] = m->altr[i + num];
    m->jptr[i] = m->jptr[i + num];
  }

  int iNumDeletedElements = m->iptr[row + 1] - m->iptr[row];
  for (int i = row + 1; i <= m->cols; i++)
    m->iptr[i] -= iNumDeletedElements;

  return 0;
}

// Перестановка двух строк матрицы, представленной в формате Sparse
int Graph::SparseMatrixLinesSwap(SparseMatrixType *m, int r1, int r2)
// При необходимости перестановки двух строк матрицы, необходимо осуществить следующие изменения:
// Предположим, необходимо переставить строки матрицы с индексами row1 = 1 и row1 = 4
// До преобразования:
//      | 3  1  1  0  1 |
//   -> | 1 -1  0  1  0 | <- 
//  M = | 1  0  0  2  1 |
//      | 0  1  2  7  1 |
//   -> | 1  0  1  1  4 | <-
// 
// rows  = 5;
// cols  = 5;
// altr  = { 3, 1, 1, 1,  1,-1, 1,  1, 2, 1,  1, 2, 7, 1,  1, 1, 1, 4 }
// jptr  = { 0, 1, 2, 4,  0, 1, 3,  0, 3, 4,  1, 2, 3, 4,  0, 2, 3, 4 }
// iptr  = { 0,           4,        7,       10,          14,          18}
// 
// 
// После преобразования:
//      | 3  1  1  0  1 |
//   -> | 1  0  1  1  4 | <- 
//  M = | 1  0  0  2  1 |
//      | 0  1  2  7  1 |
//   -> | 1 -1  0  1  0 | <-
// 
// rows  = 5;
// cols  = 5;
// altr  = { 3, 1, 1, 1,  1, 1, 1, 4,  1, 2, 1,  1, 2, 7, 1,  1,-1, 1 }
// jptr  = { 0, 1, 2, 4,  0, 2, 3, 4,  0, 3, 4,  1, 2, 3, 4,  0, 1, 3 }
// iptr  = { 0,           4,           8,       11,          15,       18}
// 
// Массив, кодирующий разреженную матрицу, делится на 5 условных фрагментов:
// 1. До первой строки    adiag[0..row1-1],      iptr[0..row1-1],      элементы altr[iptr[0]   ..iptr[row1]-1]     и их индексы jptr[iptr[0]..iptr[row1]-1]
// 2. Первая строка       adiag[row1],           iptr[row1],           элементы altr[iptr[row1]..iptr[row1+1]-1]   и их индексы jptr[iptr[row1]..iptr[row1+1]-1]
// 3. Между строками      adiag[row1+1..row2-1], iptr[row1+1..row2-1], элементы altr[iptr[row1+1]..iptr[row2]-1]   и их индексы jptr[iptr[row1+1]..iptr[row2]-1]
// 4. Вторая строка       adiag[row2],           iptr[row2],           элементы altr[iptr[row2]..iptr[row2+1]-1]   и их индексы jptr[iptr[row2]..iptr[row2+1]-1]
// 5. После второй строки adiag[row2+1..n-1],    iptr[row2+1..n-1],    элементы altr[iptr[row2+1]..iptr[n]-1]      и их индексы jptr[iptr[row21+1]..iptr[n]-1]
// Необходимо после первой части записать четвертую, потом третью, вторую и пятую.
// Сложность добавляет то, что во второй и четвертой  частях количество элементов может измениться из-за того что они могут стать или перестать быть диагональными
// Соответствующим образом необходимо модифицировать и массив диагональных элементов adiag
{
  if (r1 == r2)
    return 0;

  int row1 = min(r1, r2);
  int row2 = max(r1, r2);
  SparseMatrixType m2;
  int iNumNonZeroElements = m->iptr[m->cols];
  m2.cols = m->cols;
  m2.rows = m->rows;

  m2.altr = new double[iNumNonZeroElements];
  if (m2.altr == NULL)
  {
    fprintf(stderr, "Error. Memory for m2.altr was not allocated\n");
    return 3;
  }

  m2.jptr = new int[iNumNonZeroElements];
  if (m2.jptr == NULL)
  {
    fprintf(stderr, "Error. Memory for m2.jptr was not allocated\n");
    return 3;
  }

  m2.iptr = new int[m2.rows + 1];
  if (m2.iptr == NULL)
  {
    fprintf(stderr, "Error. Memory for m2.iptr was not allocated\n");
    return 3;
  }

  iNumNonZeroElements = 0;
  m2.iptr[0] = 0;



  // Реализация копирования первой части матрицы (до первой строки)
  for (int i = 0; i < row1; i++)
  {
    for (int j = m->iptr[i]; j < m->iptr[i + 1]; j++)
    {
      m2.altr[j] = m->altr[j];
      m2.jptr[j] = m->jptr[j];
      iNumNonZeroElements++;
    }
    m2.iptr[i + 1] = iNumNonZeroElements;
  }
  


  // Копирование "второй" строки матрицы наместо "первой"
  for (int j = m->iptr[row2]; j < m->iptr[row2 + 1]; j++)
  {
    // копируем очередной элемент на его новое место
    m2.altr[iNumNonZeroElements] = m->altr[j];
    m2.jptr[iNumNonZeroElements] = m->jptr[j];
    iNumNonZeroElements++;
  }
  // После цикла запишем значение iNumNonZeroElements в m2.iptr[i];
  m2.iptr[row1+1] = iNumNonZeroElements;



  // Реализация копирования третьей части матрицы (между строками)
  for (int i = row1+1; i < row2; i++)
  {
    for (int j = m->iptr[i]; j < m->iptr[i + 1]; j++)
    {
      m2.altr[iNumNonZeroElements] = m->altr[j];
      m2.jptr[iNumNonZeroElements] = m->jptr[j];
      iNumNonZeroElements++;
    }
    // После каждой итерации iNumNonZeroElements заносим в m2.iptr[i+1];
    m2.iptr[i + 1] = iNumNonZeroElements;
  }
  
  
  
  // Копирование "первой" строки матрицы на место "второй"
  for (int j = m->iptr[row1]; j < m->iptr[row1 + 1]; j++)
  {
    // копируем очередной элемент на его новое место
    m2.altr[iNumNonZeroElements] = m->altr[j];
    m2.jptr[iNumNonZeroElements] = m->jptr[j];
    iNumNonZeroElements++;
  }
  // После цикла запишем значение iNumNonZeroElements в m2.iptr[i];
  m2.iptr[row2 + 1] = iNumNonZeroElements;

  

  // Реализация копирования пятой части матрицы (после второй строки)
  for (int i = row2 + 1; i < m2.rows; i++)
  {
    for (int j = m->iptr[i]; j < m->iptr[i + 1]; j++)
    {
      m2.altr[iNumNonZeroElements] = m->altr[j];
      m2.jptr[iNumNonZeroElements] = m->jptr[j];
      iNumNonZeroElements++;
    }
    // После каждой итерации iNumNonZeroElements заносим в m2.iptr[i+1];
    m2.iptr[m2.rows + 1] = iNumNonZeroElements;
  }

  for (int i = 0; i < m2.iptr[m2.cols]; i++)
  {
    m->altr[i] = m2.altr[i];
    m->jptr[i] = m2.jptr[i];
  }
  
  for (int i = 0; i <= m2.cols; i++)
    m->iptr[i] = m2.iptr[i];
  
  return 0;
}

// Определение обратной матрицы
int Graph::SparseMatrixReverse_old(SparseMatrixType m, SparseMatrixType *result)
{
  int r = m.rows, c = m.cols;
  if (r != c)
    return -1;

  double *matrix, val;
  matrix = new double[2 * c * r];
  if (matrix == NULL)
  {
    fprintf(stderr, "Error. Memory for matrix was not allocated\n");
    return 3;
  }
  
  //  Инициализируем матрицу нулями
  for (int i = 0; i < r; i++)
    for (int j = 0; j < 2 * c; j++)
      matrix[2 * i * c + j] = 0.0;

  // Запишем в матрицу все ненулевые значения из разреженной матрицы
  for (int i = 0; i < r; i++)
    for (int j_ind = m.iptr[i]; j_ind < m.iptr[i+1]; j_ind++)
      matrix[2 * i * c + m.jptr[j_ind]] = m.altr[j_ind];
   
  // Правая часть матрицы должны стать единичной диагональной матрицей
  for (int i = 0; i < r; i++)
    matrix[2 * i * c + c + i] = 1.0;


  PrintDoubleMatrixToFile("RevMatr_0_initial.txt", matrix, 2 * c, r);

  
  // Проход сверху вниз для получения треугольной матрицы (нижний треугольник содержит нули)
  for (int i = 0; i < r - 1; i++)
  {
    if (abs(matrix[2 * i * c + i]) < EPS)
      for (int i2 = 0; i2 < r; i2++)
        if (abs(matrix[2 * i2 * c + i]) > EPS)
        {
          for (int j = 0; j < 2 * c; j++)
            matrix[2 * i * c + j] += matrix[2 * i2 * c + j];
          break;
        }
    for (int j = i + 1; j < r; j++)
    {
      if (abs(matrix[2 * j * c + i]) < EPS)
        continue;
      val = matrix[2 * j * c + i] / matrix[2 * i * c + i];

      for (int k = 0; k < 2 * c; k++)
      {
        if (abs(matrix[2 * i * c + k]) < EPS)
          continue;
        matrix[2 * j * c + k] -= matrix[2 * i * c + k] * val;
        if (abs(matrix[2 * j * c + k]) < EPS)
          matrix[2 * j * c + k] = 0.0;
      }
    }

    char  fn[200];
    sprintf_s(fn, 200, "Matrix_step_%d.txt", i);
    PrintDoubleMatrixToFile(fn, matrix, 2 * c, r);

  }

  
  PrintDoubleMatrixToFile("RevMatr_2_UpTriangle.txt", matrix, 2 * c, r);


  // Проход снизу вверх для получения диагональной матрицы (верхний и нижний треугольники содержат нули)
  for (int i = c-1; i > 0; i--)
    for (int j = i - 1; j >= 0; j--)
    {
      double val = matrix[2 * j * c + i] / matrix[2 * i * c + i];
      if (abs(val) < EPS)
        continue;
      for (int k = i; k < 2 * c; k++)
      {
        matrix[2 * j * c + k] -= matrix[2 * i * c + k] * val;
        if (abs(matrix[2 * j * c + k]) < EPS)
          matrix[2 * j * c + k] = 0.0;
      }
    }

  PrintDoubleMatrixToFile("RevMatr_3_BothTriangles.txt", matrix, 2 * c, r);



  // Деление каждой строки на диагональный элемент для получения едининой диагональной матрицы в левой части
  for (int i = 0; i < r; i++)
    for (int j = 2 * c - 1; j >= 0; j--)
      if (abs(matrix[2 * i * c + j]) > EPS)
        matrix[2 * i * c + j] /= matrix[2 * i * c + i];

  

  PrintDoubleMatrixToFile("RevMatr_4_Reversed.txt", matrix, 2 * c, r);




  // Подсчитаем количество ненулевых элементов для выделения памяти для Sparse-матрицы
  int iNumNonZeroElements = 0;
  for (int i = 0; i < r; i++)
    for (int j = c; j < 2 * c; j++)
      if (abs(matrix[2 * i * c + j]) > EPS)
        iNumNonZeroElements++;

  
  if (result->altr != NULL)
  {
    delete[] result->altr;
    result->altr = NULL;
  }
  result->altr = new double[iNumNonZeroElements];
  if (result->altr == NULL)
  {
    fprintf(stderr, "Error. Memory was not allocated\n");
    return -1;
  }

  if (result->jptr != NULL)
  {
    delete[] result->jptr;
    result->jptr = NULL;
  }
  result->jptr = new int[iNumNonZeroElements];
  if (result->jptr == NULL)
  {
    fprintf(stderr, "Error. Memory was not allocated\n");
    return -1;
  }

  if (result->iptr != NULL)
  {
    delete[] result->iptr;
    result->iptr = NULL;
  }
  result->iptr = new int[r + 1];
  if (result->iptr == NULL)
  {
    fprintf(stderr, "Error. Memory was not allocated\n");
    return -1;
  }

  int i_altr = 0, i_autr = 0, i_jptr = 0;

  result->iptr[0] = 0;

  for (int i = 0; i < r; i++)
  {
    result->iptr[i + 1] = result->iptr[i];
    for (int j = 0; j < c; j++)
      if (abs(matrix[2 * i * c + j + c]) > EPS)
      {
        result->altr[i_altr++] = matrix[2 * i * c + j + c];
        result->jptr[i_jptr++] = j;
        (result->iptr[i + 1])++;
      }
  }

  result->cols = c;
  result->rows = r;

  return 0;
}

int Graph::SparseMatrixReverse(SparseMatrixType m, SparseMatrixType *result)
{
  int r = m.rows, c = m.cols;
  if (r != c)
    return -1;

  double *matrix, val;
  matrix = new double[2 * c * r];
  if (matrix == NULL)
  {
    fprintf(stderr, "Error. Memory for matrix was not allocated\n");
    return 3;
  }

  // Инициализируем матрицу нулями
  for (int i = 0; i < r; i++)
    for (int j = 0; j < 2 * c; j++)
      matrix[2 * i * c + j] = 0.0;

  // Запишем в матрицу все ненулевые значения из разреженной матрицы
  for (int i = 0; i < r; i++)
    for (int j_ind = m.iptr[i]; j_ind < m.iptr[i + 1]; j_ind++)
      matrix[2 * i * c + m.jptr[j_ind]] = m.altr[j_ind];

  // Правая часть матрицы должны стать единичной диагональной матрицей
  for (int i = 0; i < r; i++)
    matrix[2 * i * c + c + i] = 1.0;
  
  // PrintDoubleMatrixToFile("RevMatr_0_initial.txt", matrix, 2 * c, r);

  // Проход сверху вниз для получения треугольной матрицы (нижний треугольник содержит нули)
  for (int i = 0; i < r - 1; i++)
  {
    // Если очередной диагональный элемент равен нулю, нужно путем сложения с другой строкой добиться обратного
    if (abs(matrix[2 * i * c + i]) < EPS)
      // Среди оставшихся строк ищем ту, где на нужном месте стоит ненулевой элемент
      for (int i2 = i + 1; i2 < r; i2++) 
        if (abs(matrix[2 * i2 * c + i]) > EPS)
        {
          // Цикл можно начать со столбца i, потому что все, что слева, уже обнулено
          for (int j = i; j < 2 * c; j++)
            matrix[2 * i * c + j] += matrix[2 * i2 * c + j];
          break;
        }

    // После восстановления ненулевого элемента на диагональ выполним обнуление всех элементов ниже в том же столбце
    for (int j = i + 1; j < r; j++)
    {
      // Если элемент и так нулевой, эту строку можно пропустить
      if (abs(matrix[2 * j * c + i]) < EPS)
        continue;

      // A[i][i] - ненулевой диагональный элемент, (i = 0..r-2)
      // A[j][i] - ненулевой элемент, который нужно обнулить, (j = j..r-1)
      // A[j][k] - очередной элемент строки, который должен быть преобразован следующим образом:
      // A[j][k] = A[j][k] - A[i][k]*A[j][i]/A[i][i], (k = i..2c-1)
      val = matrix[2 * j * c + i] / matrix[2 * i * c + i];

      for (int k = i; k < 2 * c; k++)
      {
        // Если A[i][k] == 0, то нет смысла вычислять, значение не изменится
        if (abs(matrix[2 * i * c + k]) < EPS)
          continue;

        matrix[2 * j * c + k] -= matrix[2 * i * c + k] * val;
        
        //Если новое значение меньше EPS, обнулим его во избежание накопления погрешности
        if (abs(matrix[2 * j * c + k]) < EPS)
          matrix[2 * j * c + k] = 0.0;
      }
    }
  }

  // Проход сверху вниз для получения треугольной матрицы (нижний треугольник содержит нули)
  for (int i = r - 1; i > 0; i--)
  {
    // Если очередной диагональный элемент равен нулю, нужно путем сложения с другой строкой добиться обратного
    if (abs(matrix[2 * i * c + i]) < EPS)
      // Среди оставшихся строк ищем ту, где на нужном месте стоит ненулевой элемент
      for (int i2 = i - 1; i2 >= 0; i2--)
        if (abs(matrix[2 * i2 * c + i]) > EPS)
        {
          // Цикл можно начать со столбца i, потому что все, что слева, уже обнулено
          for (int j = 0; j < 2 * c; j++)
            matrix[2 * i * c + j] += matrix[2 * i2 * c + j];
          break;
        }

    // После восстановления ненулевого элемента на диагональ выполним обнуление всех элементов выше в том же столбце
    for (int j = i - 1; j >= 0; j--)
    {
      // Если элемент и так нулевой, эту строку можно пропустить
      if (abs(matrix[2 * j * c + i]) < EPS)
        continue;

      // A[i][i] - ненулевой диагональный элемент, (i = 0..r-2)
      // A[j][i] - ненулевой элемент, который нужно обнулить, (j = j..r-1)
      // A[j][k] - очередной элемент строки, который должен быть преобразован следующим образом:
      // A[j][k] = A[j][k] - A[i][k]*A[j][i]/A[i][i], (k = i..2c-1)
      val = matrix[2 * j * c + i] / matrix[2 * i * c + i];

      for (int k = 0; k < 2 * c; k++)
      {
        // Если A[i][k] == 0, то нет смысла вычислять, значение не изменится
        if (abs(matrix[2 * i * c + k]) < EPS)
          continue;

        matrix[2 * j * c + k] -= matrix[2 * i * c + k] * val;

        //Если новое значение меньше EPS, обнулим его во избежание накопления погрешности
        if (abs(matrix[2 * j * c + k]) < EPS)
          matrix[2 * j * c + k] = 0.0;
      }
    }
  }

  // PrintDoubleMatrixToFile("RevMatr_2_BothTriangles.txt", matrix, 2 * c, r);
  
  // Деление каждой строки на диагональный элемент для получения едининой диагональной матрицы в левой части
  for (int i = 0; i < r; i++)
    for (int j = 2 * c - 1; j >= 0; j--)
      if (abs(matrix[2 * i * c + j]) > EPS)
        matrix[2 * i * c + j] /= matrix[2 * i * c + i];
  
  //PrintDoubleMatrixToFile("RevMatr_4_Reversed.txt", matrix, 2 * c, r);
  

  // Подсчитаем количество ненулевых элементов для выделения памяти для Sparse-матрицы
  int iNumNonZeroElements = 0;
  for (int i = 0; i < r; i++)
    for (int j = c; j < 2 * c; j++)
      if (abs(matrix[2 * i * c + j]) > EPS)
        iNumNonZeroElements++;


  if (result->altr != NULL)
  {
    delete[] result->altr;
    result->altr = NULL;
  }
  result->altr = new double[iNumNonZeroElements];
  if (result->altr == NULL)
  {
    fprintf(stderr, "Error. Memory was not allocated\n");
    return -1;
  }

  if (result->jptr != NULL)
  {
    delete[] result->jptr;
    result->jptr = NULL;
  }
  result->jptr = new int[iNumNonZeroElements];
  if (result->jptr == NULL)
  {
    fprintf(stderr, "Error. Memory was not allocated\n");
    return -1;
  }

  if (result->iptr != NULL)
  {
    delete[] result->iptr;
    result->iptr = NULL;
  }
  result->iptr = new int[r + 1];
  if (result->iptr == NULL)
  {
    fprintf(stderr, "Error. Memory was not allocated\n");
    return -1;
  }

  int i_altr = 0, i_autr = 0, i_jptr = 0;

  result->iptr[0] = 0;

  for (int i = 0; i < r; i++)
  {
    result->iptr[i + 1] = result->iptr[i];
    for (int j = 0; j < c; j++)
      if (abs(matrix[2 * i * c + j + c]) > EPS)
      {
        result->altr[i_altr++] = matrix[2 * i * c + j + c];
        result->jptr[i_jptr++] = j;
        (result->iptr[i + 1])++;
      }
  }

  result->cols = c;
  result->rows = r;

  return 0;
}

double Graph::SparseMatrixMinElement(SparseMatrixType m)
{
  double min = m.altr[0];
  for (int i = 1; i < m.iptr[m.rows]; i++)
    min = (min < m.iptr[i]) ? min : m.iptr[i];
  return min;
}

double Graph::SparseMatrixMaxElement(SparseMatrixType m)
{
  double max = m.altr[0];
  for (int i = 1; i < m.iptr[m.rows]; i++)
    max = (max > m.altr[i]) ? max : m.altr[i];
  return max;
}

double Graph::EigenVectorMaxElement(Eigen::VectorXd v)
{
  double max = v.coeffRef(0);
  for (int i = 1; i < int(v.size()); i++)
    max = (max > v.coeffRef(i)) ? max : v.coeffRef(i);
  return max;
}

int Graph::SparseMatrixSaveToFile(SparseMatrixType m, char * FileName)
{
  FILE *stream;
  if (fopen_s(&stream, FileName, "w") != 0)
  {
    printf("The file %s was not opened for writing\n", FileName);
    return -1;
  }

  int next_j_ind;
  for (int i = 0; i < m.rows; i++)
  {
    next_j_ind = m.iptr[i];
    for (int j = 0; j < m.cols; j++)
      if ((j == m.jptr[next_j_ind]) && (m.iptr[i] < m.iptr[i + 1]) && (next_j_ind < m.iptr[i + 1]))
      {
        fprintf(stream, "%15.10lf ", m.altr[next_j_ind]);
        next_j_ind++;
      }
      else
        fprintf(stream, "%15.10lf ", 0.0);
    fprintf(stream, "\n");
  }
  fprintf(stream, "\n");

  _fcloseall();
  return 0;
}

// Преобразование матрицы инцидентности в две разреженные матрицы: Ax и Ay
int Graph::MInConvertToSparseFormat()
// Процедура преобразует прямоугольную матрицу в Sparse формат
// Матрица описана как вектор векторов типа int. На выходе - структура, соответствующая преобразованной матрице.
// Выделение памяти (переопределение размера) реализовано автоматически внутри текущей процедуры
{
  // Входящая матрица пустая, преобразовывать нечего. Код ошибки 1.
  if (!int(MIn.size()))
    return 1;
  else
  {
    Ax.rows = int(MIn.size());
    Ay.rows = int(MIn.size());
  }

  // Матрица содержит пустые вектора, преобразовывать нечего. Код ошибки 2.
  if (int(MIn[0].size()) != iNumTreeVariables + iNumATreeVariables)
    return 2;
  else
  {
    Ax.cols = iNumTreeVariables;
    Ay.cols = iNumATreeVariables;
  }

  // Подсчитаем количество ненулевых элементов для выделения памяти для Sparse-матрицы
  int iNumNonZeroElements = 0;
  for (int i = 0; i < Ax.rows; i++)
    for (int j = 0; j < Ax.cols; j++)
      iNumNonZeroElements += int(MIn[i][j] != 0);

  Ax.altr = new double[iNumNonZeroElements];
  if (Ax.altr == NULL)
  {
    fprintf(stderr, "Error. Memory for Ax.altr was not allocated\n");
    return 3;
  }

  Ax.jptr = new int[iNumNonZeroElements];
  if (Ax.jptr == NULL)
  {
    fprintf(stderr, "Error. Memory for Ax.jptr was not allocated\n");
    return 3;
  }

  Ax.iptr = new int[Ax.rows + 1];
  if (Ax.iptr == NULL)
  {
    fprintf(stderr, "Error. Memory for Ax.iptr was not allocated\n");
    return 3;
  }

  int i_altr = 0, i_autr = 0, i_jptr = 0;

  Ax.iptr[0] = 0;

  for (int i = 0; i < Ax.rows; i++)
  {
    Ax.iptr[i + 1] = Ax.iptr[i];
    for (int j = 0; j < Ax.cols; j++)
      if (MIn[i][j])
      {
        Ax.altr[i_altr++] = MIn[i][j];
        Ax.jptr[i_jptr++] = j;
        (Ax.iptr[i + 1])++;
      }
  }

  // Подсчитаем количество ненулевых элементов для выделения памяти для Sparse-матрицы
  iNumNonZeroElements = 0;
  for (int i = 0; i < Ay.rows; i++)
    for (int j = 0; j < Ay.cols; j++)
      iNumNonZeroElements += int(MIn[i][j + iNumTreeVariables] != 0);

  Ay.altr = new double[iNumNonZeroElements];
  if (Ay.altr == NULL)
  {
    fprintf(stderr, "Error. Memory for Ay.altr was not allocated\n");
    return 3;
  }

  Ay.jptr = new int[iNumNonZeroElements];
  if (Ay.jptr == NULL)
  {
    fprintf(stderr, "Error. Memory for Ay.jptr was not allocated\n");
    return 3;
  }

  Ay.iptr = new int[Ax.rows + 1];
  if (Ay.iptr == NULL)
  {
    fprintf(stderr, "Error. Memory for Ay.iptr was not allocated\n");
    return 3;
  }

  i_altr = 0, i_autr = 0, i_jptr = 0;

  Ay.iptr[0] = 0;

  for (int i = 0; i < Ay.rows; i++)
  {
    Ay.iptr[i + 1] = Ay.iptr[i];
    for (int j = 0; j < Ay.cols; j++)
      if (MIn[i][j + iNumTreeVariables])
      {
        Ay.altr[i_altr++] = MIn[i][j + iNumTreeVariables];
        Ay.jptr[i_jptr++] = j;
        (Ay.iptr[i + 1])++;
      }
  }

  return 0;
} // int Graph::MInConvertToSparseFormat()

// Преобразование матрицы контуров в две разреженные матрицы: Sx и Sy
int Graph::MKonConvertToSparseFormat()
// Процедура преобразует прямоугольную матрицу в Sparse формат
// Матрица описана как вектор векторов типа int. На выходе - структура, соответствующая преобразованной матрице.
// Выделение памяти (переопределение размера) реализовано автоматически внутри текущей процедуры
{
  // Входящая матрица пустая, преобразовывать нечего. Код ошибки 1.
  if (!int(MKon.size()))
    return 1;
  else
  {
    Sx.rows = int(MKon.size());
    Sy.rows = int(MKon.size());
    S.rows = int(MKon.size());
  }

  // Матрица содержит пустые вектора, преобразовывать нечего. Код ошибки 2.
  if (int(MKon[0].size()) != iNumTreeVariables + iNumATreeVariables)
    return 2;
  else
  {
    Sx.cols = iNumTreeVariables;
    Sy.cols = iNumATreeVariables;
    S.cols = iNumTreeVariables + iNumATreeVariables;
  }

  // Подсчитаем количество ненулевых элементов для выделения памяти для Sparse-матрицы
  int iNumNonZeroElements = 0;
  for (int i = 0; i < Sx.rows; i++)
    for (int j = 0; j < Sx.cols; j++)
      iNumNonZeroElements += int(MKon[i][j] != 0);

  Sx.altr = new double[iNumNonZeroElements];
  if (Sx.altr == NULL)
  {
    fprintf(stderr, "Error. Memory for Sx.altr was not allocated\n");
    return 3;
  }

  Sx.jptr = new int[iNumNonZeroElements];
  if (Sx.jptr == NULL)
  {
    fprintf(stderr, "Error. Memory for Sx.jptr was not allocated\n");
    return 3;
  }

  Sx.iptr = new int[Ax.rows + 1];
  if (Sx.iptr == NULL)
  {
    fprintf(stderr, "Error. Memory for Sx.iptr was not allocated\n");
    return 3;
  }

  int i_altr = 0, i_autr = 0, i_jptr = 0;

  Sx.iptr[0] = 0;

  for (int i = 0; i < Sx.rows; i++)
  {
    Sx.iptr[i + 1] = Sx.iptr[i];
    for (int j = 0; j < Sx.cols; j++)
      if (MKon[i][j])
      {
        Sx.altr[i_altr++] = MKon[i][j];
        Sx.jptr[i_jptr++] = j;
        (Sx.iptr[i + 1])++;
      }
  }

  // Подсчитаем количество ненулевых элементов для выделения памяти для Sparse-матрицы
  iNumNonZeroElements = 0;
  for (int i = 0; i < Sy.rows; i++)
    for (int j = 0; j < Sy.cols; j++)
      iNumNonZeroElements += int(MKon[i][j + iNumTreeVariables] != 0);

  Sy.altr = new double[iNumNonZeroElements];
  if (Sy.altr == NULL)
  {
    fprintf(stderr, "Error. Memory for Sy.altr was not allocated\n");
    return 3;
  }

  Sy.jptr = new int[iNumNonZeroElements];
  if (Sy.jptr == NULL)
  {
    fprintf(stderr, "Error. Memory for Sy.jptr was not allocated\n");
    return 3;
  }

  Sy.iptr = new int[Ax.rows + 1];
  if (Sy.iptr == NULL)
  {
    fprintf(stderr, "Error. Memory for Sy.iptr was not allocated\n");
    return 3;
  }

  i_altr = 0, i_autr = 0, i_jptr = 0;

  Sy.iptr[0] = 0;

  for (int i = 0; i < Sy.rows; i++)
  {
    Sy.iptr[i + 1] = Sy.iptr[i];
    for (int j = 0; j < Sy.cols; j++)
      if (MKon[i][j + iNumTreeVariables])
      {
        Sy.altr[i_altr++] = MKon[i][j + iNumTreeVariables];
        Sy.jptr[i_jptr++] = j;
        (Sy.iptr[i + 1])++;
      }
  }


  // Подсчитаем количество ненулевых элементов для выделения памяти для Sparse-матрицы
  iNumNonZeroElements = 0;
  for (int i = 0; i < S.rows; i++)
    for (int j = 0; j < S.cols; j++)
      iNumNonZeroElements += int(MKon[i][j] != 0);

  S.altr = new double[iNumNonZeroElements];
  if (S.altr == NULL)
  {
    fprintf(stderr, "Error. Memory for S.altr was not allocated\n");
    return 3;
  }

  S.jptr = new int[iNumNonZeroElements];
  if (S.jptr == NULL)
  {
    fprintf(stderr, "Error. Memory for S.jptr was not allocated\n");
    return 3;
  }

  S.iptr = new int[Ax.rows + 1];
  if (S.iptr == NULL)
  {
    fprintf(stderr, "Error. Memory for S.iptr was not allocated\n");
    return 3;
  }

  i_altr = 0, i_autr = 0, i_jptr = 0;

  S.iptr[0] = 0;

  for (int i = 0; i < S.rows; i++)
  {
    S.iptr[i + 1] = S.iptr[i];
    for (int j = 0; j < S.cols; j++)
      if (MKon[i][j])
      {
        S.altr[i_altr++] = MKon[i][j];
        S.jptr[i_jptr++] = j;
        (S.iptr[i + 1])++;
      }
  }

  return 0;
} // int Graph::MKonConvertToSparseFormat()


int Graph::MInConvertToEigenFormat()
{
  // Входящая матрица пустая, преобразовывать нечего. Код ошибки 1.
  if (!int(MIn.size()))
    return 1;

  // Матрица содержит пустые вектора, преобразовывать нечего. Код ошибки 2.
  if (int(MIn[0].size()) != iNumTreeVariables + iNumATreeVariables)
    return 2;
  else
  {
    EAx.resize(int(MIn.size()), iNumTreeVariables);
    EAx.data().squeeze();
    EAy.resize(int(MIn.size()), iNumATreeVariables);
    EAy.data().squeeze();
  }
 
  for (int i = 0; i < int(MIn.size()); i++)
    for (int j = 0; j < iNumTreeVariables; j++)
      if (MIn[i][j])
        EAx.coeffRef(i, j) = MIn[i][j];

  for (int i = 0; i < int(MIn.size()); i++)
    for (int j = 0; j < iNumATreeVariables; j++)
      if (MIn[i][j + iNumTreeVariables])
        EAy.coeffRef(i, j) = MIn[i][j + iNumTreeVariables];

  return 0;
}

int Graph::MKonConvertToEigenFormat()
{
  // Входящая матрица пустая, преобразовывать нечего. Код ошибки 1.
  if (!int(MKon.size()))
    return 1;

  // Матрица содержит пустые вектора, преобразовывать нечего. Код ошибки 2.
  if (int(MKon[0].size()) != iNumTreeVariables + iNumATreeVariables)
    return 2;
  else
  {
    ESx.resize(int(MKon.size()), iNumTreeVariables);
    ESx.data().squeeze();
    ESy.resize(int(MKon.size()), iNumATreeVariables);
    ESy.data().squeeze();
    ES.resize(int(MKon.size()), iNumTreeVariables + iNumATreeVariables);
    ES.data().squeeze();
  }

  for (int i = 0; i < int(MKon.size()); i++)
    for (int j = 0; j < iNumTreeVariables; j++)
      if (MKon[i][j])
      {
        ESx.coeffRef(i, j) = MKon[i][j];
        ES.coeffRef(i, j) = MKon[i][j];
      }

  for (int i = 0; i < int(MKon.size()); i++)
    for (int j = 0; j < iNumATreeVariables; j++)
      if (MKon[i][j + iNumTreeVariables])
      {
        ESy.coeffRef(i, j) = MKon[i][j + iNumTreeVariables];
        ES.coeffRef(i, j + iNumTreeVariables) = MKon[i][j + iNumTreeVariables];
      }

  return 0;
}

int Graph::SaveStepValuesToFile(FILE *f)
{
  for (int i = 0; i < iNumTreeVariables; i++)
  {
    if (X.iptr[i] != X.iptr[i+1])
      fprintf_s(f, "%15.10lf", X.altr[i]);
    else
      fprintf_s(f, "%15.10lf", 0.0);
    if (i < iNumTreeVariables)
      fprintf_s(f, "; ");
  }

  for (int i = 0; i < iNumATreeVariables; i++)
  {
    if (Y.iptr[i] != Y.iptr[i + 1])
      fprintf_s(f, "%15.10lf", Y.altr[i]);
    else
      fprintf_s(f, "%15.10lf", 0.0);
    if (i < iNumATreeVariables)
      fprintf_s(f, "; ");
  }
  fprintf_s(f, "\n");
  return 0;
}

int Graph::SaveStepValuesToFile_Eigen(FILE * f)
{
  for (int i = 0; i < iNumTreeVariables; i++)
  {
    fprintf_s(f, "%15.10lf", EX.coeffRef(i));
    if (i < iNumTreeVariables)
      fprintf_s(f, "; ");
  }

  for (int i = 0; i < iNumATreeVariables; i++)
  {
    fprintf_s(f, "%15.10lf", EY.coeffRef(i));
    if (i < iNumATreeVariables)
      fprintf_s(f, "; ");
  }
  fprintf_s(f, "\n");
  return 0;
}

// Инициализация разреженой матрицы R
int Graph::InitializeR()
{
  R.cols = int(Archs.size());
  R.rows = R.cols;

  R.altr = new double[R.rows];
      if (R.altr == NULL)
      {
        fprintf(stderr, "Error. Memory for R.altr was not allocated\n");
        return 3;
      }

  R.jptr = new int[R.rows];
        if (R.jptr == NULL)
        {
          fprintf(stderr, "Error. Memory for R.jptr was not allocated\n");
          return 3;
        }

  R.iptr = new int[R.rows + 1];
        if (R.iptr == NULL)
        {
          fprintf(stderr, "Error. Memory for R.iptr was not allocated\n");
          return 3;
        }

  for (int i = 0; i < iNumTreeVariables; i++)
  {
    R.altr[i] = Archs[iTreeInd[i]].R;
    R.jptr[i] = i;
    R.iptr[i] = i;
  }

  for (int i = 0; i < iNumATreeVariables; i++)
  {
    R.altr[i + iNumTreeVariables] = Archs[iATreeInd[i]].R;
    R.jptr[i + iNumTreeVariables] = i + iNumTreeVariables;
    R.iptr[i + iNumTreeVariables] = i + iNumTreeVariables;
  }
  R.iptr[R.rows] = iNumTreeVariables + iNumATreeVariables;
  
  return 0;
}

int Graph::CalculateZ()
{ 
  int iNumNonZeroVariables = 0;
  double value;

  Z.iptr[0] = 0;

  for (int i = 0; i < iNumTreeVariables; i++)
  {
    if (X.iptr[i] != X.iptr[i+1])
    {
      value = X.altr[i] * abs(X.altr[i]);
      if (abs(value) > EPS)
      {
        Z.altr[iNumNonZeroVariables] = value;
        Z.jptr[iNumNonZeroVariables] = 0;
        iNumNonZeroVariables++;
      }
      else
        Z.jptr[iNumNonZeroVariables] = 0;
    }
    Z.iptr[i+1] = iNumNonZeroVariables;
  }

  for (int i = 0; i < iNumATreeVariables; i++)
  {
    if (Y.iptr[i] != Y.iptr[i + 1])
    {
      value = Y.altr[i] * abs(Y.altr[i]);
      if (abs(value) > EPS)
      {
        Z.altr[iNumNonZeroVariables] = value;
        Z.jptr[iNumNonZeroVariables] = 0;
        iNumNonZeroVariables++;
      }
      else
        Z.jptr[iNumNonZeroVariables] = 0;
    }
    Z.iptr[i + 1 + iNumTreeVariables] = iNumNonZeroVariables;
  }
  return 0;
}

// Инициализация разреженой матрицы H
int Graph::InitializeH()
{
  int iNumNonZeroValues = 0;
  for (int i = 0; i < iNumTreeVariables + iNumATreeVariables; i++)
    if (abs(Archs[i].H) > EPS)
      iNumNonZeroValues++;
 
  H.cols = 1;
  H.rows = iNumTreeVariables + iNumATreeVariables;

  H.altr = new double[iNumNonZeroValues];
  if (H.altr == NULL)
  {
    fprintf(stderr, "Error. Memory for H.altr was not allocated\n");
    return 3;
  }

  H.jptr = new int[iNumNonZeroValues];
  if (H.jptr == NULL)
  {
    fprintf(stderr, "Error. Memory for H.jptr was not allocated\n");
    return 3;
  }

  H.iptr = new int[H.rows + 1];
  if (H.iptr == NULL)
  {
    fprintf(stderr, "Error. Memory for H.iptr was not allocated\n");
    return 3;
  }

  iNumNonZeroValues = 0;
  H.iptr[0] = 0;

  for (int i = 0; i < iNumTreeVariables; i++)
  {
    if (abs(Archs[iTreeInd[i]].H) > EPS)
    {
      H.altr[iNumNonZeroValues] = Archs[iTreeInd[i]].H;
      H.jptr[iNumNonZeroValues++] = 0;
    }
    H.iptr[i+1] = iNumNonZeroValues;
  }

  for (int i = 0; i < iNumATreeVariables; i++)
  {
    if (abs(Archs[iATreeInd[i]].H) > EPS)
    {
      H.altr[iNumNonZeroValues] = Archs[iATreeInd[i]].H;
      H.jptr[iNumNonZeroValues++] = 0;
    }
    H.iptr[i + 1 + iNumTreeVariables] = iNumNonZeroValues;
  }

  return 0;
}

int Graph::InitializeY()
{
  Y.cols = 1;
  Y.rows = iNumATreeVariables;

  Y.altr = new double[Y.rows];
  if (Y.altr == NULL)
  {
    fprintf(stderr, "Error. Memory for Y.altr was not allocated\n");
    return 3;
  }

  Y.jptr = new int[Y.rows];
  if (Y.jptr == NULL)
  {
    fprintf(stderr, "Error. Memory for Y.jptr was not allocated\n");
    return 3;
  }

  Y.iptr = new int[Y.rows + 1];
  if (Y.iptr == NULL)
  {
    fprintf(stderr, "Error. Memory for Y.iptr was not allocated\n");
    return 3;
  }

  for (int i = 0; i < iNumATreeVariables; i++)
  {
    Y.altr[i] = 1.0;
    Y.jptr[i] = 0;
    Y.iptr[i] = i;
  }

  Y.iptr[Y.rows] = iNumATreeVariables;

  return 0;
}


// Вычисление вектора переменных Х по вектору переменных Y
int Graph::CalculateX()
// Зависимость выражается уравнением в матричной форме
// X = - W * Y;
{
  SparseMatricesMul(W, Y, &X);
  SparseMatrixMulConst(&X, -1);
  return 0;
}

void Graph::InitializeEigenY()
{
  EY.resize(iNumATreeVariables);
  for (int i = 0; i < iNumATreeVariables; i++)
    EY.coeffRef(i) = 0.0;
}

void Graph::InitializeEigenR()
{
  ER.resize(iNumTreeVariables + iNumATreeVariables, iNumTreeVariables + iNumATreeVariables);
  for (int i = 0; i < iNumTreeVariables; i++)
    ER.coeffRef(i, i) = Archs[iTreeInd[i]].R;
  for (int i = 0; i < iNumATreeVariables; i++)
    ER.coeffRef(i + iNumTreeVariables, i + iNumTreeVariables) = Archs[iATreeInd[i]].R;
}

void Graph::InitializeEigenK()
{
  EKx.resize(iNumTreeVariables, iNumTreeVariables);
  EKy.resize(iNumATreeVariables, iNumATreeVariables);
  
  for (int i = 0; i < iNumTreeVariables; i++)
    EKx.coeffRef(i, i) = Archs[iTreeInd[i]].K; 

  for (int i = 0; i < iNumATreeVariables; i++)
    EKy.coeffRef(i, i) = Archs[iATreeInd[i]].K;
}

void Graph::InitializeEigenH()
{
  EH.resize(iNumTreeVariables + iNumATreeVariables);
  for (int i = 0; i < iNumTreeVariables; i++)
    EH.coeffRef(i) = Archs[iTreeInd[i]].H;
  for (int i = 0; i < iNumATreeVariables; i++)
    EH.coeffRef(i + iNumTreeVariables) = Archs[iATreeInd[i]].H;
}

void Graph::CalculateEigenZ()
{
  EZ.resize(iNumTreeVariables + iNumATreeVariables);
  for (int i = 0; i < iNumTreeVariables; i++)
    EZ.coeffRef(i) = EX.coeffRef(i) * abs(EX.coeffRef(i));
  for (int i = 0; i < iNumATreeVariables; i++)
    EZ.coeffRef(i + iNumTreeVariables) = EY.coeffRef(i) * abs(EY.coeffRef(i));
}

int Graph::CalculateEigenW()
{
  Eigen::MatrixXd EMAx = EAx, EMW;

  //std::cout << "EMAx" << std::endl << EMAx << std::endl << std::endl;

  EMW = EMAx.inverse() * EAy;

  //std::cout << "EMW" << std::endl << EMW << std::endl << std::endl;

  EW.resize(iNumTreeVariables, iNumATreeVariables);
  
  for (int i = 0; i < iNumTreeVariables; i++)
    for (int j = 0; j < iNumATreeVariables; j++)
      if (abs(EMW.coeffRef(i, j)) > EPS)
        EW.coeffRef(i, j) = EMW.coeffRef(i, j);
  
  //std::cout << "EW" << std::endl << EW << std::endl << std::endl;
  
  return 0;
}

int Graph::CalculateEigenSp()
{
  Eigen::MatrixXd EWtemp = (ESy * EKy) - ((ESx * EKx) * EW);

  //std::cout << "EWtemp" << std::endl << EWtemp << std::endl << std::endl;

  EWtemp = EWtemp.inverse() * ES;

  //std::cout << "EWtemp" << std::endl << EWtemp << std::endl << std::endl;

  ESp.resize(iNumATreeVariables, iNumTreeVariables + iNumATreeVariables);

  for (int i = 0; i < iNumATreeVariables; i++)
    for (int j = 0; j < iNumTreeVariables + iNumATreeVariables; j++)
      if (abs(EWtemp.coeffRef(i, j)) > EPS)
        ESp.coeffRef(i, j) = EWtemp.coeffRef(i, j);

  //std::cout << "ESp" << std::endl << ESp << std::endl << std::endl;

  return 0;

}

int Graph::Calculate()
{
  SparseMatrixType SxKxW, V_rev, V, Sp, SpH, SpR, SpRZ, dY, Y1;

  SparseMatrixReverse(Ax, &Ax_rev);
  SparseMatricesMul(Ax_rev, Ay, &W);

  InitializeR();
  InitializeY();
  InitializeH();

  SparseMatrixSaveToFile(Ax, "_Ax.txt");
  SparseMatrixSaveToFile(Ay, "_Ay.txt");
  SparseMatrixSaveToFile(Sx, "_Sx.txt");
  SparseMatrixSaveToFile(Sy, "_Sy.txt");
  SparseMatrixSaveToFile( R,  "_R.txt");
  SparseMatrixSaveToFile(Kx, "_Kx.txt");
  SparseMatrixSaveToFile(Ky, "_Ky.txt");
  SparseMatrixSaveToFile( H,  "_H.txt");

  
  SparseMatricesMul(Sy, Ky, &Sp);       // Sy * Ky
  SparseMatricesMul(Sx, Kx, &SxKxW);    // Sx * Kx
  SparseMatricesMul(SxKxW, W, &SxKxW);  // (Sx * Kx) * W
  SparseMatricesSub(Sp, SxKxW, &Sp);    // (Sy * Ky) - ((Sx * Kx) * W)
  SparseMatrixReverse(Sp, &Sp);         // ((Sy * Ky) - ((Sx * Kx) * W))^(-1)
  SparseMatricesMul(Sp, S, &Sp);        // Sp = (((Sy * Ky) - ((Sx * Kx) * W))^(-1)) * S

  SparseMatricesMul(Sp, H, &S);         // S  = Sp * H
  SparseMatricesMul(Sp, R, &Sp);        // Sp = Sp * R
  CalculateX();

  int iNumIterations = 0;

  errno_t err;
  FILE *f;
  // Open for write (will make new file if file "steps_out.txt" does not exist)  
  err = fopen_s(&f, "steps_out.txt", "w");
  if (err != 0)
  {
    printf("The file 'steps_out.txt' was not opened\n");
    return -1;
  }
  SaveStepValuesToFile(f);

  int iMaxIterations = SIMULATION_TIME / TIME_STEP_SIZE;
  while (iNumIterations < iMaxIterations)
  {
    CalculateZ();
    SparseMatricesMul(Sp, Z, &SpRZ);      // SpRZ = (Sp * R) * Z
    SparseMatricesSub(S, SpRZ, &SpRZ);    // SpRZ = Sp*H - Sp*R*Z(i)
    SparseMatrixMulConst(&SpRZ, TIME_STEP_SIZE);  // SpRZ = h*[Sp*H - Sp*R*Z(i)];
    SparseMatricesAdd(Y, SpRZ, &Y);       // Y(i+1) = Y(i) + h*[Sp*H - Sp*R*Z(i)];
    CalculateX();

    SaveStepValuesToFile(f);

    if (SparseMatrixMaxElement(SpRZ) < 1e-05)
      break;
    iNumIterations++;
  }
  
  _fcloseall();
  return 0;
}

// Основная процедура вычислений с использованием библиотеки разреженных матриц
int Graph::EigenCalculate()
{
  //Eigen::SparseLU < Eigen::SparseMatrix < double > > linear_solver;
  Eigen::VectorXd RHS, dY;  
  
  InitializeEigenR();
  InitializeEigenK();
  InitializeEigenH();
  InitializeEigenY();
  
  CalculateEigenW();

  EX = (-1) * (EW * EY);

  CalculateEigenSp();
  
  int iNumIterations = 0;

  errno_t err;
  FILE *f;
  // Open for write (will make new file if file "steps_out.txt" does not exist)  
  err = fopen_s(&f, "steps_out.txt", "w");
  if (err != 0)
  {
    printf("The file 'steps_out.txt' was not opened\n");
    return -1;
  }
  SaveStepValuesToFile_Eigen(f);
  int iMaxIterations = SIMULATION_TIME / TIME_STEP_SIZE;
  while (iNumIterations < iMaxIterations)
  {
    CalculateEigenZ();

    dY = (ESp * EH) - ((ESp * ER) * EZ);
    EY = EY + TIME_STEP_SIZE * dY;
    EX = (-1) * (EW * EY);

    SaveStepValuesToFile_Eigen(f);

    if (EigenVectorMaxElement(dY) < 1e-05)
      break;
    iNumIterations++;
  }

  _fcloseall();

  return 0;
}


int Graph::getmin(int a, int b)
{
  return (a < b) ? a : b;
}


int Graph::getmax(int a, int b)
{
  return (a > b) ? a : b;
}


// ISimulatorEngine

void Graph::SaveStepValuesToVector(std::vector<double>& qvec) const
{
	std::map<int, double> Qi_Vec;

	for (int i = 0; i < iNumTreeVariables; i++)
	{
		int Qi = iTreeInd[i];
		Qi_Vec[Qi] = EX.coeffRef(i);
		//qvec.push_back(EX.coeffRef(i));
	}

	for (int i = 0; i < iNumATreeVariables; i++)
	{
		int Qi = iATreeInd[i];
		Qi_Vec[Qi] = EY.coeffRef(i);
		//qvec.push_back(EY.coeffRef(i));
	}

	qvec.clear();

	for (int i = 0; i < Qi_Vec.size(); i++)
	{
		qvec.push_back(Qi_Vec[i]);
	}
}


int Graph::runSimulation(const char* inputFileName, const SimulationParams& params, ISimulatorCallback& out) 
{
	m_forceStop = false;

	Eigen::VectorXd RHS, dY;

	InitializeEigenR();
	InitializeEigenK();
	InitializeEigenH();
	InitializeEigenY();

	CalculateEigenW();

	EX = (-1) * (EW * EY);

	CalculateEigenSp();

	int iNumIterations = 0;

	//errno_t err;
	//FILE *f;
	// Open for write (will make new file if file "steps_out.txt" does not exist)  
	//err = fopen_s(&f, "steps_out.txt", "w");
	//if (err != 0)
	//{
	//	printf("The file 'steps_out.txt' was not opened\n");
	//	return -1;
	//}

	std::vector<double> qvec;
	SaveStepValuesToVector(qvec);
	out.stepResult(0, 0, qvec);

	double dSimuTime = params.duration;

	int iMaxIterations = dSimuTime / TIME_STEP_SIZE;
	//int iMaxIterations = SIMULATION_TIME / TIME_STEP_SIZE;

	while (iNumIterations++ < iMaxIterations)
	{
		CalculateEigenZ();

		dY = (ESp * EH) - ((ESp * ER) * EZ);
		EY = EY + TIME_STEP_SIZE * dY;
		EX = (-1) * (EW * EY);

		SaveStepValuesToVector(qvec);
		out.stepResult(TIME_STEP_SIZE * iNumIterations, iNumIterations, qvec);

		if (EigenVectorMaxElement(dY) < 1e-05)
			break;

		if (m_forceStop)
			break;
	}

	m_forceStop = false;
	return 0;
}

int Graph::stopSimulation(ISimulatorCallback& out)
{
	m_forceStop = true;

	return 0;
}
 
