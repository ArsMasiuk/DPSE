#pragma once

#include <QWidget>
<<<<<<< HEAD

#include "ui_CStartPage.h"
=======
#include "ui_cstartpage.h"
>>>>>>> eba7b8a2551ccdd6fbe0bccde92601185babbbaa

class CMainWindow;


class CStartPage : public QWidget 
{
	Q_OBJECT

public:
	CStartPage(CMainWindow *parent);
	~CStartPage();

protected:
	void createActions();
	void createRecentDocs();

protected Q_SLOTS:
	void onCreateDocument();

private:
	Ui::CStartPage ui;
	CMainWindow *m_parent;
};
