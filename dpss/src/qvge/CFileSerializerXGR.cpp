/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "CFileSerializerXGR.h"

#include <QtCore/QFile>
#include <QtCore/QDataStream>

// reimp

bool CFileSerializerXGR::load(const QString& fileName, CEditorScene& scene) const
{
	// read file into document
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;

	scene.reset();

	QDataStream ds(&openFile);
	scene.restoreFrom(ds);
	scene.readOptions(ds);

	return true;
}


bool CFileSerializerXGR::save(const QString& fileName, const CEditorScene& scene) const
{
	QFile saveFile(fileName);
	if (saveFile.open(QFile::WriteOnly))
	{
		QDataStream ds(&saveFile);
		scene.storeTo(ds);

		return true;
	}

	return false;
}
