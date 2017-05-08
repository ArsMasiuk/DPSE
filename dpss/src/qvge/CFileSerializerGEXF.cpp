#include "CFileSerializerGEXF.h"
#include "CNode.h"
#include "CConnection.h"

#include <QtCore/QFile>

// reimp

bool CFileSerializerGEXF::load(const QString& fileName, CEditorScene& scene) const
{
	// read file into document
	QDomDocument doc("gexf");
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;
	if (!doc.setContent(&file)) {
		file.close();
		return false;
	}
	file.close();

	// try to parse
	scene.reset();

	QDomNodeList graph = doc.elementsByTagName("graph");
	if (graph.count())
	{
		m_edgeType = graph.at(0).toElement().attribute("defaultedgetype", "undirected");
	}

	QDomNodeList nodes = doc.elementsByTagName("node");
	for (int i = 0; i < nodes.count(); ++i)
	{
		readNode(i, nodes.at(i), scene);
	}

	QDomNodeList edges = doc.elementsByTagName("edge");
	for (int i = 0; i < edges.count(); ++i)
	{
		readEdge(i, edges.at(i), scene);
	}

	// done
	scene.activate();

	return true;
}


bool CFileSerializerGEXF::readNode(int index, const QDomNode &domNode, CEditorScene &scene) const
{
	QDomElement elem = domNode.toElement();

	CNode* node = scene.createItemOfType<CNode>();
	if (!node)
		return false;

	// common attrs
	QString id = elem.attribute("id", "");
	node->setAttribute("id", id);

	QString label = elem.attribute("label", "");
	node->setAttribute("label", label);

	// viz: attrs
	QDomNodeList viz_pos = elem.elementsByTagName("viz:position");
	if (viz_pos.size()) {
		QDomElement viz_elem = viz_pos.at(0).toElement();
		float x = viz_elem.attribute("x", "0").toFloat();
		float y = viz_elem.attribute("y", "0").toFloat();
		float z = viz_elem.attribute("z", QString(index)).toFloat();
		node->setPos(x, y);
		node->setZValue(z);
	}

	QDomNodeList viz_color = elem.elementsByTagName("viz:color");
	if (viz_color.size()) {
		QDomElement viz_elem = viz_color.at(0).toElement();
		int r = viz_elem.attribute("r", "0").toInt();
		int g = viz_elem.attribute("g", "0").toInt();
		int b = viz_elem.attribute("b", "0").toInt();
		QColor color(r, g, b);
		node->setAttribute("color", color);
	}

	QDomNodeList viz_size = elem.elementsByTagName("viz:size");
	if (viz_size.size()) {
		QDomElement viz_elem = viz_size.at(0).toElement();
		float v = viz_elem.attribute("value", "5").toFloat();
		node->resize(v);
	}

	scene.addItem(node);

	m_nodeMap[id] = node;

	return true;
}


bool CFileSerializerGEXF::readEdge(int /*index*/, const QDomNode &domNode, CEditorScene& scene) const
{
	QDomElement elem = domNode.toElement();

	CConnection* link = scene.createItemOfType<CConnection>();
	if (!link)
		return false;

	// common attrs
	QString id = elem.attribute("id", "");
	link->setAttribute("id", id);

	QString label = elem.attribute("label", "");
	link->setAttribute("label", id);

	QString source = elem.attribute("source", "");
	QString target = elem.attribute("target", "");

	CNode* start = m_nodeMap[source];
	CNode* last = m_nodeMap[target];
	link->setFirstNode(start);
	link->setLastNode(last);

	// line
	float weight = elem.attribute("weight", "-1").toFloat();
	if (weight >= 0)
		link->setAttribute("weight", weight);

	// direction
	QString edgeType = elem.attribute("defaultedgetype", "");
	if (edgeType.isEmpty())
		edgeType = m_edgeType;

	link->setAttribute("direction", edgeType);

	scene.addItem(link);

	return true;
}

