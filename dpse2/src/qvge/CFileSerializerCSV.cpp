/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "CFileSerializerCSV.h"
#include "CAttribute.h"
#include "CNode.h"
#include "CDirectConnection.h"
#include "CGraphInterface.h"

#include <QFile>
#include <QDebug>
#include <QTextStream>


// reimp

bool CFileSerializerCSV::load(const QString& fileName, CEditorScene& scene) const
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;

	// try to parse
	scene.reset();

    CGraphInterface graph(scene);

    QTextStream ts(&file);
    while (!ts.atEnd())
    {
        QString line = ts.readLine();
        QStringList items = line.split(m_delimiter, QString::SkipEmptyParts);
        if (items.size() < 3)
            continue;

        // edge - start node - end node
        auto edge = graph.addEdge(items[0], items[1], items[2]);
    }

    file.close();

    // update scene rect
    scene.setSceneRect(scene.itemsBoundingRect());

    scene.addUndoState();

	// done
	return true;
}

