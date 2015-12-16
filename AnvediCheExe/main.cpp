#include "..\Anvedi\anvedi.h"
#include <QtWidgets/QApplication>
#include <QQuickView>
#include "../qml-lib/RegisterAll.h"
#include "../qml-lib/qmlPen.h"

int main(int argc, char *argv [])
{
	QApplication a(argc, argv);
	
	/*QmlModulesInstaller::Install();
	QQuickView view(QUrl("../Anvedi/qml/main.qml"));
	view.setResizeMode(QQuickView::SizeRootObjectToView);
	view.resize(500, 500);
	view.show();*/
	
	Anvedi w;
	w.show();
	return a.exec();
}
