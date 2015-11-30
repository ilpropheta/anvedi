#include "GraphPresenter.h"

GraphPresenter::GraphPresenter(QCustomPlot* plot)
	: plot(plot)
{
	plot->yAxis->setVisible(false);
}

void GraphPresenter::OnGraphColorChanged(const QString& name, const QColor& col)
{
	auto it = displayedGraphs.find(name);
	if (it != end(displayedGraphs))
	{
		it->second->setPen(QPen(col));
		plot->replot(QCustomPlot::rpQueued);
	}
}

void GraphPresenter::OnSignalAdded(const QString& name)
{
	auto it = displayedGraphs.find(name);
	if (it != end(displayedGraphs))
	{
		it->second->setVisible(true);
		it->second->rescaleValueAxis();
		it->second->rescaleKeyAxis();
	}
	else
	{
		const auto xy = data.at(name);
		auto myY = plot->axisRect(0)->addAxis(QCPAxis::atLeft);
		myY->setVisible(false);
		auto graph = plot->addGraph(plot->xAxis, myY);
		graph->setPen(QPen(Qt::blue));
		graph->setData(xy.first, xy.second);
		graph->rescaleAxes();
		displayedGraphs[name] = graph;
	}

	plot->replot();
}

void GraphPresenter::OnSignalRemoved(const QString& name)
{
	auto it = displayedGraphs.find(name);
	if (it != end(displayedGraphs))
	{
		displayedGraphs[name]->setVisible(false);
		plot->replot();
	}
}

void GraphPresenter::OnNewData(const DataMap& newData)
{
	data.insert(begin(newData), end(newData));
}

void GraphPresenter::OnClearData()
{
	displayedGraphs.clear();
	plot->clearGraphs();
	data.clear();
	plot->replot();
}