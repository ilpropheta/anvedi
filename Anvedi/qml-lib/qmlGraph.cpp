#include "qmlGraph.h"

const QString& qmlGraph::getName() const
{
	return name;
}

void qmlGraph::setName(const QString& arg)
{
	name = arg;
}

qmlAxis* qmlGraph::getXAxis() const
{
	return xAxis;
}

void qmlGraph::setXAxis(qmlAxis* axis)
{
	xAxis = axis;
}

qmlAxis* qmlGraph::getYAxis() const
{
	return yAxis;
}

void qmlGraph::setYAxis(qmlAxis* axis)
{
	yAxis = axis;
}

qmlScatterStyle* qmlGraph::getScatter()
{
	return scatter;
}

void qmlGraph::setScatter(qmlScatterStyle* s)
{
	scatter = s;
}

qmlPen* qmlGraph::getPen()
{
	return pen;
}

void qmlGraph::setPen(qmlPen* p)
{
	pen = p;
}

QCPGraph::LineStyle qmlGraph::getLineStyle() const
{
	return lineStyle;
}

void qmlGraph::setLineStyle(QCPGraph::LineStyle l)
{
	lineStyle = l;
}

