#ifndef BIRDSNEST_H
#define BIRDSNEST_H

#include <QWidget>
#include "ui_birdsnest.h"
#include <commonlibraries.h>
#include <QTimer>
#include <QProcess>

class birdsnest : public QWidget
{
	Q_OBJECT

public:
	birdsnest(QWidget *parent = 0);
	~birdsnest();

private:
	Ui::birdsnest ui;
	//Handle_User_AIS aisp;
	Handle_AIS_Shape aisp;
	QTimer* mytimer;
	QProcess* arduino;

public slots:


	void setup();
	void update();
	void update2();

	void updatelcd();

};

#endif // BIRDSNEST_H
