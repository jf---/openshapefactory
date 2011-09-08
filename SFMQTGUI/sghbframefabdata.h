/************************************************************************************
**
** This file is part of the Flexible Digital Modeller
**
** Copyright (C) 2010,2011 Alexander Peña de Leon
**
**This work is licensed under the Creative Commons 
**Attribution-NonCommercial-ShareAlike 3.0 Australia License. 
**To view a copy of this license, 
**visit http://creativecommons.org/licenses/by-nc-sa/3.0/au/ 
**or send a letter to Creative Commons, 171 Second Street, 
**Suite 300, San Francisco, California, 94105, USA.
**
***********************************************************************************/



#ifndef SGHBFRAMEFABDATA_H
#define SGHBFRAMEFABDATA_H

#include <QWidget>
#include "ui_sghbframefabdata.h"

#include <QMap>
#include <commonlibraries.h>
#include <QSet>

	struct hbdata{
		 int Panel_id;
		 int Panel_u ;
         int Panel_v ;
         int Panel_type ;
         double Panel_x ;
         double Panel_y ;
         double Panel_z ;
         double Panel_normalxdir ;
         double Panel_normalydir ;
         double Panel_normalzdir ;
         int Panel_planecount;
		 QList<int> topology;
	     };

	struct pointlink{
		 int edge1;
		 int edge2;
         };
	struct edgelink{
		 int p1;
		 int p2;
         };

class sghbframefabdata : public QWidget
{
		Q_OBJECT

		public:


	sghbframefabdata(QWidget *parent = 0);
	~sghbframefabdata();

private:
	Ui::sghbframefabdata ui;
	Handle_User_AIS aisp;
	QList<TopoDS_Shape> hyperbolicfamilies;
	QMap<int,hbdata> data;
	QMap<QString,gp_Pnt> panelpoints;
	QMap<QString,gp_Dir> panelvecs;
	QMap<int,TopoDS_Shape> hyperbolas;
	QMap<int,QList<gp_Pln>> intplanes;
	QMap<int,QList<TopoDS_Shape>> intcrvs;
	QMap<int,QList<int>> topology;
	QMap<int,TopoDS_Shape> trimmedhyperbolas;
	TopoDS_Shape backsurface;

public slots:

	void setup();
	void update();

	void loadhyperbolafamilies();
	void loaddatafile();
	void createhyperbolas();
	void gentopology();
	void savetopology();
	void loadtopology();
	void splithyberbolas();
	void generateplanes();
	void buildedgemap();
	void buildflaps();

	//void sortplanes();
	//void generateedges();
	//void generateintersectionpoints();
	//void sortintersectionpoints();
	
	};
	

#endif // SGHBFRAMEFABDATA_H
