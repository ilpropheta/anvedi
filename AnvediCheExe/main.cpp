#include "..\Anvedi\anvedi.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv [])
{
	QApplication a(argc, argv);
	Anvedi w;
	w.show();
	return a.exec();
}
