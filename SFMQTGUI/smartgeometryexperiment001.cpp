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



#include "smartgeometryexperiment001.h"
#include "ui.h"
#include "shapefactory.h"
#include "parametricsfordummies.h"
#include "bash.h"
#include "AIS_OffsetDimension.hxx"
#include "AIS_LengthDimension.hxx"
#include "GeomAPI.hxx"
#include "Geom2d_OffsetCurve.hxx"

SmartGeometryExperiment001::SmartGeometryExperiment001(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setup();
}

SmartGeometryExperiment001::~SmartGeometryExperiment001()
{

}

void SmartGeometryExperiment001::setup()
{
SLIDERMOVED(update)
}

void SmartGeometryExperiment001::update()
{
	INITPART // code starts here

		
	    double crvwidth = getu(crvwidth)
		double crvdepth = getu(crvdepth)
		double srfheight = getu(srfheight)
		int ucount = getu(ucount)
		int vcount = getu(vcount)
		double srfoffset = getu(srfoffset)
		double leanbackratio = getu(leanback)
		double pattractorx = getu(pattractorx)
		double pattractory = getu(pattractory)
		double pattractorz = getu(pattractorz)
		double rangeeffector = getu(rangeeffector);
		double hbangle = getu(hbangle)
		double hbheight = getu(hbheight)
		double diagridratio = get(diagridratio)
		diagridratio = diagridratio /100;
		//end of get values from sliders
		
		double unitdia = tomm(350)
		//hbheight = unitdia * 0.0625;
		//hbheight = hbheight * 2;
		pointdata(pattractor,pattractorx,pattractory,pattractorz) // very comely point
		visgeo(patractorsymbol,Circle(pattractor,gp::DZ(),unitdia/2)) 
		
		//visgeo(fillcircle,FillSurface(patractorsymbol))

		pointdata(p1,0,0,0)
		pointdata(p2,crvwidth/2,crvdepth,0)
		pointdata(p3,crvwidth,0,0)

		pointdata(p4,0,0,srfheight)
		pointdata(p5,crvwidth/2,crvdepth,srfheight)
		pointdata(p6,crvwidth,-leanbackratio,srfheight)

		pointlist(crvlist1)
		addtolist(crvlist1,p1)
		addtolist(crvlist1,p2)
		addtolist(crvlist1,p3)
		visgeo(bottomcrv,SplineSShape(crvlist1))

		   
		
		pointlist(crvlist2)
		addtolist(crvlist2,p4)
		addtolist(crvlist2,p5)
		addtolist(crvlist2,p6)
		visgeo(topcrv,SplineSShape(crvlist2))

		visgeo(fillcircle,BlendSurface(bottomcrv,topcrv)) // this is the surface
		geo(thickelem,ThickSolid(fillcircle,hbheight,0))

		double srfarea = HSF::GetArea(fillcircle);
		ui.surfacearea->display(srfarea);

		double volume = srfarea * 300;
		double cpu = ui.CPU->value();
		double uv = ui.UV->value();
		double cpmm = cpu/uv;
		double cost = cpmm * volume;
		double maxbudget = ui.MB->value();
		double excessbudget = cost-maxbudget;

		ui.projectcost->display(cost);
		ui.overbudget->display(excessbudget);
		

		
		// OCC groove starts here
		TopoDS_Shape toolhb;
		PANELIZE(fillcircle,ucount,vcount) // this is a macro
		PANELOFFSET(srfoffset) // this is a macro

		//diagridratio = inc/panelcount;

		visgeo(normal1,Lineptdir(p1,v1,0,srfoffset))
		geo(diag1,Lineptpt(p1,p3))
		geo(diag2,Lineptpt(op1,op3))
		
		geo(edge1,Lineptpt(p2,p3))
		geo(oedge1,Lineptpt(op2,op3))

		geo(edge2,Lineptpt(p3,p4))
		geo(oedge2,Lineptpt(op3,op4))

		pgeo(midp2,PointonCurve(edge1,diagridratio))
		pgeo(omidp2,PointonCurve(oedge1,diagridratio))
		gp_Vec centervec2(midp2,omidp2);

		pgeo(midp3,PointonCurve(edge2,diagridratio))
		pgeo(omidp3,PointonCurve(oedge2,diagridratio))
		gp_Vec centervec3(midp3,omidp3);
		
		//point attracto will control radius 
		// we need to convert the distance of the panel to the attractor
		//and divide the value by something
		
		//visgeo(hb1,HyberbolicShell(midp1,centervec1,hbangle,radius,hbheight))
		double radius = pattractor.Distance(midp2);
		radius = radius*30;
		radius = radius/(rangeeffector * 100);
		geo(hb2,HyberbolicShell(midp2,centervec2,hbangle,radius,hbheight*1.5))
		toolhb = hb2;
		radius = pattractor.Distance(midp3);
		radius = radius*30;
		radius = radius/(rangeeffector * 100);
		//visgeo(hb3,HyberbolicShell(midp3,centervec3,hbangle,radius,hbheight*1.5))
	
		ENDPANELIZE // groove ends here

		TopoDS_Shape cutshape = HSF::BooleanSubstract(thickelem,toolhb);
		thickelem = cutshape;
		vis(thickelem)
		

	ENDPART // code ends here
}
