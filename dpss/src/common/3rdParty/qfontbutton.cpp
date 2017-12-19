#include "qfontbutton.h"


QFontButton::QFontButton(QWidget *parent) : QToolButton(parent)
{
    setToolButtonStyle(Qt::ToolButtonTextOnly);
    setPopupMode(QToolButton::MenuButtonPopup);

    setText("Aa");

    connect(this, SIGNAL(clicked()), this, SLOT(onClicked()));

    m_fontDialog = new QFontDialog(this);
    m_fontDialog->setOption(QFontDialog::DontUseNativeDialog, false);
    m_fontDialog->setSizeGripEnabled(false);

    QMenu *menu = new QMenu(this);
    setMenu(menu);

    QWidgetAction *dialogAction = new QWidgetAction(this);
    dialogAction->setDefaultWidget(m_fontDialog);
    menu->addAction(dialogAction);

    connect(menu, SIGNAL(aboutToShow()), this, SLOT(onDialogShown()));
    connect(m_fontDialog, SIGNAL(accepted()), menu, SLOT(hide()));
    connect(m_fontDialog, SIGNAL(rejected()), menu, SLOT(hide()));
    connect(m_fontDialog, SIGNAL(fontSelected(const QFont&)), this, SLOT(onDialogFontSelected(const QFont&)));
}


void QFontButton::onClicked()
{
    Q_EMIT activated(font());
}


void QFontButton::onDialogShown()
{
    m_fontDialog->setCurrentFont(font());

    m_fontDialog->setVisible(true);
}


void QFontButton::onDialogFontSelected(const QFont &font)
{
    setFont(font);

    onClicked();
}

