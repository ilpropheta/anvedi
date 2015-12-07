#pragma once
#include <QObject>
#include "TestRunner.h"

class SignalDataTests : public QObject
{
	Q_OBJECT
private slots:
	void On_add_ShouldEmit_DataAdded();
	void On_add_ShouldEmit_SignalAdded();
	void On_clear_ShouldEmit_DataCleared();
	void On_set_ShouldEmit_SignalChanged();
};

DECLARE_TEST(SignalDataTests)