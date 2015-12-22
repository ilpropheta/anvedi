#include "PerformanceTests.h"
#include <QTest>
#include "..\qcustomplot.h"
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
	uniform_real_distribution<qreal> distribution(0.0, 1.0);
	std::generate(x.begin(), x.end(), [&]{
		return distribution(generator);
	});
	return x;
}

void PerformanceTests::Measure_SetDataGraph_500kPoints()
{
	QCustomPlot plot;
	auto graph = plot.addGraph();

	QBENCHMARK {
		graph->setData(IncreasingVector(500000), IncreasingVector(500000));
	}
}

void PerformanceTests::Measure_SetDataCurve_500kPoints()
{
	QCustomPlot plot;
	auto curve = new QCPCurve(plot.xAxis, plot.yAxis);
	
	QBENCHMARK{
		curve->setData(IncreasingVector(500000), IncreasingVector(500000));
	}
}

void PerformanceTests::Measure_PlotSingleGraph_500kPoints_data()
{
	QTest::addColumn<QVector<qreal>>("x");
	QTest::addColumn<QVector<qreal>>("y");
	
	QTest::newRow("Increasing X and Increasing Y")
		<< IncreasingVector(500000)
		<< IncreasingVector(500000);
	
	QTest::newRow("Increasing X and Random Y") 
		<< IncreasingVector(500000)
		<< RandomVector(500000);
}

void PerformanceTests::Measure_PlotSingleCurve_500kPoints_data()
{
	QTest::addColumn<QVector<qreal>>("x");
	QTest::addColumn<QVector<qreal>>("y");

	QTest::newRow("Increasing X and Increasing Y")
		<< IncreasingVector(500000)
		<< IncreasingVector(500000);

	QTest::newRow("Increasing X and Random Y")
		<< IncreasingVector(500000)
		<< RandomVector(500000);
}

void PerformanceTests::Measure_PlotSingleGraph_500kPoints()
{
	QCustomPlot plot;
	QFETCH(QVector<qreal>, x);
	QFETCH(QVector<qreal>, y);

	auto graph = plot.addGraph();
	graph->setData(x, y);
	graph->rescaleAxes();

	QBENCHMARK{
		plot.replot();
	};
}

void PerformanceTests::Measure_PlotSingleCurve_500kPoints()
{
	QCustomPlot plot;
	QFETCH(QVector<qreal>, x);
	QFETCH(QVector<qreal>, y);

	auto curve = new QCPCurve(plot.xAxis, plot.yAxis);
	curve->setData(x, y);
	auto graph = plot.addPlottable(curve);
	curve->rescaleAxes();

	QBENCHMARK{
		plot.replot();
	};
}

