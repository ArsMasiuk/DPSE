#ifndef CSCENEOPTIONSDIALOG_H
#define CSCENEOPTIONSDIALOG_H

#include <QDialog>

#include <qvge/CEditorScene.h>

namespace Ui {
class CSceneOptionsDialog;
}

class CSceneOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CSceneOptionsDialog(QWidget *parent = 0);
    ~CSceneOptionsDialog();

public Q_SLOTS:
	virtual int exec(CEditorScene &scene);

private:
    Ui::CSceneOptionsDialog *ui;
};

#endif // CSCENEOPTIONSDIALOG_H
