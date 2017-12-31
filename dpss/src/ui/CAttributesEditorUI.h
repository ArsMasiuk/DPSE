#ifndef CATTRIBUTESEDITORUI_H
#define CATTRIBUTESEDITORUI_H

#include <QWidget>

namespace Ui {
class CAttributesEditorUI;
}

class CAttributesEditorUI : public QWidget
{
    Q_OBJECT

public:
    explicit CAttributesEditorUI(QWidget *parent = 0);
    ~CAttributesEditorUI();

private:
    Ui::CAttributesEditorUI *ui;
};

#endif // CATTRIBUTESEDITORUI_H
