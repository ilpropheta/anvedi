#include "SignalDataTests.h"
#include <QSignalSpy>
#include "..\SignalData.h"
#include <utility>

using namespace std;

void SignalDataTests::On_empty_add_ShouldNotEmit_DataAdded()
{
	SignalData data;
	QSignalSpy spy(&data, SIGNAL(DataAdded(const DataMap&)));
	data.add({});

	QCOMPARE(spy.count(), 0);
}

void SignalDataTests::On_add_ShouldEmit_DataAdded()
{
	SignalData data;
	QSignalSpy spy(&data, SIGNAL(DataAdded(const DataMap&)));
	data.add({ { "signal", { "signal", "red", true, { 1, 2, 3 } } } });

	QCOMPARE(spy.count(), 1); 
	auto receivedMap = spy.takeFirst().takeFirst().value<DataMap>();
	DataMap expectedMap { { "signal", { "signal", "red", true, { 1, 2, 3 } } } };
	QCOMPARE(receivedMap, expectedMap);
}

void SignalDataTests::On_add_Should_AllowOverwritingElements()
{
	SignalData data;
	QSignalSpy spy(&data, SIGNAL(DataAdded(const DataMap&)));
	data.add({ 
		{ "signal", { "signal", {}, true } }, 
		{ "line", { "line", {}, false } }
	});
	QCOMPARE(data.get("signal").visible, true);
	QCOMPARE(data.get("line").visible, false);
	
	data.add({ 
		{ "signal", { "signal", {}, false } },
		{ "other", { "other", {}, true } }
	});
	QCOMPARE(data.get("signal").visible, false);
	QCOMPARE(data.get("other").visible, true);

	QCOMPARE(spy.count(), 2);
	auto finalMap = spy.takeAt(1).takeFirst().value<DataMap>();
	DataMap expectedMap{
		{ "signal", { "signal", {}, false } },
		{ "other", { "other", {}, true } },
		{ "line", { "line", {}, false } }
	};
	QCOMPARE(finalMap, expectedMap);
}

void SignalDataTests::On_clear_ShouldEmit_DataCleared()
{
	SignalData data;
	QSignalSpy spy(&data, SIGNAL(DataCleared()));
	data.clear();

	QCOMPARE(spy.count(), 1);
}

void SignalDataTests::On_setValues_ShouldEmit_SignalValuesChanged()
{
	SignalData data;
	data.add({ { "signal", {"signal"} } });
	QSignalSpy spy(&data, SIGNAL(SignalValuesChanged(const Signal&)));
	data.setValues("signal", {1,2,3});

	QCOMPARE(spy.count(), 1);
	auto receivedSignal = spy.takeFirst().takeFirst().value<Signal>();
	Signal actualSignal{ "signal", {}, {}, {1,2,3} };
	QCOMPARE(receivedSignal, actualSignal);
}

void SignalDataTests::On_setAsDomain_ShouldEmit_DomainChanged()
{
	SignalData data;
	QSignalSpy spy(&data, SIGNAL(DomainChanged(const Signal&)));
	data.add({ { "signal", {"signal"} } });
	data.setAsDomain("signal");
	
	QCOMPARE(spy.count(), 1);
	auto actualAdded = spy.takeFirst().takeFirst().value<Signal>();

	QCOMPARE(actualAdded.name, QString("signal"));
}

void SignalDataTests::On_setAsDomainWithWrongSignal_ShouldThrowException()
{
	SignalData data;
	QVERIFY_EXCEPTION_THROWN(data.setAsDomain("unknown"), std::exception);
}

void SignalDataTests::On_domainLowerBound()
{
	SignalData data;
	data.add({ { "domain", { "domain", {}, {}, { 1, 2, 3, 4, 5 } } } });
	data.setAsDomain("domain");
	QVERIFY(std::make_pair(2.0, 1ull) == data.domainLowerBound(1.5));
	QVERIFY(std::make_pair(1.0, 0ull) == data.domainLowerBound(0.5));
	QVERIFY(std::make_pair(5.0, 4ull) == data.domainLowerBound(6.0));
	QVERIFY(std::make_pair(4.0, 3ull) == data.domainLowerBound(4.0));
}

void SignalDataTests::On_domainNextValue()
{
	SignalData data;
	data.add({ { "domain", { "domain", {}, {}, { 1, 2, 3, 4, 5 } } } });
	data.setAsDomain("domain");
	QVERIFY(1.0 == data.domainNextValue(0.0)); // next non-existent left
	QVERIFY(2.0 == data.domainNextValue(1.0));
	QVERIFY(2.0 == data.domainNextValue(1.5));
	QVERIFY(5.0 == data.domainNextValue(5.0));
	QVERIFY(5.0 == data.domainNextValue(6.0)); // next non-existent right
}

void SignalDataTests::On_domainPrevValue()
{
	SignalData data;
	data.add({ { "domain", { "domain", {}, {}, { 1, 2, 3, 4, 5 } } } });
	data.setAsDomain("domain");
	QVERIFY(1.0 == data.domainPrevValue(0.0)); // prev non-existent left
	QVERIFY(1.0 == data.domainPrevValue(1.0));
	QVERIFY(1.0 == data.domainPrevValue(2.0));
	QVERIFY(1.0 == data.domainPrevValue(1.5));
	QVERIFY(5.0 == data.domainPrevValue(6.0)); // prev non-existent right
}