#include "apdl_mex_design_001.h"

#include "ui.h"
#include "shapefactory.h"
#include "cowbellprogress.h"
#include "parametricsfordummies.h"
#include "bash.h"
#include "excelcontroller.h"

#include <QFileDialog>


apdl_mex_design_001::apdl_mex_design_001(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setup();
}

apdl_mex_design_001::~apdl_mex_design_001()
{

}

void apdl_mex_design_001::setup()
{
SLIDERMOVED(build2dgrid4) // macro link sliders to update func
//SLIDERRELEASED(build2dgrid) // macro link sliders to update func
//connect(ui.runbtn,SIGNAL(pressed()),this,SLOT(update2()));

connect(ui.importexport,SIGNAL(pressed()),this,SLOT(exportstl()));

connect(ui.vizsrf,SIGNAL(stateChanged (int)),this,SLOT(build2dgrid4()));
connect(ui.vizuvlines,SIGNAL(stateChanged (int)),this,SLOT(build2dgrid4()));
connect(ui.viz3dcurves,SIGNAL(stateChanged (int)),this,SLOT(build2dgrid4()));
connect(ui.vizintp,SIGNAL(stateChanged ( int)),this,SLOT(build2dgrid4()));

connect(ui.splinerange,SIGNAL(sliderReleased()),this,SLOT(BuildPanels2solid()));
connect(ui.spiralxoffset,SIGNAL(sliderMoved(int)),this,SLOT(BuildPanels2solid()));
connect(ui.spiralyoffset,SIGNAL(sliderMoved(int)),this,SLOT(BuildPanels2solid()));
connect(ui.spiralinclination,SIGNAL(sliderMoved(int)),this,SLOT(BuildPanels2solid()));
	
}


void apdl_mex_design_001::update1()
{
INITPART

gp_Pnt p1(0,0,0);
gp_Pnt p2(30,30,0);
gp_Pnt p3(60,0,0);

gp_Pnt p4(-5,100,0);
gp_Pnt p5(30,100,15);
gp_Pnt p6(55,100,30);


TopoDS_Shape arc = HSF::AddNewArcofCircle(p1,p2,p3);
TopoDS_Shape arc2 = HSF::move(arc,gp_Vec(0,0,30));
TopoDS_Shape extrude = HSF::AddNewBlendSurface(arc,arc2);
pointlist(line1list)
addtolist(line1list,p4)
addtolist(line1list,p5)
addtolist(line1list,p6)

TopoDS_Shape line1 = HSF::AddNewSplineSShape(line1list);
gp_Pnt midp = HSF::AddNewPointonCurve(line1,0.5);
gp_Pnt midpp = HSF::ProjectPoint(midp,extrude);
TopoDS_Shape line2 = HSF::AddNewLineptpt(midp,midpp);
TopoDS_Shape line3 = HSF::move(line1,gp_Vec(midp,midpp).Multiplied(midp.Distance(midpp)*1.5));


vis(line2)
TopoDS_Shape extrude2 = HSF::AddNewBlendSurface(line1,line3);
//vis(extrude2)

TopoDS_Shape intcrv = HSF::AddNewIntersectSrf(extrude,extrude2);
vis(intcrv)
gp_Vec vec = HSF::getVectorNormaltoSurfaceatPoint(extrude,midp);

TopoDS_Shape projcrv = HSF::AddNewProjectCurve(line1,extrude,gp::DY());
vis (projcrv)
vis(line1)

vis(extrude);


ENDPART


}
void apdl_mex_design_001::update2()
{
INITPART

QoccInputOutput* io_man = new QoccInputOutput();
QString panelpath = QoccApplication::instance()->applicationDirPath() + QString("/panels.igs");

QString srfpath = QoccApplication::instance()->applicationDirPath() + QString("/map_srf.igs");





Handle(TopTools_HSequenceOfShape) famshape1 = io_man->importIGES(panelpath);
TopoDS_Shape PNL = famshape1->Value(1);

Handle(TopTools_HSequenceOfShape) famshape2 = io_man->importIGES(srfpath);
TopoDS_Shape SRF = famshape2->Value(1);


int facecount = HSF::getshapecount(SRF,TopAbs_FACE);
qDebug() << "shapetype:" << SRF.ShapeType();
qDebug() << "facecount:" << facecount;

int wirecount = HSF::getshapecount(PNL,TopAbs_WIRE);
qDebug() << "wirecount:" << wirecount;


gp_Pnt p1 = HSF::GetCOG(SRF);
p1.SetX(p1.X() + 20000);
p1.SetY(p1.Y() + 20000);
p1.SetZ(p1.Z() + 60000);


vis(SRF)
int count = 0;

try{
TopExp_Explorer Ex;
	for (Ex.Init(PNL,TopAbs_WIRE); Ex.More(); Ex.Next())
	{
	count++;
	ui::getInstance()->Statusbar(QString::number(count));
	//if (count < 200) continue;
	if (count > 300) break;

	

	TopoDS_Shape panel = Ex.Current();
	shapelist(panelfill);
		TopExp_Explorer Ex2;
		for (Ex2.Init(panel,TopAbs_EDGE); Ex2.More(); Ex2.Next())
		{
			TopoDS_Shape edge = Ex2.Current();
			addtolist(panelfill,edge)
		}
	TopoDS_Shape fill = HSF::AddNewFillSurface(panelfill);
	gp_Pnt cog = HSF::GetCOG(fill);
	TopoDS_Shape cogp = HSF::AddNewPoint(cog);


	//vis(fill)

	if (count == 1)
		{
		/*	p1 = cog;
			p1.SetX(p1.X() + 20000);
			p1.SetY(p1.Y() + 20000);*/
		}else {
			TopoDS_Shape line1 = HSF::AddNewLineptpt(p1,cog);
			gp_Pnt intp = HSF::AddNewIntersectCrvSrf(line1,SRF);
			if (intp.Distance(gp::Origin()) < Precision::Confusion())
				{
						vis(cogp)
					    vis(line1)
				}
			
		}
    }

	} catch(...) {}

ENDPART


}

void apdl_mex_design_001::build2dgrid()
	{
	


	INITPART
	TopoDS_Shape SLAB;
	if (SRF.IsNull()) // check if the surface has already loaded
		{
		QoccInputOutput* io_man = new QoccInputOutput();
	QString srfpath = QoccApplication::instance()->applicationDirPath() + QString("/map_srf.igs"); // get the path of the surface

		QString slabspath = QoccApplication::instance()->applicationDirPath() + QString("/map_slabs.igs"); // get the path of the surface

	Handle(TopTools_HSequenceOfShape) famshape2 = io_man->importIGES(srfpath); //import the surface

	Handle(TopTools_HSequenceOfShape) famshape3 = io_man->importIGES(slabspath); //import the surface
	SRF = famshape2->Value(1); // get the surface shape
	SLAB = famshape3->Value(1); // get the surface shape
		}
	
	// get parameters
	double ucount = get(griducount)   //get the ucount from the slider
	double vcount = get(gridvcount)   //get the vcount from the slider
	double uincline = get(uincline) ; uincline /=100;


	bool showsurface = ui.vizsrf->isChecked();
	bool showuvlines = ui.vizuvlines->isChecked();
	bool show3dcurves = ui.viz3dcurves->isChecked();
	bool showintp = ui.vizintp->isChecked();

// begin code

//if(showsurface) vis(SRF) // visualize the surface
if(showsurface) vis(SLAB) // visualize the surface

// create 4 points of uvparametric square

gp_Pnt p1(0,2-uincline,0);
gp_Pnt p2(0,1-uincline,0);
gp_Pnt p3(1,1,0);
gp_Pnt p4(1,0,0);
gp_Pnt p5(0,0+uincline,0);
gp_Pnt p6(0,-1+uincline,0);




	

	// connect the dots and build uv curves of the square boundary
	TopoDS_Shape l1 = HSF::AddNewLineptpt(p1,p2);
	TopoDS_Shape l2 = HSF::AddNewLineptpt(p3,p4);
	TopoDS_Shape l3 = HSF::AddNewLineptpt(p5,p6);


	if(showuvlines){
	vis(l1)
	vis(l2)
	vis(l3)
	//vis(l4)
		}
	//*************** build top and bottom points *************
	pointlist(seg1list)  // list to hold topedge points
	pointlist(seg2list)  // list to hold bottom edge points
	pointlist(seg3list)  // list to hold bottom edge points

	FOR(i,0,ucount+1)
	double percent = (double) i/ucount;
	gp_Pnt pcrv1 = HSF::AddNewPointonCurve(l1,percent); // buildpoint on top edge
	gp_Pnt pcrv2 = HSF::AddNewPointonCurve(l2,percent); // buildpoint on bottom edge
	gp_Pnt pcrv3 = HSF::AddNewPointonCurve(l3,percent); // buildpoint on bottom edge

	addtolist(seg1list,pcrv1)
	addtolist(seg2list,pcrv2)
	addtolist(seg3list,pcrv3)
	
	END

	

	//********build u diagonal lines ******************
	shapelist(ulines);
	// build first half of lines
	FOR(i,0,seg1list.count())
		gp_Pnt p1 = seg2list.at(i);
	    gp_Pnt p2 = seg1list.at(i);
			
		TopoDS_Shape line1 = HSF::AddNewLineptpt(p1,p2);
		if(showuvlines){ vis(line1);}
		TopoDS_Shape uvline1 = HSF::applyuvcurve(line1,SRF);
		if(show3dcurves) {vis(uvline1)}
		addtolist(ulines,line1)
	END
	
	
	//**********************Build Vlines ***************************
	shapelist(vlines);
	// ********** build first half of vlines
	FOR(i,0,seg1list.count()-1)
		gp_Pnt p1 = seg2list.at(i);
	    gp_Pnt p2 = seg3list.at(i);
	
		TopoDS_Shape line1 = HSF::AddNewLineptpt(p1,p2);
		if(showuvlines) vis(line1);
		
		TopoDS_Shape uvline1 = HSF::applyuvcurve(line1,SRF);
		if(show3dcurves) vis(uvline1)
		addtolist(vlines,line1)
		
	END

	
	pointmap.clear();

	FOR(i,0,ulines.size()-1)
	int uval = i;
	TopoDS_Shape uline = ulines.at(i);
		FOR(j,0,vlines.size()-1)
			int vval = j;
			TopoDS_Shape vline = vlines.at(j);

			int nbpoints;
			gp_Pnt2d intp = HSF::AddNewIntersectCrv2d(uline,vline,nbpoints);
			gp_Pnt intp3d(intp.X(),intp.Y(),0);

			if (nbpoints > 0 ) 
				{
					gp_Pnt point3d = hsf::AddNewUVPt(SRF,intp.X(),intp.Y());
					TopoDS_Shape point = HSF::AddNewPoint(point3d);
					if(showintp)vis(point)

					QString key = "U" + QString::number(i,char(103),3)+ "_"+"V" + QString::number(j,char(103),3);
					pointmap.insert(key,point3d);
					//HSF::AddNewAnnotation(key,point3d);
				}

	

		END



	END



	/*TopoDS_Shape panels = BuildPanels();
	vis(panels);*/






	ENDPART

	//qGeomApp->myview


	}

void apdl_mex_design_001::build2dgrid2()
	{
	


	INITPART
	
	if (SRF.IsNull()) // check if the surface has already loaded
		{
	QoccInputOutput* io_man = new QoccInputOutput();
	QString srfpath = QoccApplication::instance()->applicationDirPath() + QString("/map_srf.igs"); // get the path of the surface

	Handle(TopTools_HSequenceOfShape) famshape2 = io_man->importIGES(srfpath); //import the surface
	SRF = famshape2->Value(1); // get the surface shape
		}
	
	// get parameters
	double ucount = get(griducount)   //get the ucount from the slider
	double vcount = get(gridvcount)   //get the vcount from the slider
	double uincline = get(uincline) ; uincline /=100;


	bool showsurface = ui.vizsrf->isChecked();
	bool showuvlines = ui.vizuvlines->isChecked();
	bool show3dcurves = ui.viz3dcurves->isChecked();
	bool showintp = ui.vizintp->isChecked();

// begin code

if(showsurface) vis(SRF) // visualize the surface

// create 4 points of uvparametric square

gp_Pnt p1(0,2-uincline,0);
gp_Pnt p2(0,1-uincline,0);
gp_Pnt p3(1,1,0);
gp_Pnt p4(1,0,0);
gp_Pnt p5(0,0+uincline,0);
gp_Pnt p6(0,-1+uincline,0);




	

	// connect the dots and build uv curves of the square boundary
	TopoDS_Shape l1 = HSF::AddNewLineptpt(p1,p2);
	TopoDS_Shape l2 = HSF::AddNewLineptpt(p3,p4);
	TopoDS_Shape l3 = HSF::AddNewLineptpt(p5,p6);


	if(showuvlines){
	vis(l1)
	vis(l2)
	vis(l3)
	//vis(l4)
		}
	//*************** build top and bottom points *************
	pointlist(seg1list)  // list to hold topedge points
	pointlist(seg2list)  // list to hold bottom edge points
	pointlist(seg3list)  // list to hold bottom edge points

	FOR(i,0,ucount+1)
	double percent = (double) i/ucount;
	gp_Pnt pcrv1 = HSF::AddNewPointonCurve(l1,percent); // buildpoint on top edge
	gp_Pnt pcrv2 = HSF::AddNewPointonCurve(l2,percent); // buildpoint on bottom edge
	gp_Pnt pcrv3 = HSF::AddNewPointonCurve(l3,percent); // buildpoint on bottom edge

	addtolist(seg1list,pcrv1)
	addtolist(seg2list,pcrv2)
	addtolist(seg3list,pcrv3)
	
	END

	

	//********build u diagonal lines ******************
	shapelist(ulines);
	// build first half of lines
	FOR(i,0,seg1list.count())
		gp_Pnt p1 = seg2list.at(i);
	    gp_Pnt p2 = seg1list.at(i);
			
		TopoDS_Shape line1 = HSF::AddNewLineptpt(p1,p2);
		if(showuvlines){ vis(line1);}
		TopoDS_Shape uvline1 = HSF::applyuvcurve(line1,SRF);
		if(show3dcurves) {vis(uvline1)}
		addtolist(ulines,uvline1)
	END
	
	
	//**********************Build Vlines ***************************
	shapelist(vlines);
	// ********** build first half of vlines
	FOR(i,0,seg1list.count()-1)
		gp_Pnt p1 = seg2list.at(i);
	    gp_Pnt p2 = seg3list.at(i);
	
		TopoDS_Shape line1 = HSF::AddNewLineptpt(p1,p2);
		if(showuvlines) vis(line1);
		
		TopoDS_Shape uvline1 = HSF::applyuvcurve(line1,SRF);
		if(show3dcurves) vis(uvline1)
		addtolist(vlines,uvline1)
		
	END

	
	pointmap.clear();

	FOR(i,0,ulines.size()-1)
	int uval = i;
	TopoDS_Shape uline = ulines.at(i);
		FOR(j,0,vlines.size()-1)
			int vval = j;
			TopoDS_Shape vline = vlines.at(j);

			int nbpoints;
			gp_Pnt point3d = HSF::AddNewIntersectionPoint(uline,vline,nbpoints);

			if (nbpoints > 0 ) 
				{
					//gp_Pnt point3d = hsf::AddNewUVPt(SRF,intp.X(),intp.Y());
					TopoDS_Shape point = HSF::AddNewPoint(point3d);
					if(showintp)vis(point)

					QString key = "U" + QString::number(i,char(103),3)+ "_"+"V" + QString::number(j,char(103),3);
					pointmap.insert(key,point3d);
					//HSF::AddNewAnnotation(key,point3d);
				}

	

		END



	END



	/*TopoDS_Shape panels = BuildPanels();
	vis(panels);*/






	ENDPART

	//qGeomApp->myview


	}

void apdl_mex_design_001::build2dgrid3()
	{
	


	INITPART
	
		TopoDS_Shape SLAB;
	if (SRF.IsNull() || SLAB.IsNull()) // check if the surface has already loaded
		{
	QoccInputOutput* io_man = new QoccInputOutput();
	QString srfpath = QoccApplication::instance()->applicationDirPath() + QString("/map_srf.igs"); // get the path of the surface

		QString slabspath = QoccApplication::instance()->applicationDirPath() + QString("/map_slabs.igs"); // get the path of the surface

	Handle(TopTools_HSequenceOfShape) famshape2 = io_man->importIGES(srfpath); //import the surface
	Handle(TopTools_HSequenceOfShape) famshape3 = io_man->importIGES(slabspath); //import the surface
	SRF = famshape2->Value(1); // get the surface shape
	SLAB = famshape3->Value(1); // get the surface shape
}
	
	// get parameters
	double ucount = get(griducount)   //get the ucount from the slider
	double vcount = get(gridvcount)   //get the vcount from the slider
	double uincline = get(uincline) ; uincline /=100;


	bool showsurface = ui.vizsrf->isChecked();
	bool showuvlines = ui.vizuvlines->isChecked();
	bool show3dcurves = ui.viz3dcurves->isChecked();
	bool showintp = ui.vizintp->isChecked();

// begin code

//if(showsurface) vis(SRF) // visualize the surface
if(showsurface) vis(SLAB) // visualize the surface

// create 4 points of uvparametric square

gp_Pnt p1(0,0+uincline,0);
gp_Pnt p2(0,1-uincline,0);
gp_Pnt p3(1,1,0);
gp_Pnt p4(1,0,0);



	// connect the dots and build uv curves of the square boundary
	TopoDS_Shape l1 = HSF::AddNewLineptpt(p1,p4);
	TopoDS_Shape l2 = HSF::AddNewLineptpt(p2,p3);
	


	if(showuvlines){
	vis(l1)
	vis(l2)
	}



	
	ENDPART




	}


void apdl_mex_design_001::build2dgrid4()
	{
	


	INITPART
	TopoDS_Shape SLAB;
	if (SRF.IsNull()|| SPIRAL.IsNull()) // check if the surface has already loaded
		{
		QoccInputOutput* io_man = new QoccInputOutput();
	QString srfpath = QoccApplication::instance()->applicationDirPath() + QString("/map_srf.igs"); // get the path of the surface

	   QString slabspath = QoccApplication::instance()->applicationDirPath() + QString("/map_slabs.igs"); // get the path of the surface

	   QString spiralpath = QoccApplication::instance()->applicationDirPath() + QString("/spiralcurve.igs"); // get the path of the surface

	Handle(TopTools_HSequenceOfShape) famshape2 = io_man->importIGES(srfpath); //import the surface

	Handle(TopTools_HSequenceOfShape) famshape3 = io_man->importIGES(slabspath); //import the surface
	Handle(TopTools_HSequenceOfShape) famshape4 = io_man->importIGES(spiralpath); //import the surface

	SRF = famshape2->Value(1); // get the surface shape
	SLAB = famshape3->Value(1); // get the surface shape
	SPIRAL = famshape4->Value(1);
		}
	
	// get parameters
	double ucount = get(griducount)   //get the ucount from the slider
	double vcount = get(gridvcount)   //get the vcount from the slider
	double uincline = get(uincline) ; uincline /=100;


	bool showsurface = ui.vizsrf->isChecked();
	bool showuvlines = ui.vizuvlines->isChecked();
	bool show3dcurves = ui.viz3dcurves->isChecked();
	bool showintp = ui.vizintp->isChecked();

// begin code

if(showsurface) vis(SRF) // visualize the surface
//if(showsurface) vis(SLAB) // visualize the surface

// create 4 points of uvparametric square

double yoffsetuv = uincline;
uincline = 0;
gp_Pnt p1(0,yoffsetuv+2-uincline,0);
gp_Pnt p2( 0,yoffsetuv+1-uincline,0);
gp_Pnt p3(1,yoffsetuv+1,0);
gp_Pnt p4(1,yoffsetuv+0,0);
gp_Pnt p5(0,yoffsetuv+0+uincline,0);
gp_Pnt p6(0,yoffsetuv+-1+uincline,0);

gp_Pnt ap1(0,yoffsetuv+2-uincline+1,0);
gp_Pnt ap2(0,yoffsetuv+1-uincline+1,0);
gp_Pnt ap3(1,yoffsetuv+1+1,0);
gp_Pnt ap4(1,yoffsetuv+0+1,0);

gp_Pnt bp3(1,yoffsetuv+1-1,0);
gp_Pnt bp4(1,yoffsetuv+0-1,0);
gp_Pnt bp5(0,yoffsetuv+0+uincline-1,0);
gp_Pnt bp6(0,yoffsetuv+-1+uincline-1,0);




	// connect the dots and build uv curves of the square boundary
	TopoDS_Shape al1 = HSF::AddNewLineptpt(ap1,ap2);
	TopoDS_Shape al2 = HSF::AddNewLineptpt(ap3,ap4);


	// connect the dots and build uv curves of the square boundary
	TopoDS_Shape l1 = HSF::AddNewLineptpt(p1,p2);
	TopoDS_Shape l2 = HSF::AddNewLineptpt(p3,p4);
	TopoDS_Shape l3 = HSF::AddNewLineptpt(p5,p6);

	// connect the dots and build uv curves of the square boundary
	TopoDS_Shape bl2 = HSF::AddNewLineptpt(bp3,bp4);
	TopoDS_Shape bl3 = HSF::AddNewLineptpt(bp5,bp6);




	if(showuvlines){

			vis(l1)
			vis(l2)
			vis(l3)
	
		}

	//*************** build top and bottom points *************
	pointlist(seg1list)  // list to hold topedge points
	pointlist(seg2list)  // list to hold bottom edge points
	pointlist(seg3list)  // list to hold bottom edge points

	pointlist(seg1alist)  // list to hold topedge points
	pointlist(seg2alist)  // list to hold bottom edge points
	
	pointlist(seg2blist)  // list to hold bottom edge points
	pointlist(seg3blist)  // list to hold bottom edge points


	FOR(i,0,ucount+1)
	double percent = (double) i/ucount;
	gp_Pnt pcrv1 = HSF::AddNewPointonCurve(l1,percent); // buildpoint on top edge
	gp_Pnt pcrv2 = HSF::AddNewPointonCurve(l2,percent); // buildpoint on bottom edge
	gp_Pnt pcrv3 = HSF::AddNewPointonCurve(l3,percent); // buildpoint on bottom edge

	gp_Pnt pcrv1a = HSF::AddNewPointonCurve(al1,percent); // buildpoint on top edge
	gp_Pnt pcrv2a = HSF::AddNewPointonCurve(al2,percent); // buildpoint on bottom edge

	gp_Pnt pcrv2b = HSF::AddNewPointonCurve(bl2,percent); // buildpoint on bottom edge
	gp_Pnt pcrv3b = HSF::AddNewPointonCurve(bl3,percent); // buildpoint on bottom edge

	addtolist(seg1list,pcrv1)
	addtolist(seg2list,pcrv2)
	addtolist(seg3list,pcrv3)

	addtolist(seg1alist,pcrv1a)
	addtolist(seg2alist,pcrv2a)

	addtolist(seg2blist,pcrv2b)
	addtolist(seg3blist,pcrv3b)
	
	
	
	END

	

	//********build u diagonal lines ******************
	shapelist(ulines);
	shapelist(vulines);
	// build first half of lines
	FOR(i,0,seg1list.count())
		gp_Pnt p1 = seg2list.at(i);
	    gp_Pnt p2 = seg1list.at(i);

		gp_Pnt ap1 = seg2alist.at(i);
	    gp_Pnt ap2 = seg1alist.at(i);
			
		TopoDS_Shape line1 = HSF::AddNewLineptpt(p1,p2);
		TopoDS_Shape line2 = HSF::AddNewLineptpt(ap1,ap2);
		if(showuvlines){ vis(line1);}
		if(showuvlines){ vis(line2);}
		TopoDS_Shape uvline1 = HSF::applyuvcurve(line1,SRF);
		TopoDS_Shape uvline2 = HSF::applyuvcurve(line2,SRF);

		if(show3dcurves) {vis(uvline1)}

		addtolist(ulines,line1)
		addtolist(vulines,line2)
	END
	
	
	//**********************Build Vlines ***************************
	shapelist(vlines);
	shapelist(vvlines);
	// ********** build first half of vlines
	FOR(i,0,seg1list.count())
		gp_Pnt p1 = seg2list.at(i);
	    gp_Pnt p2 = seg3list.at(i);

		gp_Pnt bp1 = seg2blist.at(i);
	    gp_Pnt bp2 = seg3blist.at(i);
	
		TopoDS_Shape line1 = HSF::AddNewLineptpt(p1,p2);
		TopoDS_Shape line2 = HSF::AddNewLineptpt(bp1,bp2);
		if(showuvlines) vis(line1);
		if(showuvlines) vis(line2);
		
		TopoDS_Shape uvline1 = HSF::applyuvcurve(line1,SRF);
		TopoDS_Shape uvline2 = HSF::applyuvcurve(line2,SRF);

		if(show3dcurves) vis(uvline1)
		addtolist(vlines,line1)
		addtolist(vvlines,line2)
		
	END

	
	pointmap.clear();
	int uoff = ulines.size()+1;
	int voff = vlines.size()+1;

	FOR(i,0,ulines.count())
	int uval = i;
	TopoDS_Shape uline = ulines.at(i);
	TopoDS_Shape vuline = vulines.at(i);

		FOR(j,0,vlines.count())
			int vval = j;
			TopoDS_Shape vline = vlines.at(j);
			TopoDS_Shape vvline = vlines.at(j);

			int nbpoints1;
			gp_Pnt2d intp1 = HSF::AddNewIntersectCrv2d(uline,vline,nbpoints1);
			gp_Pnt intp3d1(intp1.X(),intp1.Y(),0);

			int nbpoints2;
			gp_Pnt2d intp2 = HSF::AddNewIntersectCrv2d(uline,vvline,nbpoints2);
			gp_Pnt intp3d2(intp2.X(),intp2.Y(),0);

			int nbpoints3;
			gp_Pnt2d intp3 = HSF::AddNewIntersectCrv2d(vuline,vline,nbpoints3);
			gp_Pnt intp3d3(intp3.X(),intp3.Y(),0);

			int nbpoints4;
			gp_Pnt2d intp4 = HSF::AddNewIntersectCrv2d(vuline,vvline,nbpoints4);
			gp_Pnt intp3d4(intp4.X(),intp4.Y(),0);


			if (nbpoints1 > 0 ) 
				{
					gp_Pnt point3d = hsf::AddNewUVPt(SRF,intp1.X(),intp1.Y());
					TopoDS_Shape point = HSF::AddNewPoint(point3d);
					if(showintp)vis(point)

					QString key = "U" + QString::number(i,char(103),3)+ "_"+"V" + QString::number(j,char(103),3);
					if(!pointmap.contains(key)) pointmap.insert(key,point3d);
					//HSF::AddNewAnnotation(key,point3d);
				}

			if (nbpoints2 > 0 ) 
				{
					gp_Pnt point3d = hsf::AddNewUVPt(SRF,intp2.X(),intp2.Y());
					TopoDS_Shape point = HSF::AddNewPoint(point3d);
					if(showintp)vis(point)

					QString key = "U" + QString::number(i,char(103),3)+ "_"+"V" + QString::number(j+voff,char(103),3);
					if(!pointmap.contains(key)) pointmap.insert(key,point3d);
					//HSF::AddNewAnnotation(key,point3d);
				}

			if (nbpoints3 > 0 ) 
				{
					gp_Pnt point3d = hsf::AddNewUVPt(SRF,intp3.X(),intp3.Y());
					TopoDS_Shape point = HSF::AddNewPoint(point3d);
					if(showintp)vis(point)

					QString key = "U" + QString::number(i+uoff,char(103),3)+ "_"+"V" + QString::number(j,char(103),3);
					if(!pointmap.contains(key)) pointmap.insert(key,point3d);
					//HSF::AddNewAnnotation(key,point3d);
				}

			if (nbpoints4 > 0 ) 
				{
					gp_Pnt point3d = hsf::AddNewUVPt(SRF,intp4.X(),intp4.Y());
					TopoDS_Shape point = HSF::AddNewPoint(point3d);
					if(showintp)vis(point)

					QString key = "U" + QString::number(i+uoff,char(103),3)+ "_"+"V" + QString::number(j+voff,char(103),3);
					if(!pointmap.contains(key)) pointmap.insert(key,point3d);
					//HSF::AddNewAnnotation(key,point3d);
				}

	

		END



	END



	/*TopoDS_Shape panels = BuildPanels();
	vis(panels);*/






	ENDPART

	//qGeomApp->myview
	int freemem =  Standard::Purge();
	if(freemem > 0 ) qDebug() << "Deallocated:" << freemem;
	
	

	}

void apdl_mex_design_001::build2dgrid5()
	{
	
	
	INITPART
	TopoDS_Shape SLAB;
	if (SRF.IsNull()|| SPIRAL.IsNull()) // check if the surface has already loaded
		{
		QoccInputOutput* io_man = new QoccInputOutput();
	QString srfpath = QoccApplication::instance()->applicationDirPath() + QString("/map_srf.igs"); // get the path of the surface

	   QString slabspath = QoccApplication::instance()->applicationDirPath() + QString("/map_slabs.igs"); // get the path of the surface

	   QString spiralpath = QoccApplication::instance()->applicationDirPath() + QString("/spiralcurve.igs"); // get the path of the surface

	Handle(TopTools_HSequenceOfShape) famshape2 = io_man->importIGES(srfpath); //import the surface

	Handle(TopTools_HSequenceOfShape) famshape3 = io_man->importIGES(slabspath); //import the surface
	Handle(TopTools_HSequenceOfShape) famshape4 = io_man->importIGES(spiralpath); //import the surface

	SRF = famshape2->Value(1); // get the surface shape
	SLAB = famshape3->Value(1); // get the surface shape
	SPIRAL = famshape4->Value(1);
		}
	
	// get parameters
	double ucount = get(griducount)   //get the ucount from the slider
	double vcount = get(gridvcount)   //get the vcount from the slider
	double uincline = get(uincline) ; uincline /=100;


	bool showsurface = ui.vizsrf->isChecked();
	bool showuvlines = ui.vizuvlines->isChecked();
	bool show3dcurves = ui.viz3dcurves->isChecked();
	bool showintp = ui.vizintp->isChecked();

	double uspacing = 1/ucount;
	double vspacing = 1/vcount;
	bool evenodd = true;
	double xoffset = uspacing /2;

	//QMap<QString,gp_Pnt> pointmap;

	for (int i = 0;i < vcount; i++)
		{
		double cury = i * vspacing;
		evenodd = !evenodd;
			for (int j = 0;j < ucount; j++)
			{
				
				double curx;
				if (evenodd)
					{
						if(j == ucount-1) continue;
						curx = xoffset + (j * uspacing);
						
					}else{
						curx = j * uspacing;
					}
			
				//gp_Pnt p1(curx,cury,0);
				//gp_Pnt p1(j,i,0);
				gp_Pnt p1 = hsf::AddNewUVPt(SRF,curx,cury);
				
				QString key = "U" + QString::number(j,char(103),3)+ "_"+"V" + QString::number(i,char(103),3);
				pointmap.insert(key,p1);
				visgeo(point1,Point(p1))
				//visgeo(point1,Point(p1))
									
			}

		}









	ENDPART




	}
void apdl_mex_design_001::BuildPanels()
		{
		
		QoccInputOutput* io_man = new QoccInputOutput();
		Handle(TopTools_HSequenceOfShape) shapes = new TopTools_HSequenceOfShape();

		INITPART
		if (pointmap.count() > 0 ){

		double splinerange = get(splinerange); splinerange /=100;
		double spiralxoffset = get(spiralxoffset)
		double spiralyoffset = get(spiralyoffset)
		double spiralinclination = get(spiralinclination)

		double ucount = get(griducount)

		bool showsurface = ui.vizsrf->isChecked();
		bool showuvlines = ui.vizuvlines->isChecked();
		bool show3dcurves = ui.viz3dcurves->isChecked();
		bool showintp = ui.vizintp->isChecked();
	
		if(showsurface) vis(SRF) // visualize the surface

		gp_Pnt sp1(1+(spiralxoffset/100),0+(spiralyoffset/100),0);
		gp_Pnt sp2(0+(spiralxoffset/100),1+(spiralyoffset/100)+(spiralinclination/100),0);
		TopoDS_Shape spiralcrv2d = HSF::AddNewLineptpt(sp1,sp2);
		spiralcrv3d = HSF::applyuvcurve(spiralcrv2d,SRF);
		

		spiralcrv3d  = SPIRAL;
		vis(spiralcrv3d);



		/*	TopoDS_Compound multiobject;
			BRep_Builder B;
			B.MakeCompound(multiobject);*/


			
				
		
			QMapIterator<QString,gp_Pnt> i(pointmap);
			while (i.hasNext()){
			i.next();
			QString currentname = i.key();
			QString epn = HSF::GetNextUvNameCiclic(currentname,1,0,ucount*2);
			QString spn = HSF::GetNextUvNameCiclic(currentname,1,-1,ucount*2);
			QString wpn = HSF::GetNextUvNameCiclic(currentname,0,-1,ucount*2);

			ui::getInstance()->Statusbar(currentname);
			
			if(pointmap.contains(epn) && pointmap.contains(spn) && pointmap.contains(wpn)){
				gp_Pnt p1 = i.value();
				gp_Pnt p2 = pointmap.value(epn);
				gp_Pnt p3 = pointmap.value(spn);
				gp_Pnt p4 = pointmap.value(wpn);

				pointlist(polyline);
				polyline << p1 << p2 << p3 << p4 << p1;
				try {
				//TopoDS_Shape pline = HSF::AddNewPolyline(polyline);
				//TopoDS_Shape panel = HSF::AddNewFillSurface(pline);
				gp_Pln avrgplane = HSF::AddNewMeanPlane(polyline);

			/*	if (pline.IsNull()) continue;
				B.Add(multiobject,pline);*/
				
				if (!spiralcrv3d.IsNull())
					{
					gp_Pnt pnlcog = HSF::GetAveragePoint(polyline);
					gp_Pnt centerp = HSF::ProjectPoint(pnlcog,avrgplane);
					//gp_Vec pvec = HSF::getVectorNormaltoSurfaceatPoint(panel,centerp);
					
					gp_Vec pvec = avrgplane.Axis().Direction();
					
					gp_Pnt poncrv = HSF::ProjectPointonCurve(pnlcog,spiralcrv3d);
					double length = pnlcog.Distance(poncrv);
					double radius = MAX((1-(length / 17182.5)) * 1000 * splinerange ,10);

					TopoDS_Shape line1 = HSF::AddNewLineptpt(p1,p3);
					TopoDS_Shape line2 = HSF::AddNewLineptpt(p4,p2);

					double percent = Min(abs(((length / 17182.5)) * splinerange ),1.0);
					
					gp_Pnt inp1 = HSF::AddNewPointonCurve(line1,0.1);
					gp_Pnt inp2 = HSF::AddNewPointonCurve(line2,1-percent);
					gp_Pnt inp3 = HSF::AddNewPointonCurve(line1,0.9);
					gp_Pnt inp4 = HSF::AddNewPointonCurve(line2,percent);

					
					TopoDS_Shape e1 = HSF::AddNewLineptpt(p1,inp1);
					TopoDS_Shape e2 = HSF::AddNewLineptpt(p2,inp2);
					TopoDS_Shape e3 = HSF::AddNewLineptpt(p3,inp3);
					TopoDS_Shape e4 = HSF::AddNewLineptpt(p4,inp4);
					
					TopoDS_Shape srf1 = HSF::AddNewBlendSurface(e1,e2);
					TopoDS_Shape srf2 = HSF::AddNewBlendSurface(e2,e3);
					TopoDS_Shape srf3 = HSF::AddNewBlendSurface(e3,e4);
					TopoDS_Shape srf4 = HSF::AddNewBlendSurface(e4,e1);

					vis(srf1)
					vis(srf2)
					vis(srf3)
					vis(srf4)

					shapes->Append( srf1 );
					shapes->Append( srf2 );
					shapes->Append( srf3 );
					shapes->Append( srf4 );


					
					
					

					pointlist(inplist)
					inplist << inp1 << inp2 << inp3 << inp4 << inp1;
					TopoDS_Shape inpline = HSF::AddNewPolyline(inplist);
					vis(inpline);

				
					TopoDS_Shape pline = HSF::AddNewPolyline(polyline);
					vis(pline);
					
					//TopoDS_Shape circ = HSF::AddNewCircle(centerp,pvec,radius);
					//vis(circ);
					//B.Add(multiobject,circ);
					qDebug() << length;
					}

					} catch(...) {continue;}

				}
			}
				//return multiobject;
			}

         //return TopoDS_Shape();

		    //QString patternpath = QoccApplication::instance()->applicationDirPath() + QString("/pattern.igs"); 

			//io_man->exportModel( QoccInputOutput::FormatIGES, patternpath, shapes );
		ENDPART
			
		}
void apdl_mex_design_001::BuildPanels2()
{
		
		QoccInputOutput* io_man = new QoccInputOutput();
		Handle(TopTools_HSequenceOfShape) shapes = new TopTools_HSequenceOfShape();

		QString fileName = QFileDialog::getSaveFileName(this, tr("Save Topology"), " ",tr("csv (*.csv "));

	 QFile file(fileName);
	 if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
     return;

     QTextStream out(&file);
	 QString line;
	 QTextStream linestream(&line);
	 
	 
	linestream << "panel name" << "," 
	<< "p1.X()" << "," << "p1.Y()" << "," << "p1.Z()"<< ","
	<< "p2.X()" << "," << "p2.Y()" << "," << "p2.Z()"<< ","
	<< "p3.X()" << "," << "p3.Y()" << "," << "p3.Z()"<< ","
	<< "p4.X()" << "," << "p4.Y()" << "," << "p4.Z()"<< ","
	<< "inp1.X()" << "," << "inp1.Y()" << "," << "inp1.Z()"<< ","
	<< "inp2.X()" << "," << "inp2.Y()" << "," << "inp2.Z()"<< ","
	<< "inp3.X()" << "," << "inp3.Y()" << "," << "inp3.Z()"<< ","
	<< "inp4.X()" << "," << "inp4.Y()" << "," << "inp4.Z()"<< "\n";

	  





		
		INITPART
		if (pointmap.count() > 0 ){

		double splinerange = get(splinerange); splinerange /=100;
		double spiralxoffset = get(spiralxoffset)
		double spiralyoffset = get(spiralyoffset)
		double spiralinclination = get(spiralinclination)

		double ucount = get(griducount)

		bool showsurface = ui.vizsrf->isChecked();
		bool showuvlines = ui.vizuvlines->isChecked();
		bool show3dcurves = ui.viz3dcurves->isChecked();
		bool showintp = ui.vizintp->isChecked();
	
		if(showsurface) vis(SRF) // visualize the surface

		gp_Pnt sp1(1+(spiralxoffset/100),0+(spiralyoffset/100),0);
		gp_Pnt sp2(0+(spiralxoffset/100),1+(spiralyoffset/100)+(spiralinclination/100),0);
		TopoDS_Shape spiralcrv2d = HSF::AddNewLineptpt(sp1,sp2);
		spiralcrv3d = HSF::applyuvcurve(spiralcrv2d,SRF);
		

		spiralcrv3d  = SPIRAL;
		vis(spiralcrv3d);



		/*	TopoDS_Compound multiobject;
			BRep_Builder B;
			B.MakeCompound(multiobject);*/


			
		int counter = 0;		
		
			QMapIterator<QString,gp_Pnt> i(pointmap);
			while (i.hasNext()){
			i.next();
			counter ++;
			//if (counter > 30) break;
			QString currentname = i.key();
			QString epn = HSF::GetNextUvNameCiclic(currentname,1,0,ucount*2);
			QString spn = HSF::GetNextUvNameCiclic(currentname,1,-1,ucount*2);
			QString wpn = HSF::GetNextUvNameCiclic(currentname,0,-1,ucount*2);

			ui::getInstance()->Statusbar(currentname);

			


			
			if(pointmap.contains(epn) && pointmap.contains(spn) && pointmap.contains(wpn)){
				gp_Pnt p1 = i.value();
				gp_Pnt p2 = pointmap.value(epn);
				gp_Pnt p3 = pointmap.value(spn);
				gp_Pnt p4 = pointmap.value(wpn);

				pointlist(polyline);
				polyline << p1 << p2 << p3 << p4 << p1;
				try {
				//TopoDS_Shape pline = HSF::AddNewPolyline(polyline);
				//TopoDS_Shape panel = HSF::AddNewFillSurface(pline);
				gp_Pln avrgplane = HSF::AddNewMeanPlane(polyline);

			/*	if (pline.IsNull()) continue;
				B.Add(multiobject,pline);*/
				
				if (!spiralcrv3d.IsNull())
					{
					gp_Pnt pnlcog = HSF::GetAveragePoint(polyline);
					gp_Pnt centerp = HSF::ProjectPoint(pnlcog,avrgplane);
					//gp_Vec pvec = HSF::getVectorNormaltoSurfaceatPoint(panel,centerp);
					
					gp_Vec pvec = avrgplane.Axis().Direction();
					
					gp_Pnt poncrv = HSF::ProjectPointonCurve(pnlcog,spiralcrv3d);
					double length = pnlcog.Distance(poncrv);
					double radius = MAX((1-(length / 17182.5)) * 1000 * splinerange ,10);

					TopoDS_Shape line1 = HSF::AddNewLineptpt(p1,p3);
					TopoDS_Shape line2 = HSF::AddNewLineptpt(p4,p2);

					double percent = Min(abs(((length / 17182.5)) * splinerange ),1.0);
					
					gp_Pnt inp1 = HSF::AddNewPointonCurve(line1,0.1);
					gp_Pnt inp2 = HSF::AddNewPointonCurve(line2,1-percent);
					gp_Pnt inp3 = HSF::AddNewPointonCurve(line1,0.9);
					gp_Pnt inp4 = HSF::AddNewPointonCurve(line2,percent);


				
	 				linestream << "panel" << counter << "," 
					<< p1.X() << "," << p1.Y() << "," << p1.Z()<< ","
					<< p2.X() << "," << p2.Y() << "," << p2.Z()<< ","
					<< p3.X() << "," << p3.Y() << "," << p3.Z()<< ","
					<< p4.X() << "," << p4.Y() << "," << p4.Z()<< ","
					<< inp1.X() << "," << inp1.Y() << "," << inp1.Z()<< ","
					<< inp2.X() << "," << inp2.Y() << "," << inp2.Z()<< ","
					<< inp3.X() << "," << inp3.Y() << "," << inp3.Z()<< ","
					<< inp4.X() << "," << inp4.Y() << "," << inp4.Z()<< "\n";
						
					qDebug() << "diag1" << p1.Distance(p3);
					qDebug() << "diag2" << p4.Distance(p2);

					if ( p1.Distance(p3) > 30000 || p4.Distance(p2) > 30000) continue;

					pointlist(panelist)
					panelist << p1 << p2 << p3 << p4 
					<< inp1 << inp2 << inp3 << inp4 << centerp;

					/*QList<TopoDS_Shape> outputlist;
					TopoDS_Shape panel = createpanel(panelist,outputlist);
					vis(panel);

					shapes->Append(panel);*/


					pointlist(inplist)
					inplist << inp1 << inp2 << inp3 << inp4 << inp1;
			TopoDS_Shape inpline = HSF::AddNewPolyline(inplist);
			//vis(inpline);

				
			TopoDS_Shape pline = HSF::AddNewPolyline(polyline);
			//vis(pline);
					
			TopoDS_Shape circ = HSF::AddNewCircle(centerp,pvec,radius);
			vis(circ);
			TopoDS_Shape circp = hsf::AddNewProjectCurve(circ,SRF,pvec.Reversed());
			//vis(circp)
			qDebug() << viscount;

					//B.Add(multiobject,circ);
					//qDebug() << length;
					}

					} catch(...) {continue;}

				}
			}
				//return multiobject;
			}

			out << line ;
			file.close();

         //return TopoDS_Shape();

		    //QString patternpath = QoccApplication::instance()->applicationDirPath() + QString("/pattern.stp"); 
			/*QString patternpath = QFileDialog::getSaveFileName(this, tr("Save Topology"), " ",tr("iges (*.iges);; All (*.*);;"));

io_man->exportModel( QoccInputOutput::FormatIGES, patternpath, shapes )*/;
		ENDPART
			
		}
void apdl_mex_design_001::BuildPanels2solid()
{
		
		QoccInputOutput* io_man = new QoccInputOutput();
		Handle(TopTools_HSequenceOfShape) shapes = new TopTools_HSequenceOfShape();

		//QString fileName = QFileDialog::getSaveFileName(this, tr("Save Topology"), " ",tr("csv (*.csv "));
		QString fileName = QString("");

	 QFile file(fileName);
	/* if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
     return;*/

     QTextStream out(&file);
	 QString line;
	 QTextStream linestream(&line);
	 
	 
	linestream << "panel name" << "," 
	<< "p1.X()" << "," << "p1.Y()" << "," << "p1.Z()"<< ","
	<< "p2.X()" << "," << "p2.Y()" << "," << "p2.Z()"<< ","
	<< "p3.X()" << "," << "p3.Y()" << "," << "p3.Z()"<< ","
	<< "p4.X()" << "," << "p4.Y()" << "," << "p4.Z()"<< ","
	<< "inp1.X()" << "," << "inp1.Y()" << "," << "inp1.Z()"<< ","
	<< "inp2.X()" << "," << "inp2.Y()" << "," << "inp2.Z()"<< ","
	<< "inp3.X()" << "," << "inp3.Y()" << "," << "inp3.Z()"<< ","
	<< "inp4.X()" << "," << "inp4.Y()" << "," << "inp4.Z()"<< "\n";

	  





		
		INITPART
		if (pointmap.count() > 0 ){

		double splinerange = get(splinerange); splinerange /=100;
		double spiralxoffset = get(spiralxoffset)
		double spiralyoffset = get(spiralyoffset)
		double spiralinclination = get(spiralinclination)

		double ucount = get(griducount)

		bool showsurface = ui.vizsrf->isChecked();
		bool showuvlines = ui.vizuvlines->isChecked();
		bool show3dcurves = ui.viz3dcurves->isChecked();
		bool showintp = ui.vizintp->isChecked();
	
		if(showsurface) vis(SRF) // visualize the surface

		gp_Pnt sp1(1+(spiralxoffset/100),0+(spiralyoffset/100),0);
		gp_Pnt sp2(0+(spiralxoffset/100),1+(spiralyoffset/100)+(spiralinclination/100),0);
		TopoDS_Shape spiralcrv2d = HSF::AddNewLineptpt(sp1,sp2);
		spiralcrv3d = HSF::applyuvcurve(spiralcrv2d,SRF);
		

		spiralcrv3d  = SPIRAL;
		vis(spiralcrv3d);



		/*	TopoDS_Compound multiobject;
			BRep_Builder B;
			B.MakeCompound(multiobject);*/

		//clear the exportlistmap
		exportlist.clear();
		exportlist2.clear();
			
		int counter = 0;		
		
			QMapIterator<QString,gp_Pnt> i(pointmap);
			while (i.hasNext()){
			i.next();
			counter ++;
			//if (counter > 1000) break;
			QString currentname = i.key();
			QString epn = HSF::GetNextUvNameCiclic(currentname,1,0,ucount*2);
			QString spn = HSF::GetNextUvNameCiclic(currentname,1,-1,ucount*2);
			QString wpn = HSF::GetNextUvNameCiclic(currentname,0,-1,ucount*2);

			ui::getInstance()->Statusbar(currentname);

			


			
			if(pointmap.contains(epn) && pointmap.contains(spn) && pointmap.contains(wpn)){
				gp_Pnt p1 = i.value();
				gp_Pnt p2 = pointmap.value(epn);
				gp_Pnt p3 = pointmap.value(spn);
				gp_Pnt p4 = pointmap.value(wpn);

				pointlist(polyline);
				polyline << p1 << p2 << p3 << p4 << p1;
				try {
				//TopoDS_Shape pline = HSF::AddNewPolyline(polyline);
				//TopoDS_Shape panel = HSF::AddNewFillSurface(pline);
				gp_Pln avrgplane = HSF::AddNewMeanPlane(polyline);

			/*	if (pline.IsNull()) continue;
				B.Add(multiobject,pline);*/
				
				if (!spiralcrv3d.IsNull())
					{
					gp_Pnt pnlcog = HSF::GetAveragePoint(polyline);
					gp_Pnt centerp = HSF::ProjectPoint(pnlcog,avrgplane);
					//gp_Vec pvec = HSF::getVectorNormaltoSurfaceatPoint(panel,centerp);
					
					gp_Vec pvec = avrgplane.Axis().Direction();
					
					gp_Pnt poncrv = HSF::ProjectPointonCurve(pnlcog,spiralcrv3d);
					double length = pnlcog.Distance(poncrv);
					double radius = MAX((1-(length / 17182.5)) * 1000 * splinerange ,10);

					TopoDS_Shape line1 = HSF::AddNewLineptpt(p1,p3);
					TopoDS_Shape line2 = HSF::AddNewLineptpt(p4,p2);

					//double percent = Min(abs(((length / 17182.5)) * splinerange ),1.0);
					double percent = Min(abs(((length / 13000)) * splinerange ),1.0);
					qDebug() << percent;
					
					gp_Pnt inp1 = HSF::AddNewPointonCurve(line1,0.1);
					gp_Pnt inp2 = HSF::AddNewPointonCurve(line2,MAX(1-percent,0.5));
					gp_Pnt inp3 = HSF::AddNewPointonCurve(line1,0.9);
					gp_Pnt inp4 = HSF::AddNewPointonCurve(line2,Min(percent,0.5));


				
	 				linestream << "panel" << counter << "," 
					<< p1.X() << "," << p1.Y() << "," << p1.Z()<< ","
					<< p2.X() << "," << p2.Y() << "," << p2.Z()<< ","
					<< p3.X() << "," << p3.Y() << "," << p3.Z()<< ","
					<< p4.X() << "," << p4.Y() << "," << p4.Z()<< ","
					<< inp1.X() << "," << inp1.Y() << "," << inp1.Z()<< ","
					<< inp2.X() << "," << inp2.Y() << "," << inp2.Z()<< ","
					<< inp3.X() << "," << inp3.Y() << "," << inp3.Z()<< ","
					<< inp4.X() << "," << inp4.Y() << "," << inp4.Z()<< "\n";
						
					qDebug() << "diag1" << p1.Distance(p3);
					qDebug() << "diag2" << p4.Distance(p2);

					if ( p1.Distance(p3) > 30000 || p4.Distance(p2) > 30000) continue;
					
					gp_Vec p1v = hsf::getVectorNormaltoSurfaceatPoint(SRF,p1);
					gp_Vec p2v = hsf::getVectorNormaltoSurfaceatPoint(SRF,p2);
					gp_Vec p3v = hsf::getVectorNormaltoSurfaceatPoint(SRF,p3);
					gp_Vec p4v = hsf::getVectorNormaltoSurfaceatPoint(SRF,p4);
	

					double thickness = -800;

					gp_Pnt p1far = hsf::MovePointByVector(p1,p1v,thickness);
					gp_Pnt p2far = hsf::MovePointByVector(p2,p2v,thickness);
					gp_Pnt p3far = hsf::MovePointByVector(p3,p3v,thickness);
					gp_Pnt p4far = hsf::MovePointByVector(p4,p4v,thickness);
					
					TopoDS_Shape line1far = HSF::AddNewLineptpt(p1far,p3far);
					TopoDS_Shape line2far = HSF::AddNewLineptpt(p4far,p2far);

					gp_Pnt inp1far = HSF::AddNewPointonCurve(line1far,0.1);
					gp_Pnt inp2far = HSF::AddNewPointonCurve(line2far,MAX(1-percent,0.5));
					gp_Pnt inp3far = HSF::AddNewPointonCurve(line1far,0.9);
					gp_Pnt inp4far = HSF::AddNewPointonCurve(line2far,Min(percent,0.5));


					pointlist(panelist1)
					panelist1 << p1 << p2 << p3 << p4 
					<< inp1 << inp2 << inp3 << inp4 << centerp;
					QList<TopoDS_Shape> edgelist1;
					
					

					pointlist(panelist2)
					panelist2 << p1far << p2far << p3far << p4far 
					<< inp1far << inp2far << inp3far << inp4far << centerp;
					QList<TopoDS_Shape> edgelist2;
					
					TopoDS_Shape panelfront; 
					TopoDS_Shape panelback; 

					bool isflat=false;
					if(percent > 0.45)
						{
						isflat = true;
					panelfront = createflatpanel(panelist1,edgelist1);
					panelback = createflatpanel(panelist2,edgelist2);
						}
					else
						{
						isflat = false;
					panelfront = createpanel(panelist1,edgelist1);
					panelback = createpanel(panelist2,edgelist2);
						}
					

					
					

					BRep_Builder B;
					TopoDS_Shell Sh;	
					B.MakeShell(Sh);	
					
					int ecount1 = edgelist1.count();
					int ecount2 = edgelist2.count();
					int edgeloop = Min(ecount1,ecount2);

					if (ecount1 < 1 || ecount2 < 1 || ecount1 != ecount2) continue;
					for (int k=0;k<edgeloop;k++)
						{
							TopoDS_Shape shp1 = edgelist1.at(k);
							TopoDS_Shape shp2 = edgelist2.at(k);
							TopoDS_Shape face1 = hsf::AddNewBlendSurface(shp1,shp2);
							if(!face1.IsNull()) B.Add(Sh,face1);						
						}
					TopoDS_Solid Sol;	
					B.MakeSolid(Sol);
					if(!panelfront.IsNull()) B.Add(Sol,panelfront);
					if(!panelback.IsNull()) B.Add(Sol,panelback);
					if(!Sh.IsNull())B.Add(Sol,Sh);

					//vis(Sol);

					//shapes->Append(panel);
					if (isflat)
						{
					exportlist2 << Sol;
						}
					else
						{
					exportlist << Sol;
						}

					pointlist(inplist)
					inplist << inp1 << inp2 << inp3 << inp4 << inp1;
					TopoDS_Shape inpline = HSF::AddNewPolyline(inplist);
					vis(inpline);

				
					TopoDS_Shape pline = HSF::AddNewPolyline(polyline);

					int freemem =  Standard::Purge();
					if(freemem > 0 ) qDebug() << "Deallocated:" << freemem;
					//vis(pline);
					
					//TopoDS_Shape circ = HSF::AddNewCircle(centerp,pvec,radius);
					//vis(circ);
					//B.Add(multiobject,circ);
					//qDebug() << length;
					}

					} catch(...) {continue;}

				}
			}
				//return multiobject;
			}

			/*out << line ;
			file.close();*/

         //return TopoDS_Shape();

		    //QString patternpath = QoccApplication::instance()->applicationDirPath() + QString("/pattern.stp"); 
			/*QString patternpath = QFileDialog::getSaveFileName(this, tr("Save Topology"), " ",tr("iges (*.iges);; All (*.*);;"));

io_man->exportModel( QoccInputOutput::FormatIGES, patternpath, shapes );*/
		ENDPART
			
		}
void apdl_mex_design_001::BuildPanels3()
{
		
		QoccInputOutput* io_man = new QoccInputOutput();
		Handle(TopTools_HSequenceOfShape) shapes = new TopTools_HSequenceOfShape();

		QString fileName = QFileDialog::getSaveFileName(this, tr("Save Topology"), " ",tr("csv (*.csv "));

	 QFile file(fileName);
	 if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
     return;

     QTextStream out(&file);
	 QString line;
	 QTextStream linestream(&line);
	 
	 
	linestream << "panel name" << "," 
	<< "p1.X()" << "," << "p1.Y()" << "," << "p1.Z()"<< ","
	<< "p2.X()" << "," << "p2.Y()" << "," << "p2.Z()"<< ","
	<< "p3.X()" << "," << "p3.Y()" << "," << "p3.Z()"<< ","
	<< "p4.X()" << "," << "p4.Y()" << "," << "p4.Z()"<< ","
	<< "inp1.X()" << "," << "inp1.Y()" << "," << "inp1.Z()"<< ","
	<< "inp2.X()" << "," << "inp2.Y()" << "," << "inp2.Z()"<< ","
	<< "inp3.X()" << "," << "inp3.Y()" << "," << "inp3.Z()"<< ","
	<< "inp4.X()" << "," << "inp4.Y()" << "," << "inp4.Z()"<< "\n";

	  





		
		INITPART
		if (pointmap.count() > 0 ){

		double splinerange = get(splinerange); splinerange /=100;
		double spiralxoffset = get(spiralxoffset)
		double spiralyoffset = get(spiralyoffset)
		double spiralinclination = get(spiralinclination)

		double ucount = get(griducount)

		bool showsurface = ui.vizsrf->isChecked();
		bool showuvlines = ui.vizuvlines->isChecked();
		bool show3dcurves = ui.viz3dcurves->isChecked();
		bool showintp = ui.vizintp->isChecked();
	
		if(showsurface) vis(SRF) // visualize the surface

		gp_Pnt sp1(1+(spiralxoffset/100),0+(spiralyoffset/100),0);
		gp_Pnt sp2(0+(spiralxoffset/100),1+(spiralyoffset/100)+(spiralinclination/100),0);
		TopoDS_Shape spiralcrv2d = HSF::AddNewLineptpt(sp1,sp2);
		spiralcrv3d = HSF::applyuvcurve(spiralcrv2d,SRF);
		

		spiralcrv3d  = SPIRAL;
		vis(spiralcrv3d);



		/*	TopoDS_Compound multiobject;
			BRep_Builder B;
			B.MakeCompound(multiobject);*/


			
		int counter = 0;		
		
			QMapIterator<QString,gp_Pnt> i(pointmap);
			while (i.hasNext()){
			i.next();
			counter ++;
			//if (counter > 30) break;
			QString currentname = i.key();
			QString epn ;
			QString spn ;
			QString wpn ;

			 QString vpart = currentname.split("_").at(1);
			 vpart = vpart.right(vpart.length() -1);
			 int vpartnum = vpart.toInt();

			 if( vpartnum % 2 == 0) // even
				 {
	
				epn = HSF::GetNextUvName(currentname,0,1);
				spn = HSF::GetNextUvName(currentname,0,2);
				wpn = HSF::GetNextUvName(currentname,-1,1);

				 } else { // odd
				epn = HSF::GetNextUvName(currentname,1,1);
				spn = HSF::GetNextUvName(currentname,0,2);
				wpn = HSF::GetNextUvName(currentname,0,1);
				 }


		

			ui::getInstance()->Statusbar(currentname);

			


			
			if(pointmap.contains(epn) && pointmap.contains(spn) && pointmap.contains(wpn)){
				gp_Pnt p1 = i.value();
				gp_Pnt p2 = pointmap.value(epn);
				gp_Pnt p3 = pointmap.value(spn);
				gp_Pnt p4 = pointmap.value(wpn);

				pointlist(polyline);
				polyline << p1 << p2 << p3 << p4 << p1;
				try {
				//TopoDS_Shape pline = HSF::AddNewPolyline(polyline);
				//TopoDS_Shape panel = HSF::AddNewFillSurface(pline);
				gp_Pln avrgplane = HSF::AddNewMeanPlane(polyline);

			/*	if (pline.IsNull()) continue;
				B.Add(multiobject,pline);*/
				
				if (!spiralcrv3d.IsNull())
					{
					gp_Pnt pnlcog = HSF::GetAveragePoint(polyline);
					gp_Pnt centerp = HSF::ProjectPoint(pnlcog,avrgplane);
					//gp_Vec pvec = HSF::getVectorNormaltoSurfaceatPoint(panel,centerp);
					
					gp_Vec pvec = avrgplane.Axis().Direction();
					
					gp_Pnt poncrv = HSF::ProjectPointonCurve(pnlcog,spiralcrv3d);
					double length = pnlcog.Distance(poncrv);
					double radius = MAX((1-(length / 17182.5)) * 1000 * splinerange ,10);

					TopoDS_Shape line1 = HSF::AddNewLineptpt(p1,p3);
					TopoDS_Shape line2 = HSF::AddNewLineptpt(p4,p2);

					double percent = Min(abs(((length / 17182.5)) * splinerange ),1.0);
					
					gp_Pnt inp1 = HSF::AddNewPointonCurve(line1,0.1);
					gp_Pnt inp2 = HSF::AddNewPointonCurve(line2,1-percent);
					gp_Pnt inp3 = HSF::AddNewPointonCurve(line1,0.9);
					gp_Pnt inp4 = HSF::AddNewPointonCurve(line2,percent);


				
	 				linestream << "panel" << counter << "," 
					<< p1.X() << "," << p1.Y() << "," << p1.Z()<< ","
					<< p2.X() << "," << p2.Y() << "," << p2.Z()<< ","
					<< p3.X() << "," << p3.Y() << "," << p3.Z()<< ","
					<< p4.X() << "," << p4.Y() << "," << p4.Z()<< ","
					<< inp1.X() << "," << inp1.Y() << "," << inp1.Z()<< ","
					<< inp2.X() << "," << inp2.Y() << "," << inp2.Z()<< ","
					<< inp3.X() << "," << inp3.Y() << "," << inp3.Z()<< ","
					<< inp4.X() << "," << inp4.Y() << "," << inp4.Z()<< "\n";

		

					
					Standard_Real radius2 = inp1.Distance(inp3)/6; 
					if (radius2 > 1000.0) continue;
					qDebug() << "radius:" << radius2;
					TopoDS_Shape corner1 = HSF::AddNewCorner(inp4,inp1,inp2,radius2,centerp);
					TopoDS_Shape corner2 = HSF::AddNewCorner(inp1,inp2,inp3,radius2,centerp);
					TopoDS_Shape corner3 = HSF::AddNewCorner(inp2,inp3,inp4,radius2,centerp);
					TopoDS_Shape corner4 = HSF::AddNewCorner(inp3,inp4,inp1,radius2,centerp);
					
					/*vis(corner1)
					vis(corner2)
					vis(corner3)
					vis(corner4)*/

					gp_Pnt corner1start = HSF::AddNewPointonCurve(corner1,0.0);
					gp_Pnt corner1mid = HSF::AddNewPointonCurve(corner1,0.5);
					gp_Pnt corner1end = HSF::AddNewPointonCurve(corner1,1.0);
					

					gp_Pnt corner2start = HSF::AddNewPointonCurve(corner2,0.0);
					gp_Pnt corner2mid = HSF::AddNewPointonCurve(corner2,0.5);
					gp_Pnt corner2end = HSF::AddNewPointonCurve(corner2,1.0);

					gp_Pnt corner3start = HSF::AddNewPointonCurve(corner3,0.0);
					gp_Pnt corner3mid = HSF::AddNewPointonCurve(corner3,0.5);
					gp_Pnt corner3end = HSF::AddNewPointonCurve(corner3,1.0);

					gp_Pnt corner4start = HSF::AddNewPointonCurve(corner4,0.0);
					gp_Pnt corner4mid = HSF::AddNewPointonCurve(corner4,0.5);
					gp_Pnt corner4end = HSF::AddNewPointonCurve(corner4,1.0);


					TopoDS_Edge edge1 = HSF::AddNewLineptpt(p1,p2);
					TopoDS_Edge edge2 = HSF::AddNewLineptpt(p2,p3);
					TopoDS_Edge edge3 = HSF::AddNewLineptpt(p3,p4);
					TopoDS_Edge edge4 = HSF::AddNewLineptpt(p4,p1);



//-------------------------------					
					gp_Pnt corner1endfar = HSF::ProjectPointonCurve(corner1end,edge1);
					TopoDS_Shape e1seg1far = HSF::AddNewLineptpt(p1,corner1endfar);

					gp_Pnt corner2startfar = HSF::ProjectPointonCurve(corner2start,edge1);
					TopoDS_Shape e1seg2far = HSF::AddNewLineptpt(corner1endfar,corner2startfar);
					TopoDS_Shape e1seg3far = HSF::AddNewLineptpt(corner2startfar,p2);

//-------------------------------
					gp_Pnt corner2endfar = HSF::ProjectPointonCurve(corner2end,edge2);
					TopoDS_Shape e2seg1far = HSF::AddNewLineptpt(p2,corner2endfar);

					gp_Pnt corner3startfar = HSF::ProjectPointonCurve(corner3start,edge2);
					TopoDS_Shape e2seg2far = HSF::AddNewLineptpt(corner2endfar,corner3startfar);
					TopoDS_Shape e2seg3far = HSF::AddNewLineptpt(corner3startfar,p3);

//-------------------------------
					gp_Pnt corner3endfar = HSF::ProjectPointonCurve(corner3end,edge3);
					TopoDS_Shape e3seg1far = HSF::AddNewLineptpt(p3,corner3endfar);

					gp_Pnt corner4startfar = HSF::ProjectPointonCurve(corner4start,edge3);
					TopoDS_Shape e3seg2far = HSF::AddNewLineptpt(corner3endfar,corner4startfar);
					TopoDS_Shape e3seg3far = HSF::AddNewLineptpt(corner4startfar,p4);

//-------------------------------
					gp_Pnt corner4endfar = HSF::ProjectPointonCurve(corner4end,edge4);
					TopoDS_Shape e4seg1far = HSF::AddNewLineptpt(p4,corner4endfar);

					gp_Pnt corner1startfar = HSF::ProjectPointonCurve(corner1start,edge4);
					TopoDS_Shape e4seg2far = HSF::AddNewLineptpt(corner4endfar,corner1startfar);
					TopoDS_Shape e4seg3far = HSF::AddNewLineptpt(corner1startfar,p1);

//-------------------------------
					


					geo(e1seg2near,Lineptpt(corner1end,corner2start))
					geo(e2seg2near,Lineptpt(corner2end,corner3start))
					geo(e3seg2near,Lineptpt(corner3end,corner4start))
					geo(e4seg2near,Lineptpt(corner4end,corner1start))

double fp,lp;
TopoDS_Edge corner1a =  BRepBuilderAPI_MakeEdge(BRep_Tool::Curve(TopoDS::Edge(corner1),fp,lp),corner1start,corner1mid);  
TopoDS_Edge corner1b =  BRepBuilderAPI_MakeEdge(BRep_Tool::Curve(TopoDS::Edge(corner1),fp,lp),corner1mid,corner1end);   

TopoDS_Edge corner2a =  BRepBuilderAPI_MakeEdge(BRep_Tool::Curve(TopoDS::Edge(corner2),fp,lp),corner2start,corner2mid);  
TopoDS_Edge corner2b =  BRepBuilderAPI_MakeEdge(BRep_Tool::Curve(TopoDS::Edge(corner2),fp,lp),corner2mid,corner2end);   

TopoDS_Edge corner3a =  BRepBuilderAPI_MakeEdge(BRep_Tool::Curve(TopoDS::Edge(corner3),fp,lp),corner3start,corner3mid);  
TopoDS_Edge corner3b =  BRepBuilderAPI_MakeEdge(BRep_Tool::Curve(TopoDS::Edge(corner3),fp,lp),corner3mid,corner3end);   

TopoDS_Edge corner4a =  BRepBuilderAPI_MakeEdge(BRep_Tool::Curve(TopoDS::Edge(corner4),fp,lp),corner4start,corner4mid);  
TopoDS_Edge corner4b =  BRepBuilderAPI_MakeEdge(BRep_Tool::Curve(TopoDS::Edge(corner4),fp,lp),corner4mid,corner4end);


TopoDS_Shape e1seg1srf = HSF::AddNewBlendSurface(corner1b,e1seg1far);
TopoDS_Shape e1seg2srf = HSF::AddNewBlendSurface(e1seg2near,e1seg2far);
TopoDS_Shape e1seg3srf = HSF::AddNewBlendSurface(corner2a,e1seg3far);

TopoDS_Shape e2seg1srf = HSF::AddNewBlendSurface(corner2b,e2seg1far);
TopoDS_Shape e2seg2srf = HSF::AddNewBlendSurface(e2seg2near,e2seg2far);
TopoDS_Shape e2seg3srf = HSF::AddNewBlendSurface(corner3a,e2seg3far);

TopoDS_Shape e3seg1srf = HSF::AddNewBlendSurface(corner3b,e3seg1far);
TopoDS_Shape e3seg2srf = HSF::AddNewBlendSurface(e3seg2near,e3seg2far);
TopoDS_Shape e3seg3srf = HSF::AddNewBlendSurface(corner4a,e3seg3far);

TopoDS_Shape e4seg1srf = HSF::AddNewBlendSurface(corner4b,e4seg1far);
TopoDS_Shape e4seg2srf = HSF::AddNewBlendSurface(e4seg2near,e4seg2far);
TopoDS_Shape e4seg3srf = HSF::AddNewBlendSurface(corner1a,e4seg3far);


//vis(e1seg1srf)
//vis(e1seg2srf)
//vis(e1seg3srf)
//
//vis(e2seg1srf)
//vis(e2seg2srf)
//vis(e2seg3srf)
//
//vis(e3seg1srf)
//vis(e3seg2srf)
//vis(e3seg3srf)
//
//vis(e4seg1srf)
//vis(e4seg2srf)
//vis(e4seg3srf)







					
					TopoDS_Shape e1 = HSF::AddNewLineptpt(p1,inp1);
					TopoDS_Shape e2 = HSF::AddNewLineptpt(p2,inp2);
					TopoDS_Shape e3 = HSF::AddNewLineptpt(p3,inp3);
					TopoDS_Shape e4 = HSF::AddNewLineptpt(p4,inp4);
						
			
					

					pointlist(inplist)
					inplist << inp1 << inp2 << inp3 << inp4 << inp1;
					TopoDS_Shape inpline = HSF::AddNewPolyline(inplist);
					vis(inpline);

				
					TopoDS_Shape pline = HSF::AddNewPolyline(polyline);
					vis(pline);
					
					//TopoDS_Shape circ = HSF::AddNewCircle(centerp,pvec,radius);
					//vis(circ);
					//B.Add(multiobject,circ);
					qDebug() << length;
					}

					} catch(...) {continue;}

				}
			}
				//return multiobject;
			}

			out << line ;
			file.close();

         //return TopoDS_Shape();

		    //QString patternpath = QoccApplication::instance()->applicationDirPath() + QString("/pattern.igs"); 

			//io_man->exportModel( QoccInputOutput::FormatIGES, patternpath, shapes );
		ENDPART
			
		}
void apdl_mex_design_001::updategrid()
	{

	}

TopoDS_Shape apdl_mex_design_001::createpanel(QList<gp_Pnt> &inputlist,
											  QList<TopoDS_Shape> &outputlist)
	{

		TopoDS_Shape result;
		if(inputlist.isEmpty() || inputlist.count()-1 < 8) return result;
		
		//outputlist.clear();



		gp_Pnt p1 = inputlist.at(0);
		gp_Pnt p2 = inputlist.at(1);
		gp_Pnt p3 = inputlist.at(2);
		gp_Pnt p4 = inputlist.at(3);

		gp_Pnt inp1 = inputlist.at(4);
		gp_Pnt inp2 = inputlist.at(5);
		gp_Pnt inp3 = inputlist.at(6);
		gp_Pnt inp4 = inputlist.at(7);
		gp_Pnt centerp = inputlist.at(8);


		Standard_Real radius2 = inp1.Distance(inp3)/6; 
					if (radius2 > 1000.0) return result;
					//qDebug() << "radius:" << radius2;
					TopoDS_Shape corner1 = HSF::AddNewCorner(inp4,inp1,inp2,radius2,centerp);
					TopoDS_Shape corner2 = HSF::AddNewCorner(inp1,inp2,inp3,radius2,centerp);
					TopoDS_Shape corner3 = HSF::AddNewCorner(inp2,inp3,inp4,radius2,centerp);
					TopoDS_Shape corner4 = HSF::AddNewCorner(inp3,inp4,inp1,radius2,centerp);

					outputlist << corner1 << corner2 << corner3 << corner4;

				/*	
					vis(corner1)
					vis(corner2)
					vis(corner3)
					vis(corner4)*/

					gp_Pnt corner1start = HSF::AddNewPointonCurve(corner1,0.0);
					gp_Pnt corner1mid = HSF::AddNewPointonCurve(corner1,0.5);
					gp_Pnt corner1end = HSF::AddNewPointonCurve(corner1,1.0);
					

					gp_Pnt corner2start = HSF::AddNewPointonCurve(corner2,0.0);
					gp_Pnt corner2mid = HSF::AddNewPointonCurve(corner2,0.5);
					gp_Pnt corner2end = HSF::AddNewPointonCurve(corner2,1.0);

					gp_Pnt corner3start = HSF::AddNewPointonCurve(corner3,0.0);
					gp_Pnt corner3mid = HSF::AddNewPointonCurve(corner3,0.5);
					gp_Pnt corner3end = HSF::AddNewPointonCurve(corner3,1.0);

					gp_Pnt corner4start = HSF::AddNewPointonCurve(corner4,0.0);
					gp_Pnt corner4mid = HSF::AddNewPointonCurve(corner4,0.5);
					gp_Pnt corner4end = HSF::AddNewPointonCurve(corner4,1.0);


					TopoDS_Shape edge1 = HSF::AddNewLineptpt(p1,p2);
					TopoDS_Shape edge2 = HSF::AddNewLineptpt(p3,p2);
					TopoDS_Shape edge3 = HSF::AddNewLineptpt(p4,p3);
					TopoDS_Shape edge4 = HSF::AddNewLineptpt(p4,p1);

					outputlist << edge1 << edge2 << edge3 << edge4 ;

//-------------------------------					
					gp_Pnt corner1endfar = HSF::ProjectPointonCurve(corner1end,edge1);
					TopoDS_Shape e1seg1far = HSF::AddNewLineptpt(p1,corner1endfar);

					gp_Pnt corner2startfar = HSF::ProjectPointonCurve(corner2start,edge1);
					TopoDS_Shape e1seg2far = HSF::AddNewLineptpt(corner1endfar,corner2startfar);
					TopoDS_Shape e1seg3far = HSF::AddNewLineptpt(corner2startfar,p2);

					//outputlist << e1seg1far << e1seg2far << e1seg3far;

//-------------------------------
					gp_Pnt corner2endfar = HSF::ProjectPointonCurve(corner2end,edge2);
					TopoDS_Shape e2seg1far = HSF::AddNewLineptpt(p2,corner2endfar);

					gp_Pnt corner3startfar = HSF::ProjectPointonCurve(corner3start,edge2);
					TopoDS_Shape e2seg2far = HSF::AddNewLineptpt(corner2endfar,corner3startfar);
					TopoDS_Shape e2seg3far = HSF::AddNewLineptpt(corner3startfar,p3);

					//outputlist << e2seg1far << e2seg2far << e2seg3far;
//-------------------------------
					gp_Pnt corner3endfar = HSF::ProjectPointonCurve(corner3end,edge3);
					TopoDS_Shape e3seg1far = HSF::AddNewLineptpt(p3,corner3endfar);

					gp_Pnt corner4startfar = HSF::ProjectPointonCurve(corner4start,edge3);
					TopoDS_Shape e3seg2far = HSF::AddNewLineptpt(corner3endfar,corner4startfar);
					TopoDS_Shape e3seg3far = HSF::AddNewLineptpt(corner4startfar,p4);
					//outputlist << e3seg1far << e3seg2far << e3seg3far;
//-------------------------------
					gp_Pnt corner4endfar = HSF::ProjectPointonCurve(corner4end,edge4);
					TopoDS_Shape e4seg1far = HSF::AddNewLineptpt(p4,corner4endfar);

					gp_Pnt corner1startfar = HSF::ProjectPointonCurve(corner1start,edge4);
					TopoDS_Shape e4seg2far = HSF::AddNewLineptpt(corner4endfar,corner1startfar);
					TopoDS_Shape e4seg3far = HSF::AddNewLineptpt(corner1startfar,p1);

					//outputlist << e4seg1far << e4seg2far << e4seg3far;

//-------------------------------
					


					geo(e1seg2near,Lineptpt(corner1end,corner2start))
					geo(e2seg2near,Lineptpt(corner2end,corner3start))
					geo(e3seg2near,Lineptpt(corner3end,corner4start))
					geo(e4seg2near,Lineptpt(corner4end,corner1start))

					outputlist << e1seg2near << e2seg2near << e3seg2near << e4seg2near;



double fp,lp;
TopoDS_Shape corner1a =  BRepBuilderAPI_MakeEdge(BRep_Tool::Curve(TopoDS::Edge(corner1),fp,lp),corner1start,corner1mid);  
TopoDS_Shape corner1b =  BRepBuilderAPI_MakeEdge(BRep_Tool::Curve(TopoDS::Edge(corner1),fp,lp),corner1mid,corner1end);   

TopoDS_Shape corner2a =  BRepBuilderAPI_MakeEdge(BRep_Tool::Curve(TopoDS::Edge(corner2),fp,lp),corner2start,corner2mid);  
TopoDS_Shape corner2b =  BRepBuilderAPI_MakeEdge(BRep_Tool::Curve(TopoDS::Edge(corner2),fp,lp),corner2mid,corner2end);   

TopoDS_Shape corner3a =  BRepBuilderAPI_MakeEdge(BRep_Tool::Curve(TopoDS::Edge(corner3),fp,lp),corner3start,corner3mid);  
TopoDS_Shape corner3b =  BRepBuilderAPI_MakeEdge(BRep_Tool::Curve(TopoDS::Edge(corner3),fp,lp),corner3mid,corner3end);   

TopoDS_Shape corner4a =  BRepBuilderAPI_MakeEdge(BRep_Tool::Curve(TopoDS::Edge(corner4),fp,lp),corner4start,corner4mid);  
TopoDS_Shape corner4b =  BRepBuilderAPI_MakeEdge(BRep_Tool::Curve(TopoDS::Edge(corner4),fp,lp),corner4mid,corner4end);

//outputlist << corner1a <<corner1b << corner2a << corner2b << corner3a << corner3b << corner4a << corner4b;

TopoDS_Shape e1seg1srf = HSF::AddNewBlendSurface(corner1b,e1seg1far);
TopoDS_Shape e1seg2srf = HSF::AddNewBlendSurface(e1seg2near,e1seg2far);
TopoDS_Shape e1seg3srf = HSF::AddNewBlendSurface(corner2a,e1seg3far);

TopoDS_Shape e2seg1srf = HSF::AddNewBlendSurface(corner2b,e2seg1far);
TopoDS_Shape e2seg2srf = HSF::AddNewBlendSurface(e2seg2near,e2seg2far);
TopoDS_Shape e2seg3srf = HSF::AddNewBlendSurface(corner3a,e2seg3far);

TopoDS_Shape e3seg1srf = HSF::AddNewBlendSurface(corner3b,e3seg1far);
TopoDS_Shape e3seg2srf = HSF::AddNewBlendSurface(e3seg2near,e3seg2far);
TopoDS_Shape e3seg3srf = HSF::AddNewBlendSurface(corner4a,e3seg3far);

TopoDS_Shape e4seg1srf = HSF::AddNewBlendSurface(corner4b,e4seg1far);
TopoDS_Shape e4seg2srf = HSF::AddNewBlendSurface(e4seg2near,e4seg2far);
TopoDS_Shape e4seg3srf = HSF::AddNewBlendSurface(corner1a,e4seg3far);


//vis(e1seg1srf)
//vis(e1seg2srf)
//vis(e1seg3srf)
//
//vis(e2seg1srf)
//vis(e2seg2srf)
//vis(e2seg3srf)
//
//vis(e3seg1srf)
//vis(e3seg2srf)
//vis(e3seg3srf)
//
//vis(e4seg1srf)
//vis(e4seg2srf)
//vis(e4seg3srf)

BRep_Builder B;

//Shell	
TopoDS_Shell Sh;	
B.MakeShell(Sh);	

B.Add(Sh,e1seg1srf);
B.Add(Sh,e1seg2srf);
B.Add(Sh,e1seg3srf);

B.Add(Sh,e2seg1srf);
B.Add(Sh,e2seg2srf);
B.Add(Sh,e2seg3srf);

B.Add(Sh,e3seg1srf);
B.Add(Sh,e3seg2srf);
B.Add(Sh,e3seg3srf);

B.Add(Sh,e4seg1srf);
B.Add(Sh,e4seg2srf);
B.Add(Sh,e4seg3srf);

// Solid	
/*TopoDS_Solid Sol;	
B.MakeSolid(Sol);	
B.Add(Sol,Sh);*/	

int outputcount = outputlist.count();


return Sh;

	}



	TopoDS_Shape apdl_mex_design_001::createflatpanel(QList<gp_Pnt> &inputlist,
											  QList<TopoDS_Shape> &outputlist)
	{

		TopoDS_Shape result;
		if(inputlist.isEmpty() || inputlist.count()-1 < 8) return result;
		
		//outputlist.clear();



		gp_Pnt p1 = inputlist.at(0);
		gp_Pnt p2 = inputlist.at(1);
		gp_Pnt p3 = inputlist.at(2);
		gp_Pnt p4 = inputlist.at(3);

		TopoDS_Shape edge1 = HSF::AddNewLineptpt(p1,p2);
		TopoDS_Shape edge2 = HSF::AddNewLineptpt(p3,p2);
		TopoDS_Shape edge3 = HSF::AddNewLineptpt(p4,p3);
		TopoDS_Shape edge4 = HSF::AddNewLineptpt(p4,p1);

		outputlist << edge1 << edge2 << edge3 << edge4 ;

		TopoDS_Shape face1 = hsf::AddNewBlendSurface(edge1,edge3);



BRep_Builder B;

//Shell	
TopoDS_Shell Sh;	
B.MakeShell(Sh);	

B.Add(Sh,face1);


int outputcount = outputlist.count();


return Sh;

	}



void apdl_mex_design_001::importexport()
{


	QString		fileName;
	QString		fileType;
	QFileInfo	fileInfo;

	QoccInputOutput::FileFormat format;
	QoccInputOutput reader;

 

	fileName = QFileDialog::getOpenFileName ( this,
				  							  tr("Import File"),
											 "",
											  tr( "All drawing types (*.brep *.rle *.igs *iges *.stp *.step *stl);;"
											      "BREP (*.brep *.rle);;"
												  "STEP (*.step *.stp);;"
												  "IGES (*.iges *.igs);;"
												  "STL (*.stl)" ));
	if (!fileName.isEmpty())
	{
		fileInfo.setFile(fileName);
		fileType = fileInfo.suffix();
		if (fileType.toLower() == tr("brep") || fileType.toLower() == tr("rle"))
		{
			format = QoccInputOutput::FormatBREP;
		}
		if (fileType.toLower() == tr("step") || fileType.toLower() == tr("stp"))
		{
			format = QoccInputOutput::FormatSTEP;
		}
		if (fileType.toLower() == tr("iges") || fileType.toLower() == tr("igs"))
		{
			format = QoccInputOutput::FormatIGES;
		}
		if (fileType.toLower() == tr("stl"))
		{
			format = QoccInputOutput::FormatSTL;
		}
		
		
		
		
Handle(TopTools_HSequenceOfShape) famshape1 = reader.importIGES(fileInfo.absoluteFilePath());

TopoDS_Shape PNL = famshape1->Value(1);

Handle(TopTools_HSequenceOfShape) shapes = new TopTools_HSequenceOfShape();

shapes->Append(PNL);

QString patternpath = QFileDialog::getSaveFileName(this, tr("Save Topology"), " ",tr("iges (*.iges);; All (*.*);;"));

reader.exportModel( QoccInputOutput::FormatSTL, patternpath, shapes );
}
}

void apdl_mex_design_001::exportstl()
	{
	QFileInfo fileInfo;

QString	fileName = QFileDialog::getSaveFileName(this, tr("Import File"),"",tr( "All drawing types (*.brep *.rle *.igs *iges *.stp *.step *stl);;"
											      "BREP (*.brep *.rle);;"
												  "STEP (*.step *.stp);;"
												  "IGES (*.iges *.igs);;"
												  "STL (*.stl)" ));

	fileInfo.setFile(fileName);

	QString dirname = fileInfo.absoluteDir().absolutePath();
	QString filename = fileInfo.fileName();
	QString basename = fileInfo.baseName();
	QString bundlename = fileInfo.bundleName();

	if (exportlist.count() > 0)
		{

		QoccInputOutput::FileFormat format;
		QoccInputOutput reader;

		 int shapecount = exportlist.count()-1;
		 int chunk = 30;
		 int numfiles = floor((double) shapecount / (double) chunk);
		 int shapecounter=0;
		 int filecounter = 0;
		 Handle(TopTools_HSequenceOfShape) shapes = new TopTools_HSequenceOfShape();

		 QListIterator<TopoDS_Shape> i(exportlist);
		 while (i.hasNext())
			 {
				shapecounter++;
				TopoDS_Shape current = i.next();
				shapes->Append(current);

				if(shapecounter == chunk)
					{
					
					shapecounter = 0;
					filecounter++;

					QString curfilename = dirname + tr("/") + basename + QString::number(filecounter) + tr(".stp");
					reader.exportModel( QoccInputOutput::FormatSTEP, curfilename, shapes );


					shapes->Clear();
					}
				

				
			 }

		 shapecount = exportlist2.count()-1;
		 chunk = 30;
		 numfiles = floor((double) shapecount / (double) chunk);
		 shapecounter=0;
		 //filecounter = 0;
		 shapes->Clear();
		 
		 QListIterator<TopoDS_Shape> j(exportlist2);
		 while (j.hasNext())
			 {
				shapecounter++;
				TopoDS_Shape current = j.next();
				shapes->Append(current);

				if(shapecounter == chunk)
					{
					
					shapecounter = 0;
					filecounter++;

					QString curfilename = dirname + tr("/") + basename + QString::number(filecounter) + tr("_Flat.stp");
					reader.exportModel( QoccInputOutput::FormatSTEP, curfilename, shapes );


					shapes->Clear();
					}
				

				
			 }

		


		}


	}
	
	
