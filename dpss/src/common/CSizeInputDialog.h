#ifndef CSIZEINPUTDIALOG_H
#define CSIZEINPUTDIALOG_H

#include <QDialog>
#include <QSize>
#include <QSizeF>

namespace Ui {
class CSizeInputDialog;
}

class CSizeInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CSizeInputDialog(QWidget *parent = 0);
    ~CSizeInputDialog();

    static QSize getSize(const QString &title, const QString &label, const QSize& defaultSize,
        const QSize& minSize = QSize(), const QSize& maxSize = QSize(),
        const QString& unit = QString::null);

	static QSizeF getSizeF(const QString &title, const QString &label, const QSizeF& defaultSize,
		const QSizeF& minSize = QSizeF(), const QSizeF& maxSize = QSizeF(),
		const QString& unit = QString::null, 
		int decimals = 4);

private Q_SLOTS:
	void on_cbHeightAsWidth_toggled(bool);

private:
    Ui::CSizeInputDialog *ui;
};

#endif // CSIZEINPUTDIALOG_H
