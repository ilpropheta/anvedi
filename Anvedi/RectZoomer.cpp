#include "RectZoomer.h"
#include "qcustomplot.h"
#include <QMouseEvent>

RectZoomer::RectZoomer(QCustomPlot* plot)
	  // note: QCustomPlot as parent is ok because RectZoomer is destroyed before QCustomPlot in Anvedi
	: rubberBand(QRubberBand::Rectangle, plot), plot(plot)
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
		origin = mevent->pos();
		rubberBand.setGeometry(QRect(origin, QSize()));
		rubberBand.show();
		break;
	case Qt::MiddleButton:
		for (auto i = 0; i < plot->graphCount(); ++i)
		{
			plot->graph(i)->rescaleAxes();
		}
		plot->replot();
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
			// scale of x 
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
	rubberBand.setGeometry(QRect(origin, mevent->pos()).normalized());
}
