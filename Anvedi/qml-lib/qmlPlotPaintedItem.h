#pragma once

#include <QtQuick>
#include "qcustomplot.h"
#include "qmlGraph.h"
#include "qmlLegend.h"

class QCPAbstractPlottable;

struct ListInfo
{
	QList<qmlGraph*> m_graphs;
	QCustomPlot* plot;
};

class qmlPlotPaintedItem : public QQuickPaintedItem
{
	Q_OBJECT
	Q_PROPERTY(QColor background READ getBackground WRITE setBackground)
	Q_PROPERTY(qmlLegend* legend READ getLegend WRITE setLegend)
	Q_PROPERTY(QQmlListProperty<qmlGraph> graphs READ getGraphs)
public:
	qmlPlotPaintedItem(QQuickItem* parent = 0);

	void paint(QPainter* painter);
	
	QColor getBackground() const;
	void setBackground(QColor);
	QQmlListProperty<qmlGraph> getGraphs();
	qmlLegend* getLegend() const;
	void setLegend(qmlLegend* g);

	// support for QQmlListProperty<qmlGraph>
	static void appendGraph(QQmlListProperty<qmlGraph> *list, qmlGraph *pdt);
	static int graphSize(QQmlListProperty<qmlGraph> *p);
	static qmlGraph *graphAt(QQmlListProperty<qmlGraph> *p, int index);
	static void clearGraphs(QQmlListProperty<qmlGraph> *p);

public slots:
	void addData(int index, QVariantList x, QVariantList  y);
	void exportPDF(const QString& name, int w=0, int h=0);
private slots:
	void onGraphClicked(QCPAbstractPlottable* plottable);
	void onCustomReplot();
	void onUpdateCustomPlotSize();
protected:
	void routeMouseEvents(QMouseEvent* event);
	void routeWheelEvents(QWheelEvent* event);
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent *event) override;
private:
	QCustomPlot m_CustomPlot;
	QColor m_backgroundColor;
	ListInfo m_listInfo;
};