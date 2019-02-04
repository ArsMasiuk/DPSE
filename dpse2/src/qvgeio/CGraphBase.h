/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2019 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

#include <QMap>
#include <QByteArray>
#include <QVariant>
#include <QList>


typedef QMap<QByteArray, QVariant> GraphAttributes;


struct AttrInfo
{
	QByteArray id;
	QString name;
	int valueType = 0;
	QVariant defaultValue;
};

typedef QMap<QByteArray, AttrInfo> AttributeInfos;


struct NodePort
{
	QString name;
	//QPoint pos;
};

typedef QMap<QString, NodePort> NodePorts;


struct Node
{
	QByteArray id;
	GraphAttributes attrs;
	NodePorts ports;
};


struct Edge
{
	QByteArray id;
	GraphAttributes attrs;

	QByteArray startNodeId;
	QByteArray startPortId;
	QByteArray endNodeId;
	QByteArray endPortId;
};


struct Graph
{
	QList<Node> nodes;
	QList<Edge> edges;
	GraphAttributes attrs;

	AttributeInfos nodeAttrs;
	AttributeInfos edgeAttrs;
	AttributeInfos graphAttrs;

	// methods
	void clear();
};

