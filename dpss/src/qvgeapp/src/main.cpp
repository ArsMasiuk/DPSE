/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "qvgeMainWindow.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/Icons/Icon"));
    qvgeMainWindow w;
    w.init(argc, argv);
    w.show();
	return a.exec();
}
