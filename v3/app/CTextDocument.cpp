#include "CTextDocument.h"

CTextDocument::CTextDocument()
{
    static int count = 0;

    m_docName = QString("New Text %1").arg(count++);
}
