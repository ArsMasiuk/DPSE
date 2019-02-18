/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2019 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

class CEditorScene;


class ISceneEditController
{
public:
	virtual void onActivated(CEditorScene& scene) = 0;
	virtual void onDeactivated(CEditorScene& scene) = 0;
	virtual void draw(CEditorScene& scene, QPainter *painter, const QRectF &r) = 0;
};
