#include "SignalDataTests.h"
#include <QSignalSpy>
#include "..\SignalData.h"

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

void SignalDataTests::On_set_ShouldEmit_SignalChanged()
{
	SignalData data;
	data.add({ { "signal", {} } });
	QSignalSpy spy(&data, SIGNAL(SignalChanged(const Signal&)));
	data.set("signal", [](Signal&){});

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
