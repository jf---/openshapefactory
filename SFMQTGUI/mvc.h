#ifndef MVC_H
#define MVC_H

#include <QWidget>
#include "ui_mvc.h"

#include <commonlibraries.h>
#include <serial.h>
#include <QTimer>
#include <QProcess>
#include "surface.h"

class mvcPattern;

class mvc : public QWidget
{
	Q_OBJECT

public:
	mvc(QWidget *parent = 0);
	~mvc();

private:
	Ui::mvcClass ui;
	QoccController* mycontroller;
	Handle_AIS_Shape aisp;
	

	QTimer* mytimer;
	Serial* myserial;
	QProcess* arduino;
	surface mySurface;
	



public slots:

	void setup();
	void update();
	void draw(TopoDS_Shape theshape);
	void updatelcd();


};

#endif // MVC_H
