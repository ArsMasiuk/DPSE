#pragma once

#include <QtCore/QSettings>

#include "qvge/IFileSerializer.h"


class CNode;

class CFileSerializerGR : public IFileSerializer
{
public:
	// reimp
	virtual QString description() const {
		return "Old mine ventilation network graph";
	}

	virtual QString filters() const {
		return "*.gr";
	}

	virtual QString defaultFileExtension() const {
		return "gr";
	}

	virtual bool loadSupported() const {
		return true;
	}

	virtual bool load(const QString& fileName, CEditorScene& scene) const;

	virtual bool saveSupported() const {
		return false;
	}

	virtual bool save(const QString& /*fileName*/, const CEditorScene& /*scene*/) const {
		return false;
	}

private:
	bool readNode(int index, const QSettings& set, CEditorScene& scene) const;
	bool readLink(int index, const QSettings& set, CEditorScene& scene) const;
	bool readFan(int index, const QSettings& set, CEditorScene& scene) const;

	void readNodeParams(int index, CNode* node, const QSettings& set) const;

	mutable QMap<int, CNode*> m_nodeMap;
};

