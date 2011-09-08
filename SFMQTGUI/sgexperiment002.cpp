#include "sgexperiment002.h"
#include "ui.h"
#include "shapefactory.h"
#include "parametricsfordummies.h"
#include "bash.h"



sgexperiment002::sgexperiment002(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setup();
}

sgexperiment002::~sgexperiment002()
{

}

void sgexperiment002::setup()
{
SLIDERMOVED(update2)
}

void sgexperiment002::update()
{
INITPART // initializes occ
double x = get(x)
double y = get(y)
double z = get(z)
double r1 = get(r1)
double hbangle = get(hbangle)
pointdata(origin,0,0,0)
pointdata(p1,x,y,z)
vectordata(up,0,0,1)
visgeo(circle1,Circle(origin,up,100))
visgeo(circle2,Circle(p1,up,r1))

TopoDS_Shape rotatedcircle = HSF::rotate(circle2,origin,up,hbangle);

shapelist(lineset1)

FOR(i,0,30)
gp_Pnt pcrv1 = HSF::AddNewPointonCurve(circle1,i/30.0);
gp_Pnt pcrv2 = HSF::AddNewPointonCurve(rotatedcircle,i/30.0);

geo(pcrv1vis,Point(pcrv1))
geo(pcrv2vis,Point(pcrv2))
geo(l1,Lineptpt(pcrv1,pcrv2))
addtolist(lineset1,l1)

END

gp_Pnt fp1 = HSF::AddNewPointonCurve(circle1,0);
gp_Pnt fp2 = HSF::AddNewPointonCurve(rotatedcircle,0);
geo(firstline,Lineptpt(fp1,fp2))
addtolist(lineset1,firstline)

geo(loft1,LoftSurface(lineset1))

PANELIZE(loft1,10,10)

geo(spike1,Lineptdir(p1,v1,0,-10))
geo(spike2,Lineptdir(p2,v2,0,-10))
geo(spike3,Lineptdir(p3,v3,0,-10))
geo(spike4,Lineptdir(p4,v4,0,-10))

visgeo(flap1,BlendSurface(spike2,spike3))
visgeo(flap2,BlendSurface(spike3,spike4))
visgeo(flap3,BlendSurface(spike4,spike1))


ENDPANELIZE




ENDPART // finished occ

}




void sgexperiment002::update2()
{
INITPART // initializes occ
double x = get(x)
double y = get(y)
double z = get(z)
double r1 = get(r1)
double hbangle = get(hbangle)

pointdata(hborigin,0,0,0)
vectordata(hbaxis,0,0,1)

visgeo(circle1,Circle(hborigin,hbaxis,r1))

gp_Pnt radiuspoint = HSF::AddNewPointonCurve(circle1,0.0);

gp_Vec outvector(hborigin,radiuspoint);
visgeo(outline, Lineptdir(radiuspoint,hbaxis,-100,100))
TopoDS_Shape rotline = HSF::rotate(outline,radiuspoint,outvector,hbangle);

visgeo(hyperboloid, RevolveSurface(rotline,hborigin,hbaxis))


//PANELIZE(hyperboloid,10,10)
//
//geo(spike1,Lineptdir(p1,v1,0,-10))
//geo(spike2,Lineptdir(p2,v2,0,-10))
//geo(spike3,Lineptdir(p3,v3,0,-10))
//geo(spike4,Lineptdir(p4,v4,0,-10))
//
//visgeo(flap1,BlendSurface(spike2,spike3))
//visgeo(flap2,BlendSurface(spike3,spike4))
//visgeo(flap3,BlendSurface(spike4,spike1))
//
//
//ENDPANELIZE

//Geom_Hyperbola hb;







ENDPART // finished occ

}



