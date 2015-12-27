#include "PlotCursor.h"
#include "qcustomplot.h"
#include "utils.h"
#include <complex>
#include <QEvent>
#include "SignalData.h"

PlotCursor::PlotCursor(QCustomPlot* parent, SignalData& data)
	: cursor(nullptr), plot(parent), data(data)
{
	// a straight line that spans infinitely in both directions
	cursor = new QCPItemStraightLine(parent);
	initLinePos();

	// todo: switch to complementary color if background color is close to cursor color
	cursor->setPen({ QBrush(Qt::black), 1.5 });

	// connect to QCustomPlot events
	QObject::connect(plot, &QCustomPlot::mousePress, this, &PlotCursor::OnMouseEvent);
	QObject::connect(plot, &QCustomPlot::mouseMove, this, &PlotCursor::OnMouseEvent);
}

void PlotCursor::initLinePos()
{
	cursor->point1->setCoords(0.0, 0.0);
	cursor->point2->setCoords(0.0, 1.0);
}

void PlotCursor::reset()
{
	initLinePos();
	plot->replot();
	set(std::numeric_limits<qreal>::min());
}

void PlotCursor::set(qreal xVal)
{
	auto domainValInfo = data.domainLowerBound(xVal);
	if (!std::isnan(domainValInfo.first))
	{
		const auto domainVal = domainValInfo.first;
		cursor->point1->setCoords(domainVal, cursor->point1->value());
		cursor->point2->setCoords(domainVal, cursor->point2->value());
		plot->replot(QCustomPlot::rpHint);

		emit CursorChanged(domainVal, domainValInfo.second);
	}
}

void PlotCursor::moveForward()
{
	set(data.domainNextValue(cursor->point1->coords().x()));
}

void PlotCursor::moveBackward()
{
	set(data.domainPrevValue(cursor->point1->coords().x()));
}

qreal PlotCursor::xPos() const
{
	return cursor->point1->key();
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

void PlotCursor::OnKeyboardPressed(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key_Left:
		moveBackward();
		break;
	case Qt::Key_Right:
		moveForward();
		break;
	case Qt::Key_Home:
		moveBegin();
		break;
	case Qt::Key_End:
		moveEnd();
		break;
	}
}

void PlotCursor::OnCursorInRT()
{
	moveEnd();
}

void PlotCursor::moveBegin()
{
	set(std::numeric_limits<qreal>::min());
}

void PlotCursor::moveEnd()
{
	set(std::numeric_limits<qreal>::max());
}

void PlotCursor::OnSetCursorFollowingInRealTime(bool flag)
{
	if (!flag)
	{
		QObject::disconnect(&data, &SignalData::SignalAdded, this, 0);
	}
	else
	{
		QObject::connect(&data, &SignalData::SignalAdded, this, &PlotCursor::OnCursorInRT);
	}
}
