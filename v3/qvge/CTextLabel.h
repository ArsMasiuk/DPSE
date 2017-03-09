#pragma once

#include <QPointF> 
#include <QSizeF>
#include <QFont> 
#include <QString> 

class CTextLabel
{
public:
	CTextLabel();
	~CTextLabel();

	void setText(const QString& txt, const QFont& = QFont());

	const QSizeF& localSize() const { return m_size; }
	const QString& text() const { return m_text; }

private:
	QSizeF m_size;
	QString m_text;
};

