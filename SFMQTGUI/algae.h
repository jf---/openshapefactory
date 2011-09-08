#ifndef ALGAE_H
#define ALGAE_H

#include <QWidget>
#include "ui_algae.h"
#include <commonlibraries.h>

class algae : public QWidget
{
	Q_OBJECT

public:
	algae(QWidget *parent = 0);
	~algae();
	TopoDS_Shape face;

private:
	Ui::algae ui;
	Handle_User_AIS aisp;


public slots:
	void setup();
	void update();
	void buildveins();
	void buildveins2();

	gp_Pnt minpoint(TopoDS_Shape srf, gp_Pnt startp, double rad);
	gp_Pnt minpoint(TopoDS_Shape srf, gp_Pnt startp,gp_Pnt prevp, double rad,TopoDS_Shape &vis);
};

#endif // ALGAE_H
