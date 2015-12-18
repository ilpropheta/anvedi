#pragma once
#include <QObject>
#include "TestRunner.h"

class WorkspaceSerializerTests : public QObject
{
	Q_OBJECT
private slots:
	void ReadTest();
};

DECLARE_TEST(WorkspaceSerializerTests)