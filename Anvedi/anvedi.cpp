#include "anvedi.h"

using namespace std;

Anvedi::Anvedi(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	cursor = make_unique<PlotCursor>(ui.plot, 0.01);
	QObject::connect(cursor.get(), SIGNAL(CursorChanged(qreal)), this, SLOT(OnCursorChanged(qreal)));
	ui.console->SetEngine(std::make_shared<QShellEngine_Qt>());

	signalListPresenter = make_unique<SignalListPresenter>(ui.signalList, ui.signalCountLabel);
	graphPresenter = make_unique<GraphPresenter>(ui.plot);

	// list -> graph
	QObject::connect(signalListPresenter.get(), SIGNAL(GraphColorChanged(const QString&, const QColor&)), graphPresenter.get(), SLOT(OnGraphColorChanged(const QString&, const QColor&)));
	QObject::connect(signalListPresenter.get(), SIGNAL(DisplayGraph(const QString&)), graphPresenter.get(), SLOT(OnSignalAdded(const QString&)));
	QObject::connect(signalListPresenter.get(), SIGNAL(HideGraph(const QString&)), graphPresenter.get(), SLOT(OnSignalRemoved(const QString&)));

	// main controller -> list
	QObject::connect(this, SIGNAL(NewData(const DataMap&)), signalListPresenter.get(), SLOT(OnNewData(const DataMap&)));
	QObject::connect(this, &Anvedi::NewData, [this](){
		emit ui.filterEdit->textEdited(ui.filterEdit->text());
	});
	QObject::connect(ui.filterEdit, SIGNAL(textEdited(QString)), signalListPresenter.get(), SLOT(OnSignalFilterEdited(const QString&)));
	QObject::connect(this, SIGNAL(DataCleared()), signalListPresenter.get(), SLOT(OnClearData()));

	// main controller -> graph
	QObject::connect(this, SIGNAL(NewData(const DataMap&)), graphPresenter.get(), SLOT(OnNewData(const DataMap&)));
	QObject::connect(this, SIGNAL(DataCleared()), graphPresenter.get(), SLOT(OnClearData()));
}

void Anvedi::AddGraph(const QString& name, const QVector<qreal>& x, const QVector<qreal>& y)
{
	emit GraphAdded(name);
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

	emit NewData({ { "parabola", { x, y } } });
	emit NewData({ { "pippo", { x1, y1 } } });
}

void Anvedi::OnDataClear()
{
	emit DataCleared();
}

// to move
void Anvedi::OnCursorChanged(qreal xVal)
{
	auto plot = ui.plot;
	const auto graphCount = plot->graphCount();

	for (auto i = 0; i < graphCount; ++i)
	{
		auto cursorValue = plot->graph(i)->data()->lowerBound(xVal)->value;
		auto channelVal = ui.signalList->item(i, 1);
		channelVal->setText(QString("%1").arg(cursorValue));
	}
}
