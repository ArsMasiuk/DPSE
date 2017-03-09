#include "CTabController.h"
#include "IDocument.h"


CTabController::CTabController(QWidget *parent) : QTabWidget(parent)
{

}

void CTabController::onDocumentCreated(IDocument *doc)
{
    QWidget *tab = doc->editor();

    int index = addTab(tab, doc->documentName());

    setCurrentIndex(index);
}
