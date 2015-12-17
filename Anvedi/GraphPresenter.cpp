#include "GraphPresenter.h"
#include <QColorDialog>
#include "Utils.h"
#include "SignalData.h"

GraphPresenter::GraphPresenter(QCustomPlot* plot, SignalData& data)
	: plot(plot), data(data)
{
	plot->yAxis->setVisible(false);
	//plot->xAxis->setAutoTickStep(false);
	//plot->xAxis->setTickStep(10);
	plot->xAxis->setTickLabels(false);

	// model
	QObject::connect(&data, SIGNAL(DataAdded(const DataMap&)), this, SLOT(OnNewData(const DataMap&)));
	QObject::connect(&data, SIGNAL(DataCleared()), this, SLOT(OnClearData()));
	QObject::connect(&data, SIGNAL(SignalColorChanged(const Signal&)), this, SLOT(OnGraphColorChanged(const Signal&)));
	QObject::connect(&data, SIGNAL(SignalVisibilityChanged(const Signal&)), this, SLOT(OnGraphVisibilityChanged(const Signal&)));
	QObject::connect(&data, SIGNAL(SignalValuesChanged(const Signal&)), this, SLOT(OnGraphDataChanged(const Signal&)));
	QObject::connect(&data, SIGNAL(DomainChanged(const Signal&)), this, SLOT(OnDomainChanged(const Signal&)));
}

void GraphPresenter::OnNewData(const DataMap& d)
{
	for (const auto& signal : d)
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
		SetGraphicInfoFrom(*graph, signal);
		SetGraphDataFrom(*graph, signal);
	}
	plot->replot();
}

void GraphPresenter::OnGraphVisibilityChanged(const Signal& signal)
{
	OnGraph(signal, [&](QCPGraph* graph){
		SetGraphicInfoFrom(*graph, signal);
	});
}

void GraphPresenter::OnGraphDataChanged(const Signal& signal)
{
	OnGraph(signal, [&](QCPGraph* graph){
		SetGraphDataFrom(*graph, signal);
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
	auto tickPen = plot->xAxis->basePen();
	if (close(color, tickPen.color()))
	{
		tickPen.setColor(invert(color));
		plot->xAxis->setBasePen(tickPen);
	}
	plot->replot(QCustomPlot::rpQueued);
}

void GraphPresenter::SetGraphDataFrom(QCPGraph& graph, const Signal& signal)
{
	if (data.getDomain())
	{
		graph.setData(data.getDomain()->y, signal.y);
		graph.rescaleAxes();
	}
}

void GraphPresenter::SetGraphicInfoFrom(QCPGraph& graph, const Signal& signal)
{
	graph.setPen(QPen(signal.color));
	graph.setVisible(signal.visible);
}

void GraphPresenter::OnDomainChanged(const Signal& domain)
{
	data.onSignals([this](const Signal& signal){
		this->OnGraphDataChanged(signal);
	});
}
