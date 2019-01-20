﻿#include "CStartPage.h"
#include "CMainWindow.h"

#include <QCommandLinkButton>
#include <QSpacerItem>
#include <QFileInfo>


CStartPage::CStartPage(CMainWindow *parent) : QWidget(parent)
{
	ui.setupUi(this);

	m_parent = parent;

	createActions();
	createRecentDocs();
}

CStartPage::~CStartPage() 
{
}


void CStartPage::createActions()
{
	auto &docTypes = m_parent->getRegisteredDocumentTypes();

	// create document actions
	for (auto &doc : docTypes)
	{
		if (doc.canCreate)
		{
			QCommandLinkButton *newFileButton = new QCommandLinkButton(
				tr("Create") + " " + doc.name, doc.description, this
			);
			
			QAction *newFile = new QAction(newFileButton);
			newFile->setData(doc.type);

			connect(newFileButton, &QCommandLinkButton::clicked, newFile, &QAction::triggered);
			connect(newFile, &QAction::triggered, this, &CStartPage::onCreateDocument);

			ui.LeftWidget->layout()->addWidget(newFileButton);
		}
	}

	// spacer
	ui.LeftWidget->layout()->addItem(new QSpacerItem(1, 50));


	// open document actions
	QCommandLinkButton *openFileButton = new QCommandLinkButton(
		tr("Open..."), tr("Open existing document(s)"), this
	);

	openFileButton->setIcon(QIcon(":/Icons/Open"));

	connect(openFileButton, &QCommandLinkButton::clicked, m_parent, &CMainWindow::selectAndOpenDocument);

	ui.LeftWidget->layout()->addWidget(openFileButton);
}


void CStartPage::onCreateDocument()
{
	QAction *act = dynamic_cast<QAction*>(sender());
	if (act)
		m_parent->createNewDocument(act->data().toByteArray());
}


void CStartPage::createRecentDocs()
{
	auto &recentList = m_parent->getRecentFilesList();

	for (const QString &fileName : recentList)
	{
		QFileInfo fi(fileName);

		QCommandLinkButton *fileButton = new QCommandLinkButton(
			fi.baseName(),
			fileName,
			this
		);

		ui.RightWidget->layout()->addWidget(fileButton);
	}


	if (recentList.size())
		ui.RightWidget->layout()->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
}

