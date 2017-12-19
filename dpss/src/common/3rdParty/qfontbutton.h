#ifndef QFONTBUTTON_H
#define QFONTBUTTON_H

#include <QToolButton>
#include <QWidgetAction>
#include <QFontDialog>
#include <QMenu>


class QFontButton : public QToolButton
{
    Q_OBJECT

public:
    QFontButton(QWidget *parent = Q_NULLPTR);

Q_SIGNALS:
    void activated(const QFont &font);

private Q_SLOTS:
    void onClicked();
    void onDialogShown();
    void onDialogFontSelected(const QFont&);

private:
    QFontDialog *m_fontDialog;
};


#endif // QFONTBUTTON_H
