#ifndef KILIANROOF_H
#define KILIANROOF_H

#include <QWidget>
#include "ui_kilianroof.h"
#include <commonlibraries.h>
#include <MeshVS_Mesh.hxx>
#include <AIS_Gauss.hxx>


  struct panelinstance {
        int panelindex;
        QList<double> parameters;
		double distancetocenter;
		int centerindex;
		QList<QList<gp_Pnt>> triangles;
    };


class kilianroof : public QWidget
{
	Q_OBJECT

public:
	kilianroof(QWidget *parent = 0);
	~kilianroof();

private:
	Ui::kilianroof ui;
	Handle_User_AIS aisp;
	Handle( MeshVS_Mesh  ) aMesh;
	TopoDS_Shape mastersurface;
	QList<panelinstance> panelarray;
	TopoDS_Shape lastfolder;
	Handle(AIS_Gauss) agauss;

	int prevucount;
	int prevvcount;
	bool prevareachk;
	bool prevlengthschk;
	bool prevangleschk;

public slots:

	void setup();
	void update(int vistate);
	void updatemesh(int vistate);
	void loadsoumaya(int vistate);

	void updatemove();
	void updatereleased();
	void breakshape();
	void buildgaussiantopo();
	

	TopoDS_Shape makesplinesection(TopoDS_Shape edge, double percent, double width, double height);
};

#endif // KILIANROOF_H
