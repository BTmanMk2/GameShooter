#include <QtCore/QCoreApplication>
#include <QtWidgets\qapplication.h>
#include "TestMain.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TestMain w(argc, argv);

	w.show();
	return a.exec();
}
