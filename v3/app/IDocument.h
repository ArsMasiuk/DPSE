#ifndef IDOCUMENT_H
#define IDOCUMENT_H

#include <QString>
#include <QWidget>


class IDocument
{
public:
    virtual ~IDocument() {}

    // factorisation
    virtual QByteArray id() const = 0;
    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual IDocument* createDocument() = 0;

    // io
    virtual const QString& documentName() const = 0;
//    virtual bool isStored() const = 0;
//    virtual void setStored(bool stored) = 0;
//    virtual QString fileName() const = 0;
//    virtual void setFileName(const QString& name) = 0;

    // view (?)
    virtual QWidget* editor() = 0;
};


#endif // IDOCUMENT_H
