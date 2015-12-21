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
	QObject::connect(ui.actionResetZoom, SIGNAL(triggered()), rectZoomer.get(), SLOT(OnResetZoom()));
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
	static const int timerInterval_ms = 200;
	static const size_t packetCount = 10;

	if (isPaused)
	{
		EnableMenuInRT(false);
		dataTimer.start(timerInterval_ms);
		return;
	}

	EnableMenuInRT(false);
	m_data.clear();
	m_data.add({
		{ "domain", { "domain", "red", true, {} } },
		{ "sin", { "sin", "blue", false, {} } },
		{ "cos", { "cos", "green", true, {} } },
	});
	m_data.setAsDomain("domain");

	connect(&dataTimer, &QTimer::timeout, [this]{
		double key = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
		static double lastPointKey = 0;

		QVector<qreal> domain(packetCount);
		QVector<qreal> sinx(packetCount, std::numeric_limits<qreal>::quiet_NaN());
		QVector<qreal> cosx(packetCount, std::numeric_limits<qreal>::quiet_NaN());

		std::generate(domain.begin(), domain.end(), [&]{
			auto tmp = timeStep;
			timeStep += 0.0001;
			return tmp;
		});

		for (auto i = 0; i < packetCount; ++i)
		{
			sinx[i] = sin(domain[i] * 2 * 3.14);
			cosx[i] = cos(domain[i] * 2 * 3.14);
		}

		m_data.addValues({
			{"domain", domain}, { "sin", sinx}, { "cos", cosx }
		});

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
