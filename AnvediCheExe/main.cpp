#include "..\Anvedi\anvedi.h"
#include <QtWidgets/QApplication>
#include <QQuickView>
#include "../qml-lib/RegisterAll.h"
#include "../qml-lib/qmlPen.h"
#include <numeric>
#include <random>

using namespace std;

static QVector<qreal> IncreasingVector(size_t count)
{
	QVector<qreal> x; x.resize(count);
	iota(x.begin(), x.end(), 0.0);
	return x;;
}

static QVector<qreal> RandomVector(size_t count)
{
	QVector<qreal> x; x.resize(count);
	default_random_engine generator;
	uniform_real_distribution<qreal> distribution(0.0, 100.0);
	std::generate(x.begin(), x.end(), [&]{
		return distribution(generator);
	});
	return x;
}

int main(int argc, char *argv [])
{
	QApplication a(argc, argv);
	
	using namespace std;

	vector<double> domain{ 1.0, 2.0, 3.0 };
	vector<double> sig1{ 10, 20, 30 };
	vector<double> sig2{ 100, 200, 300 };

	vector<double> dS{ 0.5, 1.5, 2.0, 5.0 };
	vector<double> dS1{ 5, 15, 22, 50 };
	vector<double> dS2{ 50, 150, 202, 500 };

	auto distCalc = [&](vector<double>::iterator to){
		return std::distance(dS.begin(), to);
	};

	for (auto it = dS.begin(); it != dS.end(); ++it)
	{
		if (*it > domain.back()) // insert all
		{
			domain.insert(domain.end(), it, dS.end());
			sig1.insert(sig1.end(), dS1.begin() + distCalc(it), dS1.end());
			sig2.insert(sig2.end(), dS2.begin() + distCalc(it), dS2.end());
			break;
		}
		if (*it < domain.front())
		{
			domain.insert(domain.begin(), *it);
			sig1.insert(sig1.begin(), *(dS1.begin() + distCalc(it)));
			sig2.insert(sig2.begin(), *(dS2.begin() + distCalc(it)));
		}
		else
		{
			auto eRange = equal_range(begin(domain), end(domain), *it);
			if (eRange.first == eRange.second) // not in vector
			{
				domain.insert(eRange.first, *it);
				sig1.insert(sig1.begin() + distCalc(it), *(dS1.begin() + distCalc(it)));
				sig2.insert(sig2.begin() + distCalc(it), *(dS2.begin() + distCalc(it)));
			}
			else // overwrite
			{
				*eRange.first = *it;
				*(sig1.begin() + distCalc(it)) = *(dS1.begin() + distCalc(it));
				*(sig2.begin() + distCalc(it)) = *(dS2.begin() + distCalc(it));
			}
		}
	}

	/*QmlModulesInstaller::Install();
	QQuickView view(QUrl("../Anvedi/qml/main.qml"));
	view.setResizeMode(QQuickView::SizeRootObjectToView);
	view.resize(500, 500);
	view.show();*/

	Anvedi w;
	w.show();

	//QCustomPlot plot;
	//plot.setGeometry(40, 50, 500, 500);
	//plot.setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	//plot.show();
	//QVector<double> x(500000), y(500000); 
	//for (int i = 0; i < 500000; ++i)
	//{
	//	x[i] = i / 250000.0 - 1; // x goes from -1 to 1
	//	y[i] = x[i] * x[i]; // let's plot a parabola
	//}

	//// create graph and assign data to it:
	//auto graph = plot.addGraph();
	//QElapsedTimer timer; timer.start();
	////graph->setData(x, y);
	//graph->addData({0,1,2,3,4,5}, {0,1,4,9,26,25});
	//QMessageBox msg;
	//msg.setText(QString("setdata: %1").arg(timer.elapsed()));
	//msg.exec();
	//// give the axes some labels:
	//plot.xAxis->setLabel("x");
	//plot.yAxis->setLabel("y");
	//// autoscale:
	//timer.start();
	//graph->rescaleAxes();
	//msg.setText(QString("rescale: %1").arg(timer.elapsed()));
	//msg.exec();
	//timer.start();
	//plot.replot();
	//msg.setText(QString("replot: %1").arg(timer.elapsed()));
	//msg.exec();

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
