#include "RectZoomer.h"
#include "qcustomplot.h"
#include <QMouseEvent>

class IZoomAction
{
public:
	virtual QPoint OnMousePress(const QCustomPlot& plot, const QPoint& clickPos) = 0;
	virtual QPoint OnMouseMove(const QCustomPlot& plot, const QPoint& clickPos) = 0;
};

struct RectZoomAction : IZoomAction
{
	QPoint OnMousePress(const QCustomPlot& plot, const QPoint& clickPos) override { return clickPos; }
	QPoint OnMouseMove(const QCustomPlot& plot, const QPoint& clickPos) override	{ return clickPos; }
};

struct HorizontalZoomAction : IZoomAction
{
	static std::pair<int, int> GetYAxisRect(const QCustomPlot& plot)
	{
		int xp1, yp1, xp2, yp2;
		plot.axisRect()->rect().getCoords(&xp1, &yp1, &xp2, &yp2);
		return{ 0, yp2 };
	}
	
	virtual QPoint OnMousePress(const QCustomPlot& plot, const QPoint& clickPos) override
	{
		return{ clickPos.x(), GetYAxisRect(plot).first };
	}

	virtual QPoint OnMouseMove(const QCustomPlot& plot, const QPoint& clickPos) override
	{
		return{ clickPos.x(), GetYAxisRect(plot).second };
	}
};

IZoomAction* GetZoomAction(QMouseEvent* mevent)
{
	static RectZoomAction rectZoomAction;
	static HorizontalZoomAction horizontalZoomAction;

	if (mevent && (mevent->modifiers().testFlag(Qt::ControlModifier)))
		return &rectZoomAction;
	return &horizontalZoomAction;
}

RectZoomer::RectZoomer(QCustomPlot* plot)
	  // note: QCustomPlot as parent is ok because RectZoomer is destroyed before QCustomPlot in Anvedi
	  : rubberBand(QRubberBand::Rectangle, plot), plot(plot), zoomAction(GetZoomAction(nullptr))
{
	// connect to QCustomPlot events
	QObject::connect(plot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(OnMousePress(QMouseEvent*)));
	QObject::connect(plot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(OnMouseMove(QMouseEvent*)));
	QObject::connect(plot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(OnMouseRelease(QMouseEvent*)));
}

void RectZoomer::OnMousePress(QMouseEvent* mevent)
{
	if (!plot->graphCount())
		return;

	switch (mevent->button())
	{
	case Qt::RightButton:
		zoomAction = GetZoomAction(mevent);
		origin = zoomAction->OnMousePress(*plot, mevent->pos());
		rubberBand.setGeometry(QRect(origin, QSize()));
		rubberBand.show();
		break;
	case Qt::MiddleButton:
		OnResetZoom();
		break;
	}
}

void RectZoomer::OnMouseRelease(QMouseEvent*)
{
	if (rubberBand.isVisible())
	{
		int xp1, yp1, xp2, yp2;
		auto zoomRect = rubberBand.geometry();
		zoomRect.getCoords(&xp1, &yp1, &xp2, &yp2);
		if (xp1 < (xp2 + 1)) // skip just click
		{
			const auto x1 = plot->xAxis->pixelToCoord(xp1);
			const auto x2 = plot->xAxis->pixelToCoord(xp2);
			// scale x 
			plot->xAxis->setRange(x1, x2);

			const auto axisRect0 = plot->axisRect(0);
			auto numbOfY = axisRect0->axisCount(QCPAxis::atLeft);
			for (auto i = 0; i < numbOfY; ++i)
			{
				auto currentY = axisRect0->axis(QCPAxis::atLeft, i);
				currentY->setRange(currentY->pixelToCoord(yp1), currentY->pixelToCoord(yp2));
			}
			plot->replot();
		}
		rubberBand.hide();
	}
}

void RectZoomer::OnMouseMove(QMouseEvent* mevent)
{
	if (rubberBand.isVisible())
		rubberBand.setGeometry(QRect(origin, zoomAction->OnMouseMove(*plot, mevent->pos())).normalized());
}

void RectZoomer::OnResetZoom()
{
	for (auto i = 0; i < plot->graphCount(); ++i)
	{
		plot->graph(i)->rescaleAxes();
	}
	plot->replot();
}
