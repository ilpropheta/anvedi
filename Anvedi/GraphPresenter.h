#pragma once
#include <QObject>
#include <map>
#include <utility>
#include "qcustomplot.h"
#include "SignalData.h"

class QCustomPlot;

class GraphPresenter : public QObject
{
	Q_OBJECT
public:
	GraphPresenter(QCustomPlot* plot);
public slots:
	void OnNewData(const DataMap& data);
	void OnClearData();
	void OnGraphDataChanged(const Signal& signal);
	void OnGraphColorChanged(const Signal& signal);
	void OnGraphVisibilityChanged(const Signal& signal);
	void OnBackgroundChanged(const QColor& color);
private:
	QCustomPlot* plot;
	std::map<QString, QCPGraph*> displayedGraphs;
};