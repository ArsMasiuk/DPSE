#include "spincombobox.h"


namespace QSint
{


SpinComboBox::SpinComboBox(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
	mainLayout->setSpacing(0);

    m_editor = new QSpinBox(this);
    mainLayout->addWidget(m_editor);
	m_editor->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	m_button = new QToolButton(this);
	mainLayout->addWidget(m_button);
	m_button->setFixedWidth(16);
	m_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	m_button->setPopupMode(QToolButton::MenuButtonPopup);
	m_button->setVisible(false);

    connect(m_button, SIGNAL(triggered(QAction*)), this, SLOT(onAction(QAction*)));

    connect(m_editor, SIGNAL(valueChanged(int)), this, SLOT(onEditorValueChanged(int)));
}


void SpinComboBox::setMinimum(int val)
{
    m_editor->setMinimum(val);
}


void SpinComboBox::setMaximum(int val)
{
    m_editor->setMaximum(val);
}


void SpinComboBox::setValueList(const QList<int> &values)
{
	m_button->setVisible(!values.isEmpty());
	if (values.isEmpty())
		return;

	QMenu *m = new QMenu(this);
	for (int i : values)
	{
        QAction *a = m->addAction(QString::number(i));
		a->setData(i);
	}

	m_button->setMenu(m);
}


// protected members

// protected slots

void SpinComboBox::onEditorValueChanged(int val)
{
	Q_EMIT valueChanged(val);
}


void SpinComboBox::onAction(QAction* act)
{
	int v = act->data().toInt();

	m_editor->setValue(v);
}



} // namespace
