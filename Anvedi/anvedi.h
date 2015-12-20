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
#include "RectZoomer.h"
#include "PlotInfo.h"

class Anvedi : public QMainWindow
{
	Q_OBJECT
public:
	Anvedi(QWidget *parent = 0);
public slots:
	void OnExit();
	void OnDataImport();
	void OnDataExport();
	void OnDataClear();
protected:
	void keyPressEvent(QKeyEvent * e);
private:
	Ui::AnvediClass ui;

	std::unique_ptr<PlotCursor> cursor;
	std::unique_ptr<RectZoomer> rectZoomer;
	std::unique_ptr<SignalListPresenter> signalListPresenter;
	std::unique_ptr<GraphPresenter> graphPresenter;

	SignalData m_data;
	PlotInfo m_plotInfo;
};

#endif // ANVEDI_H
