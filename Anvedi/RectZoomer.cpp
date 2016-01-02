#include "RectZoomer.h"
#include "qcustomplot.h"
#include <QMouseEvent>
#include "SignalData.h"
#include "Utils.h"

class IZoomAction
{
public:
	virtual QPoint OnMousePress(const QCustomPlot& plot, const QPoint& clickPos) = 0;
	virtual QPoint OnMouseMove(const QCustomPlot& plot, const QPoint& clickPos) = 0;
};

struct RectZoomAction : IZoomAction
{
	QPoint OnMousePress(const QCustomPlot&, const QPoint& clickPos) override { return clickPos; }
	QPoint OnMouseMove(const QCustomPlot&, const QPoint& clickPos) override	{ return clickPos; }
};

struct HorizontalZoomAction : IZoomAction
{
	static std::pair<int, int> GetYAxisRect(const QCustomPlot& plot)
	{
		int xp1, yp1, xp2, yp2;
		plot.axisRect()->rect().getCoords(&xp1, &yp1, &xp2, &yp2);
		return{ yp1-1, yp2 };
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


RectZoomer::RectZoomer(QCustomPlot* plot, const SignalData& data)
	  // note: QCustomPlot as parent is ok because RectZoomer is destroyed before QCustomPlot in Anvedi
	  : rubberBand(QRubberBand::Rectangle, plot), plot(plot), data(data), zoomAction(GetZoomAction(nullptr))
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
		int topLeftX, topLeftY, bottomRightX, bottomRightY;
		const auto zoomRect = rubberBand.geometry();
		zoomRect.getCoords(&topLeftX, &topLeftY, &bottomRightX, &bottomRightY);
		if (topLeftX < (bottomRightX + 1)) // skip just click
		{
			ZoomInPixelCoordinates(topLeftX, bottomRightX, bottomRightY, topLeftY);
			plot->replot();
		}
		rubberBand.hide();
	}
}

void RectZoomer::OnMouseMove(QMouseEvent* mevent)
{
	if (rubberBand.isVisible())
	{
		rubberBand.setGeometry(QRect(
			origin, 
			zoomAction->OnMouseMove(*plot, mevent->pos())).normalized());
	}
}

void RectZoomer::OnResetZoom()
{
	for (auto i = 0; i < plot->graphCount(); ++i)
	{
		auto graph = plot->graph(i);
		const auto& signal = data.get(graph->name());
		graph->valueAxis()->setRange(signal.graphic.rangeLower, signal.graphic.rangeUpper);
	}
	plot->xAxis->rescale();
	plot->replot();
}

void SetAxisRangeInPixelCoordsWithSaturation(QCPAxis& axis, double rangeLo, double rangeUp)
{
	const auto currentRange = axis.range();
	axis.setRange(SaturateLeft(axis.pixelToCoord(rangeLo), currentRange.lower), SaturateRight(axis.pixelToCoord(rangeUp), currentRange.upper));
}

void RectZoomer::ZoomInPixelCoordinates(double loX, double upX, double loY, double upY)
{
	SetAxisRangeInPixelCoordsWithSaturation(*plot->xAxis, loX, upX);

	for (auto i = 0; i < plot->graphCount(); ++i)
	{
		SetAxisRangeInPixelCoordsWithSaturation(*plot->graph(i)->valueAxis(), loY, upY);
	}
}
