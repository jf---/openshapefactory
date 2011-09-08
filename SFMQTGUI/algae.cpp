#include "algae.h"
#include "ui.h"
#include "shapefactory.h"
#include "parametricsfordummies.h"
#include "bash.h"
#include "AIS_PointCloud.hxx"
#include <QFileDialog>
#include <QProgressDialog>
#include <QTime>

algae::algae(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setup();
}

algae::~algae()
{

}

void algae::setup()
{
SLIDERMOVED(update)
connect(ui.veins,SIGNAL(pressed()),this,SLOT(buildveins2()));
}

void algae::update()
{
INITPART



double x = get(x)
double y = get(y)
double radius = get(radius)
double depth = get(depth)

gp_Pnt origin(x,y,0);
gp_Pnt origin2(x,y,depth);
gp_Pnt origin3(x + (radius/3),y,depth * 1.3);

gp_Vec up = gp::DZ();

visgeo(p1,Point(origin))
visgeo(p2,Point(origin2))
visgeo(circ,Circle(origin,up,radius))
visgeo(circ2,Circle(origin2,up,radius*1.2))
visgeo(circ3,Circle(origin3,up,radius*1.9))

shapelist(constraints)
constraints  << circ << circ2 << circ3;
visgeo(consurface,ConstrainedSurface(constraints))
face = consurface;




ENDPART



}

void algae::buildveins()
	{

	INITPART

	//vis(face)
	gp_Pnt cog = hsf::GetCOG(face);
	gp_Pln pln1(cog,gp::DX());
	
	gp_Vec up = gp::DZ();
	double radius = get(radius);
	TopoDS_Shape circ = hsf::AddNewCircle(cog,up,radius);
	//TopoDS_Shape middle = hsf::AddNewIntersectSrf(pln1,face);
	//vis(middle)	
	

	

	TopoDS_Shape edge1 = hsf::getedgefromshape(circ);
	

	double count = 100;
	FOR(i,0,count+1)
	double ratio = (double) i / count;
	gp_Pnt rootp = hsf::AddNewPointonCurve(edge1,ratio);
	visgeo(p1,Point(rootp))
	gp_Vec tng = hsf::getVectorTangentToCurveAtPoint(edge1,ratio);
	tng.Rotate(gp_Ax1(rootp,up),90);
	gp_Pln tngpln(rootp,tng);
	TopoDS_Shape intcrv = hsf::AddNewIntersectSrf(tngpln,face);
	vis(intcrv)


	END



	ENDPART



	}

void algae::buildveins2()
	{

	INITPART

	//vis(face)
	gp_Pnt cog = hsf::GetCOG(face);
	gp_Pnt cogsrf = hsf::ProjectPoint(cog,face);
	gp_Pln pln1(cog,gp::DX());
	
	gp_Vec up = gp::DZ();
	double radius = get(radius);
	
	TopoDS_Shape middle = hsf::AddNewIntersectSrf(pln1,face);
	vis(middle)

		TopoDS_Shape edge1 = hsf::getedgefromshape(middle);
	
		double count = 100;
		double ucount = 15;
		FOR(i,2,count-2)
		double ratio = (double) i/count;
		gp_Pnt pcrv = hsf::AddNewPointonCurve(edge1,ratio);
		visgeo(pcrvp,Point(pcrv))
		gp_Pnt prevp = pcrv;
		gp_Pnt prevp2 = pcrv;
			FOR(j,0,ucount+1)
				gp_Pnt minp;
			if (j == 0)
				{
				minp = minpoint(face,prevp,radius*0.01);
				} else
				{
				TopoDS_Shape congeo;
				minp = minpoint(face,prevp,prevp2,radius*0.3,congeo);
				vis(congeo)
				}
			TopoDS_Shape line = hsf::AddNewLineptpt(prevp,minp);
			vis(line)
			prevp2 = prevp;
			prevp = minp;
			
			QString linetxt;
			QTextStream linestream(&linetxt);
			linestream << "branch:" << i << "limb:" << j; 
			ui::getInstance()->Statusbar(linetxt);
			END

		END

		
	gp_Pnt minp = minpoint(face, cogsrf, radius * 0.01);
	visgeo(p1,Point(minp))

	
	

	




	ENDPART



	}

gp_Pnt algae::minpoint(TopoDS_Shape srf, gp_Pnt startp, double rad)
	{

	gp_Vec up = hsf::getVectorNormaltoSurfaceatPoint(srf,startp);

	TopoDS_Shape sph = hsf::AddNewSphereSurface(startp,rad);
	TopoDS_Shape intcrv = hsf::AddNewIntersectSrf(sph,srf);
	
	double count =20;
	
	double minlength = rad*100;
	gp_Pnt minpoint;
	FOR(i,0,count)
	double ratio = (double) i/count;
	gp_Pnt p1 = hsf::AddNewPointonCurve(intcrv,ratio);
	TopoDS_Shape line1 = hsf::AddNewLineptpt(startp,p1);
	TopoDS_Shape minline = hsf::AddNewProjectCurve(line1,srf,up);
//	vis(minline)
	double linelength = hsf::GetLength(minline);
	if(linelength < minlength)
		{
			minlength = linelength;
			minpoint = p1;
		}

	END

	
return minpoint;

	}


gp_Pnt algae::minpoint(TopoDS_Shape srf, gp_Pnt startp,gp_Pnt prevp, double rad,TopoDS_Shape &vis)
	{

	INITPART
	gp_Vec up = hsf::getVectorNormaltoSurfaceatPoint(srf,startp);

	TopoDS_Shape sph = hsf::AddNewSphereSurface(startp,rad);
	TopoDS_Shape intcrv = hsf::AddNewIntersectSrfW(sph,srf);
	vis(intcrv)
	shapelist(edges);
	edges = hsf::getshapelist(intcrv,TopAbs_EDGE );
	pointlist(points)
		double pcount = 3;
	FOR(i,0,edges.length())
		TopoDS_Shape edge = edges.at(i);
		FOR(j,0,pcount+1)
		double ratio = (double) j/pcount;
		
		gp_Pnt pcrv = hsf::AddNewPointonCurve(edge,ratio);
		if (!points.contains(pcrv))
			{
				points << pcrv;
			}
		
		END
	END
	
	
	double count = points.length();
	double minlength = rad*100;
	double maxangle = 0;
	gp_Pnt minpoint;
	FOR(i,0,count)
	double ratio = (double) i/count;
	
	gp_Pnt p1 = points.at(i);
	gp_Pnt p2 = hsf::MovePointByVector(p1,up,100);
	
	TopoDS_Shape line1 = hsf::AddNewLineptpt(startp,p1);
	vis(line1);
	gp_Pnt midp = hsf::AddNewPointonCurve(line1,0.5);
	gp_Pnt intp = hsf::ProjectPoint(midp,srf);
	visgeo(midpoint,Point(midp))
	visgeo(midpointp,Point(intp))

	double seg1 = startp.Distance(intp);
	double seg2 = intp.Distance(p1);
	TopoDS_Shape minline = hsf::AddNewArcofCircle(startp,intp,p1);
	//TopoDS_Shape minline = hsf::AddNewIntersectSrfW(srf,extr);
	
    visgeo(point2,Point(p1))
	vis(minline)
	

	double linelength = seg1 + seg2 ;//hsf::GetLength(minline);
	double dis = prevp.Distance(p1);
	gp_Vec v1(prevp,startp);
	gp_Vec v2(startp,p1);
	double radian = v1.Angle(v2);
	double deg = radian * (180/PI);
	//hsf::AddNewAnnotation(QString::number(deg),p1);
	if(linelength < minlength && dis > rad/2  )
		{
			minlength = linelength;
			minpoint = p1;
			maxangle = deg;
		}

	END
	visgeo(point1,Point(minpoint))
	//vis = folder;
	//qDebug() << maxangle;
return minpoint;

	}
