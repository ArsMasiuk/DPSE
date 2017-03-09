#include "CDocumentController.h"
#include "IDocument.h"


CDocumentController::CDocumentController(QObject *parent) : QObject(parent)
{
}

CDocumentController::~CDocumentController()
{
    // destroy factories
    qDeleteAll(m_docFactories);
}

bool CDocumentController::addDocumentFactory(IDocument *factory)
{
    if (!factory)
        return false;

    if (m_docFactories.contains(factory->id()))
        return false;

    m_docFactories[factory->id()] = factory;

    Q_EMIT documentFactoryAdded(factory);

    return true;
}

IDocument* CDocumentController::createDocument(const QByteArray& factoryId)
{
    if (!m_docFactories.contains(factoryId))
        return NULL;

    IDocument* doc = m_docFactories[factoryId]->createDocument();
    if (doc)
    {
        m_documents.append(doc);

        Q_EMIT documentCreated(doc);
    }

    return doc;
}
