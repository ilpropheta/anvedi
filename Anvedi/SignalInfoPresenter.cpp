#include "SignalInfoPresenter.h"
#include "SignalData.h"

SignalInfoPresenter::SignalInfoPresenter(SignalData& data)
	: data(data)
{
	ui.setupUi(&configDialog);
}

void SignalInfoPresenter::Config(const QString& signalName)
{
	const auto& signal = data.get(signalName);
	ui.spinRangeMin->setValue(signal.graphic.rangeLower);
	ui.spinRangeMax->setValue(signal.graphic.rangeUpper);
	if (configDialog.exec() == QDialog::Accepted)
	{
		data.setRange(signal.name, ui.spinRangeMin->value(), ui.spinRangeMax->value());
	}
}
