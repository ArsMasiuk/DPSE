#pragma once

#include <QtTreePropertyBrowser>


class CPropertyEditorUIBase : public QtTreePropertyBrowser
{
	Q_OBJECT

public:
	CPropertyEditorUIBase(QWidget * parent = Q_NULLPTR);
	~CPropertyEditorUIBase();

	QtBrowserItem* selectItemByName(const QString& name);
	QtProperty* getCurrentTopProperty() const;
	QString getCurrentTopPropertyName() const;
};
