#pragma once
#include <QObject>
#include "TestRunner.h"
#include <memory>
#include <QTableWidget>
#include <QLineEdit>
#include "..\SignalData.h"
#include "..\SignalListPresenter.h"

class SignalListPresenterTests : public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();
	// model -> (signal) -> presenter
	void OnNewData_Should_PopulateTableWidget();
	void OnClear_Should_EmptyTableWidget();
	void OnSignalVisibilityChanged_Should_ChangeCorrespondingTableEntry();
	void OnDomainChanged_Should_BoldCorrespondingTableEntry();
	// filter -> presenter
	void OnSignalFilterEdited_Should_HideNonMatchingRows();
private:
	void PrepareTest();

	std::unique_ptr<QTableWidget> signalList;
	std::unique_ptr<QLineEdit> filterEdit;
	std::unique_ptr<QLabel> signalCntLabel;
	std::unique_ptr<QLabel> domainLabel;
	SignalData data;

	std::unique_ptr<SignalListPresenter> presenter;
};

DECLARE_TEST(SignalListPresenterTests)