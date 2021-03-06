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
	GraphPresenter(QCustomPlot* plot, QScrollBar* rangeScroll, const SignalData& data, PlotInfo& plotInfo);
public slots:
	// data
	void OnNewData(const DataMap& data);
	void OnSignalRemoved(const QString& who);
	void OnSignalRenamed(const QString& oldName, const Signal& signal);
	void OnClearData();
	void OnGraphDataChanged(const Signal& signal);
	void OnGraphsDataAdded(const QVector<qreal>&, const std::map<QString, QVector<qreal>>&);
	void OnGraphStyleInfoChanged(const Signal& signal);
	void OnGraphVisibilityChanged(const Signal& signal);
	void OnGraphRangeChanged(const Signal& signal);
	void OnGraphTicksChanged(const Signal& signal);
	void OnDomainChanged(const Signal& domain);
	void OnCursorValueChanged(qreal, size_t);
	// plot info
	void OnBackgroundChanged(const QColor& color);
	void OnXRangeChanged(const QCPRange&);
	// xrange scrollbar
	void rangeScrollbarValueChanged(int);
private:
	void MakeGraphOrUseExistent(const Signal& signal, std::function<void(QCPGraph*)> action);
	void MakeGraphOrUseExistent_WithFinalReplot(const Signal& signal, std::function<void(QCPGraph*)> action);
	// utilities to set graph properties
	void SetGraphDataFrom(QCPGraph& graph, const Signal& signal);
	void SetGraphicInfoFrom(QCPGraph& graph, const Signal& signal);
	void SetAxisInfo(QCPGraph& graph, const Signal& signal);
	void SetRangeInfo(QCPGraph& graph, const Signal& signal);
	// setting axis, grid, etc color and pen
	void SetAxisColor(QCPAxis * yAxis);
	// RT
	bool IsFirstRTPacket(const std::map<QString, QVector<qreal>>& dataSlice);
	
	QCustomPlot* plot;
	QScrollBar* rangeScroll;
	std::map<QString, QCPGraph*> displayedGraphs;
	const SignalData& data;
	PlotInfo& plotInfo;
};