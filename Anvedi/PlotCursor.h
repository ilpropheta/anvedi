#pragma once
#include <QObject>

class QCPItemStraightLine;
class QCustomPlot;
class QMouseEvent;
class QKeyEvent;

class PlotCursor : public QObject
{
	Q_OBJECT
public:
	PlotCursor(QCustomPlot* parent, qreal pStepSize);

	// movement
	void set(qreal xVal);
	void moveForward();
	void moveBackward();

	// getters
	qreal xPos() const;
	qreal step() const;

	// setters
	void setStep(qreal newStep);
public slots:
	void OnMouseEvent(QMouseEvent*);
signals:
	void CursorChanged(qreal);
private:
	void move(qreal delta);

	QCPItemStraightLine *cursor;
	QCustomPlot* plot;
	qreal cursorStepSize;
};

