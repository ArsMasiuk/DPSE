#ifndef CNEWATTRIBUTEDIALOG_H
#define CNEWATTRIBUTEDIALOG_H

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

private:
    Ui::CNewAttributeDialog *ui;
};

#endif // CNEWATTRIBUTEDIALOG_H
