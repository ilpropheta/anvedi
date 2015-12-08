#include "GraphPresenter.h"
#include <QColorDialog>

GraphPresenter::GraphPresenter(QCustomPlot* plot)
	: plot(plot)
{
	plot->yAxis->setVisible(false);
	//plot->xAxis->setAutoTickStep(false);
	//plot->xAxis->setTickStep(10);
	plot->xAxis->setTickLabels(false);
}

void GraphPresenter::OnNewData(const DataMap& data)
{
	for (const auto& signal : data)
	{
		if (signal.second.visible)
			OnGraphVisibilityChanged(signal.second);
	}
}

void GraphPresenter::OnGraphColorChanged(const Signal& signal)
{
	auto it = displayedGraphs.find(signal.name);
	if (it != end(displayedGraphs))
	{
		it->second->setPen(QPen(signal.color));
		plot->replot(QCustomPlot::rpQueued);
	}
}

void GraphPresenter::OnGraph(const Signal& signal, std::function<void(QCPGraph*)> action)
{
	auto it = displayedGraphs.equal_range(signal.name);
	if (it.first != it.second)
	{
		action(it.first->second);
	}
	else
	{
		auto myY = plot->axisRect(0)->addAxis(QCPAxis::atLeft);
		myY->setVisible(false);
		auto graph = plot->addGraph(plot->xAxis, myY);
		displayedGraphs.emplace_hint(it.first, signal.name, graph);
		graph->setData(signal.x, signal.y);
		graph->setPen(QPen(signal.color));
		graph->rescaleAxes();
		graph->setVisible(signal.visible);
	}
	plot->replot();
}

void GraphPresenter::OnGraphVisibilityChanged(const Signal& signal)
{
	OnGraph(signal, [&](QCPGraph* graph){
		graph->setPen(QPen(signal.color));
		graph->setVisible(signal.visible);
	});
}

void GraphPresenter::OnGraphDataChanged(const Signal& signal)
{
	OnGraph(signal, [&](QCPGraph* graph){
		graph->setData(signal.x, signal.y);
		graph->rescaleAxes();
	});
}

void GraphPresenter::OnClearData()
{
	displayedGraphs.clear();
	plot->clearGraphs();
	plot->replot();
}

void GraphPresenter::OnBackgroundChanged(const QColor& color)
{
	plot->setBackground(color);
	plot->replot(QCustomPlot::rpQueued);
}
