#include "SignalInfoPresenter.h"
#include "SignalData.h"
#include <algorithm>

SignalInfoPresenter::SignalInfoPresenter(SignalData& data)
	: data(data)
{
	ui.setupUi(&configDialog);
}

template<typename T>
QString ToString(const QVector<T>& vec)
{
	QString ss;
	for (const auto& elem : vec)
		ss.append(QString("%1 ").arg(elem));
	return ss;
}

QVector<qreal> GetTicks(const QString& vec)
{
	auto strList = vec.split(' ', QString::SkipEmptyParts);
	QVector<qreal> ticks; ticks.reserve(strList.size());
	std::transform(strList.begin(), strList.end(), std::back_inserter(ticks), [](const QString& str){
		return str.toDouble();
	});
	return ticks;
}

QVector<QString> GetTickLabels(const QString& vec)
{
	return vec.split(',', QString::SkipEmptyParts).toVector();
}

void SignalInfoPresenter::Config(const QString& signalName)
{
	const auto& signal = data.get(signalName);
	configDialog.setWindowTitle(QString("Edit Signal Info - %1").arg(signalName));
	ui.spinRangeMin->setValue(signal.graphic.rangeLower);
	ui.spinRangeMax->setValue(signal.graphic.rangeUpper);
	ui.editTicks->setText(ToString(signal.graphic.ticks));

	if (configDialog.exec() == QDialog::Accepted)
	{
		if (ui.spinRangeMin->value() == ui.spinRangeMax->value() && ui.spinRangeMax->value() == 0.0)
			data.setAutoRange(signal.name);
		else
			data.setRange(signal.name, ui.spinRangeMin->value(), ui.spinRangeMax->value());
		data.setTicks(signalName, GetTicks(ui.editTicks->text()));
		data.setTickLabels(signalName, GetTickLabels(ui.editTickLabels->text()));
	}
}
