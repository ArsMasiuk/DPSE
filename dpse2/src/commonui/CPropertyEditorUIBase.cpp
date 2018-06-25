#include "CPropertyEditorUIBase.h"


CPropertyEditorUIBase::CPropertyEditorUIBase(QWidget * parent) : QtTreePropertyBrowser(parent) 
{
	setResizeMode(Interactive);
}

CPropertyEditorUIBase::~CPropertyEditorUIBase() 
{
	
}


QtBrowserItem* CPropertyEditorUIBase::selectItemByName(const QString& name)
{
	QList<QtBrowserItem*> items = topLevelItems();
	for (auto item : items)
	{
		if (item->property()->propertyName() == name)
		{
			setCurrentItem(item);
			return item;
		}
	}

	return NULL;
}


QtProperty* CPropertyEditorUIBase::getCurrentTopProperty() const
{
	auto item = currentItem();
	if (!item)
		return NULL;

	while (item->parent())
		item = item->parent();

	return item->property();
}


QString CPropertyEditorUIBase::getCurrentTopPropertyName() const
{
	QtProperty *prop = getCurrentTopProperty();
	if (prop)
		return prop->propertyName();
	else
		return "";
}