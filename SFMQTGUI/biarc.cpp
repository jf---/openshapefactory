#include "biarc.h"
#include "ui.h"
#include "shapefactory.h"
#include "parametricsfordummies.h"
#include "bash.h"
#include <QFileDialog>
#include <QProgressDialog>
#include <QTime>
#include <QDebug>



biarc::biarc(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setup();
}

biarc::~biarc()
{

}

void biarc::setup()
{
SLIDERRELEASED(update2)
}

void biarc::update()
{
INITPART

pointlist(splineset)
pointdata(p1,getdval(x1),getdval(y1),getdval(z1))
pointdata(p2,getdval(x2),getdval(y2),getdval(z2))
pointdata(p3,getdval(x3),getdval(y3),getdval(z3))
pointdata(p4,getdval(x4),getdval(y4),getdval(z4))
pointdata(p5,getdval(x5),getdval(y5),getdval(z5))

visgeo(point1,SphereSurface(p1,1))
visgeo(point2,SphereSurface(p2,1))
visgeo(point3,SphereSurface(p3,1))
visgeo(point4,SphereSurface(p4,1))
visgeo(point5,SphereSurface(p5,1))

splineset << p1 << p2 << p3 << p4 << p5;
TopoDS_Shape spline = hsf::AddNewSplineSShape(splineset);
vis(spline)

FOR(i,1,100)
gp_Pnt pcrv = hsf::AddNewPointonCurve(spline,(double)i/100);
gp_Pnt pcrv2 = pcrv;
pcrv2.SetZ(0);

if(pcrv.Distance(pcrv2))
	{
	TopoDS_Shape l1 = hsf::AddNewLineptpt(pcrv2,pcrv);
	vis(l1)
	}

	visgeo(pointcrv,Point(pcrv))
	visgeo(pointcrv2,Point(pcrv2))
END

ENDPART



}

void biarc::update2()
{
INITPART

if(EOS.IsNull())

{
QoccInputOutput* io_man = new QoccInputOutput();
QString filename = QFileDialog::getOpenFileName ( this,
							 tr("Import File"),"");

if (!(filename.length() > 0)) return; 
Handle(TopTools_HSequenceOfShape) famshape1 = io_man->importIGES(filename);
EOS = famshape1->Value(1);



}

if (EOS.IsNull()) return;





//double scale = 200;
//QList<gp_Pnt> splineset;
//gp_Pnt p1(getdval(x1) * scale,getdval(y1)* scale,getdval(z1)* scale);
//gp_Pnt p2(getdval(x2) * scale,getdval(y2) * scale,getdval(z2) * scale);
//gp_Pnt p3(getdval(x3) * scale,getdval(y3)* scale,getdval(z3) * scale);
//gp_Pnt p4(getdval(x4) * scale,getdval(y4) * scale,getdval(z4) * scale);
//gp_Pnt p5(getdval(x5) * scale,getdval(y5) * scale,getdval(z5) * scale);
//
////visgeo(point1,SphereSurface(p1,1))
////visgeo(point2,SphereSurface(p2,1))
////visgeo(point3,SphereSurface(p3,1))
////visgeo(point4,SphereSurface(p4,1))
////visgeo(point5,SphereSurface(p5,1))
//
//splineset << p1 << p2 << p3 << p4 << p5;
//TopoDS_Shape spline = hsf::AddNewSplineSShape(splineset);
////vis(spline)
//double k = getdval(k);


QList<TopoDS_Shape> facelist;
	QMap<QString,TopoDS_Shape> facemap;
	
	int count = 0;
	TopoDS_Shape prevface;

	TopExp_Explorer EX;
	TopExp_Explorer EX2;
	for (EX.Init(EOS,TopAbs_FACE); EX.More(); EX.Next())
	{
	count++;
	TopoDS_Face aF1 = TopoDS::Face(EX.Current());
	gp_Pnt cog = hsf::GetCOG(aF1);
	QString facename = QString("F") + QString::number(count);
	//hsf::AddNewAnnotation(facename,cog);

	
	if ( count > 1)
	{
		QList<TopoDS_Shape> edgelist1 = hsf::getshapelist(aF1,TopAbs_EDGE);
		//QList<TopoDS_Shape> edgelist2 = hsf::getshapelist(prevface,TopAbs_EDGE);
		
		TopoDS_Shape edge1 = edgelist1.at(0);
		TopoDS_Shape edge2 = edgelist1.at(1);
		TopoDS_Shape edge3 = edgelist1.at(2);
		TopoDS_Shape edge4 = edgelist1.at(3);

		//vis(edge2);
		//vis(edge4);
		gp_Vec v1 = hsf::getVectorTangentToCurveAtPoint(edge1,0);
		gp_Vec v2 = hsf::getVectorTangentToCurveAtPoint(edge1,1);

		gp_Pnt p1 = hsf::AddNewPointonCurve(edge1,0);
		gp_Pnt p2 = hsf::AddNewPointonCurve(edge1,1);
		TopoDS_Shape lin1 = hsf::AddNewLineptpt(p1,p2);
		
		gp_Pnt midp1 = hsf::AddNewPointonCurve(edge1,0.5);
		gp_Pnt midp2 = hsf::AddNewPointonCurve(lin1,0.5);

		if(midp1.Distance(midp2) > Precision::Confusion())
		{

		TopoDS_Shape biarc1 = biarcoptimized(edge1,0.00001,0.99999,0.00001,0.99999,0);

		//TopoDS_Shape biarc2 = hsf::biarcoptimized(edge2,0.01,0.99,0.01,0.99,0);
		vis(biarc1)

		}
		//vis(edge2)
		//TopoDS_Shape prevf1 = edgelist.at(0);
	}

	//facelist << aF1;

	//prevface = aF1;
	}




	ENDPART


//
//double inf=0;
//double sup=0;
//double count = MAX(10,k);
//for(int i =1;i<count+1; i++){
//	sup = inf + ((double)1/count);
//	TopoDS_Shape bopti = biarcoptimized(EOS, inf,sup,0.05,0.95,3000);
//	vis(bopti)
//	inf = sup;
//}
//
//vis(EOS)
//
//ENDPART
}


TopoDS_Shape biarc::lineptarc(gp_Pnt p1,gp_Pnt p2,gp_Vec t1,double maxrad)
{

gp_Pnt sp = p1;
gp_Pnt ep = p2;

gp_Pnt t1p = hsf::MovePointByVector(sp,t1,1);
gp_Vec t2(p1,p2);

gp_Pln Spln(sp,t1);
gp_Pln Epln(ep,t2);

gp_Pln pln1 = hsf::AddNewPlane(sp,t1p,ep);
TopoDS_Shape axis = hsf::AddNewIntersectSrf(Spln,Epln);

gp_Pnt o1 = hsf::AddNewIntersectCrvPln(axis,pln1);
TopoDS_Shape circ1 = hsf::AddNewCircle(sp,ep,o1);
Handle(Geom_Curve) circ1crv = hsf::convertshapetocurve(circ1);
TopoDS_Shape arc1 = BRepBuilderAPI_MakeEdge(circ1crv,sp,ep);

return arc1;
}

QList<TopoDS_Shape> biarc::biarcjunctiondomain(gp_Pnt p1,gp_Pnt p2,gp_Vec t1,gp_Vec t2, double arcratio)
{

QList<TopoDS_Shape>  outputs;
gp_Pnt sp = p1;
gp_Pnt ep = p2;

gp_Pnt t1p = hsf::MovePointByVector(sp,t1,1);
//gp_Vec t2(p1,p2);



gp_Pln Spln(sp,t1);
gp_Pln Epln(ep,t2);



gp_Pln pln1 = hsf::AddNewPlane(sp,t1p,ep);

TopoDS_Shape axis = hsf::AddNewIntersectSrf(Spln,Epln);
if ( axis.IsNull()) 
	{
	qDebug() << "axis is null";
	return outputs;
	}

gp_Vec axisvec = hsf::getVectorTangentToCurveAtPoint(axis,0.5);
gp_Pnt o1 = hsf::AddNewIntersectCrvPln(axis,pln1);

gp_Vec SVec(Spln.Axis().Direction());
gp_Vec EVec(Epln.Axis().Direction());

double angleoriented = (3.14159265 -SVec.AngleWithRef(EVec,axisvec))/2;

gp_Pln rotpln1 = Spln.Rotated(gp_Ax1(o1,axisvec),angleoriented);
//gp_Pln rotpln2 = rotpln1.Rotated(gp_Ax1(o1,axisvec),90); //rotate 90 degs in radians

gp_Pln rotpln2 = rotpln1.Rotated(gp_Ax1(o1,axisvec),1.57079633); //rotate 90 degs in radians


gp_Pnt p2mirror1 = ep.Mirrored(gp_Ax2(rotpln1.Location(),rotpln1.Axis().Direction(),rotpln1.XAxis().Direction()));
gp_Pnt p2mirror2 = ep.Mirrored(gp_Ax2(rotpln2.Location(),rotpln2.Axis().Direction(),rotpln2.XAxis().Direction()));
gp_Pnt farpoint;



double dis1 = p1.Distance(p2mirror1);
double dis2 = p1.Distance(p2mirror2);

if(dis1 > dis2){farpoint = p2mirror1;} else {farpoint = p2mirror2;}


TopoDS_Shape circ1 = hsf::AddNewCircle(ep,sp,farpoint);
Handle(Geom_Curve) circ1crv = hsf::convertshapetocurve(circ1);
TopoDS_Shape arc1 = BRepBuilderAPI_MakeEdge(circ1crv,ep,sp);

TopoDS_Shape point2m1 = hsf::AddNewPoint(p2mirror1);
TopoDS_Shape point2m2 = hsf::AddNewPoint(p2mirror2);
outputs << arc1 << point2m1 << point2m2 <<axis;



return outputs;

	
	
}

//get a smaller biarc between the range ratio1-ratio2
QList<TopoDS_Shape> biarc::biarconcurve(TopoDS_Shape crv, double ratio1,double ratio2,double arcratio,double infinite)
{

gp_Pnt p1 = hsf::AddNewPointonCurve(crv,ratio1);
gp_Pnt p2 = hsf::AddNewPointonCurve(crv,ratio2);

gp_Vec t1 = hsf::getVectorTangentToCurveAtPoint(crv,ratio1);
gp_Vec t2 = hsf::getVectorTangentToCurveAtPoint(crv,ratio2);

QList<TopoDS_Shape> biarc = biarcptdir(p1,p2,t1,t2,arcratio,infinite);
return biarc;


}

QList<TopoDS_Shape> biarc::biarcptdir(gp_Pnt p1,gp_Pnt p2,gp_Vec t1,gp_Vec t2, double arcratio,double infinite)
	{

QList<TopoDS_Shape> returndata;
QList<TopoDS_Shape> domain = biarcjunctiondomain(p1,p2,t1,t2,arcratio);
if (!(domain.count() > 0)) return returndata; 
gp_Pnt j = hsf::AddNewPointonCurve(domain.at(0),arcratio);

TopoDS_Shape biarc1 = lineptarc(p1,j,t1,infinite);
TopoDS_Shape biarc2 = lineptarc(p2,j,t2,infinite);

returndata << biarc1 << biarc2 << domain.at(0) << domain.at(1) << domain.at(2);
return returndata;


	}

//build 10 biarcs and get closest one to the crv.
TopoDS_Shape biarc::biarcoptimized(TopoDS_Shape crv, double ratio1,double ratio2,double percentinf,double percentsup,double infinite)
	{

INITPART
double r1 = percentinf;
double r2 = percentinf + (percentsup-percentinf)/9;
double r3 = percentinf + (percentsup-percentinf)* 2/9;
double r4 = percentinf + (percentsup-percentinf)* 3/9;
double r5 = percentinf + (percentsup-percentinf)* 4/9;
double r6 = percentinf + (percentsup-percentinf)* 5/9;
double r7 = percentinf + (percentsup-percentinf)* 6/9;
double r8 = percentinf + (percentsup-percentinf)* 7/9;
double r9 = percentinf + (percentsup-percentinf)* 8/9;
double r10 = percentsup;

QList<TopoDS_Shape> biarc1,biarc2,biarc3,biarc4,biarc5,biarc6,biarc7,biarc8,biarc9,biarc10;

try
{
biarc1 = biarconcurve(crv, ratio1, ratio2,r1, infinite);
biarc2 = biarconcurve(crv, ratio1, ratio2,r2, infinite);
biarc3 = biarconcurve(crv, ratio1, ratio2,r3, infinite);
biarc4 = biarconcurve(crv, ratio1, ratio2,r4, infinite);
biarc5 = biarconcurve(crv, ratio1, ratio2,r5, infinite);
biarc6 = biarconcurve(crv, ratio1, ratio2,r6, infinite);
biarc7 = biarconcurve(crv, ratio1, ratio2,r7, infinite);
biarc8 = biarconcurve(crv, ratio1, ratio2,r8, infinite);
biarc9 = biarconcurve(crv, ratio1, ratio2,r9, infinite);
biarc10 = biarconcurve(crv, ratio1, ratio2,r10, infinite);

} catch(...)
{
	return folder;
}



int count = biarc1.length();
//vis(biarc1.at(0)) vis(biarc1.at(1)) vis(biarc1.at(2)) vis(biarc1.at(3)) vis(biarc1.at(4))
//vis(biarc2.at(0)) vis(biarc2.at(1)) vis(biarc2.at(2)) vis(biarc2.at(3))
//vis(biarc3.at(0)) vis(biarc3.at(1)) vis(biarc3.at(2)) vis(biarc3.at(3))
//vis(biarc4.at(0)) vis(biarc4.at(1)) vis(biarc4.at(2)) vis(biarc4.at(3))
//vis(biarc5.at(0)) vis(biarc5.at(1)) vis(biarc5.at(2)) vis(biarc5.at(3))
//vis(biarc6.at(0)) vis(biarc6.at(1)) vis(biarc6.at(2)) vis(biarc6.at(3))
//vis(biarc7.at(0)) vis(biarc7.at(1)) vis(biarc7.at(2)) vis(biarc7.at(3))
//vis(biarc8.at(0)) vis(biarc8.at(1)) vis(biarc8.at(2)) vis(biarc8.at(3))
//vis(biarc9.at(0)) vis(biarc9.at(1)) vis(biarc9.at(2)) vis(biarc9.at(3))
//vis(biarc10.at(0)) vis(biarc10.at(1)) vis(biarc10.at(2)) vis(biarc10.at(3))
double dis1a ;
double dis2a ;
double dis3a ;
double dis4a ;
double dis5a ;
double dis6a ;
double dis7a ;
double dis8a ;
double dis9a ;
double dis10a;

double dis1b;
double dis2b;
double dis3b;
double dis4b;
double dis5b;
double dis6b;
double dis7b;
double dis8b;
double dis9b;
double dis10b;


if(biarc1.count() > 0) dis1a = hsf::GetMaxDis(biarc1.at(0),crv);
if(biarc2.count() > 0) dis2a = hsf::GetMaxDis(biarc2.at(0),crv);
if(biarc3.count() > 0) dis3a = hsf::GetMaxDis(biarc3.at(0),crv);
if(biarc4.count() > 0) dis4a = hsf::GetMaxDis(biarc4.at(0),crv);
if(biarc5.count() > 0) dis5a = hsf::GetMaxDis(biarc5.at(0),crv);
if(biarc6.count() > 0) dis6a = hsf::GetMaxDis(biarc6.at(0),crv);
if(biarc7.count() > 0) dis7a = hsf::GetMaxDis(biarc7.at(0),crv);
if(biarc8.count() > 0) dis8a = hsf::GetMaxDis(biarc8.at(0),crv);
if(biarc9.count() > 0) dis9a = hsf::GetMaxDis(biarc9.at(0),crv);
if(biarc10.count() > 0) dis10a = hsf::GetMaxDis(biarc10.at(0),crv);

if(biarc1.count() > 0) dis1b = hsf::GetMaxDis(biarc1.at(1),crv);
if(biarc2.count() > 0) dis2b = hsf::GetMaxDis(biarc2.at(1),crv);
if(biarc3.count() > 0) dis3b = hsf::GetMaxDis(biarc3.at(1),crv);
if(biarc4.count() > 0) dis4b = hsf::GetMaxDis(biarc4.at(1),crv);
if(biarc5.count() > 0) dis5b = hsf::GetMaxDis(biarc5.at(1),crv);
if(biarc6.count() > 0) dis6b = hsf::GetMaxDis(biarc6.at(1),crv);
if(biarc7.count() > 0) dis7b = hsf::GetMaxDis(biarc7.at(1),crv);
if(biarc8.count() > 0) dis8b = hsf::GetMaxDis(biarc8.at(1),crv);
if(biarc9.count() > 0) dis9b = hsf::GetMaxDis(biarc9.at(1),crv);
if(biarc10.count() > 0) dis10b = hsf::GetMaxDis(biarc10.at(1),crv);

double dis1 = MAX(dis1a,dis1b);
double dis2 = MAX(dis2a,dis2b);
double dis3 = MAX(dis3a,dis3b);
double dis4 = MAX(dis4a,dis4b);
double dis5 = MAX(dis5a,dis5b);
double dis6 = MAX(dis6a,dis6b);
double dis7 = MAX(dis7a,dis7b);
double dis8 = MAX(dis8a,dis8b);
double dis9 = MAX(dis9a,dis9b);
double dis10 = MAX(dis10a,dis10b);




QMap<double,QList<TopoDS_Shape>> map;
if(biarc1.count() > 0) map.insert(dis1,biarc1);
if(biarc2.count() > 0) map.insert(dis2,biarc2);
if(biarc3.count() > 0) map.insert(dis3,biarc3);
if(biarc4.count() > 0) map.insert(dis4,biarc4);
if(biarc5.count() > 0) map.insert(dis5,biarc5);
if(biarc6.count() > 0) map.insert(dis6,biarc6);
if(biarc7.count() > 0) map.insert(dis7,biarc7);
if(biarc8.count() > 0) map.insert(dis8,biarc8);
if(biarc9.count() > 0) map.insert(dis9,biarc9);
if(biarc10.count() > 0) map.insert(dis10,biarc10);

if (map.keys().count() < 1) return folder;

QList<double> distances = map.keys();

qSort(distances);

double minval = distances.at(0);

TopoDS_Shape arc1 = map.value(minval).at(0);
TopoDS_Shape arc2 = map.value(minval).at(1);

vis(arc1) vis(arc2)

gp_Pnt p1oncrv = hsf::AddNewPointonCurve(arc1,0);
gp_Pnt p2oncrv = hsf::AddNewPointonCurve(arc1,1);
gp_Pnt p3oncrv = hsf::AddNewPointonCurve(arc2,1);
//visgeo(point1,Point(p1oncrv))
//visgeo(point2,Point(p2oncrv))
//visgeo(point3,Point(p3oncrv))

try
{
gp_Pnt arcenter1 = hsf::GetCenterCircle(arc1);
gp_Pnt poncrv1 = hsf::AddNewPointonCurve(arc1,0.5);
TopoDS_Shape line1 = hsf::AddNewLineptpt(arcenter1,poncrv1);
//if(arcenter1.Distance(poncrv1) < 10000) 
vis(line1);


gp_Pnt arcenter2 = hsf::GetCenterCircle(arc2);
gp_Pnt poncrv2 = hsf::AddNewPointonCurve(arc2,0.5);
TopoDS_Shape line2 = hsf::AddNewLineptpt(arcenter2,poncrv2);
//if(arcenter2.Distance(poncrv2) < 10000) 
vis(line2);
}
catch(...)
{
}




return folder;

	}