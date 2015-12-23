#include "qmlPlotPaintedItem.h"
#include "qcustomplot.h"
#include "qmlLabel.h"
#include "qmlTick.h"
#include <QDebug>

qmlPlotPaintedItem::qmlPlotPaintedItem(QQuickItem* parent) : QQuickPaintedItem(parent)
{
	setFlag(QQuickItem::ItemHasContents, true);
	setAcceptedMouseButtons(Qt::AllButtons);

	connect(this, &QQuickPaintedItem::widthChanged, this, &qmlPlotPaintedItem::onUpdateCustomPlotSize);
	connect(this, &QQuickPaintedItem::heightChanged, this, &qmlPlotPaintedItem::onUpdateCustomPlotSize);

	m_CustomPlot.setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	connect(&m_CustomPlot, SIGNAL(plottableClick(QCPAbstractPlottable*, QMouseEvent*)), this, SLOT(onGraphClicked(QCPAbstractPlottable*)));
	connect(&m_CustomPlot, &QCustomPlot::afterReplot, this, &qmlPlotPaintedItem::onCustomReplot);

	m_listInfo.plot = &m_CustomPlot;
}

void qmlPlotPaintedItem::addData(int index, QVariantList x, QVariantList y)
{
	auto g = m_CustomPlot.graph(index);
	QVector<qreal> xx, yy;
	for (auto i = 0; i < x.size(); ++i)
	{
		xx.push_back(x[i].toReal());
		yy.push_back(y[i].toReal());
	}
	g->addData(xx, yy);
	g->rescaleAxes();
	
	m_CustomPlot.replot();
}

void qmlPlotPaintedItem::paint(QPainter* painter)
{
	QPixmap    picture(boundingRect().size().toSize());
	QCPPainter qcpPainter(&picture);
	m_CustomPlot.toPainter(&qcpPainter);
	painter->drawPixmap(QPoint(), picture);
}

void qmlPlotPaintedItem::setBackground(QColor c)
{
	m_backgroundColor = c;
	m_CustomPlot.setBackground(c);
}

QColor qmlPlotPaintedItem::getBackground() const
{
	return m_backgroundColor;
}

QQmlListProperty<qmlGraph> qmlPlotPaintedItem::getGraphs()
{
	return QQmlListProperty<qmlGraph>(
		this, 
		&m_listInfo, 
		&qmlPlotPaintedItem::appendGraph,
		&qmlPlotPaintedItem::graphSize,
		&qmlPlotPaintedItem::graphAt,
		&qmlPlotPaintedItem::clearGraphs
	);
}

qmlLegend* qmlPlotPaintedItem::getLegend() const
{
	return nullptr;
}

void qmlPlotPaintedItem::setLegend(qmlLegend* g)
{
	m_CustomPlot.legend->setVisible(true);
	m_CustomPlot.legend->setFont(g->getFont());
}

inline ListInfo& Info(QQmlListProperty<qmlGraph> *list)
{
	return *static_cast<ListInfo*>(list->data);
}

void qmlPlotPaintedItem::appendGraph(QQmlListProperty<qmlGraph> *list, qmlGraph *pdt)
{
	auto& info = Info(list);
	auto& m_CustomPlot = *info.plot;
	info.m_graphs.append(pdt);

	auto makeAxis = [&](QCPAxis* ref, QCPAxis::AxisType type, qmlAxis* qmlAx){
		if (qmlAx)
		{
			if (!qmlAx->isDefault())
				ref = m_CustomPlot.axisRect(0)->addAxis(type);

			if (auto label = qmlAx->getLabel())
			{
				if (!label->getText().isEmpty())
					ref->setLabel(label->getText());
				if (label->getColor().isValid())
					ref->setLabelColor(label->getColor());
				if (!label->getFont().isEmpty())
					ref->setLabelFont(label->getFont());
			}
			
			if (auto tick = qmlAx->getTick())
			{
				if (!tick->getFont().isEmpty())
					ref->setTickLabelFont(tick->getFont());
				const auto& tickVec = tick->getTickVector();
				if (!tickVec.empty())
				{
					ref->setAutoTicks(false);
					ref->setTickVector(tickVec);
				}
				const auto& tickLab = tick->getTickLabels();
				if (!tickLab.empty())
				{
					ref->setAutoTickLabels(false);
					ref->setTickVectorLabels(tickLab);
				}
			}

			//xAxis->setTickLabelFont(QFont("serif", 12));
			//xAxis->setAutoTicks(false);
			//xAxis->setAutoTickLabels(false);
			//xAxis->setTickVectorLabels({ "1", "2", "3", "4" });
			//xAxis->setTickVector({ 1, 2, 3, 4 });
		}
		return ref;
	};

	auto graph = m_CustomPlot.addGraph(
		makeAxis(m_CustomPlot.xAxis, QCPAxis::atBottom, pdt->getXAxis()),
		makeAxis(m_CustomPlot.yAxis, QCPAxis::atLeft, pdt->getYAxis()));

	graph->setName(pdt->getName());
	graph->setPen(pdt->getPen()->getPen());
	graph->setLineStyle(pdt->getLineStyle());

	if (auto scatterInfo = pdt->getScatter())
	{
		graph->setScatterStyle(scatterInfo->getStyle());
	}
}

void qmlPlotPaintedItem::clearGraphs(QQmlListProperty<qmlGraph> *p)
{
	auto& info = Info(p);
	info.m_graphs.clear();
	info.plot->clearGraphs();
}

int qmlPlotPaintedItem::graphSize(QQmlListProperty<qmlGraph> *p)
{
	return Info(p).m_graphs.size();
}

qmlGraph * qmlPlotPaintedItem::graphAt(QQmlListProperty<qmlGraph> *p, int index)
{
	return Info(p).m_graphs.at(index);
}

void qmlPlotPaintedItem::mousePressEvent(QMouseEvent* event)
{
	routeMouseEvents(event);
}

void qmlPlotPaintedItem::mouseReleaseEvent(QMouseEvent* event)
{
	routeMouseEvents(event);
}

void qmlPlotPaintedItem::mouseMoveEvent(QMouseEvent* event)
{
	routeMouseEvents(event);
}

void qmlPlotPaintedItem::mouseDoubleClickEvent(QMouseEvent* event)
{
	routeMouseEvents(event);
}

void qmlPlotPaintedItem::routeWheelEvents(QWheelEvent* event)
{
	QWheelEvent* newEvent = new QWheelEvent(event->pos(), event->delta(), event->buttons(), event->modifiers(), event->orientation());
	QCoreApplication::postEvent(&m_CustomPlot, newEvent);
}

void qmlPlotPaintedItem::wheelEvent(QWheelEvent *event)
{
	routeWheelEvents(event);
}

void qmlPlotPaintedItem::onGraphClicked(QCPAbstractPlottable* plottable)
{
}

void qmlPlotPaintedItem::routeMouseEvents(QMouseEvent* event)
{
	QMouseEvent* newEvent = new QMouseEvent(event->type(), event->localPos(), event->button(), event->buttons(), event->modifiers());
	QCoreApplication::postEvent(&m_CustomPlot, newEvent);
}

void qmlPlotPaintedItem::onUpdateCustomPlotSize()
{
	m_CustomPlot.setGeometry(0, 0, width(), height());
	m_CustomPlot.setViewport(QRect(0, 0, (int) width(), (int) height()));
}

void qmlPlotPaintedItem::onCustomReplot()
{
	update();
}

void qmlPlotPaintedItem::exportPDF(const QString& name, int w/*=0*/, int h/*=0*/)
{
	m_CustomPlot.savePdf(name, false, w, h);
}