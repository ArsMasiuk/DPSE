#ifndef QMENUTOOLBUTTON_H
#define QMENUTOOLBUTTON_H

#include <QToolButton>
#include <QIcon>
#include <QVariant>
#include <QAction>


class QMenuToolButton : public QToolButton
{
    Q_OBJECT

public:
    QMenuToolButton(QWidget *parent = Q_NULLPTR);

    QAction* addAction(const QString &text, const QVariant &data = QVariant());
    QAction* addAction(const QIcon &icon, const QString &text, const QVariant &data = QVariant());

Q_SIGNALS:
    void activated(QVariant data);

protected:
    virtual void actionEvent(QActionEvent *event);

private Q_SLOTS:
    void onAction(QAction* act);
};

#endif // QMENUTOOLBUTTON_H
