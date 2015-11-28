#ifndef ANVEDI_H
#define ANVEDI_H

#include <QtWidgets/QMainWindow>
#include "ui_anvedi.h"

class Anvedi : public QMainWindow
{
	Q_OBJECT

public:
	Anvedi(QWidget *parent = 0);
	~Anvedi();

private:
	Ui::AnvediClass ui;
};

#endif // ANVEDI_H
