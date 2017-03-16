#include "CFileSerializerGR.h"
#include "CBranchEditorScene.h"

#include "qvge/CConnection.h"


// reimp

bool CFileSerializerGR::load(const QString& fileName, CEditorScene& scene) const
{
	QSettings iniFile(fileName, QSettings::IniFormat);
	
	if (iniFile.status() != QSettings::NoError)
		return false;

	scene.reset();

	// load items
	int size = iniFile.value("Units/Count", 0).toInt();
	if (size <= 0)
	{
		// done
		scene.activate();

		return true;
	}

	// nodes
	for (int i = 0; i < size; ++i)
	{
		iniFile.beginGroup(QString("Unit %1").arg(i));

		QString typeId = iniFile.value("Type", "").toString();

		if (typeId == "Node")
			readNode(i, iniFile, scene);
		else
		if (typeId == "Cooler")
			readFan(i, iniFile, scene);

		iniFile.endGroup();
	}

	// links
	for (int i = 0; i < size; ++i)
	{
		iniFile.beginGroup(QString("Unit %1").arg(i));

		QString typeId = iniFile.value("Type", "").toString();

		if (typeId == "Link")
			readLink(i, iniFile, scene);

		iniFile.endGroup();
	}

	// done
	scene.activate();

	return true;
}


void CFileSerializerGR::readNodeParams(int index, CNode* node, const QSettings& set) const
{
	float x = set.value("X", 0).toFloat();
	float y = set.value("Y", 0).toFloat();
	QRgb color = set.value("Color", 0).toInt();

	node->setPos(x, y);
	(dynamic_cast<QAbstractGraphicsShapeItem*>(node))->setBrush(QColor(color));

	QString title = set.value("Number", "").toString();
	node->setAttribute("id", title);

	m_nodeMap[index] = node;
}


bool CFileSerializerGR::readNode(int index, const QSettings& set, CEditorScene& scene) const
{
	CBranchNode* node = scene.createItemOfType<CBranchNode>();
	if (!node)
		return false;
	
	readNodeParams(index, node, set);

	scene.addItem(node);

	return true;
}


bool CFileSerializerGR::readFan(int index, const QSettings& set, CEditorScene& scene) const
{
	CFanNode* node = scene.createItemOfType<CFanNode>();
	if (!node)
		return false;

	readNodeParams(index, node, set);

	// attr
    node->setAttribute("R", set.value("R"));
    node->setAttribute("H", set.value("H"));
    node->setAttribute("Q", set.value("Q"));

	scene.addItem(node);

	return true;
}


bool CFileSerializerGR::readLink(int /*index*/, const QSettings& set, CEditorScene& scene) const
{
	CBranchConnection* link = scene.createItemOfType<CBranchConnection>();
	if (!link)
		return false;

	int begin = set.value("BNode", 0).toInt();
	int end = set.value("ENode", 0).toInt();

	CNode* start = m_nodeMap[begin];
	CNode* last = m_nodeMap[end];
	link->setFirstNode(start);
	link->setLastNode(last);

	QRgb color = set.value("Color", 0).toInt();
	link->setAttribute("color", QColor(color));
	
	bool dot = set.value("Dotted", false).toBool();
	if (dot)
		link->setAttribute("style", "dotted");

	QString title = set.value("Number", "").toString();
	link->setAttribute("id", title);

	// attr
    link->setAttribute("L", set.value("L", 0).toStringList().join('.').toFloat());
    link->setAttribute("S", set.value("F1", 0).toStringList().join('.').toFloat());
    //link->setAttribute("LINK_F2", set.value("F2", 0).toStringList().join('.').toFloat());
    link->setAttribute("R", set.value("R", 0).toStringList().join('.').toFloat());
    link->setAttribute("H", set.value("H", 0).toStringList().join('.').toFloat());
    link->setAttribute("Q", set.value("Q", 0).toStringList().join('.').toFloat());

	switch (set.value("Model", 0).toInt())
	{
		case 1:		link->setAttribute("MODEL", "leak");	break;
		case 2:		link->setAttribute("MODEL", "rrv");		break;
		case 3:		link->setAttribute("MODEL", "grrv");	break;
		default:;
	}

	scene.addItem(link);

	return true;
}
