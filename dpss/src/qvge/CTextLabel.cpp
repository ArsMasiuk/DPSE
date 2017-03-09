#include "CTextLabel.h"

#include <QFontMetricsF>


CTextLabel::CTextLabel()
{
}

CTextLabel::~CTextLabel()
{
}

void CTextLabel::setText(const QString& text, const QFont& font)
{
	m_text = text;
	m_size = QFontMetricsF(font).size(0, text);

	//Super::setText(txt.replace("\n", "<br>"));

	//m_size = Super::size();	// too long...
}

