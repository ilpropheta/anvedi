#include "anvedi.h"

using namespace std;

Anvedi::Anvedi(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	cursor = make_unique<PlotCursor>(ui.plot, 0.01);
	ui.console->SetEngine(std::make_shared<QShellEngine_Qt>());

	QVector<double> x(101), y(101); // initialize with entries 0..100
	for (int i = 0; i < 101; ++i)
	{
		x[i] = i / 50.0 - 1; // x goes from -1 to 1
		y[i] = x[i] * x[i]; // let's plot a quadratic function
	}
	// create graph and assign data to it:
	ui.plot->addGraph();
	ui.plot->graph(0)->setData(x, y);
	ui.plot->xAxis->setLabel("x");
	ui.plot->yAxis->setLabel("y");
	ui.plot->xAxis->setRange(-1, 1);
	ui.plot->yAxis->setRange(0, 1);
	ui.plot->replot();
}

void Anvedi::OnExit()
{
	QApplication::exit();
}
