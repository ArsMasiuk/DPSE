/*
This file is a part of
DPSE - Distributed Parallel Simulation Environment

(c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "dpseMainWindow.h"
#include "dpseVersion.h"
#include "dpseUIController.h"
#include "CFileSerializerDDS.h"

#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QFileInfo>

#include <appbase/CPlatformServices.h>

#include <qvge/CNodeEditorScene.h>


dpseMainWindow::dpseMainWindow()
{
	QString bitString;
	int bits = CPlatformServices::GetPlatformBits();
	if (bits > 0) bitString = QString(" %1bit").arg(bits);

    QApplication::setOrganizationName("DonNTU");
    QApplication::setApplicationName("Distributed Parallel Simulation Environment");
    QApplication::setApplicationVersion(dpseVersion.toString() + tr(" (Beta)") + bitString);

	CDocumentFormat gexf = { "GEXF", "*.gexf", {"gexf"}, true, true };
	CDocumentFormat graphml = { "GraphML", "*.graphml", {"graphml"}, false, true };
//    CDocumentFormat gr = { "Old plain GR", "*.gr", false, true };
	CDocumentFormat xgr = { "XML Graph", "*.xgr", {"xgr"}, true, true };
    CDocumentFormat gml = { "GML", "*.gml", { "gml" }, false, true };
    CDocumentFormat dds = { "DDS", "*.dds", { "dds" }, false, true };
    CDocument graph = { tr("Graph Document"), tr("Directed or undirected graph"), "graph", true,
                        {gexf, graphml, gml, xgr, dds} };
    addDocument(graph);
}


void dpseMainWindow::init(const QStringList& args)
{
    Super::init(args);

    statusBar()->showMessage(tr("DPSE started."));
}


bool dpseMainWindow::createDocument(const QByteArray &docType)
{
    // scene
    if (docType == "graph")
    {
        m_graphEditController = new dpseUIController(this);

        return true;
    }

    // unknown type
    return false;
}


void dpseMainWindow::onNewDocumentCreated(const QByteArray &docType)
{
	// wizard
	if (docType == "graph")
	{
        m_graphEditController->onNewDocumentCreated();
	}
}


bool dpseMainWindow::openDocument(const QString &fileName, QByteArray &docType)
{
	QString format = QFileInfo(fileName).suffix().toLower();

	// default graph formats
    if (format == "graphml" || format == "gexf" || format == "xgr" || format == "gml")
	{
		docType = "graph";

        if (createDocument(docType) && m_graphEditController->loadFromFile(fileName, format))
		{
			return true;
		}

		return false;
	}


	// dds format
	if (format == "dds")
	{
		docType = "graph";

		if (createDocument(docType))
		{
			CFileSerializerDDS dds;

			return dds.load(fileName, *(m_graphEditController->scene()));
		}

		return false;
	}

	// unknown
    return false;
}


bool dpseMainWindow::saveDocument(const QString &fileName, const QString &/*selectedFilter*/, const QByteArray &docType)
{
    // graph
	if (docType == "graph")
	{
		QString extType = QFileInfo(fileName).suffix().toLower();

        return m_graphEditController->saveToFile(fileName, extType);
	}

    // unknown type
    return false;
}


QString dpseMainWindow::getAboutText() const
{
	return Super::getAboutText()
		+ QString(
			"<p>This is a free software."
			"<br>It comes without warranty of any kind. Use it on your own risk."
			"<p>&copy; 2016-2018 Ars L. Masiuk");
}


void dpseMainWindow::doReadSettings(QSettings& settings)
{
	Super::doReadSettings(settings);

	if (m_graphEditController)
	{
		m_graphEditController->doReadSettings(settings);
	}
}


void dpseMainWindow::doWriteSettings(QSettings& settings)
{
	Super::doWriteSettings(settings);

	if (m_graphEditController)
	{
		m_graphEditController->doWriteSettings(settings);
	}
}
