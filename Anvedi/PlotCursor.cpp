#include "PlotCursor.h"
#include "qcustomplot.h"
#include "utils.h"
#include <complex>
#include <QEvent>

PlotCursor::PlotCursor(QCustomPlot* parent, qreal pStepSize)
	: cursor(nullptr), plot(parent), cursorStepSize(pStepSize)
{
	cursor = new QCPItemStraightLine(parent);
	initLinePos();

	// todo: switch to complementary color if background color is close to cursor color
	cursor->setPen(QPen(QBrush(Qt::black), 1.5));

	// connect to QCustomPlot events
	QObject::connect(plot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(OnMouseEvent(QMouseEvent*)));
	QObject::connect(plot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(OnMouseEvent(QMouseEvent*)));
}

void PlotCursor::initLinePos()
{
	cursor->point1->setCoords(plot->xAxis->pixelToCoord(0.0), 0.0);
	cursor->point2->setCoords(plot->xAxis->pixelToCoord(0.0), 1.0);
}

void PlotCursor::reset()
{
	initLinePos();
	plot->replot();

	if (plot->graphCount())
	{
		auto val = plot->graph(0)->data()->begin()->key;
		set(val);
	}
}

void PlotCursor::set(qreal xVal)
{
	if (!plot->graphCount() || plot->graph(0)->data()->empty())
		return;

	// assumes the same domain for each graph
	const auto xData = plot->graph(0)->data();
	const auto rangeMin = xData->begin()->key;
	const auto rangeMax = (xData->end() - 1)->key;
	// saturate
	auto domainVal = (xVal >= rangeMin) ? xVal : rangeMin;
	const auto lbIt = xData->lowerBound(domainVal);
	domainVal = (domainVal > rangeMax) ? rangeMax : lbIt->key;
	const auto domPos = (domainVal >= rangeMax) ? (xData->size() - 1) : std::distance(xData->begin(), lbIt);

	cursor->point1->setCoords(domainVal, cursor->point1->coords().y());
	cursor->point2->setCoords(domainVal, cursor->point2->coords().y());
	plot->replot(QCustomPlot::rpHint);

	emit CursorChanged(domainVal, domPos);
}

void PlotCursor::move(qreal delta)
{
	auto xVal = cursor->point1->coords().x();
	xVal += delta;
	set(xVal);
}

void PlotCursor::moveForward()
{
	move(cursorStepSize);
}

void PlotCursor::moveBackward()
{
	move(-cursorStepSize);
}

qreal PlotCursor::xPos() const
{
	return cursor->point1->coords().x();
}

qreal PlotCursor::step() const
{
	return cursorStepSize;
}

void PlotCursor::setStep(qreal newStepSize)
{
	cursorStepSize = newStepSize;
}

void PlotCursor::OnMouseEvent(QMouseEvent* e)
{
	if (e->buttons() == Qt::LeftButton)
	{
		const auto mouseClickAxesCoords = plot->xAxis->pixelToCoord(e->pos().x());
		set(mouseClickAxesCoords);
	}
}

void PlotCursor::OnBackgroundChanged(const QColor& c)
{
	const auto currPen = cursor->pen();
	if (close(currPen.color(), c))
		cursor->setPen(QPen(invert(c), currPen.width()));
}
