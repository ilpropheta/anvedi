#include "GraphPresenterTests.h"
#include "..\qcustomplot.h"
#include "..\SignalData.h"
#include "..\GraphPresenter.h"
#include <QTest>

template<typename TestCode>
void DoGraphPresenterTest(TestCode t)
{
	QCustomPlot plot;
	SignalData data;
	GraphPresenter presenter(&plot, data);
	t(presenter, plot, data);
}

void GraphPresenterTests::On_DataAdded_Should_CreateVisibleGraphs()
{
	DoGraphPresenterTest([](GraphPresenter& p, QCustomPlot& plot, SignalData& data){
		data.add({
			{ "line", { "line", "red", true, { 1, 2, 3 } } },
			{ "cubic", { "cubic", "blue", false, { 1, 8, 27 } } },
		});

		QCOMPARE(plot.graphCount(), 1);
		QCOMPARE(plot.graph(0)->pen().color(), QColor("red"));
		QCOMPARE(plot.graph(0)->visible(), true);
		QCOMPARE(plot.graph(0)->data()->size(), 0); // because domain is not set yet
	});
}

void GraphPresenterTests::On_DomainChanged_Should_SetDataToGraphs()
{
	DoGraphPresenterTest([](GraphPresenter& p, QCustomPlot& plot, SignalData& data){
		data.add({
			{ "cubic", { "cubic", "blue", true, { 1, 8, 27 } } },
			{ "line", { "line", "red", true, { 1, 2, 3 } } }	
		});
		data.setAsDomain("line");

		// line
		QCOMPARE(plot.graph(1)->data()->size(), 3);
		const auto lineData = plot.graph(0)->data()->values();
		QCOMPARE(lineData[0].key, 1.0);
		QCOMPARE(lineData[0].value, 1.0);
		QCOMPARE(lineData[1].key, 2.0);
		QCOMPARE(lineData[1].value, 2.0);
		QCOMPARE(lineData[2].key, 3.0);
		QCOMPARE(lineData[2].value, 3.0);

		// cubic
		QCOMPARE(plot.graph(0)->data()->size(), 3);
		const auto cubicData = plot.graph(0)->data()->values();
		QCOMPARE(cubicData[0].key, 1.0);
		QCOMPARE(cubicData[0].value, 1.0);
		QCOMPARE(cubicData[1].key, 2.0);
		QCOMPARE(cubicData[1].value, 8.0);
		QCOMPARE(cubicData[2].key, 3.0);
		QCOMPARE(cubicData[2].value, 27.0);
	});
}