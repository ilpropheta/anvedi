#include "PlotInfo.h"
#include "qcustomplot.h"

void PlotInfo::setPlot(QCustomPlot* p)
{
	plot = p;
}

const QColor& PlotInfo::getBackgroundColor() const
{
	return backgroundColor;
}

void PlotInfo::setBackgroundColor(const QColor& color)
{
	backgroundColor = color;
	emit BackgroundColorChanged(color);
}

void PlotInfo::setXRange(const std::pair<qreal, qreal>& range)
{
	plot->xAxis->setRange(range.first, range.second);
}

const std::pair<qreal, qreal> PlotInfo::getXRange() const
{
	const auto range = plot->xAxis->range();
	return { range.lower, range.upper };
}

template<typename Action>
void OnGraph(QCustomPlot* plot, const QString& name, Action action)
{
	for (auto i = 0; i < plot->graphCount(); ++i)
	{
		if (plot->graph(i)->name() == name)
		{
			action(plot->graph(i));
			break;
		}
	}
}

template<typename Action>
void OnAllGraphs(QCustomPlot* plot, Action action)
{
	for (auto i = 0; i < plot->graphCount(); ++i)
	{
		action(plot->graph(i));
	}
}

void PlotInfo::setYRange(const QString& name, const std::pair<qreal, qreal>& range)
{
	OnGraph(plot, name, [&range, this](QCPGraph* graph){
		graph->valueAxis()->setRange(range.first, range.second);
		plot->replot();
	});
}

void PlotInfo::autoScaleY(const QString& name)
{
	OnGraph(plot, name, [this](QCPGraph* graph){
		graph->valueAxis()->rescale();
		plot->replot();
	});
}

void PlotInfo::autoScaleX()
{
	plot->xAxis->rescale();
	plot->replot();
}

void PlotInfo::autoScaleAllY()
{
	OnAllGraphs(plot, [](QCPGraph* graph){
		graph->valueAxis()->rescale();
	});
	plot->replot();
}

void PlotInfo::exportToPdf(const QString& fileName, qreal w, qreal h)
{
	plot->savePdf(fileName, false, w, h);
}

int PlotInfo::getRealTimePageSize() const
{
	return realTimePageSize;
}

void PlotInfo::setRealTimePageSize(int ps)
{
	realTimePageSize = ps;
}
