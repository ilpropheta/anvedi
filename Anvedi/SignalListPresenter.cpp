#include "SignalListPresenter.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QColorDialog>

SignalListPresenter::SignalListPresenter(QTableWidget* signalList, QLabel* signalCntLabel)
	: signalList(signalList), signalCntLabel(signalCntLabel)
{
	const auto& tableHFont = signalList->horizontalHeader()->font();
	QFontMetrics fm(tableHFont);
	const auto colorLabelWidth = fm.width(signalList->horizontalHeaderItem(2)->text());
	signalList->setColumnWidth(2, colorLabelWidth + 7);

	QObject::connect(signalList, &QTableWidget::itemClicked, [this](QTableWidgetItem* item){
		if (item->column() == 0)
		{
			if (item->checkState() == Qt::Checked)
			{
				this->signalList->cellWidget(item->row(), 2)->setEnabled(true);
				emit DisplayGraph(item->text());
			}
			else
			{
				this->signalList->cellWidget(item->row(), 2)->setEnabled(false);
				emit HideGraph(item->text());
			}
		}
	});
}

void SignalListPresenter::OnNewData(const DataMap& data)
{
	auto currentCount = signalList->rowCount();
	signalList->setRowCount(currentCount + data.size());

	for (const auto& sign : data)
	{
		const auto& name = sign.first;
		auto chanNameItem = new QTableWidgetItem(name);
		auto chanValueItem = new QTableWidgetItem();
		auto colorButton = new QPushButton();
		colorButton->setEnabled(false);
		colorButton->setStyleSheet(QString("QPushButton {background-color: blue;}"));
		signalList->setItem(currentCount, 0, chanNameItem);
		signalList->setItem(currentCount, 1, chanValueItem);
		signalList->setCellWidget(currentCount, 2, colorButton);

		chanNameItem->setCheckState(Qt::Unchecked);
		chanValueItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

		colorButton->setAutoFillBackground(true);
		QObject::connect(colorButton, &QPushButton::clicked, [name, colorButton, this]{
			auto color = QColorDialog::getColor();
			QString s = "background-color: ";
			colorButton->setStyleSheet(s + color.name());
			emit GraphColorChanged(name, color);
		});
		currentCount++;
	}
}

void SignalListPresenter::OnClearData()
{
	signalCntLabel->setText("Showing 0 signals of 0");
	signalList->setRowCount(0);
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