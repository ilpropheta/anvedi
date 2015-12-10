#include <QtWidgets/qapplication.h>
#include "TestRunner.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	return RUN_ALL_TESTS_COLOR(argc, argv);
}
