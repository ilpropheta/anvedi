#pragma once
#include <QObject>
#include "TestRunner.h"

class PerformanceTests : public QObject
{
	Q_OBJECT
private slots:
	void Measure_SetDataGraph_500kPoints();
	void Measure_SetDataCurve_500kPoints();

	void Measure_PlotSingleGraph_500kPoints_data();
	void Measure_PlotSingleCurve_500kPoints_data();
	void Measure_PlotSingleGraph_500kPoints();
	void Measure_PlotSingleCurve_500kPoints();

	void Measure_MultipleGraphs_200kPoints();
	void Measure_MultipleGraphs_10kPoints_1_1_PenWidth();
};

DECLARE_TEST(PerformanceTests)