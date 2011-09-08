#ifndef APDL_MEX_DESIGN_001_H
#define APDL_MEX_DESIGN_001_H

#include <QWidget>
#include "ui_apdl_mex_design_001.h"
#include <commonlibraries.h>

class apdl_mex_design_001 : public QWidget
{
	Q_OBJECT

public:
	apdl_mex_design_001(QWidget *parent = 0);
	~apdl_mex_design_001();

private:
	Ui::apdl_mex_design_001 ui;
	Handle_User_AIS aisp;
	TopoDS_Shape SRF;
	TopoDS_Shape SPIRAL;
	TopoDS_Shape spiralcrv3d;
	QMap<QString,gp_Pnt> pointmap;
	QList<TopoDS_Shape> exportlist;
	QList<TopoDS_Shape> exportlist2;

public slots:
	void setup();
	void update1();
	void update2();
	void build2dgrid();
	void build2dgrid2();
	void build2dgrid3();
	void build2dgrid4();
	void build2dgrid5(); // create grid using euclidean grid
	void BuildPanels();
	void BuildPanels2(); // last working one without thickness
	void BuildPanels2solid();// last working with thickness
	void BuildPanels3(); 
	void importexport();
	void exportstl();


	TopoDS_Shape createpanel(QList<gp_Pnt> &inputlist,QList<TopoDS_Shape> &outputlist);
	TopoDS_Shape createflatpanel(QList<gp_Pnt> &inputlist,QList<TopoDS_Shape> &outputlist);

	void updategrid();
	
};

#endif // APDL_MEX_DESIGN_001_H
