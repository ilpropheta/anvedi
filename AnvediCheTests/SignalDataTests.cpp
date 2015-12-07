#include "SignalDataTests.h"
#include <QSignalSpy>
#include "..\SignalData.h"

void SignalDataTests::On_add_ShouldEmit_DataAdded()
{
	SignalData data;
	QSignalSpy spy(&data, SIGNAL(DataAdded(const DataMap&)));
	data.add({});

	QCOMPARE(spy.count(), 1); 
}

void SignalDataTests::On_clear_ShouldEmit_DataCleared()
{
	SignalData data;
	QSignalSpy spy(&data, SIGNAL(DataCleared()));
	data.clear();

	QCOMPARE(spy.count(), 1);
}

void SignalDataTests::On_add_ShouldEmit_SignalAdded()
{
	SignalData data;
	QSignalSpy spy(&data, SIGNAL(SignalAdded(const Signal&)));
	data.add("signal", { "signal", Qt::red, true, {}, {1.0, 2.0, 3.0} });
		
	QCOMPARE(spy.count(), 1);
	auto actualAdded = spy.takeFirst().takeFirst().value<Signal>();

	QCOMPARE(actualAdded.name, QString("signal"));
	QVector<qreal> expectedY {1.0, 2.0, 3.0};
	QCOMPARE(actualAdded.y, expectedY);
}

void SignalDataTests::On_set_ShouldEmit_SignalChanged()
{
	SignalData data;
	QSignalSpy spy(&data, SIGNAL(SignalChanged(const Signal&)));
	data.set("signal", [](Signal&){});

	QCOMPARE(spy.count(), 1);
}
