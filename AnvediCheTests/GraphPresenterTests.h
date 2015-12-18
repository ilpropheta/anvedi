#pragma once
#include <QObject>
#include "TestRunner.h"

class GraphPresenterTests : public QObject
{
	Q_OBJECT
private slots:
	void On_DataAdded_Should_CreateVisibleGraphs();
	void On_DomainChanged_Should_SetDataToGraphs();
};

DECLARE_TEST(GraphPresenterTests)