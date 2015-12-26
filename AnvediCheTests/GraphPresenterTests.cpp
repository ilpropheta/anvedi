#include "GraphPresenterTests.h"
#include "..\qcustomplot.h"
#include "..\SignalData.h"
#include "..\GraphPresenter.h"
#include <QTest>
#include "..\PlotInfo.h"

template<typename TestCode>
void DoGraphPresenterTest(TestCode t)
{
	QCustomPlot plot;
	SignalData data;
	PlotInfo info;
	GraphPresenter presenter(&plot, data, info);
	t(presenter, plot, data);
}

void GraphPresenterTests::On_DataAdded_Should_CreateGraphs()
{
	DoGraphPresenterTest([](GraphPresenter& p, QCustomPlot& plot, SignalData& data){
		data.add({
			{ "line", { "line", "red", true, { 1, 2, 3 } } },
			{ "cubic", { "cubic", "blue", false, { 1, 8, 27 } } },
		});

		QCOMPARE(plot.graphCount(), 2);
		QCOMPARE(plot.graph(1)->pen().color(), QColor("red"));
		QCOMPARE(plot.graph(1)->name(), QString("line"));
		QCOMPARE(plot.graph(1)->visible(), true);
		QCOMPARE(plot.graph(1)->data()->size(), 0); // because domain is not set yet
	});
}

void GraphPresenterTests::On_DomainChanged_Should_SetDataToGraphs_And_ScaleXAxis()
{
	DoGraphPresenterTest([](GraphPresenter& p, QCustomPlot& plot, SignalData& data){
		data.add({
			{ "cubic", { "cubic", "blue", true, { 1, 8, 27 } } },
			{ "line", { "line", "red", true, { 1, 2, 3 } } }	
		});
		data.setAsDomain("line");

		// line
		QCOMPARE(plot.graph(1)->data()->size(), 3);
		const auto lineData = plot.graph(1)->data()->values();
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

		// x range [1.0 - 3.0]
		QCOMPARE(plot.xAxis->range().lower, 1.0);
		QCOMPARE(plot.xAxis->range().upper, 3.0);
	});
}

void GraphPresenterTests::On_DataCleared_Should_RemoveGraphs()
{
	DoGraphPresenterTest([](GraphPresenter& p, QCustomPlot& plot, SignalData& data){
		data.add({
			{ "cubic", { "cubic", "blue", true, { 1, 8, 27 } } },
			{ "line", { "line", "red", true, { 1, 2, 3 } } }
		});
		data.setAsDomain("line");

		QCOMPARE(plot.graphCount(), 2);
		
		data.clear();
		QCOMPARE(plot.graphCount(), 0);
	});
}

void GraphPresenterTests::On_SignalVisibilityChanged_Should_UpdateGraphVisibility()
{
	DoGraphPresenterTest([](GraphPresenter& p, QCustomPlot& plot, SignalData& data){
		data.add({
			{ "cubic", { "cubic", "blue", false, { 1, 8, 27 } } },
			{ "line", { "line", "red", true, { 1, 2, 3 } } }
		});

		QCOMPARE(plot.graph(0)->visible(), false);
		
		data.setVisible("cubic", true);
		QCOMPARE(plot.graph(0)->name(), QString("cubic"));
		QCOMPARE(plot.graph(0)->visible(), true);
	});
}

void GraphPresenterTests::On_SignalColorChanged_Should_UpdateGraphColor()
{
	DoGraphPresenterTest([](GraphPresenter& p, QCustomPlot& plot, SignalData& data){
		data.add({
			{ "line", { "line", "red", true, { 1, 2, 3 } } }
		});

		QCOMPARE(plot.graph(0)->pen().color(), QColor("red"));

		data.setColor("line", "blue");

		QCOMPARE(plot.graph(0)->pen().color(), QColor("blue"));
	});
}

void GraphPresenterTests::On_CursorValueChanged_Should_UpdateXRange()
{
	DoGraphPresenterTest([](GraphPresenter& p, QCustomPlot& plot, SignalData& data){
		data.add({
			{ "line", { "line", "red", true, { 1, 2, 3, 4, 5 } } }
		});
		data.setAsDomain("line");
		plot.xAxis->setRange(2, 4); 

		p.OnCursorValueChanged(1.0, {}); // more left than current range
		QCOMPARE(plot.xAxis->range(), QCPRange(1.0, 3.0));

		p.OnCursorValueChanged(4.0, {}); // more right than current range
		QCOMPARE(plot.xAxis->range(), QCPRange(2.0, 4.0));

		p.OnCursorValueChanged(3.0, {}); // in current range
		QCOMPARE(plot.xAxis->range(), QCPRange(2.0, 4.0));
	});
}
