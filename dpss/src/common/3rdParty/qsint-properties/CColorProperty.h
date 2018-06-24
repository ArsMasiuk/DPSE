#ifndef CCOLORPROPERTY_H
#define CCOLORPROPERTY_H

#include "CBaseProperty.h"
#include "CButtonBasedEditor.h"

#include "QColorComboBox.h"


class CColorProperty;


class CColorButtonEditor : public TButtonBasedEditor<QColorComboBox>
{
	Q_OBJECT
public:
	CColorButtonEditor(QColorComboBox* colorComboEditor, CColorProperty* property);

protected Q_SLOTS:
	void onPicked(QString);

protected:
	virtual void onEditButtonActivated();

	CColorProperty* m_property;
};


class CColorProperty : public CBaseProperty
{
public:
    CColorProperty(const QByteArray& id, const QString &name, const QColor& color);
    CColorProperty(CBaseProperty *top, const QByteArray& id, const QString &name, const QColor& color);

    void setColor(const QColor& color);
    const QColor& getColor() const;

    void setColorsList(const QStringList& colorNames);

    void allowListColorsOnly(bool on);

    // reimp
    virtual QVariant getVariantValue() const;
    virtual void validateValue();
    virtual void displayValue();

    virtual QWidget* createEditor() const;
    virtual void valueToEditor();
    virtual void valueFromEditor();

protected:
    mutable QColor m_color;

    mutable QColorComboBox m_colorEditor;
    bool m_listColorsOnly;
};

#endif // CCOLORPROPERTY_H