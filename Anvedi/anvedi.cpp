#include "anvedi.h"
#include <QtScript\QScriptContext>
#include "SignalHandle.h"
#include "WorkspaceSerializer.h"
#include "ScriptManager.h"
#include <algorithm>

using namespace std;

Anvedi::Anvedi(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_plotInfo.setPlot(ui.plot);
	m_plotInfo.setBackgroundColor(Qt::white);

	cursor = make_unique<PlotCursor>(ui.plot, m_data);
	rectZoomer = make_unique<RectZoomer>(ui.plot);
	signalListPresenter = make_unique<SignalListPresenter>(ui.signalList, ui.filterEdit, ui.signalCountLabel, ui.domainLabel, m_data);
	graphPresenter = make_unique<GraphPresenter>(ui.plot, m_data);

	ScriptManager::InitWorkspace(m_data, m_plotInfo, *ui.console);

	// window -> zoomer
	QObject::connect(ui.actionResetAxes, SIGNAL(triggered()), rectZoomer.get(), SLOT(OnResetZoom()));
	// cursor -> list
	QObject::connect(cursor.get(), SIGNAL(CursorChanged(qreal, size_t)), signalListPresenter.get(), SLOT(OnCursorValueChanged(qreal, size_t)));
	// cursor -> graph
	QObject::connect(cursor.get(), SIGNAL(CursorChanged(qreal, size_t)), graphPresenter.get(), SLOT(OnCursorValueChanged(qreal, size_t)));
	// plot info -> graph
	QObject::connect(&m_plotInfo, SIGNAL(BackgroundColorChanged(const QColor&)), graphPresenter.get(), SLOT(OnBackgroundChanged(const QColor&)));
	// plot info -> cursor
	QObject::connect(&m_plotInfo, SIGNAL(BackgroundColorChanged(const QColor&)), cursor.get(), SLOT(OnBackgroundChanged(const QColor&)));
	// window -> plot info
	QObject::connect(ui.actionChangeBackground, &QAction::triggered, [this]{
		const auto color = QColorDialog::getColor(m_plotInfo.getBackgroundColor(), this, "Change the background color");
		if (color.isValid())
			m_plotInfo.setBackgroundColor(color);
	});

	// the following is experimental
	QObject::connect(ui.actionStartRT, SIGNAL(triggered()), this, SLOT(OnRTStart()));
	QObject::connect(ui.actionStopRT, SIGNAL(triggered()), this, SLOT(OnRTStop()));
	QObject::connect(ui.actionPauseRT, SIGNAL(triggered()), this, SLOT(OnRTPause()));
	QObject::connect(ui.actionCursorRT, SIGNAL(triggered(bool)), this, SLOT(OnRTFollowingCursor(bool)));
}

void Anvedi::OnExit()
{
	QApplication::exit();
}

void Anvedi::OnDataImport()
{
	const auto files = QFileDialog::getOpenFileNames(this, "Import as JSON", ".", "*.json");
	for (const auto& file : files)
	{
		WorkspaceSerializer::Read(file, m_data, m_plotInfo);
	}
	cursor->reset();
}

void Anvedi::OnDataExport()
{
	auto file = QFileDialog::getSaveFileName(this, "Export as JSON", ".", "*.json");
	if (!file.endsWith(".json"))
		file.append(".json");
	WorkspaceSerializer::Write(file, m_data, m_plotInfo);
}

void Anvedi::OnDataClear()
{
	m_data.clear();
	ScriptManager::InitWorkspace(m_data, m_plotInfo, *ui.console);
	cursor->reset();
}

void Anvedi::keyPressEvent(QKeyEvent * e)
{
	if (e->key() == Qt::Key_Escape)
		QApplication::exit();
	cursor->OnKeyboardPressed(e);
}

// the following is experimental
void Anvedi::EnableMenuInRT(bool enable)
{
	ui.actionImport->setEnabled(enable);
	ui.actionExport->setEnabled(enable);
	ui.actionClear->setEnabled(enable);
	ui.actionStartRT->setEnabled(enable);
}

void Anvedi::OnRTStart()
{
	static const int timerInterval_ms = 100;
	static const size_t packetCount = 100;

	if (isPaused)
	{
		EnableMenuInRT(false);
		dataTimer.start(timerInterval_ms);
		return;
	}

	SignalData data; PlotInfo info;
	WorkspaceSerializer::Read(R"(..\anvedi\json\physx.json)", data, info);
	timeStep = 0u;
	auto domain = data.getDomain()->y;
	DataMap toSet;
	vector < pair<QString, QVector<qreal>>> toSend;
	data.onSignals([&](const Signal& signal){
		toSet.emplace(signal.name, Signal{ signal.name, signal.color, signal.visible });
		toSend.emplace_back(make_pair(signal.name, std::move(signal.y)));
	});
	m_data.clear();
	m_data.add(std::move(toSet));
	m_data.setAsDomain(data.getDomain()->name);

	EnableMenuInRT(false);

	connect(&dataTimer, &QTimer::timeout, [toSend, this]{
		map<QString, QVector<qreal>> sliceToSend;
		for (const auto& elem : toSend)
		{
			QVector<qreal> data; data.reserve(20);
			for (auto i = 0; i < 20 && (timeStep + i < elem.second.size()); ++i)
			{
				data.push_back(elem.second.at(timeStep+i));
			}
			sliceToSend.emplace(elem.first, data);
		}
		timeStep += 20;
		if (!sliceToSend.begin()->second.empty())
			m_data.addValues(sliceToSend);
	});
	dataTimer.start(timerInterval_ms);
}

void Anvedi::OnRTPause()
{
	dataTimer.stop();
	isPaused = true;
	EnableMenuInRT(true);
	ui.actionStartRT->setText("Resume");
}

void Anvedi::OnRTStop()
{
	dataTimer.stop();
	this->disconnect(&dataTimer);
	isPaused = false;
	timeStep = 0.0;
	EnableMenuInRT(true);
	ui.actionStartRT->setText("Start");
}

void Anvedi::OnRTFollowingCursor(bool following)
{
	cursor->followInRT(following);
}
