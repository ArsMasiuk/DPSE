#include "CStartPage.h"
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
<<<<<<< HEAD
    const auto &docTypes = m_parent->getRegisteredDocumentTypes();
=======
	auto &docTypes = m_parent->getRegisteredDocumentTypes();
>>>>>>> eba7b8a2551ccdd6fbe0bccde92601185babbbaa

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
<<<<<<< HEAD
    const auto &recentList = m_parent->getRecentFilesList();
=======
	auto &recentList = m_parent->getRecentFilesList();
>>>>>>> eba7b8a2551ccdd6fbe0bccde92601185babbbaa

	for (const QString &fileName : recentList)
	{
		QFileInfo fi(fileName);

		QCommandLinkButton *fileButton = new QCommandLinkButton(
			fi.baseName(),
			fileName,
			this
		);

<<<<<<< HEAD
        ui.RightWidget->layout()->addWidget(fileButton);
	}


    ui.RightWidget->layout()->addItem(new QSpacerItem(1, 10, QSizePolicy::MinimumExpanding));
=======
		ui.RightWidget->layout()->addWidget(fileButton);
	}


	if (recentList.size())
		ui.RightWidget->layout()->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
>>>>>>> eba7b8a2551ccdd6fbe0bccde92601185babbbaa
}

