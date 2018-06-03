/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2017 Ars L.Masiuk(ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include <QImageWriter>
#include <QImage>
#include <QPainter>
#include <QMap>
#include <QByteArray>
#include <QSet>

#include "CImageExport.h"
#include "CEditorScene.h"


QString CImageExport::filters() const
{
	static QList<QByteArray> formats = QImageWriter::supportedImageFormats();
	if (formats.isEmpty())
		return QString();

	static QMap<QByteArray, QString> formatNames = {
		{ "bmp", "Windows Bitmap (*.bmp)" },
        { "ico", "Windows Icon (*.ico *.cur)" },
		{ "gif", "Graphic Interchange Format (*.gif)" },
        { "jpg", "Joint Photographic Experts Group (*.jpg *.jpeg)" },
		{ "png", "Portable Network Graphics (*.png)" },
		{ "pbm", "Portable Bitmap (*.pbm)" },
		{ "pgm", "Portable Graymap (*.pgm)" },
		{ "ppm", "Portable Pixmap (*.ppm)" },
		{ "svg", "Scalable Vector Graphics (*.svg)" },
        { "tif", "Tagged Image File Format (*.tif *.tiff)" },
		{ "xbm", "X11 Bitmap (*.xbm)" },
		{ "xpm", "X11 Pixmap (*.xpm)" },
		{ "wbmp", "Wireless Bitmap (*.wbmp)" },
		{ "webp", "WebP (*.webp)" },
		{ "icns", "Apple Icon Image (*.icns)" }
	};

	static QMap<QByteArray, QByteArray> recodeMap = {
		{ "jpeg", "jpg" },
		{ "tiff", "tif" },
		{ "cur", "ico" }
	};

	static QString filter;
	if (filter.isEmpty())
	{
		QSet<QByteArray> usedFormats;

		for (auto format : formats)
		{
			auto suffix = format.toLower();
			if (recodeMap.contains(suffix))
				usedFormats << recodeMap[suffix];
			else
				usedFormats << suffix;
		}

		for (auto format : usedFormats)
		{
			if (formatNames.contains(format))
				filter += formatNames[format] + ";;";
			else
				filter += format + " (*." + format + ");;";
		}

		filter.chop(2);
	}

	return filter;
}


bool CImageExport::save(const QString& fileName, CEditorScene& scene) const
{
	QImage image(scene.sceneRect().size().toSize(), QImage::Format_ARGB32);
	image.fill(Qt::white);
	QPainter painter(&image);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);
	scene.render(&painter);

	return image.save(fileName);
}
