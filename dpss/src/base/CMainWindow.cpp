#include "CMainWindow.h"

#include <QFileDialog>
#include <QDockWidget>
#include <QStatusBar>
#include <QMenuBar>
#include <QInputDialog>
#include <QTimer>
#include <QDebug>
#include <QProcess>
#include <QCoreApplication>
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QDesktopWidget>
#include <QCloseEvent>


CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_isChanged(false)
{
	QApplication::setOrganizationName("home");
	QApplication::setApplicationName("application");
}

CMainWindow::~CMainWindow()
{
}


void CMainWindow::addDocument(const CDocument& doc)
{
    if (doc.canCreate)
    {
        m_docTypeCreate << doc.type;
    }

    m_docTypes[doc.type] = doc;
}


// protected

void CMainWindow::init(int argc, char *argv[])
{
    createMainMenu();
    createFileToolbar();

    updateTitle();
    updateActions();

	readSettings();

    processParams(argc, argv);
}


void CMainWindow::closeEvent(QCloseEvent *event)
{
	if (saveOnExit()) {
		writeSettings();
		event->accept();
	}
	else {
		event->ignore();
	}
}


void CMainWindow::processParams(int argc, char *argv[])
{
    if (argc >= 3)
    {
        QByteArray command(argv[1]);

        if (command == "create")
        {
            doCreateNewDocument(argv[2]);
            return;
        }

        if (command == "open")
        {
            doOpenDocument(argv[2]);
            return;
        }
    }
}


void CMainWindow::createMainMenu()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));

    m_newDocument = m_fileMenu->addAction(QIcon(":/Icons/New"), tr("&New..."));
    m_newDocument->setStatusTip(tr("Create new document"));

    fillNewFileMenu();

    m_openDocument = m_fileMenu->addAction(QIcon(":/Icons/Open"), tr("&Open..."), this, SLOT(on_actionOpen_triggered()));
    m_openDocument->setStatusTip(tr("Open a document"));
    m_openDocument->setShortcut(QKeySequence::Open);

    m_saveDocument = m_fileMenu->addAction(QIcon(":/Icons/Save"), tr("&Save"), this, SLOT(on_actionSave_triggered()));
    m_saveDocument->setStatusTip(tr("Save current document"));
    m_saveDocument->setShortcut(QKeySequence::Save);

    m_saveAsDocument = m_fileMenu->addAction(tr("Save As..."), this, SLOT(on_actionSaveAs_triggered()));
    m_saveAsDocument->setStatusTip(tr("Save current document under another name"));
    m_saveAsDocument->setShortcut(QKeySequence::SaveAs);

    m_fileMenu->addSeparator();

    QAction *exitApp = m_fileMenu->addAction(tr("E&xit"), this, SLOT(close()));
    exitApp->setStatusTip(tr("Leave the application"));
    exitApp->setShortcut(QKeySequence::Quit);
}


void CMainWindow::fillNewFileMenu()
{
    if (m_docTypeCreate.isEmpty())
        return;

    if (m_docTypeCreate.count() == 1)
    {
        const CDocument& doc = m_docTypes[*m_docTypeCreate.begin()];
        m_newDocument->setText(tr("New") + " " + doc.name);
        m_newDocument->setStatusTip(doc.description);

        connect(m_newDocument, SIGNAL(triggered()), this, SLOT(createNewDocument()));
        m_newDocument->setEnabled(true);
    }
    else
    {
        QMenu *newActionsMenu = new QMenu();
        m_newDocument->setMenu(newActionsMenu);

        for (const QByteArray& docType : m_docTypeCreate)
        {
            const CDocument& doc = m_docTypes[docType];
            QAction *newAction = newActionsMenu->addAction(doc.name);
            newAction->setData(docType);
            newAction->setStatusTip(doc.description);
        }

        connect(newActionsMenu, SIGNAL(triggered(QAction*)), this, SLOT(createNewDocument(QAction*)));
        m_newDocument->setEnabled(true);
    }
}


void CMainWindow::createFileToolbar()
{
    QToolBar *fileToolbar = addToolBar(tr("File"));
    fileToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    fileToolbar->addAction(m_newDocument);
    fileToolbar->addAction(m_openDocument);
    fileToolbar->addAction(m_saveDocument);
    //fileToolbar->addAction(m_saveAsDocument);
}


void CMainWindow::updateTitle()
{
    QString docName = m_currentFileName;

    if (m_currentFileName.isEmpty())
        docName = tr("New File");

    if (m_isChanged)
        docName = "* " + docName;

    setWindowTitle(QString("%1 - %2").arg(docName, QApplication::applicationName()));
}


void CMainWindow::updateActions()
{
    m_saveDocument->setEnabled(m_currentDocType.size());
    m_saveAsDocument->setEnabled(m_currentDocType.size());
}


void CMainWindow::onDocumentChanged()
{
    // already changed - do nothing
    if (m_isChanged)
        return;

    m_isChanged = true;

    updateTitle();
    updateActions();
}


// actions

void CMainWindow::createNewDocument()
{
    doCreateNewDocument(*m_docTypeCreate.begin());
}


void CMainWindow::createNewDocument(QAction *act)
{
    QByteArray docType = act->data().toByteArray();
    doCreateNewDocument(docType);
}


void CMainWindow::doCreateNewDocument(const QByteArray &docType)
{
    // document presents - run new instance
    if (m_currentDocType.size())
    {
        QStringList args;
        args << "create" << docType;
        QProcess::startDetached(QCoreApplication::applicationFilePath(), args);

        return;
    }

    // no document - create in place
    if (onCreateNewDocument(docType))
    {
        m_currentDocType = docType;
        m_isChanged = false;

        updateTitle();
        updateActions();
        return;
    }

    // failed
    qDebug() << "Cannot create document.";
}


bool CMainWindow::onCreateNewDocument(const QByteArray &docType)
{
    qDebug() << docType;

    return true;
}


void CMainWindow::on_actionOpen_triggered()
{
    QString title = tr("Open File");
    QString filter = tr("Any File (*.*)");
    onOpenDocumentDialog(title, filter);

    QString fileName = QFileDialog::getOpenFileName(NULL, title, m_currentFileName, filter, &m_lastOpenFilter);
    if (fileName.isEmpty())
        return;

    QString normalizedName = QDir::toNativeSeparators(fileName);

    doOpenDocument(normalizedName);
}


void CMainWindow::doOpenDocument(const QString &fileName)
{
    // check if the document already opened in another instance
    CMainWindow *window = findDocumentWindow(fileName);
    if (window == this)
        return;
    else
        if (window)
        {
            window->raise();
            return;
        }

    // file does not exist
    if (!QFile::exists(fileName))
    {
        QMessageBox::critical(NULL, tr("Open Error: %1").arg(fileName), tr("Document file does not exist or not accessible."));

        return;
    }

    // document presents - run new instance
    if (m_currentDocType.size())
    {
        QStringList args;
        args << "open" << fileName;
        QProcess::startDetached(QCoreApplication::applicationFilePath(), args);

        return;
    }

    // no document - open in place
    if (onOpenDocument(fileName, m_currentDocType))
    {
        m_currentFileName = fileName;
        m_isChanged = false;

        statusBar()->showMessage(tr("Document opened successfully."));

        updateTitle();
        updateActions();
    }
    else
    {
        QMessageBox::critical(NULL, tr("Open Error: %1").arg(fileName), tr("Document cannot be opened. Check access rights and path."));
    }
}


void CMainWindow::onOpenDocumentDialog(QString &title, QString &filter)
{
    filter = "";

    QString allFilters;

    for (const CDocument& doc : m_docTypes)
    {
        for (const auto& format : doc.formats)
        {
            if (format.canRead)
            {
                filter += format.name + " (" + format.filters + ") ;;";
                allFilters += format.filters + " ";
            }
        }
    }

    if (allFilters.size())
    {
        allFilters.chop(1);
        allFilters = tr("Any supported format (%1)").arg(allFilters);
        filter += allFilters;

        if (m_lastOpenFilter.isEmpty())
            m_lastOpenFilter = allFilters;
    }
}


void CMainWindow::on_actionSave_triggered()
{
	save();
}


void CMainWindow::on_actionSaveAs_triggered()
{
	saveAs();
}


bool CMainWindow::save()
{
	if (m_currentFileName.isEmpty())
		return saveAs();

	return doSaveDocument(m_currentFileName, "", m_currentDocType);
}


bool CMainWindow::saveAs()
{
    if (m_currentDocType.isEmpty())
        return true;

    QString filter;
    const CDocument& doc = m_docTypes[m_currentDocType];
    for (const auto& format : doc.formats)
    {
        if (format.canSave)
        {
            filter += format.name + " (" + format.filters + ") ;;";
        }
    }

    if (filter.size())
        filter.chop(3);

    QString title = tr("Save File");
    onSaveDocumentDialog(title, filter);

    QString selectedFilter = m_lastSaveFilter;
    QString fileName = QFileDialog::getSaveFileName(NULL, title, m_currentFileName, filter, &selectedFilter);
    if (fileName.isEmpty())
        return false;

    QString normalizedName = QDir::toNativeSeparators(fileName);

    return doSaveDocument(normalizedName, selectedFilter, m_currentDocType);
}


bool CMainWindow::doSaveDocument(const QString &fileName, const QString &selectedFilter, const QByteArray &docType)
{
    if (onSaveDocument(fileName, selectedFilter, docType))
    {
        m_currentFileName = fileName;
        m_isChanged = false;
        m_lastSaveFilter = selectedFilter;

        statusBar()->showMessage(tr("Document saved successfully."));

        updateTitle();
        updateActions();

		return true;
    }
    else
    {
        QMessageBox::critical(NULL, tr("Save Error"), tr("Document cannot be saved. Check access rights and path."));
    }

	return false;
}


bool CMainWindow::saveOnExit()
{
	if (!m_isChanged)
		return true;

	const QMessageBox::StandardButton ret = QMessageBox::warning(
		this, 
		QApplication::applicationName(),
		tr("The document has been modified.\n"
				"Do you want to save your changes?"),
		QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

	switch (ret) 
	{
	case QMessageBox::Save:
		return save();
	case QMessageBox::Cancel:
		return false;
	default:
		break;
	}

	return true;
}


CMainWindow* CMainWindow::findDocumentWindow(const QString &fileName)
{
    QString normalizedName = QDir::toNativeSeparators(QFileInfo(fileName).canonicalFilePath());
#ifdef WIN32
    normalizedName = normalizedName.toLower();
#endif

    if (normalizedName == m_currentFileName)
        return this;

    foreach (QWidget *widget, QApplication::topLevelWidgets())
    {
        CMainWindow *mainWin = qobject_cast<CMainWindow*>(widget);
        if (mainWin)
        {
            QString curFileName = mainWin->m_currentFileName;
#ifdef WIN32
            curFileName = curFileName.toLower();
#endif
           if (normalizedName == curFileName)
                return mainWin;
        }
    }

    return NULL;
}


void CMainWindow::readSettings()
{
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	
	const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
	if (geometry.isEmpty()) {
		const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
		resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
		move((availableGeometry.width() - width()) / 2,
			(availableGeometry.height() - height()) / 2);
	}
	else {
		restoreGeometry(geometry);
	}

	if (settings.value("maximized", true).toBool())
		showMaximized();
	else
		showNormal();
}


void CMainWindow::writeSettings()
{
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	settings.setValue("geometry", saveGeometry());
	settings.setValue("maximized", isMaximized());
}
