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
	data.add({ { "signal", {} } });

	QCOMPARE(spy.count(), 1); 
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
	data.add({ { "signal", {} } });
	QSignalSpy spy(&data, SIGNAL(SignalValuesChanged(const Signal&)));
	data.setValues("signal", {});

	QCOMPARE(spy.count(), 1);
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
