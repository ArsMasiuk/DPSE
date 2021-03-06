/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

#include <qvge/IFileSerializer.h>

class CNode;


class CFileSerializerDDS : public IFileSerializer
{
public:
	// reimp
	virtual QString description() const {
        return "DDS Table";
	}

	virtual QString filters() const {
        return "*.dds";
	}

	virtual QString defaultFileExtension() const {
        return "dds";
	}

	virtual bool loadSupported() const {
		return true;
	}

	virtual bool load(const QString& fileName, CEditorScene& scene) const;

	virtual bool saveSupported() const {
		return false;
	}

	virtual bool save(const QString& /*fileName*/, CEditorScene& /*scene*/) const {
		return false;
	}
};



