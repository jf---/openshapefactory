#include "zha_jcm.h"

#include "ui.h"
#include "shapefactory.h"
#include "cowbellprogress.h"
#include "parametricsfordummies.h"
#include "bash.h"

ZHA_JCM::ZHA_JCM( QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setup();
}

ZHA_JCM::~ZHA_JCM()
{
}

void ZHA_JCM::purgememory()
	{
		
	int purgestatus = Standard::Purge();
	if (purgestatus > 0) 
		{
		ui::getInstance()->Statusbar(QString("Memory Purge Worked"));
		} else {
		ui::getInstance()->Statusbar(QString("Memory Purge Did Not Work"));
		}

	
	}
void ZHA_JCM::setup()
{
	//SLIDERRELEASED(update) //macro to initialize sliders on release
	SLIDERMOVED(update2) //macro to initialize sliders on move
	connect(ui.intersect,SIGNAL(stateChanged(int)), this, SLOT(update2()));
	connect(ui.node,SIGNAL(stateChanged(int)), this, SLOT(update2()));
	//QString vendor = QString((const char*)glGetString(GL_VENDOR));
}

void ZHA_JCM::update()
	{
		INITPART // code starts here
		double middleoffset = get(x)
		double middlez = get(y)
		double topz = get(z)
		double radius1 = get(radius1)
		double radius2 = get(radius2)
		double radius3 = get(radius3)
		double ucount = get(ucount)
		double vcount = get(vcount)
		// here we are making points
		
		pointdata(origin1,0,0,0)
		pointdata(origin2,0,middleoffset,middlez)
		pointdata(origin3,0,0,topz)
		// and then vectors
		vectordata(up,0,0,1)
		visgeo(c1,Circle(origin1,up,radius1))
		visgeo(c2,Circle(origin2,up,radius2))
		visgeo(c3,Circle(origin3,up,radius3))

		shapelist(sections)
		addtolist(sections,c1)
		addtolist(sections,c2)
		addtolist(sections,c3)
		geo(surface1,LoftSurface(sections))

		PANELIZE(surface1,ucount,vcount) // this is a macro
		PANELOFFSET(20) // this is a macro
		geo(diag,Lineptpt(op1,op3))
		pgeo(midp,PointonCurve(diag,0.5))
		geo(cog,Point(midp))
		geo(edge1,Lineptpt(p1,p2))
		geo(edge2,Lineptpt(p2,p3))
		geo(edge3,Lineptpt(p3,p4))
		geo(edge4,Lineptpt(p4,p1))
		pgeo(midp1,PointonCurve(edge1,0.1))
		pgeo(midp2,PointonCurve(edge2,0.1))
		pgeo(midp3,PointonCurve(edge3,0.5))
		pgeo(midp4,PointonCurve(edge4,0.5))

		geo(midps,Point(midp))

	/*	shapelist(mycons)
		addtolist(mycons,edge1)
		addtolist(mycons,edge2)
		addtolist(mycons,edge3)
		addtolist(mycons,edge4)
		addtolist(mycons,midps)
		visgeo(surf1,ConstrainedSurface(mycons))*/

		pointlist(spline1list)
		addtolist(spline1list,midp1)
		addtolist(spline1list,midp2)
		addtolist(spline1list,midp4)
		addtolist(spline1list,midp3)
		geo(spline1,SplineSShape(spline1list))

		visgeo(extrude1,Extrude(spline1,v1,20))
		visgeo(extrude2,Extrude(spline1,v1,30))

		
		ENDPANELIZE // this is a macro
		ENDPART
	}


void ZHA_JCM::update2()
	{
		INITPART // code starts here
		double x = get(x)
		double y = get(y)
		double z = get(z)
		double radius1 = get(radius1)
		double height = get(radius2)
		double radius3 = get(radius3)
		double angle = get(ucount)
		double vcount = get(vcount)

	// here we are making points
	pointdata(origin,x,y,z)
	pointdata(origin2,x,y,z+height)
	vectordata(up,0,0,1)
	geo(circle1,Circle(origin,up,radius1))
	 //define rectangle
	pgeo(recv1,PointonCurve(circle1,0))
	pgeo(recv2,PointonCurve(circle1,0.25))
	pgeo(recv3,PointonCurve(circle1,0.50))
	pgeo(recv4,PointonCurve(circle1,0.75))

	visgeo(recl1,Lineptpt(recv1,recv2))
	visgeo(recl2,Lineptpt(recv2,recv3))
	visgeo(recl3,Lineptpt(recv3,recv4))
	visgeo(recl4,Lineptpt(recv4,recv1))
		
	int pedgecount = 10;
    pointlist(reclist)
	FOR(i,0,pedgecount-1) pgeo(prec,PointonCurve(recl1,(double)i/(pedgecount-1))) addtolist(reclist,prec) END
	FOR(i,0,pedgecount-1) pgeo(prec,PointonCurve(recl2,(double)i/(pedgecount-1))) addtolist(reclist,prec) END
	FOR(i,0,pedgecount-1) pgeo(prec,PointonCurve(recl3,(double)i/(pedgecount-1))) addtolist(reclist,prec) END
	FOR(i,0,pedgecount-1) pgeo(prec,PointonCurve(recl4,(double)i/(pedgecount-1))) addtolist(reclist,prec) END
	addtolist(reclist,recv1)
	visgeo(rec1,SplineSShape(reclist))
	
	shapedata uppercurve = rec1;
		
		geo(circle2,Circle(origin2,up,radius3))
		TopoDS_Shape circlet2 = HSF::rotate(circle2,origin2,up,angle);
		TopoDS_Shape circlet3 = HSF::rotate(circle2,origin2,up,-angle);
		visgeo(blend1,BlendSurface(uppercurve,circle2))
		shapelist(hcols)
		shapelist(vcols)
		shapelist(hspcols)
		shapelist(vspcols)
		
		FOR(i,1,20)
		double uratio = (double)i/19;
		pgeo(pcrv1,PointonCurve(uppercurve,uratio))
		pgeo(pcrv2,PointonCurve(circlet2,uratio))
		pgeo(pcrv3,PointonCurve(circlet3,uratio))
		geo(line1,Lineptpt(pcrv1,pcrv2))
		geo(line2,Lineptpt(pcrv1,pcrv3))
		pointlist(splinelist1)
		pointlist(splinelist2)
		shapelist(loftlist1)
		shapelist(loftlist2)
		int pcount = 6;
			FOR(j,1,pcount+1)
				double vratio = (double)j/pcount;
				pgeo(pspl1,PointonCurve(line1,vratio))
				pspl1 = HSF::ProjectPoint(pspl1,blend1);
				pgeo(pspl2,PointonCurve(line2,vratio))
				pspl2 = HSF::ProjectPoint(pspl2,blend1);
				addtolist(splinelist1,pspl1)
				addtolist(splinelist2,pspl2)
				gp_Vec v1 = HSF::getVectorNormaltoSurfaceatPoint(blend1,pspl1);
				geo(cross1,Lineptdir(pspl1,v1,HSF::GetLength(line1)/10,0))
				addtolist(loftlist1,cross1)
				gp_Vec v2 = HSF::getVectorNormaltoSurfaceatPoint(blend1,pspl2);
				geo(cross2,Lineptdir(pspl2,v2,HSF::GetLength(line2)/10,0))
				addtolist(loftlist2,cross2)
			END
			

			geo(spline1,SplineSShape(splinelist1))
			geo(spline2,SplineSShape(splinelist2))
			visgeo(loft1,LoftSurface(loftlist1))
			visgeo(loft2,LoftSurface(loftlist2))
			addtolist(hcols,loft1)
			addtolist(vcols,loft2)
			addtolist(hspcols,spline1)
			addtolist(vspcols,spline2)
		END


			if (hcols.size() < 1) return;
			if (vcols.size() < 1) return;
			if (hspcols.size() < 1) return;
			if (vspcols.size() < 1) return;
			

		if(ui.intersect->isChecked()){
		  int count = 0;
		  for (int i = 0; i < hspcols.size(); ++i) 
		  {
		   for (int j = 0; j < vspcols.size(); ++j) 
		  {
		  try{
			
			 if(ui.node->isChecked())
				 {
				 int nbpoints;
				 pgeo(interp,IntersectionPoint(hcols.at(i),vcols.at(j),nbpoints)) 
				 count++;
				 TopoDS_Shape curshape = hspcols.at(i);
				 gp_Vec vcirc = HSF::getVectorNormaltoSurfaceatPoint(blend1,					 interp);
				 
				 geo(c1,Circle(interp,-vcirc,HSF::GetLength(curshape)/80))
				 visgeo(node,Extrude(c1,vcirc,0.09))

					 QString out;
					 QTextStream stout(&out);

					 stout << "curint" << i << "," << j;
					 ui::getInstance()->Statusbar(out);
				 }
			  } catch (...) {}

		  
		  }
		  }
		}


		ENDPART
	}
	