#include "anvedi.h"
#include <QtScript\QScriptContext>
#include "SignalHandle.h"
#include "WorkspaceSerializer.h"
#include "ScriptManager.h"

using namespace std;

Anvedi::Anvedi(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_plotInfo.setPlot(ui.plot);
	m_plotInfo.setBackgroundColor(Qt::white);

	cursor = make_unique<PlotCursor>(ui.plot, m_data);
	rectZoomer = make_unique<RectZoomer>(ui.plot, m_data);
	signalListPresenter = make_unique<SignalListPresenter>(ui.signalList, ui.filterEdit, ui.signalCountLabel, ui.domainLabel, m_data);
	graphPresenter = make_unique<GraphPresenter>(ui.plot, ui.rangeScroll, m_data, m_plotInfo);
	rtPresenter = make_unique<RealTimePresenter>(m_data, RTMenuInfo{ui.actionStartRT, ui.actionStopRT, ui.actionConfig, ui.actionImport, ui.actionExport, ui.actionClear});

	ScriptManager::InitWorkspace(m_data, m_plotInfo, *ui.console);

	QObject::connect(ui.actionResetAxes, &QAction::triggered, [this]{
		m_data.setAutoRangeAll();
	});
	// cursor -> list
	QObject::connect(cursor.get(), SIGNAL(CursorChanged(qreal, size_t)), signalListPresenter.get(), SLOT(OnCursorValueChanged(qreal, size_t)));
	// cursor -> graph
	QObject::connect(cursor.get(), SIGNAL(CursorChanged(qreal, size_t)), graphPresenter.get(), SLOT(OnCursorValueChanged(qreal, size_t)));
	// RT cursor -> cursor
	QObject::connect(ui.actionCursorRT, SIGNAL(triggered(bool)), cursor.get(), SLOT(OnSetCursorFollowingInRealTime(bool)));	
	// plot info -> cursor
	QObject::connect(&m_plotInfo, SIGNAL(BackgroundColorChanged(const QColor&)), cursor.get(), SLOT(OnBackgroundChanged(const QColor&)));
	// window -> plot info
	QObject::connect(ui.actionChangeBackground, &QAction::triggered, [this]{
		const auto color = QColorDialog::getColor(m_plotInfo.getBackgroundColor(), this, "Change the background color");
		if (color.isValid())
			m_plotInfo.setBackgroundColor(color);
	});	

	ui.actionCursorRT->trigger();
}

void Anvedi::OnExit()
{
	QApplication::exit();
}

void Anvedi::OnDataImport()
{
	const auto files = QFileDialog::getOpenFileNames(this, "Import as JSON", ".", "*.json");
	if (files.isEmpty())
		return;
	for (const auto& file : files)
	{
		WorkspaceSerializer::Read(file, m_data, m_plotInfo);
	}
	cursor->reset();
}

void Anvedi::OnDataExport()
{
	auto file = QFileDialog::getSaveFileName(this, "Export as JSON", ".", "*.json");
	if (file.isEmpty())
		return;
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

void Anvedi::OnChartImport()
{
	const auto file = QFileDialog::getOpenFileName(this, "Import plot", ".", "*.plt");
	if (file.isEmpty())
		return;
	WorkspaceSerializer::Read(file, m_data, m_plotInfo, [this](Signal&& signal){
		m_data.addEmptyIfNotExists(signal.name);
		m_data.setSignalGraphic(signal.name, std::move(signal.graphic));
	});
}

void Anvedi::OnChartExport()
{
	auto file = QFileDialog::getSaveFileName(this, "Export plot", ".", "*.plt");
	if (file.isEmpty())
		return;
	if (!file.endsWith(".plt"))
		file.append(".plt");
	WorkspaceSerializer::Write(file, m_data, m_plotInfo, false);
}

