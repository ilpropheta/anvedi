#pragma once
#include <QObject>
#include "TestRunner.h"
#include "..\SignalData.h"

class WorkspaceSerializerTests : public QObject
{
	Q_OBJECT
private slots:
	void ReadTest();
	void WriteTest();
private:
	void VerifyActualDataAndPersistedFile(const QString& file, const DataMap& expectedDataMap, const QString& expectedDomain, const QColor& expectedBackground);
};

DECLARE_TEST(WorkspaceSerializerTests)