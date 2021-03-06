#include "GraphPresenter.h"
#include <QColorDialog>
#include "Utils.h"
#include "SignalData.h"
#include "PlotInfo.h"

static const int stepRes = 1;

GraphPresenter::GraphPresenter(QCustomPlot* plot, QScrollBar* rangeScroll, const SignalData& data, PlotInfo& plotInfo)
	: plot(plot), rangeScroll(rangeScroll), data(data), plotInfo(plotInfo)
{
	plot->yAxis->setVisible(false);
	plot->xAxis->setAutoSubTicks(false);
	plot->xAxis->setSubTickCount(0);
	plot->xAxis->setTickLabels(false);
	
	rangeScroll->setVisible(false);
	rangeScroll->setSingleStep(1);
	rangeScroll->setPageStep(100);

	plot->setInteraction(QCP::Interaction::iRangeZoom);

	// model
	QObject::connect(&data, SIGNAL(DataAdded(const DataMap&)), this, SLOT(OnNewData(const DataMap&)));
	QObject::connect(&data, SIGNAL(SignalRenamed(const QString&, const Signal&)), this, SLOT(OnSignalRenamed(const QString&, const Signal&)));
	QObject::connect(&data, SIGNAL(SignalRemoved(const QString&)), this, SLOT(OnSignalRemoved(const QString&)));
	QObject::connect(&data, SIGNAL(DataCleared()), this, SLOT(OnClearData()));
	QObject::connect(&data, SIGNAL(DataCleared()), this, SLOT(OnClearData()));
	QObject::connect(&data, SIGNAL(SignalGraphicChanged(const Signal&)), this, SLOT(OnGraphStyleInfoChanged(const Signal&)));
	QObject::connect(&data, SIGNAL(SignalColorChanged(const Signal&)), this, SLOT(OnGraphVisibilityChanged(const Signal&)));
	QObject::connect(&data, SIGNAL(SignalVisibilityChanged(const Signal&)), this, SLOT(OnGraphVisibilityChanged(const Signal&)));
	QObject::connect(&data, SIGNAL(SignalValuesChanged(const Signal&)), this, SLOT(OnGraphDataChanged(const Signal&)));
	QObject::connect(&data, SIGNAL(SignalRangeChanged(const Signal&)), this, SLOT(OnGraphRangeChanged(const Signal&)));
	QObject::connect(&data, SIGNAL(SignalTicksChanged(const Signal&)), this, SLOT(OnGraphTicksChanged(const Signal&)));
	QObject::connect(&data, SIGNAL(SignalAdded(const QVector<qreal>&, const std::map<QString, QVector<qreal>>&)), this, SLOT(OnGraphsDataAdded(const QVector<qreal>&, const std::map<QString, QVector<qreal>>&)));
	QObject::connect(&data, SIGNAL(DomainChanged(const Signal&)), this, SLOT(OnDomainChanged(const Signal&)));
	// plot
	QObject::connect(plot->xAxis, SIGNAL(rangeChanged(const QCPRange&)), this, SLOT(OnXRangeChanged(const QCPRange&)));
	// plot-info
	QObject::connect(&plotInfo, SIGNAL(BackgroundColorChanged(const QColor&)), this, SLOT(OnBackgroundChanged(const QColor&)));
	// xrange scrollbar
	QObject::connect(rangeScroll, SIGNAL(valueChanged(int)), this, SLOT(rangeScrollbarValueChanged(int)));
}

void GraphPresenter::OnNewData(const DataMap& d)
{
	for (const auto& signal : d)
	{
		// for now, consider also non-visible graphs
		MakeGraphOrUseExistent(signal.second, [&](QCPGraph* graph){			
			SetGraphDataFrom(*graph, signal.second);
			SetGraphicInfoFrom(*graph, signal.second);
		});
	}
	plot->replot();
}

void GraphPresenter::MakeGraphOrUseExistent_WithFinalReplot(const Signal& signal, std::function<void(QCPGraph*)> action)
{
	MakeGraphOrUseExistent(signal, action);
	plot->replot();
}

void GraphPresenter::MakeGraphOrUseExistent(const Signal& signal, std::function<void(QCPGraph*)> action)
{
	auto it = displayedGraphs.equal_range(signal.name);
	if (it.first != it.second)
	{
		action(it.first->second);
	}
	else
	{
		auto myY = plot->axisRect(0)->addAxis(QCPAxis::atLeft);
		myY->setLabel(signal.name);
		myY->setVisible(false);
		myY->setSubTickCount(0);
		myY->setAutoSubTicks(false);
		auto graph = plot->addGraph(plot->xAxis, myY);
		graph->setName(signal.name);
		displayedGraphs.emplace_hint(it.first, signal.name, graph);
		SetGraphicInfoFrom(*graph, signal);
		SetGraphDataFrom(*graph, signal);
	}
}

void GraphPresenter::OnGraphStyleInfoChanged(const Signal& signal)
{
	MakeGraphOrUseExistent_WithFinalReplot(signal, [&](QCPGraph* graph){
		SetGraphicInfoFrom(*graph, signal);
		SetRangeInfo(*graph, signal);
	});
}

void GraphPresenter::OnGraphVisibilityChanged(const Signal& signal)
{
	MakeGraphOrUseExistent_WithFinalReplot(signal, [&](QCPGraph* graph){
		SetGraphicInfoFrom(*graph, signal); 
	});
}

void GraphPresenter::OnGraphDataChanged(const Signal& signal)
{
	MakeGraphOrUseExistent_WithFinalReplot(signal, [&](QCPGraph* graph){
		SetGraphDataFrom(*graph, signal);
	});
}

void GraphPresenter::OnGraphRangeChanged(const Signal& signal)
{
	MakeGraphOrUseExistent_WithFinalReplot(signal, [&](QCPGraph* graph){
		SetRangeInfo(*graph, signal);
	});
}

void GraphPresenter::OnGraphTicksChanged(const Signal& signal)
{
	MakeGraphOrUseExistent_WithFinalReplot(signal, [&](QCPGraph* graph){
		SetAxisInfo(*graph, signal);
	});
}

void GraphPresenter::OnClearData()
{
	// each axes is hold by the rect. When the graphs are removed
	// we don't need their Y axis anymore...
	for (auto i = 0; i < plot->graphCount(); ++i)
		plot->axisRect(0)->removeAxis(plot->graph(i)->valueAxis());
	displayedGraphs.clear();	
	plot->clearGraphs();
	rangeScroll->setVisible(false);
	plot->replot();
}

void GraphPresenter::OnBackgroundChanged(const QColor& color)
{
	plot->setBackground(color);
	auto tickPen = plot->xAxis->basePen();
	if (InvertPenColorIfNearTo(tickPen, color))
	{
		plot->xAxis->setBasePen(tickPen);
	}
	for (const auto& displayedGraph : displayedGraphs)
	{
		SetAxisColor(displayedGraph.second->valueAxis());
	}
	plot->replot();
}

void GraphPresenter::SetGraphDataFrom(QCPGraph& graph, const Signal& signal)
{
	if (data.getDomain())
	{
		graph.setData(data.getDomain()->y, signal.y);
		SetRangeInfo(graph, signal);
	}
}

void GraphPresenter::SetGraphicInfoFrom(QCPGraph& graph, const Signal& signal)
{
	graph.setPen(QPen(signal.graphic.color));
	graph.setVisible(signal.graphic.visible);
	SetAxisInfo(graph, signal);
}

void GraphPresenter::SetAxisInfo(QCPGraph& graph, const Signal& signal)
{
	auto yAxis = graph.valueAxis();
	if (!signal.graphic.ticks.empty())
	{
		yAxis->setAutoTicks(false);
		yAxis->grid()->setVisible(true);
		yAxis->setTickVector(signal.graphic.ticks);
		if (!signal.graphic.tickLabels.empty())
		{
			yAxis->setAutoTickLabels(false);
			yAxis->setTickVectorLabels(signal.graphic.tickLabels);
		}
		else
		{
			yAxis->setAutoTickLabels(true);
		}
		SetAxisColor(yAxis);
		yAxis->setVisible(signal.graphic.visible);
	}
	else
	{
		yAxis->setVisible(false);
	}
}

void GraphPresenter::SetRangeInfo(QCPGraph& graph, const Signal& signal)
{
	graph.valueAxis()->setRange(signal.graphic.rangeLower, signal.graphic.rangeUpper);
}

void GraphPresenter::OnDomainChanged(const Signal& domain)
{
	data.onSignals([this](const Signal& signal){
		MakeGraphOrUseExistent(signal, [&](QCPGraph* graph){
			SetGraphDataFrom(*graph, signal);
		});
	});
	plot->xAxis->rescale();
	rangeScroll->setRange(0, 0);
}

/*	When the plot is zoomed in, value can be within a non-visible area.
	In this case the range is changed accordingly.

	* -> non visible part of the plot
	_ -> visible part of the plot
	^ -> value position
	
	E.g.:

	***______******
		^
	Nothing happens

	***______******
		     ^
	Should become:

	****______*****
			 ^
	   -     +

	The upper part of the xAxis range is set to ^ and
	the lower part is increased by '+' quantity. 
	+ is simply obtained by subtracting ^ and the upper part of the current xRange.
	
	The opposite case is similar.
*/
void GraphPresenter::OnCursorValueChanged(qreal value, size_t)
{
	const auto xAxisRange = plot->xAxis->range();
	if (value < xAxisRange.lower) // value is more left than rangeX
	{
		plot->xAxis->setRange(value, xAxisRange.size() + value);
	}
	else if (value > xAxisRange.upper) // value is more right than rangeX
	{
		plot->xAxis->setRange(value - xAxisRange.size(), value);
	}
}

void GraphPresenter::OnXRangeChanged(const QCPRange& newXRange)
{
	if (rangeScroll && data.getDomain())
	{
		const auto& domainVals = data.getDomain()->y;
		const auto showScroll = newXRange.lower > domainVals.front() || newXRange.upper < domainVals.back();
		if (showScroll)
		{
			const auto leftRemaining = data.domainLowerBound(newXRange.lower).second;
			const auto rightRemaining = domainVals.size() - data.domainLowerBound(newXRange.upper).second;
			rangeScroll->setMaximum(leftRemaining+rightRemaining);
			if (newXRange.upper <= domainVals.back())
			{
				rangeScroll->setValue(leftRemaining);
			}
			rangeScroll->setPageStep(domainVals.size() - rangeScroll->maximum());
		}
		else
		{
			rangeScroll->setRange(0, 0);
		}
		rangeScroll->setVisible(showScroll);
	}
	plot->replot();
}

// Note: an RTStarted event would be definitely better...
bool GraphPresenter::IsFirstRTPacket(const std::map<QString, QVector<qreal>>& dataSlice)
{
	auto firstDataAsGraph = displayedGraphs.find(dataSlice.begin()->first);
	// domain may already have data...
	if (firstDataAsGraph->first != data.getDomain()->name)
	{
		return firstDataAsGraph->second->data()->empty();
	}
	// if we are sending just domain data we are fucked :)
	return displayedGraphs.at((++dataSlice.begin())->first)->data()->empty();
}

void GraphPresenter::OnGraphsDataAdded(const QVector<qreal>& domainSlice, const std::map<QString, QVector<qreal>>& dataSlice)
{
	if (domainSlice.empty())
		return;

	// logic here should be moved out to kind of "PlotBrowser"
	const auto rtPageSize = plotInfo.getRealTimePageSize();
	// first packet?
	if (rtPageSize && IsFirstRTPacket(dataSlice))
	{
		plot->xAxis->setRange(domainSlice.front(), rtPageSize);
	}

	for (const auto& data : dataSlice)
	{
		auto graph = displayedGraphs.at(data.first);
		graph->addData(domainSlice, data.second);
		graph->rescaleValueAxis();
	}

	auto xRange = plot->xAxis->range();
	// new page?
	if (rtPageSize && (xRange.upper < domainSlice.back()))
	{
		xRange.lower = xRange.upper;
		xRange.upper += rtPageSize;
		plot->xAxis->setRange(xRange);
	}

	plot->replot();
}

void GraphPresenter::SetAxisColor(QCPAxis * yAxis)
{
	const auto bkg = plotInfo.getBackgroundColor();
	auto tickPen = yAxis->tickPen();
	if (InvertPenColorIfNearTo(tickPen, bkg))
	{
		yAxis->setTickPen(tickPen);
	}
	
	auto axisPen = yAxis->basePen();
	if (InvertPenColorIfNearTo(axisPen, bkg))
	{
		yAxis->setBasePen(axisPen);
	}

	if (close(yAxis->labelColor(), bkg))
		yAxis->setLabelColor(invert(bkg));

	if (close(yAxis->tickLabelColor(), bkg))
		yAxis->setTickLabelColor(invert(bkg));

	if (InvertPenColorIfNearTo(tickPen, bkg))
	{
		yAxis->setTickPen(tickPen);
	}
	auto gridPen = yAxis->grid()->pen();
	if (InvertPenColorIfNearTo(gridPen, bkg))
	{
		yAxis->grid()->setPen(gridPen);
	}
}

void GraphPresenter::rangeScrollbarValueChanged(int value)
{
	if (data.getDomain() && data.getDomain()->y.size() > value)
	{
		const auto xRange = plot->xAxis->range();
		auto newLowerValue = data.getDomain()->y[value];
		const auto lowerDelta = (newLowerValue - xRange.lower);
		if (fabs(lowerDelta)<10e-7)
			return;
		auto newUpperValue = xRange.upper + lowerDelta;
		if (newUpperValue > data.getDomain()->y.back()) // saturate right
		{
			newLowerValue = xRange.lower;
			newUpperValue = data.getDomain()->y.back();
		}
		plot->xAxis->setRange(newLowerValue, newUpperValue);
	}
}

void GraphPresenter::OnSignalRemoved(const QString& who)
{
	displayedGraphs.erase(displayedGraphs.find(who));
	for (auto i = 0; i < plot->graphCount(); ++i)
	{
		if (plot->graph(i)->name() == who)
		{
			plot->axisRect(0)->removeAxis(plot->graph(i)->valueAxis());
			plot->removeGraph(i);
			break;
		}
	}
	plot->replot();
}

void GraphPresenter::OnSignalRenamed(const QString& oldName, const Signal& signal)
{
	auto graph = displayedGraphs.find(oldName);
	graph->second->setName(signal.name);
	graph->second->valueAxis()->setLabel(signal.name);
	displayedGraphs[signal.name] = graph->second;
	displayedGraphs.erase(graph);
	plot->replot();
}
