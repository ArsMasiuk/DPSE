﻿#pragma once

#include "IFileSerializer.h"

#include <QtXml/QDomDocument>

class CNode;


class CFileSerializerGEXF : public IFileSerializer 
{
public:

	// reimp
	virtual QString description() const {
		return "Graph Exchange XML Format";
	}

	virtual QString filters() const {
		return "*.gexf";
	}

	virtual QString defaultFileExtension() const {
		return "gexf";
	}

	virtual bool loadSupported() const {
		return true;
	}

	virtual bool load(const QString& fileName, CEditorScene& scene) const;

	virtual bool saveSupported() const {
		return false;
	}

	virtual bool save(const QString& fileName, const CEditorScene& scene) const {
		return false;
	}

private:
	bool readNode(int index, const QDomNode &domNode, CEditorScene& scene) const;
	bool readEdge(int index, const QDomNode &domNode, CEditorScene& scene) const;

	mutable QMap<QString, CNode*> m_nodeMap;

	enum EdgeType {
		Directed,
		Undirected,
		Mutual
	};
	mutable QString m_edgeType;
};



