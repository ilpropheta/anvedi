#pragma once
#include <QObject>
#include <map>
#include <utility>
#include "qcustomplot.h"

class QCustomPlot;

using DataMap = std::map < QString, std::pair<QVector<qreal>, QVector<qreal>> > ;

class GraphPresenter : public QObject
{
	Q_OBJECT
public:
	GraphPresenter(QCustomPlot* plot);
	public slots:
	void OnGraphColorChanged(const QString&, const QColor&);
	void OnSignalAdded(const QString& name);
	void OnSignalRemoved(const QString& name);
	void OnNewData(const DataMap& data);
	void OnClearData();
private:
	QCustomPlot* plot;
	DataMap data; // estrarre model
	std::map<QString, QCPGraph*> displayedGraphs;
};