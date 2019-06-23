#pragma once

#include <QList>
#include <QMenu>
#include <QBoxLayout>
#include <QSpinBox>
#include <QToolButton>
#include <QAction>


namespace QSint
{


/**
    \brief Spin editor with additional drop down menu.
    \since 0.4

    \image html SpinComboBox.png An example of SpinComboBox
*/
class SpinComboBox : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(int maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(int value READ value WRITE setValue)

public:
    /** Constructor.
      */
    explicit SpinComboBox(QWidget *parent = 0);

    int value() const           { return m_editor->value(); }
    int minimum() const         { return m_editor->minimum(); }
    int maximum() const         { return m_editor->maximum(); }
	QAbstractSpinBox::CorrectionMode correctionMode() const					{ return m_editor->correctionMode(); }

    void setValue(int val)      { m_editor->setValue(val); }
    void setMinimum(int val);
    void setMaximum(int val);
	void setCorrectionMode(QAbstractSpinBox::CorrectionMode cm) 			{ m_editor->setCorrectionMode(cm); }

	void setValueList(const QList<int> &values);

Q_SIGNALS:
    /** Signal emitted when the current value is changed either by the user or programmatically.
     */
    void valueChanged(int val);

protected Q_SLOTS:
	void onEditorValueChanged(int val);
    void onAction(QAction* action);

protected:
    QSpinBox *m_editor;
	QToolButton *m_button;
};


} // namespace
