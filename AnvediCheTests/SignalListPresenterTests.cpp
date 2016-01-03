#include <SignalListPresenterTests.h>
#include <QTableWidget>
#include <QLineEdit>
#include <QLabel>
#include "..\SignalListPresenter.h"
#include "..\SignalData.h"
#include "..\PlotCursor.h"
#include "..\qcustomplot.h"

using namespace std;

void SignalListPresenterTests::initTestCase()
{
	// currently just copy/paste from ui_anvedi.h
	signalList.reset(new QTableWidget());
	if (signalList->columnCount() < 4)
		signalList->setColumnCount(4);
	QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
	signalList->setHorizontalHeaderItem(0, __qtablewidgetitem);
	QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
	signalList->setHorizontalHeaderItem(1, __qtablewidgetitem1);
	QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
	signalList->setHorizontalHeaderItem(2, __qtablewidgetitem2);
	QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
	signalList->setHorizontalHeaderItem(3, __qtablewidgetitem3);
	signalList->setObjectName(QStringLiteral("signalList"));
	signalList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	signalList->setDragDropOverwriteMode(false);
}

void SignalListPresenterTests::OnNewData_Should_PopulateTableWidget()
{
	PrepareTest();
	data.add({ 
		{ "domain", { "domain"} },
		{ "signal", { "signal"} },
	});

	QCOMPARE(2, signalList->rowCount());
}

void SignalListPresenterTests::OnClear_Should_EmptyTableWidget()
{
	PrepareTest();
	data.add({
		{ "domain", { "domain" } },
		{ "signal", { "signal" } },
	});
	data.clear();

	QCOMPARE(signalList->rowCount(), 0);
}

void SignalListPresenterTests::OnSignalVisibilityChanged_Should_ChangeCorrespondingTableEntry()
{
	PrepareTest();
	data.add({
		{ "domain", { "domain"} },
	});
	
	QCOMPARE(signalList->item(0, 0)->checkState(), Qt::Unchecked);
	data.setVisible("domain", true);
	QCOMPARE(signalList->item(0, 0)->checkState(), Qt::Checked);
}

void SignalListPresenterTests::OnDomainChanged_Should_BoldCorrespondingTableEntry()
{
	PrepareTest();
	data.add({
		{ "domain", { "domain" } },
		{ "signal", { "signal" } },
	});
	
	QCOMPARE(signalList->item(0, 0)->font().bold(), false);
	data.setAsDomain("domain");
	QCOMPARE(signalList->item(0, 0)->font().bold(), true);
}

// filter -> presenter

void SignalListPresenterTests::OnSignalFilterEdited_Should_HideNonMatchingRows()
{
	PrepareTest();
	data.add({
		{ "domain", { "domain" } },
		{ "signal", { "signal" } },
	});

	QTest::keyClicks(filterEdit.get(), "doma");

	QVERIFY(!signalList->isRowHidden(0));
	QVERIFY(signalList->isRowHidden(1));
}

void SignalListPresenterTests::PrepareTest()
{	
	data.clear();
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

void SignalListPresenterTests::OnCursorChanged_Should_DisplaySignalValuesAtDomain()
{
	PrepareTest();
	data.add({
		{ "domain", { "domain", {1,2,3} } },
		{ "signal", { "signal", {1,4,9} } },
	});
	data.setAsDomain("domain");

	QCustomPlot plot;
	PlotCursor cursor(&plot, data);
	QObject::connect(&cursor, SIGNAL(CursorChanged(qreal, size_t)), presenter.get(), SLOT(OnCursorValueChanged(qreal, size_t)));

	cursor.set(1.0);
	QCOMPARE(signalList->item(0, 1)->text(), QString("1"));
	QCOMPARE(signalList->item(1, 1)->text(), QString("1"));

	cursor.set(2.0);
	QCOMPARE(signalList->item(0, 1)->text(), QString("2"));
	QCOMPARE(signalList->item(1, 1)->text(), QString("4"));

	cursor.set(3.0);
	QCOMPARE(signalList->item(0, 1)->text(), QString("3"));
	QCOMPARE(signalList->item(1, 1)->text(), QString("9"));

	cursor.set(5.0);
	QCOMPARE(signalList->item(0, 1)->text(), QString("3"));
	QCOMPARE(signalList->item(1, 1)->text(), QString("9"));
}
