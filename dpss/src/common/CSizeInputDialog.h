#ifndef CSIZEINPUTDIALOG_H
#define CSIZEINPUTDIALOG_H

#include <QDialog>

namespace Ui {
class CSizeInputDialog;
}

class CSizeInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CSizeInputDialog(QWidget *parent = 0);
    ~CSizeInputDialog();

private:
    Ui::CSizeInputDialog *ui;
};

#endif // CSIZEINPUTDIALOG_H
