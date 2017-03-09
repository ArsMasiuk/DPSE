#ifndef CDOCUMENTCONTROLLER_H
#define CDOCUMENTCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QByteArray>
#include <QList>


class IDocument;


class CDocumentController: public QObject
{
    Q_OBJECT

public:
    CDocumentController(QObject *parent = 0);
    virtual ~CDocumentController();

    virtual bool addDocumentFactory(IDocument *factory);

    virtual IDocument* createDocument(const QByteArray& factoryId);

Q_SIGNALS:
    void documentFactoryAdded(IDocument *factory);
    void documentCreated(IDocument *doc);

protected:
    QMap<QByteArray, IDocument*> m_docFactories;

    QList<IDocument*> m_documents;
};

#endif // CDOCUMENTCONTROLLER_H
