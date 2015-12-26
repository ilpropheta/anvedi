#pragma once
#include <QObject>
#include <map>
#include <utility>
#include "qcustomplot.h"
#include "SignalData.h"

class QCustomPlot;
class SignalData;
class PlotInfo;

class GraphPresenter : public QObject
{
	Q_OBJECT
public:
	GraphPresenter(QCustomPlot* plot, const SignalData& data, PlotInfo& plotInfo);
public slots:
	// data
	void OnNewData(const DataMap& data);
	void OnClearData();
	void OnGraphDataChanged(const Signal& signal);
	void OnGraphsDataAdded(const QVector<qreal>&, const std::map<QString, QVector<qreal>>&);
	void OnGraphVisibilityChanged(const Signal& signal);
	void OnDomainChanged(const Signal& domain);
	void OnCursorValueChanged(qreal, size_t);
	// plot info
	void OnBackgroundChanged(const QColor& color);
	void OnXRangeChanged(const QCPRange&);
private:
	void MakeGraphOrUseExistent(const Signal& signal, std::function<void(QCPGraph*)> action);
	void MakeGraphOrUseExistent_WithFinalReplot(const Signal& signal, std::function<void(QCPGraph*)> action);
	void SetGraphDataFrom(QCPGraph& graph, const Signal& signal);
	void SetGraphicInfoFrom(QCPGraph& graph, const Signal& signal);

	QCustomPlot* plot;
	std::map<QString, QCPGraph*> displayedGraphs;
	const SignalData& data;
	PlotInfo& plotInfo;
};