/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/


#include "CTextLabelEdit.h"
#include "CItem.h"

#include <QTextCursor>
#include <QTextDocument>


CTextLabelEdit::CTextLabelEdit()
{
	setTextInteractionFlags(Qt::TextEditorInteraction);

	connect(document(), &QTextDocument::contentsChanged, this, &CTextLabelEdit::updateGeometry);
}

CTextLabelEdit::~CTextLabelEdit()
{
}


void CTextLabelEdit::updateGeometry()
{
	if (m_item)
	{
		QPointF center = m_item->getLabelCenter();
		double w = boundingRect().width();
		double h = boundingRect().height();
		setPos(center.x() - w/2, center.y() - h/2);
	}
}


bool CTextLabelEdit::sceneEvent(QEvent *event)
{
	if (event->type() == QEvent::KeyRelease)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);
		if (keyEvent->matches(QKeySequence::Cancel))	// Esc
		{
			finishEdit(true);
			return true;
		}
	}

	if (event->type() == QEvent::FocusOut)
	{
		finishEdit(true);
		return true;
	}

	return QGraphicsTextItem::sceneEvent(event);
}


void CTextLabelEdit::startEdit(CItem *item)
{
	m_item = item;

	if (m_item == nullptr)
		return;

	auto scene = m_item->getScene();
	if (scene == nullptr)
		return;

	m_storedText = m_item->getAttribute("label").toString();
	m_item->showLabel(false);

	setPlainText(m_storedText);
	setFont(m_item->getAttribute("label.font").value<QFont>());

	updateGeometry();

	//QTextCursor c = textCursor();
	//c.select(QTextCursor::Document);
	//setTextCursor(c);
	
	setFocus();

	scene->addItem(this);
	show();
}


void CTextLabelEdit::finishEdit(bool accept)
{
	if (m_item == nullptr)
		return;

	auto scene = m_item->getScene();
	if (scene == nullptr)
		return;

	QString text = toPlainText();
	if (accept && m_storedText != text)
	{
		m_item->setAttribute("label", text);

		scene->addUndoState();
	}

	m_item->showLabel(true);

	m_item = nullptr;
	scene->removeItem(this);
}
