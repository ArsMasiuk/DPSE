/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

#include <QtCore/QString>

#include "CEditorScene.h"


/**
	Common interface to file format serializers.
*/
class IFileSerializer
{
public:
	virtual QString description() const = 0;
	virtual QString filters() const = 0;
	virtual QString defaultFileExtension() const = 0;

	virtual bool loadSupported() const = 0;
	virtual bool load(const QString& fileName, CEditorScene& scene) const = 0;

	virtual bool saveSupported() const = 0;
	virtual bool save(const QString& fileName, const CEditorScene& scene) const = 0;
};
