#include "qvgeMainWindow.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    qvgeMainWindow w;
	w.showMaximized();
	return a.exec();
}