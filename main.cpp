#include "qtGuiCV.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qtGuiCV w;
	w.show();
	return a.exec();
	
}
