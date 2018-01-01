#ifndef CNewAttributeDialog_H
#define CNewAttributeDialog_H

#include <QDialog>

namespace Ui {
class CNewAttributeDialog;
}

class CNewAttributeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CNewAttributeDialog(QWidget *parent = 0);
    ~CNewAttributeDialog();

    QString getId() const;
    int getType() const;
    QVariant getValue() const;

private:
    Ui::CNewAttributeDialog *ui;
};

#endif // CNewAttributeDialog_H
