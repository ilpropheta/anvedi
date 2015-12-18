#pragma warning (disable : 4996)
#include "WorkspaceSerializerTests.h"
#include "..\SignalData.h"
#include "..\PlotInfo.h"
#include "..\WorkspaceSerializer.h"
#include <QSignalSpy>
#include <QDebug>
#include <QDirIterator>

void WorkspaceSerializerTests::ReadTest()
{
	SignalData data;
	PlotInfo plotInfo;
	QSignalSpy modelSpy(&data, SIGNAL(DataAdded(const DataMap&)));
	QSignalSpy plotSpy(&plotInfo, SIGNAL(BackgroundColorChanged(const QColor&)));
	
	WorkspaceSerializer::Read(":/AnvediCheTests/test-data/cubic.json", data, plotInfo);	

	QCOMPARE(1, modelSpy.count());
	QCOMPARE(1, plotSpy.count());

	auto actualAdata = modelSpy.takeFirst().takeFirst().value<DataMap>();
	
	DataMap expectedData = {
		{ "cubic", 
			{
				"cubic", "#008000", true, {1,8,27,64,125,216,343,512,729,1000}
			} 
		},
		{ "line",
			{
				"line", "#ffff00", false, { 1,2,3,4,5,6,7,8,9,10 }
			}
		}
	};
	
	QCOMPARE(actualAdata, expectedData);
}
