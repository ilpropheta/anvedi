#include "anvedi.h"
#include <QtScript\QScriptContext>
#include "SignalHandle.h"

using namespace std;

Anvedi::Anvedi(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	cursor = make_unique<PlotCursor>(ui.plot, 0.01);
	scriptManager = make_unique<ScriptManager>(m_data, *ui.console);
	signalListPresenter = make_unique<SignalListPresenter>(ui.signalList, ui.filterEdit, ui.signalCountLabel, ui.domainLabel, m_data);
	graphPresenter = make_unique<GraphPresenter>(ui.plot);

	// cursor -> list
	QObject::connect(cursor.get(), SIGNAL(CursorChanged(qreal, size_t)), signalListPresenter.get(), SLOT(OnCursorValueChanged(qreal, size_t)));
	
	// model -> filter
	QObject::connect(&m_data, &SignalData::DataAdded, [this]{
		emit ui.filterEdit->textEdited(ui.filterEdit->text());
	});

	// model -> graph
	QObject::connect(&m_data, SIGNAL(DataAdded(const DataMap&)), graphPresenter.get(), SLOT(OnNewData(const DataMap&)));
	QObject::connect(&m_data, SIGNAL(DataCleared()), graphPresenter.get(), SLOT(OnClearData()));
	QObject::connect(&m_data, SIGNAL(SignalColorChanged(const Signal&)), graphPresenter.get(), SLOT(OnGraphColorChanged(const Signal&)));
	QObject::connect(&m_data, SIGNAL(SignalVisibilityChanged(const Signal&)), graphPresenter.get(), SLOT(OnGraphVisibilityChanged(const Signal&)));
	QObject::connect(&m_data, SIGNAL(SignalChanged(const Signal&)), graphPresenter.get(), SLOT(OnGraphDataChanged(const Signal&)));
}

void Anvedi::OnExit()
{
	QApplication::exit();
}

void Anvedi::OnDataImport()
{
	//auto files = QFileDialog::getOpenFileNames(this, "Import files", ".", "*.*");

	QVector<double> x(101), y(101), x1(101), y1(101);
	for (int i = 0; i < 101; ++i)
	{
		x[i] = i / 50.0 - 1; // x goes from -1 to 1
		y[i] = x[i] * x[i]; // let's plot a quadratic function
	}

	for (int i = 0; i < 101; ++i)
	{
		x1[i] = i / 50.0 - 1; // x goes from -1 to 1
		y1[i] = x1[i] * x1[i] * x1[i]; // let's plot a quadratic function
	}

	m_data.add({
		{ "parabola", Signal{ "parabola", Qt::blue, true, x, y } },
		{ "pippo", Signal{ "pippo", Qt::red, false, x1, y1 } }
	});

	cursor->reset();
}

void Anvedi::OnDataClear()
{
	m_data.clear();
	scriptManager->InitWorkspace(m_data, *ui.console);
	cursor->reset();
}
