#ifndef ANVEDI_H
#define ANVEDI_H

#include <QtWidgets/QMainWindow>
#include "ui_anvedi.h"
#include "PlotCursor.h"
#include <memory>
#include "SignalListPresenter.h"
#include "GraphPresenter.h"
#include <map>
#include "SignalData.h"
#include "ScriptManager.h"
#include "RectZoomer.h"

class Anvedi : public QMainWindow
{
	Q_OBJECT
public:
	Anvedi(QWidget *parent = 0);
	public slots:
	void OnExit();
	void OnDataImport();
	void OnDataClear();
private:
	Ui::AnvediClass ui;

	std::unique_ptr<PlotCursor> cursor;
	std::unique_ptr<RectZoomer> rectZoomer;
	std::unique_ptr<SignalListPresenter> signalListPresenter;
	std::unique_ptr<GraphPresenter> graphPresenter;

	std::unique_ptr<ScriptManager> scriptManager;

	SignalData m_data; // model
};

#endif // ANVEDI_H
