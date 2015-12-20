#pragma once
#include <QObject>
#include "TestRunner.h"

class SignalDataTests : public QObject
{
	Q_OBJECT
private slots:
	void On_empty_add_ShouldNotEmit_DataAdded();
	void On_add_ShouldEmit_DataAdded();
	void On_add_Should_AllowOverwritingElements();
	void On_clear_ShouldEmit_DataCleared();
	void On_setValues_ShouldEmit_SignalValuesChanged();
	void On_setAsDomain_ShouldEmit_DomainChanged();
	void On_setAsDomainWithWrongSignal_ShouldThrowException();
	void On_domainLowerBound();
	void On_domainNextValue();
	void On_domainPrevValue();
};

DECLARE_TEST(SignalDataTests)