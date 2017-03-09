// test

#include <QDataStream>
#include <QElapsedTimer>
#include <QDebug>
#include <QFile>
#include <QLocale>

#include "CBranchEditorScene.h"
#include "mainwindow.h"

#include "qvge/CNode.h"

#include <windows.h>


struct DiffResult
{
	int startDiff1, lastDiff1;
	int startDiff2, lastDiff2;
	int diffSize;
};

DiffResult diff(const QByteArray &b1, const QByteArray &b2);
int diffme(u_char* old, off_t oldsize, u_char *_new, off_t newsize);

// temp
#ifndef BSDIFF
int diffme(u_char* old, off_t oldsize, u_char *_new, off_t newsize)
{
    return -1;
}
#endif


void processTests(QTextStream& ts, const QByteArray& snap, CBranchEditorScene* m_editorScene)
{
	int bestSize = INT_MAX;
	float bestPerc, bestMs;

	QByteArray last;
	QDataStream ls(&last, QIODevice::WriteOnly);

	QElapsedTimer tm;
	tm.start();
	m_editorScene->storeTo(ls);

	quint64 statesSize = last.size() + snap.size();

	float us1 = (float)tm.nsecsElapsed() / 1000000.0;
	//qDebug() << "Modified state size:" << statesSize << ";" << "Store time:" << us1 << "ms;";
	ts << statesSize << ";" << us1 << ";";

	// comp (Memento)
	tm.restart();
	QByteArray compr1 = qCompress(snap);
	QByteArray compr2 = qCompress(last);

	float us2 = (float)tm.nsecsElapsed() / 1000000.0;
	int csize2 = (compr1.size() + compr2.size());
	float rate2 = (float)csize2 / (float)statesSize * 100.0;
	//qDebug() << "Compressed state size:" << (compr1.size() + compr2.size()) << ";" << rate2 << "%;" << "Compress time:" << us2 << "ms;";
	ts << csize2 << ";" << rate2 << ";" << us2 << ";";

	if (bestSize > csize2) {
		bestSize = csize2; bestPerc = rate2; bestMs = us2;
	}

	// diff search
	tm.restart();
	DiffResult r = diff(snap, last);

	float us3 = (float)tm.nsecsElapsed() / 1000000.0;
	int csize3 = r.diffSize;
	float rate3 = (float)csize3 / (float)statesSize * 100.0;
	//qDebug() << "Searched state size:" << r.diffSize * 2 << ";" << rate3 << "%;" << "Search time:" << us3 << "ms;";
	ts << csize3 << ";" << rate3 << ";" << us3 << ";";

	if (bestSize > csize3) {
		bestSize = csize3; bestPerc = rate3; bestMs = us3;
	}

	// diff comp
	tm.restart();
	QByteArray diffPart1 = snap.mid(r.startDiff1, r.lastDiff1 - r.startDiff1);
	QByteArray comp1 = qCompress(diffPart1);

	QByteArray diffPart2 = last.mid(r.startDiff2, r.lastDiff2 - r.startDiff2);
	QByteArray comp2 = qCompress(diffPart2);

	float us4 = (float)tm.nsecsElapsed() / 1000000.0;
	int csize4 = (comp1.size() + comp2.size());
	float rate4 = (float)csize4 / (float)statesSize * 100.0;
	//qDebug() << "Located compressed state size:" << (comp1.size() + comp2.size()) << ";" << rate4 << "%;" << "Located compress time:" << us4 << "ms;";
	ts << csize4 << ";" << rate4 << ";" << us4 << ";";

	if (bestSize > csize4) {
		bestSize = csize4; bestPerc = rate4; bestMs = us4;
	}

	// diff SES
	//if (r.diffSize > 1)
	//{
	int size1 = diffme((uchar*)diffPart2.data(), diffPart2.size(), (uchar*)diffPart1.data(), diffPart1.size());
	int size2 = diffme((uchar*)diffPart1.data(), diffPart1.size(), (uchar*)diffPart2.data(), diffPart2.size());

	float us5 = (float)tm.nsecsElapsed() / 1000000.0;
	int csize5 = (size1 + size2);
	float rate5 = (float)csize5 / (float)statesSize * 100.0;
	//qDebug() << "SES state size:" << (size1 + size2) << ";" << rate5 << "%;" << "SES time:" << us5 << "ms;";
	ts << csize5 << ";" << rate5 << ";" << us5 << ";";

	if (bestSize > csize5) {
		bestSize = csize5; bestPerc = rate5; bestMs = us5;
	}
	//}


	float mementoRate = (float)bestSize / (float)csize2 * 100.0;
	bestMs = us1 + us3 + us4 + us5;

	ts << bestSize << ";" << bestPerc << ";" << bestMs << ";" << mementoRate << ";";
	ts << "\n";
}


void MainWindow::on_actionUndoTest_triggered()
{
	QFile csv("g:/undotest.csv");
	csv.open(QFile::WriteOnly);
	QTextStream ts(&csv);
	QLocale loc(QLocale::German);
	ts.setLocale(loc);

	// initial state
	QByteArray snap;
	QDataStream ds(&snap, QIODevice::WriteOnly);
	m_editorScene->storeTo(ds);

	//qDebug() << "Initial state size: " << snap.size();

	ts << "Experiment;Serialized.Size;ms;Memento.Size;%;ms;Search.Size;%;ms;Search.Comp.Size;%;ms;SES.Size;%;ms;Best.Size;%;ms" << "\n";

	// run modifications loop

	// 1. single point movement
	int counter = 0;
	QList<QGraphicsItem*> allItems = m_editorScene->items();
	for (QGraphicsItem* item : allItems)
	{
		if (counter >= 30)
			break;

		CNode* cnode = dynamic_cast<CNode*>(item);
		if (cnode)
		{
			QPointF oldPos = cnode->pos();
			cnode->setPos(cnode->pos() + QPointF(qrand() % 100 - 50, qrand() % 100 - 50));

			// compare state
			counter++;
			//qDebug() << "----------- Entry" << counter;

			ts << "Node move " << counter << ";";

			processTests(ts, snap, m_editorScene);

			// restore state
			cnode->setPos(oldPos);
		}
	}

	// 2. single point remove
	for (int i = 0; i < 30; ++i)
	{
		int counter = 0;
		bool done = false;
		QList<QGraphicsItem*> allItems = m_editorScene->items();

		for (QGraphicsItem* item : allItems)
		{
			CNode* cnode = dynamic_cast<CNode*>(item);
			if (cnode)
			{
				if (counter++ == i)
				{
					ts << "Node remove " << counter << ";";

					// initial state
					QByteArray snap;
					QDataStream ds(&snap, QIODevice::WriteOnly);
					m_editorScene->storeTo(ds);

					delete cnode;

					processTests(ts, snap, m_editorScene);

					QDataStream ds1(&snap, QIODevice::ReadOnly);
					m_editorScene->restoreFrom(ds1);

					done = true;
					break;
				}
			}
		}

		if (!done)
			break;
	}


	// 3. single point unlink
	for (int i = 0; i < 30; ++i)
	{
		int counter = 0;
		bool done = false;
		QList<QGraphicsItem*> allItems = m_editorScene->items();

		foreach(QGraphicsItem* item, allItems)
		{
			CNode* cnode = dynamic_cast<CNode*>(item);
			if (cnode)
			{
				if (counter++ == i)
				{
					ts << "Node unlink " << counter << ";";

					QByteArray snap;
					QDataStream ds(&snap, QIODevice::WriteOnly);
					m_editorScene->storeTo(ds);

					cnode->unlink();

					processTests(ts, snap, m_editorScene);

					QDataStream ds1(&snap, QIODevice::ReadOnly);
					m_editorScene->restoreFrom(ds1);

					done = true;
					break;
				}
			}
		}

		if (!done)
			break;
	}


	// 4. single branch remove
	for (int i = 0; i < 30; ++i)
	{
		int counter = 0;
		bool done = false;
		QList<QGraphicsItem*> allItems = m_editorScene->items();

		foreach(QGraphicsItem* item, allItems)
		{
			CConnection* cnode = dynamic_cast<CConnection*>(item);
			if (cnode)
			{
				if (counter++ == i)
				{
					ts << "Branch remove " << counter << ";";

					QByteArray snap;
					QDataStream ds(&snap, QIODevice::WriteOnly);
					m_editorScene->storeTo(ds);

					delete cnode;

					processTests(ts, snap, m_editorScene);

					QDataStream ds1(&snap, QIODevice::ReadOnly);
					m_editorScene->restoreFrom(ds1);

					done = true;
					break;
				}
			}
		}

		if (!done)
			break;
	}
}

