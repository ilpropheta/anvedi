#include "..\Anvedi\anvedi.h"
#include <QtWidgets/QApplication>

using namespace std;

int main(int argc, char *argv [])
{
	QApplication a(argc, argv);
	
	Anvedi w;
	w.show();

	try
	{
		return a.exec();
	}
	catch (const std::exception& ex)
	{
		QMessageBox msg;
		msg.setWindowTitle("Unexpected error");
		msg.setInformativeText("Anfame!!!");
		msg.setFont(QFont("serif", 15));
		msg.setIconPixmap(QPixmap("mario.jpg"));
		msg.setDefaultButton(QMessageBox::Ok);
		msg.show();
		msg.exec();
		return -1;
	}
}
