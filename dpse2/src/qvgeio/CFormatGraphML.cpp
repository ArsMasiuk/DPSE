/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "CFormatGraphML.h"

#include <QFile>
#include <QDebug>


// reimp

bool CFormatGraphML::save(const QString& fileName, Graph& graph, QString* lastError) const
{
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly))
		return false;

	QXmlStreamWriter xsw(&file);
	xsw.setCodec("UTF-8");
	xsw.setAutoFormatting(true);
	xsw.setAutoFormattingIndent(4);

	xsw.writeStartDocument();
	xsw.writeStartElement("graphml");
	xsw.writeAttribute("xmlns", "http://graphml.graphdrawing.org/xmlns");
	xsw.writeAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	xsw.writeAttribute("xsi:schemaLocation", "http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd");

	// attrs
	writeAttributes(xsw, graph.graphAttrs, "");
	writeAttributes(xsw, graph.nodeAttrs, "node");
	writeAttributes(xsw, graph.edgeAttrs, "edge");

	xsw.writeStartElement("graph");

	// graph attrs
	if (graph.edgeAttrs.contains("direction"))
	{
		xsw.writeAttribute("edgedefault", graph.edgeAttrs["direction"].defaultValue.toString());
	}

	// nodes
	writeNodes(xsw, graph);

	// edges
	writeEdges(xsw, graph);


	xsw.writeEndElement();	// graph

	xsw.writeEndElement();	// graphml
	xsw.writeEndDocument();

	return true;
}


void CFormatGraphML::writeNodes(QXmlStreamWriter &xsw, const Graph& graph) const
{
	for (const auto& node : graph.nodes)
	{
		xsw.writeStartElement("node");
		
		// topology
		xsw.writeAttribute("id", node.id);

		for (const auto& port : node.ports)
		{
			xsw.writeStartElement("port");
			
			xsw.writeAttribute("name", port.name);
			// TO DO: placement, attrs...

			xsw.writeEndElement();
		}

		// attributes
		// TO DO

		xsw.writeEndElement();
	}
}


void CFormatGraphML::writeEdges(QXmlStreamWriter &xsw, const Graph& graph) const
{
	for (const auto& edge: graph.edges)
	{
		xsw.writeStartElement("edge");

		// topology
		xsw.writeAttribute("id", edge.id);
		xsw.writeAttribute("source", edge.startNodeId);
		xsw.writeAttribute("target", edge.endNodeId);
		if (edge.startPortId.size())
			xsw.writeAttribute("sourceport", edge.startPortId);
		if (edge.endPortId.size())
			xsw.writeAttribute("endport", edge.endPortId);

		// attributes
		// TO DO

		xsw.writeEndElement();
	}
}


void CFormatGraphML::writeAttributes(QXmlStreamWriter &xsw, const AttributeInfos &attrs, const QByteArray &classId) const
{
	for (auto& attr : attrs)
	{
		xsw.writeStartElement("key");
		xsw.writeAttribute("attr.name", attr.name);
		xsw.writeAttribute("attr.id", attr.id);

		if (classId.size())
			xsw.writeAttribute("for", classId);

		switch (attr.valueType)
		{
		case QVariant::Int:			xsw.writeAttribute("attr.type", "integer"); break;
		case QVariant::Double:		xsw.writeAttribute("attr.type", "double"); break;
		case QMetaType::Float:		xsw.writeAttribute("attr.type", "float"); break;
		case QMetaType::Bool:		xsw.writeAttribute("attr.type", "boolen"); break;
		case QMetaType::QString:	xsw.writeAttribute("attr.type", "string"); break;
		default:;
		}

		if (attr.defaultValue.isValid())
		{
			xsw.writeTextElement("default", attr.defaultValue.toString());
		}

		xsw.writeEndElement();	// key
	}
}


bool CFormatGraphML::load(const QString& fileName, Graph& graph, QString* lastError) const
{
	// read file into document
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;

	QString errorString;
	int errorLine, errorColumn;

	QDomDocument doc("graphml");
	if (!doc.setContent(&file, false, &errorString, &errorLine, &errorColumn))
	{
		file.close();

		if (lastError)
			*lastError = QObject::tr("%1\nline: %2, column: %3").arg(errorString).arg(errorLine).arg(errorColumn);

		return false;
	}

	file.close();

	// try to parse
	graph.clear();

	QDomNodeList tree = doc.elementsByTagName("graph");
	if (tree.count())
	{
		m_edgeType = tree.at(0).toElement().attribute("edgedefault", "undirected");
	}

	KeyAttrMap cka;

	QDomNodeList keys = doc.elementsByTagName("key");
	for (int i = 0; i < keys.count(); ++i)
	{
		readAttrKey(i, keys.at(i), graph, cka);
	}

	QDomNodeList nodes = doc.elementsByTagName("node");
	for (int i = 0; i < nodes.count(); ++i)
	{
		readNode(i, nodes.at(i), graph, cka);
	}

	QDomNodeList edges = doc.elementsByTagName("edge");
	for (int i = 0; i < edges.count(); ++i)
	{
		readEdge(i, edges.at(i), graph, cka);
	}

	// done
	return true;
}


bool CFormatGraphML::readAttrKey(int /*index*/, const QDomNode& domNode, Graph& graph, KeyAttrMap& cka) const
{
	QDomElement elem = domNode.toElement();

	QString attrId = elem.attribute("id", "");
	QString classId = elem.attribute("for", "");
	QString nameId = elem.attribute("attr.name", "");
	QString valueType = elem.attribute("attr.type", "");

	if (attrId.isEmpty())
		return false;

	AttrInfo attr;
	attr.id = attrId.toLatin1();
	attr.name = nameId.isEmpty() ? attrId : nameId;

	if (valueType == "integer" || valueType == "long") {
		attr.valueType = QVariant::Int;
		attr.defaultValue.setValue(elem.text().toInt());
	}
	//else if (valueType == "long") {
	//	attr.valueType = QVariant::LongLong;
	//	attr.defaultValue.setValue(elem.text().toLongLong());
	//}
	else if (valueType == "double") {
		attr.valueType = QVariant::Double;
		attr.defaultValue.setValue(elem.text().toDouble());
	}
	else if (valueType == "float") {
		attr.valueType = QMetaType::Float;
		attr.defaultValue.setValue(elem.text().toFloat());
	}
	else if (valueType == "boolean") {
		attr.valueType = QMetaType::Bool;
		attr.defaultValue.setValue(!!elem.text().toInt());
	}
	else {
		attr.valueType = QMetaType::QString;
		attr.defaultValue.setValue(elem.text());
	}

	QByteArray attrClassId = classId.toLower().toLatin1();
	AttributeInfos& attrInfos =
		(attrClassId == "node") ? graph.nodeAttrs :
		(attrClassId == "edge") ? graph.edgeAttrs :
		graph.graphAttrs;

	attrInfos[attr.id] = attr;

	if (attrClassId == "graph") attrClassId = "";

	cka[attr.id] = ClassAttrId(attrClassId, attr.id);	// d0 = node:x

	return true;
}


bool CFormatGraphML::readNode(int /*index*/, const QDomNode &domNode, Graph& graph, const KeyAttrMap& cka) const
{
	QDomElement elem = domNode.toElement();

	Node node;

	// common attrs
	auto id = elem.attribute("id", "").toLocal8Bit();
	node.id = id;

	QDomNodeList data = elem.elementsByTagName("data");
	for (int i = 0; i < data.count(); ++i)
	{
		QDomNode dm = data.at(i);
		QDomElement de = dm.toElement();

		QString key = de.attribute("key", "");
		ClassAttrId classAttrId = cka[key.toLocal8Bit()];
		QByteArray attrId = classAttrId.second;

		if (!attrId.isEmpty())
		{
			node.attrs[attrId] = de.text();

			if (attrId == "tooltip")
				node.attrs["label"] = de.text();
			else
			if (attrId == "x_coordinate")
				node.attrs["x"] = de.text();
			else
			if (attrId == "y_coordinate")
				node.attrs["y"] = de.text();
		}
	}

	// ports
	QDomNodeList portNodes = elem.elementsByTagName("port");
	for (int i = 0; i < data.count(); ++i)
	{
		QDomNode dm = data.at(i);
		QDomElement de = dm.toElement();

		QString portName = de.attribute("name", "");
		if (portName.isEmpty())
			continue;

		NodePort port;
		port.name = portName;
		node.ports[portName] = port;
	}

	graph.nodes.append(node);

	return true;
}


bool CFormatGraphML::readEdge(int /*index*/, const QDomNode &domNode, Graph& graph, const KeyAttrMap& cka) const
{
	QDomElement elem = domNode.toElement();
	
	Edge edge;
	edge.startNodeId = elem.attribute("source", "").toLocal8Bit();
	edge.startPortId = elem.attribute("sourceport", "").toLocal8Bit();
	edge.endNodeId = elem.attribute("target", "").toLocal8Bit();
	edge.endPortId = elem.attribute("targetport", "").toLocal8Bit();

	// common attrs
	QString id = elem.attribute("id", "");
	edge.id = id.toLocal8Bit();

	QDomNodeList data = elem.elementsByTagName("data");
	for (int i = 0; i < data.count(); ++i)
	{
		QDomNode dm = data.at(i);
		QDomElement de = dm.toElement();

		QString key = de.attribute("key", "");
		QByteArray attrId = cka[key.toLatin1()].second;
		if (!attrId.isEmpty())
		{
			edge.attrs[attrId] = de.text();
		}
	}

	graph.edges.append(edge);

	return true;
}


