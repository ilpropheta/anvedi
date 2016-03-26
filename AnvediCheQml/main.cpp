#include <QtWidgets/QApplication>
#include <QtQuick/QQuickView>
#include "../Anvedi/qml-lib/RegisterAll.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QmlModulesInstaller::Install();
	QQuickView view(QUrl("../Anvedi/qml/main.qml"));
	view.setResizeMode(QQuickView::SizeRootObjectToView);
	view.resize(800, 600);
	view.show();
	return a.exec();
}
