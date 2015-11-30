#pragma once
#include <QObject>
#include "GraphPresenter.h"

class QTableWidget;
class QLabel;

class SignalListPresenter : public QObject
{
	Q_OBJECT
public:
	SignalListPresenter(QTableWidget* signalList, QLabel* signalCntLabel);
	public slots:
	void OnNewData(const DataMap& data);
	void OnClearData();
	void OnSignalFilterEdited(const QString& filter);
signals:
	void GraphColorChanged(const QString&, const QColor&);
	void DisplayGraph(const QString&);
	void HideGraph(const QString&);
private:
	QTableWidget* signalList;
	QLabel* signalCntLabel;
};