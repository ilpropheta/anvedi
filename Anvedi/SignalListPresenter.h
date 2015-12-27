#pragma once
#include <QObject>
#include "GraphPresenter.h"
#include "SignalInfoPresenter.h"

class QTableWidget;
class QLabel;
class SignalData;
class QLineEdit;
class QTableWidgetItem;

class SignalListPresenter : public QObject
{
	Q_OBJECT
public:
	SignalListPresenter(QTableWidget* signalList, QLineEdit* filterEdit, QLabel* signalCntLabel, QLabel* domainLabel, SignalData& data);
public slots:
	void OnNewData(const DataMap& data);
	void OnClearData();
	void OnSignalFilterEdited(const QString& filter);
	void OnSignalVisibilityChanged(const Signal& signal);
	void OnSignalColorChanged(const Signal& signal);
	void OnCursorValueChanged(qreal xVal, size_t idx);
	void OnDomainChanged(const Signal& newDomain);
private:
	SignalInfoPresenter infoPresenter;
	QTableWidget* signalList;
	QLineEdit* filterEdit;
	QLabel* signalCntLabel;
	QLabel* domainLabel;
	QTableWidgetItem* domain;
	SignalData& data;
};