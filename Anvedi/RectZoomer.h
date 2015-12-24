#pragma once	
#include <QObject>
#include <QPoint>
#include <QRubberBand>

class QCustomPlot;
class QMouseEvent;

class RectZoomer : public QObject
{
	Q_OBJECT
public:
	RectZoomer(QCustomPlot* plot);
public slots:
	void OnMousePress(QMouseEvent*);
	void OnMouseRelease(QMouseEvent*);
	void OnMouseMove(QMouseEvent*);
	void OnResetZoom();
private:
	QRubberBand rubberBand;
	QPoint origin;
	QCustomPlot* plot;
	class IZoomAction* zoomAction;
};

