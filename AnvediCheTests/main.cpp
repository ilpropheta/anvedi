#include <QtCore/QCoreApplication>
#include "TestRunner.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	return RUN_ALL_TESTS_COLOR(argc, argv);
}
