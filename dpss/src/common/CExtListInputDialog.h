#ifndef CExtListInputDialog_H
#define CExtListInputDialog_H

#include <QDialog>

namespace Ui {
class CExtListInputDialog;
}

class CExtListInputDialog : public QDialog
{
    Q_OBJECT

public:
    ~CExtListInputDialog();

    static int getItemIndex(const QString &title, const QString &label,
                            const QStringList& texts, const QList<QIcon>& icons, int selectedIndex = -1);

private:
	explicit CExtListInputDialog(QWidget *parent = 0);
	
	Ui::CExtListInputDialog *ui;
};


#endif // CExtListInputDialog_H
