#ifndef SGEXPERIMENT002_H
#define SGEXPERIMENT002_H

#include <QWidget>
#include "ui_sgexperiment002.h"
#include <QMap>
#include <commonlibraries.h>
#include <QSet>

class sgexperiment002 : public QWidget
{
	Q_OBJECT

public:
	sgexperiment002(QWidget *parent = 0);
	~sgexperiment002();

private:
	Ui::sgexperiment002 ui;
	Handle_User_AIS aisp;
public slots:

	void setup();
	void update();
	void update2();
};

#endif // SGEXPERIMENT002_H
