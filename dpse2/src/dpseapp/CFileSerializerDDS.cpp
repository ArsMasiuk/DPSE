/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "CFileSerializerDDS.h"

#include <qvge/CAttribute.h>
#include <qvge/CNode.h>
#include <qvge/CDirectEdge.h>
#include <qvge/CGraphInterface.h>
#include <qvge/CNodeEditorScene.h>

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QRegularExpression>


// reimp

bool CFileSerializerDDS::load(const QString& fileName, CEditorScene& scene) const
{
    CNodeEditorScene* nodeScene = dynamic_cast<CNodeEditorScene*>(&scene);
    if (nodeScene == nullptr)
        return false;

	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;

	// try to parse
	scene.reset();

    CGraphInterface graph(*nodeScene);

    QTextStream ts(&file);
    while (!ts.atEnd())
    {
        QString line = ts.readLine();
        QStringList items = line.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
        if (items.size() < 3)
            continue;

        // start node - end node - edge
        auto edge = graph.addEdge(items[2], items[0], items[1]);
		if (edge)
		{
			if (items.size() >= 4)
				edge->setAttribute("R", items[3].toDouble());

			if (items.size() >= 5)
				edge->setAttribute("S", items[4].toDouble());

			if (items.size() >= 6)
				edge->setAttribute("L", items[5].toDouble());

			if (items.size() >= 7)
				edge->setAttribute("H", items[6].toDouble());
		}
    }

    file.close();

    // update scene rect
    scene.setSceneRect(scene.itemsBoundingRect());

    scene.addUndoState();

	// done
	return true;
}

