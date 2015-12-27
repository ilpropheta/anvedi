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
	rectZoomer = make_unique<RectZoomer>(ui.plot, m_data);
	signalListPresenter = make_unique<SignalListPresenter>(ui.signalList, ui.filterEdit, ui.signalCountLabel, ui.domainLabel, m_data);
	graphPresenter = make_unique<GraphPresenter>(ui.plot, m_data, m_plotInfo);
	rtPresenter = make_unique<RealTimePresenter>(m_data, RTMenuInfo{ui.actionStartRT, ui.actionStopRT, ui.actionConfig, ui.actionImport, ui.actionExport, ui.actionClear});

	ScriptManager::InitWorkspace(m_data, m_plotInfo, *ui.console);

	// window -> zoomer
	QObject::connect(ui.actionResetAxes, SIGNAL(triggered()), rectZoomer.get(), SLOT(OnResetZoom()));
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

