#ifndef BIARC_H
#define BIARC_H

#include <QWidget>
#include "ui_biarc.h"
#include <commonlibraries.h>

class biarc : public QWidget
{
	Q_OBJECT

public:
	biarc(QWidget *parent = 0);
	~biarc();

private:
	Ui::biarc ui;
		Handle_User_AIS aisp;

		TopoDS_Shape EOS;


public slots:
	void setup();
	void update();
	void update2();
	TopoDS_Shape lineptarc(gp_Pnt p1,gp_Pnt p2,gp_Vec t1,double maxrad);
	QList<TopoDS_Shape> biarcjunctiondomain(gp_Pnt p1,gp_Pnt p2,gp_Vec t1,gp_Vec t2, double arcratio);
	QList<TopoDS_Shape> biarcptdir(gp_Pnt p1,gp_Pnt p2,gp_Vec t1,gp_Vec t2, double arcratio,double infinite);
	QList<TopoDS_Shape> biarconcurve(TopoDS_Shape crv, double ratio1,double ratio2,double arcratio,double infinite);

	TopoDS_Shape biarcoptimized(TopoDS_Shape crv, double ratio1,double ratio2,double percentinf,double percentsup,double infinite);
	
};

#endif // BIARC_H
