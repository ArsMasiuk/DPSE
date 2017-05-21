/*
This file is a part of
DPSE - Distributed Parallel Simulation Environment

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <QtCore/QTimer>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	QTimer::singleShot(100, &w, SLOT(showMaximized()));

	a.processEvents();
	return a.exec();
}
