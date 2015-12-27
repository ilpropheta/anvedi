#pragma warning (disable : 4996)
#include "WorkspaceSerializerTests.h"
#include "..\SignalData.h"
#include "..\PlotInfo.h"
#include "..\WorkspaceSerializer.h"
#include <QSignalSpy>
#include <QDebug>
#include <QDirIterator>

void WorkspaceSerializerTests::VerifyActualDataAndPersistedFile(const QString& file, const DataMap& expectedDataMap, const QString& expectedDomain, const QColor& expectedBackground)
{
	SignalData actualSignalData;
	PlotInfo actualPlotInfo;
	QSignalSpy dataSpy(&actualSignalData, SIGNAL(DataAdded(const DataMap&)));
	QSignalSpy plotSpy(&actualPlotInfo, SIGNAL(BackgroundColorChanged(const QColor&)));

	WorkspaceSerializer::Read(file, actualSignalData, actualPlotInfo);

	QCOMPARE(1, dataSpy.count());
	QCOMPARE(1, plotSpy.count());

	auto actualdata = dataSpy.takeFirst().takeFirst().value<DataMap>();
	QCOMPARE(actualdata, expectedDataMap);
	QCOMPARE(actualSignalData.getDomain()->name, expectedDomain);

	auto actualColor = plotSpy.takeFirst().takeFirst().value<QColor>();
	QCOMPARE(actualColor, expectedBackground);
}


void WorkspaceSerializerTests::ReadTest()
{
	DataMap expectedData = {
		{ "cubic",
			{
				"cubic", { 1, 8, 27, 64, 125, 216, 343, 512, 729, 1000 }, { "#008000", true }
			}
		},
		{ "line",
			{
				"line", { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }, { "#ffff00", false }
			}
		}
	};

	VerifyActualDataAndPersistedFile(":/AnvediCheTests/test-data/cubic.json", expectedData, "line", "#000000");
}

void WorkspaceSerializerTests::WriteTest()
{
	DataMap dataMap = {
		{ "cubic", 
			{
				"cubic", { 1, 8, 27, 64, 125, 216, 343, 512, 729, 1000 }, { "#008000", true }
			} 
		},
		{ "line",
			{
				"line", { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }, { "#ffff00", false }
			}
		}
	};
	PlotInfo plot;
	plot.setBackgroundColor("red");
	SignalData data;
	data.add(dataMap);
	data.setAsDomain("line");

	QString tmpFileName = std::tmpnam(nullptr);
	struct guard
	{
		guard(const QString& fileName)
			: fileName(fileName)
		{
		}
		~guard()
		{
			QFile f(fileName);
			f.remove();
		}
		const QString& fileName;
	} fileDeleter(tmpFileName);

	WorkspaceSerializer::Write(tmpFileName, data, plot);
	
	VerifyActualDataAndPersistedFile(tmpFileName, dataMap, "line", "red");
}
