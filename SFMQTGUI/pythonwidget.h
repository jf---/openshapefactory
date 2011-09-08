#ifndef PYTHONWIDGET_H
#define PYTHONWIDGET_H

#include <QWidget>
#include "ui_pythonwidget.h"
#include <commonlibraries.h>
#include "PythonQt.h"
#include "ui.h"

class pythonwidget : public QWidget
{
	Q_OBJECT

public:
	pythonwidget(QWidget *parent = 0);
	~pythonwidget();

private:
	Ui::pythonwidget ui;
	Handle_User_AIS aisp;


public slots:
	void setup();
	void update();
};

#endif // PYTHONWIDGET_H
