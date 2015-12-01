#include "SignalListPresenter.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QColorDialog>
#include <QLineEdit>

SignalListPresenter::SignalListPresenter(QTableWidget* signalList, QLineEdit* filterEdit, QLabel* signalCntLabel, QLabel* domainLabel, SignalData& data)
	: signalList(signalList), filterEdit(filterEdit), signalCntLabel(signalCntLabel), domainLabel(domainLabel), data(data)
{
	const auto& tableHFont = signalList->horizontalHeader()->font();
	QFontMetrics fm(tableHFont);
	const auto colorLabelWidth = fm.width(signalList->horizontalHeaderItem(2)->text());
	signalList->setColumnWidth(2, colorLabelWidth + 7);

	// table
	QObject::connect(signalList, &QTableWidget::itemClicked, [this](QTableWidgetItem* item){
		if (item->column() == 0)
		{
			const auto isChecked = item->checkState() == Qt::Checked;
			this->signalList->cellWidget(item->row(), 2)->setEnabled(isChecked);
			this->data.setVisible(item->text(), isChecked);
		}
	});

	// filter
	QObject::connect(filterEdit, SIGNAL(textEdited(QString)), this, SLOT(OnSignalFilterEdited(const QString&)));

	// model
	QObject::connect(&data, SIGNAL(DataAdded(const DataMap&)), this, SLOT(OnNewData(const DataMap&)));
	QObject::connect(&data, SIGNAL(DataCleared()), this, SLOT(OnClearData()));
	QObject::connect(&data, SIGNAL(SignalColorChanged(const Signal&)), this, SLOT(OnSignalColorChanged(const Signal&)));
	QObject::connect(&data, SIGNAL(SignalVisibilityChanged(const Signal&)), this, SLOT(OnSignalVisibilityChanged(const Signal&)));
}

inline QString MakeBackgroundStylesheet(const QColor& color)
{
	return "background-color: " + color.name();
}

void SignalListPresenter::OnNewData(const DataMap& dataMap)
{
	OnClearData();
	auto currentCount = signalList->rowCount();
	signalList->setRowCount(currentCount + dataMap.size());

	for (const auto& sign : dataMap)
	{
		const auto& name = sign.first;
		const auto& currentSignal = sign.second;
		auto chanNameItem = new QTableWidgetItem(name);
		auto chanValueItem = new QTableWidgetItem();
		auto colorButton = new QPushButton(signalList);
		colorButton->setEnabled(currentSignal.visible);
		colorButton->setStyleSheet(MakeBackgroundStylesheet(currentSignal.color));
		signalList->setItem(currentCount, 0, chanNameItem);
		signalList->setItem(currentCount, 1, chanValueItem);
		signalList->setCellWidget(currentCount, 2, colorButton);

		chanNameItem->setCheckState(currentSignal.visible ? Qt::Checked : Qt::Unchecked);
		chanValueItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

		colorButton->setAutoFillBackground(true);
		const auto& currColor = currentSignal.color;
		QObject::connect(colorButton, &QPushButton::clicked, [name, currColor, colorButton, this]{
			const auto color = QColorDialog::getColor(currColor, this->signalList, QString("Change color of %1").arg(name));
			if (color.isValid())
			{
				colorButton->setStyleSheet(MakeBackgroundStylesheet(color));
				this->data.setColor(name, color);
			}
		});
		currentCount++;
	}

	OnSignalFilterEdited(filterEdit->text());
}

void SignalListPresenter::OnClearData()
{
	signalCntLabel->setText("Showing 0 signals of 0");
	signalList->setRowCount(0);
	domainLabel->setText("Domain at cursor: ");
}

template<typename Pred>
void HideElementIf(QTableWidget& list, QLabel& cntLabel, Pred&& f)
{
	auto filterCnt = 0;
	for (auto i = 0; i < list.rowCount(); ++i)
	{
		auto item = list.item(i, 0);
		auto filterRes = f(item);
		filterCnt += !filterRes;
		list.setRowHidden(i, filterRes);
	}
	cntLabel.setText(QString("Showing %1 signals of %2").arg(filterCnt).arg(list.rowCount()));
}

void SignalListPresenter::OnSignalFilterEdited(const QString& filter)
{
	const auto trimmed = filter.trimmed();
	if (trimmed == "[checked]")
	{
		HideElementIf(*signalList, *signalCntLabel, [](QTableWidgetItem* item){
			return item->checkState() != Qt::Checked;
		});
	}
	else if (trimmed == "[unchecked]")
	{
		HideElementIf(*signalList, *signalCntLabel, [](QTableWidgetItem* item){
			return item->checkState() == Qt::Checked;
		});
	}
	else
	{
		QRegExp regExp(filter, Qt::CaseInsensitive, QRegExp::Wildcard);
		HideElementIf(*signalList, *signalCntLabel, [&](QTableWidgetItem* item){
			return !item->text().contains(regExp);
		});
	}
}

void SignalListPresenter::OnSignalVisibilityChanged(const Signal& signal)
{
	auto items = signalList->findItems(signal.name, Qt::MatchExactly);
	items.at(0)->setCheckState(signal.visible ? Qt::Checked : Qt::Unchecked);
	OnSignalFilterEdited(filterEdit->text());
}

void SignalListPresenter::OnSignalColorChanged(const Signal& signal)
{
	auto items = signalList->findItems(signal.name, Qt::MatchExactly);
	signalList->cellWidget(items.at(0)->row(), 2)->setStyleSheet(MakeBackgroundStylesheet(signal.color));

	/*QAbstractItemModel *model = signalList->model();
	QModelIndexList matches = model->match(model->index(0, 0), Qt::UserRole, signal.name);
	for(const auto &index : matches)
	{
	auto colorItem = signalList->cellWidget(index.row(), 2);
	colorItem->setStyleSheet(MakeBackgroundStylesheet(signal.color));
	}*/
}

void SignalListPresenter::OnCursorValueChanged(qreal xVal, size_t idx)
{
	domainLabel->setText(QString("Domain at cursor: %1").arg(xVal));
	auto i = 0;
	data.onSignals([&, this](Signal& signal){
		auto signValueItem = signalList->item(i, 1);
		if (signal.y.size() > idx)
		{
			signValueItem->setText(QString("%1").arg(signal.y[idx]));
		}
		i++;
	});
}
