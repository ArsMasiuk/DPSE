#include "CAttributesEditorUI.h"
#include "ui_CAttributesEditorUI.h"

#include <ui/CNewAttributeDialog.h>

#include <qvge/CEditorScene.h>
#include <qvge/CItem.h>

#include <CIntegerProperty.h>
#include <CDoubleProperty.h>
#include <CBoolProperty.h>
#include <CStringProperty.h>

#include <QMessageBox>


CAttributesEditorUI::CAttributesEditorUI(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CAttributesEditorUI)
{
	ui->setupUi(this);
}

CAttributesEditorUI::~CAttributesEditorUI()
{
	delete ui;
}


void CAttributesEditorUI::setupFromItems(CEditorScene& scene, QList<CItem*> &items)
{
	m_scene = &scene;
	m_items = items;

	ui->Editor->setUpdatesEnabled(false);

	ui->Editor->clear();

	struct AttrData
	{
		QVariant data;
		int dataType;
		bool used;
	};

	QSet<QByteArray> ids;
	QMap<QByteArray, AttrData> attrs;

	// merge ids
	for (auto item : items)
	{
		auto localAttrs = item->getLocalAttributes();
		ids = ids.unite(localAttrs.keys().toSet());
	}

	// merge attrs
	for (auto id : ids)
	{
		for (auto item : items)
		{
			if (item->hasLocalAttribute(id))
			{
				auto itemValue = item->getAttribute(id);
				attrs[id].dataType = itemValue.type();

				if (attrs[id].used)
				{
					if (attrs[id].data != itemValue)
					{
						attrs[id].data = QVariant();
						attrs[id].used = true;
					}
				}
				else
				{
					attrs[id].data = itemValue;
				}
			}
			else
			{
				attrs[id].data = QVariant();
				attrs[id].used = true;
			}
		}
	}

	for (auto it = attrs.constBegin(); it != attrs.constEnd(); ++it)
	{
		CBaseProperty* prop = NULL;

		switch (it.value().dataType)
		{
			case QVariant::Bool:
			{
				prop = new CBoolProperty(
					it.key(),
					it.key(),
					it.value().data.toBool());
				break;
			}

			case QVariant::Int:
			case QVariant::UInt:
			case QVariant::LongLong:
			case QVariant::ULongLong:
			{
				prop = new CIntegerProperty(
					it.key(),
					it.key(),
					it.value().data.toInt());
				break;
			}

			case QVariant::Double:
			{
				prop = new CDoubleProperty(
					it.key(),
					it.key(),
					it.value().data.toDouble());
				break;
			}

			case QVariant::String:
			{
				prop = new CStringProperty(
					it.key(),
					it.key(),
					it.value().data.toString());
				break;
			}

			default:;   //  unknown
		}

		if (prop)
			ui->Editor->add(prop);
	}

	ui->Editor->setUpdatesEnabled(true);
}


void CAttributesEditorUI::on_AddButton_clicked()
{
	if (!m_scene || m_items.isEmpty())
		return;

	auto r = CNewAttributeDialog::getAttribute();
	if (r.id.isEmpty())
		return;

	bool used = false;

	for (auto sceneItem : m_items)
	{
		if (sceneItem->hasLocalAttribute(r.id))
			continue;

		sceneItem->setAttribute(r.id, r.v);
		used = true;
	}

	if (!used)
		return;

	// store state
	m_scene->addUndoState();

	// rebuild tree
	setupFromItems(*m_scene, m_items);
}


void CAttributesEditorUI::on_RemoveButton_clicked()
{
	if (!m_scene || m_items.isEmpty())
		return;

	auto prop = dynamic_cast<CBaseProperty*>(ui->Editor->currentItem());
	if (!prop)
		return;

	auto attrId = prop->getId();
	if (attrId.isEmpty())
		return;

	int r = QMessageBox::question(NULL,
		tr("Remove Attribute"),
		tr("Remove attribute '%1' from selected item(s)?").arg(QString(attrId)),
		QMessageBox::Yes, QMessageBox::Cancel);

	if (r == QMessageBox::Cancel)
		return;

	bool used = false;

	for (auto sceneItem : m_items)
	{
		used |= sceneItem->removeAttribute(attrId);
	}

	if (!used)
		return;

	// store state
	m_scene->addUndoState();

	// rebuild tree
	setupFromItems(*m_scene, m_items);
}

