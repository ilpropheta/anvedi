#include "RegisterAll.h"
#include <QtQuick>
#include "qmlPlotPaintedItem.h"
#include "qmlGraph.h"
#include "qcustomplot.h"

void QmlModulesInstaller::Install()
{
	qmlRegisterType<qmlPlotPaintedItem>("CustomPlot", 1, 0, "CustomPlotItem");
	qmlRegisterType<qmlGraph>("CustomPlot", 1, 0, "Graph");
	qmlRegisterType<qmlScatterStyle>("CustomPlot", 1, 0, "ScatterStyle");
	qmlRegisterUncreatableType<QCPScatterStyle>("CustomPlot", 1, 0, "Scatter", "Objects of this type cannot be created");
	qmlRegisterUncreatableType<QCPGraph>("CustomPlot", 1, 0, "Plot", "Objects of this type cannot be created");
	qmlRegisterType<qmlPen>("CustomPlot", 1, 0, "Pen");
	qmlRegisterType<qmlLegend>("CustomPlot", 1, 0, "Legend");
	qmlRegisterType<qmlAxis>("CustomPlot", 1, 0, "Axis");
}

