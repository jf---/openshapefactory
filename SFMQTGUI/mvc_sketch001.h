#ifndef MVC_SKETCH001_H
#define MVC_SKETCH001_H

#include <QWidget>
#include "ui_mvc_sketch001.h"
#include <qocccontroller.h>
#include <commonlibraries.h>

class mvc_sketch001 : public QWidget
{
	Q_OBJECT

public:
	mvc_sketch001(QWidget *parent = 0);
	~mvc_sketch001();

private:
	Ui::mvc_sketch001 ui;

	QoccController* mycontroller;
	Handle_User_AIS aisp;
	

public slots:

	void setup();
	void update();
	void update2();
	void update3();
	void draw(TopoDS_Shape theshape);

};

#endif // MVC_SKETCH001_H
