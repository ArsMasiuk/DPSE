#ifndef CTABCONTROLLER_H
#define CTABCONTROLLER_H

#include <QTabWidget>


class IDocument;


class CTabController : public QTabWidget
{
    Q_OBJECT

public:
    explicit CTabController(QWidget *parent = 0);

Q_SIGNALS:

public Q_SLOTS:
    void onDocumentCreated(IDocument *doc);
};

#endif // CTABCONTROLLER_H
