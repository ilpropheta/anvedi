#pragma once
#include <QObject>
#include "TestRunner.h"

class SignalDataTests : public QObject
{
	Q_OBJECT
private slots:
	void On_empty_add_ShouldNotEmit_DataAdded();
	void On_add_ShouldEmit_DataAdded();
	void On_clear_ShouldEmit_DataCleared();
	void On_set_ShouldEmit_SignalChanged();
	void On_setAsDomain_ShouldEmit_DomainChanged();
	void On_setAsDomainWithWrongSignal_ShouldThrowException();
};

DECLARE_TEST(SignalDataTests)