#include "anvedi.h"

using namespace std;

Anvedi::Anvedi(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	cursor = make_unique<PlotCursor>(ui.plot, 0.01);
	QObject::connect(cursor.get(), SIGNAL(CursorChanged(qreal)), this, SLOT(OnCursorChanged(qreal)));
	ui.console->SetEngine(std::make_shared<QShellEngine_Qt>());

	const auto& tableHFont = ui.signalList->horizontalHeader()->font();
	QFontMetrics fm(tableHFont);
	auto colorLabelWidth = fm.width(ui.signalList->horizontalHeaderItem(2)->text());
	ui.signalList->setColumnWidth(2, colorLabelWidth + 7);
	
	QVector<double> x(101), y(101); // initialize with entries 0..100
	for (int i = 0; i < 101; ++i)
	{
		x[i] = i / 50.0 - 1; // x goes from -1 to 1
		y[i] = x[i] * x[i]; // let's plot a quadratic function
	}
	AddGraph("parabola", x, y);
}

void Anvedi::AddGraph(const QString& name, const QVector<qreal>& x, const QVector<qreal>& y)
{
	auto plot = ui.plot;
	auto myY = plot->axisRect(0)->addAxis(QCPAxis::atLeft);
	myY->setVisible(false);
	auto graph = plot->addGraph(plot->xAxis, myY);
	plot->graph(0)->setPen(QPen(Qt::blue));
	graph->setData(x, y);
	graph->rescaleAxes();
	plot->replot();

	auto signalList = ui.signalList;
	const auto currentCount = signalList->rowCount();
	signalList->setRowCount(currentCount + 1);
	auto chanNameItem = new QTableWidgetItem(name);
	chanNameItem->setCheckState(Qt::Unchecked);
	auto chanValueItem = new QTableWidgetItem();
	chanValueItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	auto colorButton = new QPushButton();
	colorButton->setStyleSheet(QString("QPushButton {background-color: blue;}"));
	signalList->setItem(currentCount, 0, chanNameItem);
	signalList->setItem(currentCount, 1, chanValueItem);
	signalList->setCellWidget(currentCount, 2, colorButton);
	colorButton->setAutoFillBackground(true);
	QObject::connect(colorButton, &QPushButton::clicked, [colorButton, currentCount, this]{
		emit GraphColorChanged(currentCount);
		//auto newColor = OnActionChangeColor(i - 1);
		//QString s = "background-color: ";
		//colorButton->setStyleSheet(s + newColor.name());
	});
}

void Anvedi::OnExit()
{
	QApplication::exit();
}

void Anvedi::OnDataImport()
{
	auto files = QFileDialog::getOpenFileNames(this, "Import files", ".", "*.*");

}

void Anvedi::OnDataClear()
{
	ui.plot->clearGraphs();
	ui.plot->replot();
}

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
