#ifndef BOOLSUBSTRACTGUI_H
#define BOOLSUBSTRACTGUI_H

#include <QWidget>
#include "ui_boolsubstractgui.h"
#include "qoccinputoutput.h"
#include "QoccController.h"
#include "User_AIS.hxx"

class boolsubstractgui : public QWidget
{
	Q_OBJECT

public:
	boolsubstractgui(QWidget *parent = 0);
	~boolsubstractgui();
	QoccInputOutput* io_man;
	QoccController* myController;

private:
	Ui::boolsubstractgui ui;

	double posx;
	double posy;
	double posz;
	double angle;
	double radius;
	double length;

	//hb parameters
	double hb_ucount;
	double hb_vcount;
	double hb_zscale;

	Handle(User_AIS) aisShape;
	Handle(User_AIS) hb_AISShape;
	Handle(AIS_Shape) stockais;
	Handle(AIS_Shape) hbpreviewais;

public slots:
	void executeboolean1();
	void executeboolean2();
	void executeboolean3();
	void executeboolean4();
	void executeboolean5();
	void executeboolean6();
	void executeboolean7();
	void executeboolean8();
	void executeboolean9();
	void executeSphereRandom1();
	void executeSphereRandom1optimized();
	void executesplittripod();
	void executestadium();
	void setvalues();
	void deleteallgeo();


};

#endif // BOOLSUBSTRACTGUI_H
