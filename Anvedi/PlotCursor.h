#pragma once
#include <QObject>

class QCPItemStraightLine;
class QCustomPlot;
class QMouseEvent;
class QKeyEvent;
class QCPItemText;
class SignalData;

class PlotCursor : public QObject
{
	Q_OBJECT
public:
	PlotCursor(QCustomPlot* parent, SignalData& data);

	// movement
	void reset();
	void set(qreal xVal);
	void moveForward();
	void moveBackward();
	void moveBegin();
	void moveEnd();
	// getters
	qreal xPos() const;
	// setters
	void setStep(qreal newStep);	
public slots:
	void OnMouseEvent(QMouseEvent*);
	void OnBackgroundChanged(const QColor&);
	void OnKeyboardPressed(QKeyEvent*);
	void OnCursorInRT();
	void OnSetCursorFollowingInRealTime(bool flag);
signals:
	void CursorChanged(qreal, size_t);
private:
	void initLinePos();

	QCPItemStraightLine *cursor;
	QCustomPlot* plot;
	SignalData& data;
};

