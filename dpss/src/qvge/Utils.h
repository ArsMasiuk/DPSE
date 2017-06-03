/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

#include <QString>
#include <QVariant>


class Utils
{
public:
	static QString variantToText(const QVariant& v);

	template<class X>
	static void insertUnique(X& dest, const X& from);
};


template<class X>
void Utils::insertUnique(X& dest, const X& from)
{
	for (auto it = from.constBegin(); it != from.constEnd(); ++it)
	{
		if (!dest.contains(it.key()))
			dest[it.key()] = it.value();
	}
}
