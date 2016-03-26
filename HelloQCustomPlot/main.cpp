
#include <QtWidgets/QApplication>
#include "../Anvedi/qcustomplot.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QCustomPlot plot; plot.setGeometry(40, 40, 800, 600);
	
	// generate some data:
	QVector<double> x(101), y(101); for (int i = 0; i < 101; ++i)
	{
		x[i] = i / 50.0 - 1; // x goes from -1 to 1
		y[i] = x[i] * x[i]; // let's plot a parabola
	}
	// create graph and assign data to it:
	auto graph = plot.addGraph();
	graph->setData(x, y);
	// give the axes some labels:
	plot.xAxis->setLabel("x");
	plot.yAxis->setLabel("y");
	plot.legend->setVisible(true);
	// autoscale:
	graph->rescaleAxes();

	// interactions
	plot.setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

	plot.replot();

	plot.show();
	return a.exec();
}
