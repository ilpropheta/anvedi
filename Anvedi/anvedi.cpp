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
}

void Anvedi::OnExit()
{
	QApplication::exit();
}

void Anvedi::OnDataImport()
{
	//auto files = QFileDialog::getOpenFileNames(this, "Import files", ".", "*.*");

	//QVector<double> x(101), y(101), y1(101);
	//for (int i = 0; i < 101; ++i)
	//{
	//	x[i] = i / 50.0 - 1; // x goes from -1 to 1
	//	y[i] = x[i] * x[i]; // let's plot a quadratic function
	//}

	//for (int i = 0; i < 101; ++i)
	//{
	//	y1[i] = x[i] * x[i] * x[i]; // let's plot a quadratic function
	//}

	//m_data.add({
	//	{ "parabola", Signal{ "parabola", Qt::blue, true, y } },
	//	{ "pippo", Signal{ "pippo", Qt::red, false, y1 } },
	//	{ "line", Signal{ "line", Qt::red, false, x } }
	//});

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
