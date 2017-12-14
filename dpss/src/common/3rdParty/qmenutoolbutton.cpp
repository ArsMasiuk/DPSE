#include "qmenutoolbutton.h"

#include <QMenu>


QMenuToolButton::QMenuToolButton(QWidget *parent) : QToolButton(parent)
{
    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(onAction(QAction*)));
}


QAction* QMenuToolButton::addAction(const QString &text, const QVariant &v)
{
    QAction* act = new QAction(text);
    act->setData(v);
    QToolButton::addAction(act);
    return act;
}


QAction* QMenuToolButton::addAction(const QIcon &icon, const QString &text, const QVariant &v)
{
    QAction* act = new QAction(icon, text);
    act->setData(v);
    QToolButton::addAction(act);
    return act;
}


void QMenuToolButton::actionEvent(QActionEvent *event)
{
    QToolButton::actionEvent(event);

    if (defaultAction() == NULL)
    {
        if (actions().count())
            setDefaultAction(actions().first());
    }
}


void QMenuToolButton::onAction(QAction* act)
{
    setDefaultAction(act);

    if (menu())
        menu()->setDefaultAction(act);

    Q_EMIT activated(act->data());
}


