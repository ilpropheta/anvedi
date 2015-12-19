#pragma once
#include <QObject>
#include "TestRunner.h"

class GraphPresenterTests : public QObject
{
	Q_OBJECT
private slots:
	// model -> (signal) -> presenter
	void On_DataAdded_Should_CreateVisibleGraphs();
	void On_DomainChanged_Should_SetDataToGraphs();
	void On_DataCleared_Should_RemoveGraphs();
	void On_SignalVisibilityChanged_Should_UpdateGraphVisibility();
	void On_SignalColorChanged_Should_UpdateGraphColor();
};

DECLARE_TEST(GraphPresenterTests)