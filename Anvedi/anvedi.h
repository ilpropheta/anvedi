#ifndef ANVEDI_H
#define ANVEDI_H

#include <QtWidgets/QMainWindow>
#include "ui_anvedi.h"
#include "PlotCursor.h"
#include <memory>

class Anvedi : public QMainWindow
{
	Q_OBJECT
public:
	Anvedi(QWidget *parent = 0);
public slots:
	void OnExit();
	void OnDataImport();
	void OnDataClear();
	void OnCursorChanged(qreal);
signals:
	void GraphColorChanged(int);
private:
	void AddGraph(const QString& name, const QVector<qreal>& x, const QVector<qreal>& y);
	
	Ui::AnvediClass ui;

	// graphic
	std::unique_ptr<PlotCursor> cursor;
};

#endif // ANVEDI_H
