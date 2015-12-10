#include <SignalListPresenterTests.h>
#include <QTableWidget>
#include <QLineEdit>
#include <QLabel>
#include "..\SignalListPresenter.h"
#include "..\SignalData.h"

using namespace std;

void SignalListPresenterTests::initTestCase()
{
	// currently just copy/paste from ui_anvedi.h
	signalList.reset(new QTableWidget());
	if (signalList->columnCount() < 3)
		signalList->setColumnCount(3);
	QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
	signalList->setHorizontalHeaderItem(0, __qtablewidgetitem);
	QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
	signalList->setHorizontalHeaderItem(1, __qtablewidgetitem1);
	QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
	signalList->setHorizontalHeaderItem(2, __qtablewidgetitem2);
	signalList->setObjectName(QStringLiteral("signalList"));
	signalList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	signalList->setDragDropOverwriteMode(false);

	filterEdit.reset(new QLineEdit());
	signalCntLabel.reset(new QLabel());
	domainLabel.reset(new QLabel());
	presenter = make_unique<SignalListPresenter>(
		signalList.get(),
		filterEdit.get(),
		signalCntLabel.get(),
		domainLabel.get(),
		data);
}

void SignalListPresenterTests::OnNewData_Should_PopulateTableWidget()
{
	data.add({ 
		{ "domain", { "domain"} },
		{ "signal", { "signal"} },
	});

	QCOMPARE(2, signalList->rowCount());
}

void SignalListPresenterTests::OnSignalFilterEdited_Should_HideNonMatchingRows()
{
	data.add({
		{ "domain", { "domain"} },
		{ "signal", { "signal"} },
	});

	QTest::keyClicks(filterEdit.get(), "doma");

	QVERIFY(!signalList->isRowHidden(0));
	QVERIFY(signalList->isRowHidden(1));
}