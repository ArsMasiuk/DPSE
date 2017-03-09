#ifndef CTEXTDOCUMENT_H
#define CTEXTDOCUMENT_H

#include "IDocument.h"

#include <QTextEdit>


class CTextDocument :
        public QTextEdit,
        public IDocument
{
public:
    CTextDocument();

    // reimp - factory
    virtual QByteArray id() const {
        return "text";
    }

    virtual QString name() const {
        return QObject::tr("Text Document");
    }

    virtual QString description() const {
        return QObject::tr("Plain text document");
    }

    virtual IDocument* createDocument() {
        return new CTextDocument();
    }

    // reimp - io
    virtual const QString& documentName() const {
        return m_docName;
    }

    // reimp - view
    virtual QWidget* editor() {
        return this;
    }

protected:
    QString m_docName;
};

#endif // CTEXTDOCUMENT_H
