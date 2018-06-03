/*
This file is a part of
DPSE - Distributed Parallel Simulation Environment

(c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

#include <QPlainTextEdit>
#include <QStatusBar>

#include <appbase/CMainWindow.h>


class CNodeEditorUIController;


class dpseMainWindow : public CMainWindow
{
    Q_OBJECT

public:
    typedef CMainWindow Super;

    friend class CNodeEditorUIController;

    dpseMainWindow();

    virtual void init(int argc, char *argv[]);

protected:
    virtual bool createDocument(const QByteArray &docType);
	virtual void onNewDocumentCreated(const QByteArray &docType);
    virtual bool openDocument(const QString &fileName, QByteArray &docType);
    virtual bool saveDocument(const QString &fileName, const QString &selectedFilter, const QByteArray &docType);

	virtual QString getAboutText() const;

	virtual void doReadSettings(QSettings& settings);
	virtual void doWriteSettings(QSettings& settings);
	
private:
    CNodeEditorUIController *m_graphEditController = NULL;

    QPlainTextEdit *m_textEditor = NULL;
};

