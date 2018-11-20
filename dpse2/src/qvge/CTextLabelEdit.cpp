/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/


#include "CTextLabelEdit.h"
#include "CItem.h"

#include <QTextCursor>


CTextLabelEdit::CTextLabelEdit()
{
	setTextInteractionFlags(Qt::TextEditorInteraction);
}

CTextLabelEdit::~CTextLabelEdit()
{
}


bool CTextLabelEdit::sceneEvent(QEvent *event)
{
	if (event->type() == QEvent::KeyRelease)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);
		if (keyEvent->matches(QKeySequence::Cancel))	// Esc
		{
			finishEdit(false);
			return true;
		}
	}

	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);
		if (keyEvent->matches(QKeySequence::InsertParagraphSeparator))	// Enter
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

	setPlainText(m_item->getAttribute("label").toString());
	setFont(m_item->getAttribute("label.font").value<QFont>());

	QTextCursor c = textCursor();
	c.select(QTextCursor::Document);
	setTextCursor(c);
	
	setPos(item->getSceneItem()->pos());
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

	if (accept && 
		m_item->getAttribute("label").toString() != toPlainText()	)
	{
		m_item->setAttribute("label", toPlainText());

		scene->addUndoState();
	}

	m_item = nullptr;
	scene->removeItem(this);
}
