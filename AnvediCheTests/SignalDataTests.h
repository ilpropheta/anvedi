#pragma once
#include <QObject>
#include "TestRunner.h"

class SignalDataTests : public QObject
{
	Q_OBJECT
private slots:
	void OnAddDataShouldEmitDataAdded();
};

DECLARE_TEST(SignalDataTests)