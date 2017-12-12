#ifndef CNODEPROPERTIESUI_H
#define CNODEPROPERTIESUI_H

#include <QWidget>

namespace Ui {
class CNodePropertiesUI;
}

class CNodePropertiesUI : public QWidget
{
    Q_OBJECT

public:
    explicit CNodePropertiesUI(QWidget *parent = 0);
    ~CNodePropertiesUI();

private:
    Ui::CNodePropertiesUI *ui;
};

#endif // CNODEPROPERTIESUI_H
