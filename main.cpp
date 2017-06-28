#include <QtCore/QCoreApplication>
#include <QtWidgets\QApplication>
#include "TestMain.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TestMain w(argc, argv);
	w.setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
	w.showFullScreen();

	//w.show();
	return a.exec();
}