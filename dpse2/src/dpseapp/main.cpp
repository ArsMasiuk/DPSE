/*
This file is a part of
DPSE - Distributed Parallel Simulation Environment

(c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include <QtWidgets/QApplication>

#include "dpseMainWindow.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setWindowIcon(QIcon(":/Icons/AppIcon"));
	dpseMainWindow w;
	w.init(argc, argv);
    w.show();
	return a.exec();
}
