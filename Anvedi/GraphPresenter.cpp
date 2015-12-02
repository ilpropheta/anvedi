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

void GraphPresenter::OnGraphVisibilityChanged(const Signal& signal)
{
	auto it = displayedGraphs.find(signal.name);
	if (it != end(displayedGraphs))
	{
		it->second->setVisible(signal.visible);
		it->second->rescaleValueAxis();
		it->second->rescaleKeyAxis();
	}
	else
	{
		auto myY = plot->axisRect(0)->addAxis(QCPAxis::atLeft);
		myY->setVisible(false);
		auto graph = plot->addGraph(plot->xAxis, myY);
		graph->setPen(QPen(signal.color));
		graph->setData(signal.x, signal.y);
		graph->rescaleAxes();
		displayedGraphs[signal.name] = graph;
		graph->setVisible(signal.visible);
	}

	plot->replot();
}

void GraphPresenter::OnClearData()
{
	displayedGraphs.clear();
	plot->clearGraphs();
	plot->replot();
}

void GraphPresenter::OnGraphDataChanged(const Signal& signal)
{
	if (signal.visible)
	{
		auto it = displayedGraphs.find(signal.name);
		if (it != end(displayedGraphs))
		{
			it->second->setData(signal.x, signal.y);
			it->second->rescaleAxes();
		}
		else
		{
			auto myY = plot->axisRect(0)->addAxis(QCPAxis::atLeft);
			myY->setVisible(false);
			auto graph = plot->addGraph(plot->xAxis, myY);
			graph->setPen(QPen(signal.color));
			graph->setData(signal.x, signal.y);
			graph->rescaleAxes();
			displayedGraphs[signal.name] = graph;
		}
		plot->replot();
	}
}

void GraphPresenter::OnChangeBackground()
{
	const auto color = QColorDialog::getColor(backgroundColor, plot, "Change the background color");
	if (color.isValid())
	{
		plot->setBackground(color);
		backgroundColor = color;
		plot->replot(QCustomPlot::rpQueued);
		emit BackgroundChanged(color);
	}
}
