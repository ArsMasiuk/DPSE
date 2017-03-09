#include "CUndoManager.h"
#include "CEditorScene.h"

#include <QDataStream>
#include <QElapsedTimer>
#include <QDebug>


CUndoManager::CUndoManager(CEditorScene &scene)
	: m_scene(&scene)
{

}

CUndoManager::~CUndoManager()
{

}


// private

struct DiffResult
{
	int startDiff1, lastDiff1;
	int startDiff2, lastDiff2;
	int diffSize;
};


DiffResult diff(const QByteArray &b1, const QByteArray &b2)
{
	DiffResult r;

	int minSize = qMin(b1.size(), b2.size());
	r.startDiff1 = -1;
	r.startDiff2 = -1;

	// first unmatch
	for (int i = 0; i < minSize; ++i)
	{
		if (b1[i] != b2[i])
		{
			r.startDiff1 = i;
			r.startDiff2 = i;
			break;
		}
	}

	if (r.startDiff1 < 0)
	{
		r.lastDiff1 = b1.size();
		r.lastDiff2 = b2.size();
		r.startDiff1 = b1.size() - (r.lastDiff1 - minSize);
		r.startDiff2 = b2.size() - (r.lastDiff2 - minSize);
		r.diffSize = (r.lastDiff1 - r.startDiff1) + (r.lastDiff2 - r.startDiff2);
		return r;
	}

	// last unmatch
	r.lastDiff1 = b1.size() - 1;
	r.lastDiff2 = b2.size() - 1;

	for (int i = r.startDiff1; i < minSize; ++i, --r.lastDiff1, --r.lastDiff2)
	{
		if (b1[r.lastDiff1] != b2[r.lastDiff2])
		{
			break;
		}
	}

	// check
	r.diffSize = (++r.lastDiff1 - r.startDiff1) + (++r.lastDiff2 - r.startDiff1);
	return r;
}


typedef quint32 CheckPatternType;
const int MinCheckPatternSize = sizeof(CheckPatternType);


DiffResult diffSub(const QByteArray &b1, const QByteArray &b2, const DiffResult& in)
{
	DiffResult r;
	r.startDiff1 = in.startDiff1;
	r.startDiff2 = in.startDiff2;
	r.diffSize = 0;

	// at end of first string?
	if ((in.startDiff1 >= in.lastDiff1 - MinCheckPatternSize+1) || (in.startDiff2 >= in.lastDiff2 - MinCheckPatternSize+1))
	{
		r.lastDiff1 = in.lastDiff1/* + 1*/;
		r.lastDiff2 = in.lastDiff2/* + 1*/;
		r.diffSize = (r.lastDiff1 - r.startDiff1) + (r.lastDiff2 - r.startDiff2);
		return r;
	}

	// check the strings
	for (int i = in.startDiff1; i < in.lastDiff1 - MinCheckPatternSize + 1; ++i)
	{
		CheckPatternType s1 = *(CheckPatternType*)(b1.constData() + i);

		for (int j = in.startDiff2; j < in.lastDiff2 - MinCheckPatternSize + 1; ++j)
		{
			CheckPatternType s2 = *(CheckPatternType*)(b2.constData() + j);

			if (s1 == s2)
			{
				// signature found; dump result
				DiffResult t;
				t.startDiff1 = in.startDiff1;
				t.startDiff2 = in.startDiff2;
				t.lastDiff1 = i;
				t.lastDiff2 = j;
				t.diffSize = (t.lastDiff1 - t.startDiff1) + (t.lastDiff2 - t.startDiff2);

				// store the result if distance is closer
				//int tdist = t.lastDiff1 + t.lastDiff2;
				//int rdist = r.lastDiff1 + r.lastDiff2;
				if (t.diffSize < r.diffSize || !r.diffSize)
					r = t;

				break;
			}
		}
	}

	return r;
}


// reimp

struct ExtDiffResult
{
	int size;
	int count;
};


ExtDiffResult doExtDiff(const QByteArray &t1, const QByteArray &t2, DiffResult r)
{
	ExtDiffResult res;
	res.size = res.count = 0;

	//    r.lastDiff1++;
	//    r.lastDiff2++;

	DiffResult rr = r;
	rr.startDiff2 = rr.startDiff1;

	//qDebug() << t1.mid(rr.startDiff1, rr.lastDiff1 - rr.startDiff1);
	//qDebug() << t2.mid(rr.startDiff2, rr.lastDiff2 - rr.startDiff2);

	while (rr.diffSize)
	{
		rr = diffSub(t1, t2, rr);

		// loop matched
		if (rr.diffSize)
		{
			res.count++;
			res.size += rr.diffSize;

			//qDebug() << t1.mid(rr.startDiff1, rr.lastDiff1 - rr.startDiff1);
			//qDebug() << t2.mid(rr.startDiff2, rr.lastDiff2 - rr.startDiff2);

			int i1 = rr.lastDiff1 + MinCheckPatternSize;
			int i2 = rr.lastDiff2 + MinCheckPatternSize;
			while (i1 < r.lastDiff1 && i2 < r.lastDiff2 && t1[i1] == t2[i2])
			{
				i1++;
				i2++;
			}

			// done?
			if (i1 >= r.lastDiff1 && i2 >= r.lastDiff2)
			{
				return res;
			}

			rr.startDiff1 = i1;
			rr.startDiff2 = i2;
			rr.lastDiff1 = r.lastDiff1;
			rr.lastDiff2 = r.lastDiff2;

			if (i1 >= r.lastDiff1)
			{
				rr.diffSize = (r.lastDiff2 - i2);

				res.count++;
				res.size += rr.diffSize;

				//qDebug() << t1.mid(rr.startDiff1, rr.lastDiff1 - rr.startDiff1);
				//qDebug() << t2.mid(rr.startDiff2, rr.lastDiff2 - rr.startDiff2);

				return res;
			}

			if (i2 >= r.lastDiff2)
			{
				rr.diffSize = (r.lastDiff1 - i1);

				res.count++;
				res.size += rr.diffSize;

				//qDebug() << t1.mid(rr.startDiff1, rr.lastDiff1 - rr.startDiff1);
				//qDebug() << t2.mid(rr.startDiff2, rr.lastDiff2 - rr.startDiff2);

				return res;
			}

			// continue...


			//qDebug() << t1.mid(rr.startDiff1, rr.lastDiff1 - rr.startDiff1);
			// qDebug() << t2.mid(rr.startDiff2, rr.lastDiff2 - rr.startDiff2);
		}
		else
		{
			res.count++;
			res.size += (r.lastDiff1 - r.startDiff1);
			res.size += (r.lastDiff2 - r.startDiff2);
		}
	}

	return res;
}


// reimp

void CUndoManager::reset()
{
	m_currentState.clear();
}


void test()
{
	QByteArray t1 = "Just spoken with Akakiy Pavsikakiev and found him total stupid";
	QByteArray t2 = "Just spoken about Akakiy Pavsikakiev with Petro and he found Akakiy total useless and stupid";

	// diff
	DiffResult r = diff(t1, t2);

	// ext. diff
	if (r.startDiff1 > 0 && r.diffSize > 100)
	{
		ExtDiffResult res = doExtDiff(t1, t2, r);
		qDebug() << "ExtDiffResult: count" << res.count <<"size" << res.size;
	}
}



#include <windows.h>

int diffme(u_char* old, off_t oldsize, u_char *_new, off_t newsize);


void CUndoManager::addState()
{
	//test();
	return;


	// to do: diff
	QByteArray snap;
	QDataStream ds(&snap, QIODevice::WriteOnly);

	qDebug() << "---------------------------------------------------";

	QElapsedTimer tm;
	tm.start();

	m_scene->storeTo(ds);

	quint64 t1 = tm.elapsed();
	qDebug() << "Time:" << t1 << "Serialized Size:" << snap.size() + m_currentState.size();
	tm.restart();

	// compression
	QByteArray compr = qCompress(snap);
	QByteArray compr2 = qCompress(m_currentState);

	quint64 t2 = tm.elapsed();
	qDebug() << "Time:" << t2 << "Compressed Size:" << compr.size() + compr2.size();
		//<< "%" << compr.size() * 100.0 / snap.size();
	tm.restart();

	// diff
	DiffResult r = diff(snap, m_currentState);

	quint64 t3 = tm.elapsed();
	qDebug() << "Time:" << t3 << "Diff Size:" << r.diffSize;
	tm.restart();

	// diff comp
	QByteArray diffPart1 = snap.mid(r.startDiff1, r.lastDiff1 - r.startDiff1);
	QByteArray comp1 = qCompress(diffPart1);

	QByteArray diffPart2 = m_currentState.mid(r.startDiff2, r.lastDiff2 - r.startDiff2);
	QByteArray comp2 = qCompress(diffPart2);

	quint64 t4 = tm.elapsed();
	qDebug() << "Time:" << t4 << "CompDiff Size:" << comp1.size() + comp2.size();
	tm.restart();

	// ext. diff
	if (r.diffSize > 1)
	{
		//ExtDiffResult res = doExtDiff(snap, m_currentState, r);
		
		int size1 = diffme((uchar*)m_currentState.data(), m_currentState.size(), (uchar*)snap.data(), snap.size());
		int size2 = diffme((uchar*)snap.data(), snap.size(), (uchar*)m_currentState.data(), m_currentState.size());

		quint64 t5 = tm.elapsed();

		//qDebug() << "Time:" << t5 << "ExtDiffResult: count" << res.count << "size" << res.size;
		qDebug() << "Time:" << t5 << "ExtDiff Size: " << size1 + size2;
		tm.restart();



		int size3 = diffme((uchar*)diffPart2.data(), diffPart2.size(), (uchar*)diffPart1.data(), diffPart1.size());
		int size4 = diffme((uchar*)diffPart1.data(), diffPart1.size(), (uchar*)diffPart2.data(), diffPart2.size());

		quint64 t6 = tm.elapsed();

		qDebug() << "Time:" << t6 << "Diff.ExtDiff Size: " << size3 + size4;
		tm.restart();
	}

	// temp
	m_currentState = snap;
}

void CUndoManager::undo()
{

}

void CUndoManager::redo()
{

}
