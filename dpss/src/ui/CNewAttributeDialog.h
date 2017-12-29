#ifndef CNEWATTRIBUTEDIALOG_H
#define CNEWATTRIBUTEDIALOG_H

#include <QDialog>
#include <QVariant>
#include <QByteArray>

namespace Ui {
class CNewAttributeDialog;
}

class CNewAttributeDialog : public QDialog
{
    Q_OBJECT

public:
	struct Result {
		QByteArray id;
		QString name;
		QVariant v;
	};

	static Result getAttribute();

    explicit CNewAttributeDialog(QWidget *parent = 0);
    ~CNewAttributeDialog();

    QByteArray getAttrId() const;
    QString getAttrName() const;
    QVariant getDefaultValue() const;

private:
    Ui::CNewAttributeDialog *ui;
};


#endif // CNEWATTRIBUTEDIALOG_H
