#include "CButtonBasedEditor.h"

#include <QHBoxLayout>
#include <QEvent>
#include <QDebug>


CButtonBasedEditor::CButtonBasedEditor(QWidget *hostedEditor, QWidget *parent) :
    QWidget(parent),
    m_hostedEditor(hostedEditor)
{
    QHBoxLayout* hbl = new QHBoxLayout();
    hbl->setContentsMargins(0,0,0,0);
    hbl->setSpacing(0);
    setLayout(hbl);

    hbl->addWidget(m_hostedEditor);

    m_button = new QToolButton(this);
    m_button->setText("...");
    hbl->addWidget(m_button);

    connect(m_button, SIGNAL(clicked()), this, SLOT(onEditButtonActivated()));

	connect(this, SIGNAL(destroyed(QObject*)), this, SLOT(onDestroyed(QObject*)));
}


CButtonBasedEditor::~CButtonBasedEditor()
{
	disconnect(this);
}


void CButtonBasedEditor::enableButton(bool on)
{
   m_button->setVisible(on);
}


bool CButtonBasedEditor::event(QEvent *e)
{
    if (e->type() == QEvent::FocusIn)
    {
        m_hostedEditor->setFocus();

		e->accept();
        return true;
    }

	if (e->type() == QEvent::DeferredDelete)
	{
		layout()->removeWidget(m_hostedEditor);
		m_hostedEditor->setParent(NULL);
		m_hostedEditor->hide();
		m_hostedEditor = NULL;

		return QWidget::event(e);
	}

    return QWidget::event(e);
}


void CButtonBasedEditor::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);

    m_hostedEditor->show();
}


void CButtonBasedEditor::hideEvent(QHideEvent *e)
{
	QWidget::hideEvent(e);
}


void CButtonBasedEditor::onDestroyed(QObject* obj)
{
	if (obj == this)
	{
		layout()->removeWidget(m_hostedEditor);
		m_hostedEditor->setParent(NULL);
		m_hostedEditor->hide();
		m_hostedEditor = NULL;
	}
}
