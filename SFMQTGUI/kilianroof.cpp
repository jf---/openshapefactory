#include "kilianroof.h"
#include "ui.h"
#include "shapefactory.h"
#include "parametricsfordummies.h"
#include "bash.h"
#include <QFileDialog>
#include <QProgressDialog>
#include <QTime>
#include <QDebug>

#include <StlMesh_Mesh.hxx>
#include <MeshVS_TextPrsBuilder.hxx> 


#include <RWStl.hxx>
#include <OSD_Path.hxx>

#include <MeshVS_Drawer.hxx>
#include <MeshVS_DrawerAttribute.hxx>
#include <MeshVS_MeshPrsBuilder.hxx>
#include <XSDRAWSTLVRML_DataSource.hxx>
#include <Graphic3d_MaterialAspect.hxx>
#include <MeshVS_ElementalColorPrsBuilder.hxx>

#include <ShapeUpgrade_ShapeDivideContinuity.hxx>
#include <ShapeUpgrade_ShapeDivideArea.hxx>
#include <ShapeFix.hxx>

#include "AIS_Gauss.hxx"


// this is for the use of the KMlocal library for kmeans clustering 
// experiment starting on august 30 2011

#include <cstdlib>			// C standard includes
#include <iostream>			// C++ I/O
#include <string>			// C++ strings
#include "KMlocal.h"			// k-means algorithms

using namespace std;			// make std:: available

// execution parameters (see KMterm.h and KMlocal.h)
KMterm  term(100, 0, 0, 0,              // run for 100 stages
             0.10, 0.10, 3,             // other typical parameter values 
             0.50, 10, 0.95);




void kmeanscluster(int familycount,QList<panelinstance> &mypanels)
{
	
	int		k	= familycount;			// number of centers
    int		dim	= mypanels.at(0).parameters.count();			// dimension
    int		nPts	= mypanels.count();			// number of data points

    KMdata dataPts(dim, nPts);			// allocate data storage

	KMpointArray	pa = dataPts.getPts();
	for (int i = 0; i < nPts; i++) {
	panelinstance curpanel = mypanels.at(i);
	for (int d = 0; d < dim; d++) {
		double curparm = curpanel.parameters.at(d);
	    pa[i][d] = curparm;
	}
    }

   // kmUniformPts(dataPts.getPts(), nPts, dim);	// generate random points

	KMdataArray myarr = dataPts.getPts();
	KMpoint curval = myarr[1,1];

	dataPts.buildKcTree();			// build filtering structure
    KMfilterCenters ctrs(k, dataPts);		// allocate centers

    						// run the algorithm
    KMlocalLloyds       kmAlg(ctrs, term);	// repeated Lloyd's
    // KMlocalSwap      kmAlg(ctrs, term);	// Swap heuristic
    // KMlocalEZ_Hybrid kmAlg(ctrs, term);	// EZ-Hybrid heuristic
    // KMlocalHybrid    kmAlg(ctrs, term);	// Hybrid heuristic
    ctrs = kmAlg.execute();			// execute
    						// print number of stages
    cout << "Number of stages: " << kmAlg.getTotalStages() << "\n";
						// print average distortion
    cout << "Average distortion: " << ctrs.getDist()/nPts << "\n";
    ctrs.print();				// print final centers

    KMctrIdxArray closeCtr = new KMctrIdx[dataPts.getNPts()];
    double* sqDist = new double[dataPts.getNPts()];
    ctrs.getAssignments(closeCtr, sqDist);

   /* *kmOut	<< "(Cluster assignments:\n"
		<< "    Point  Center  Squared Dist\n"
		<< "    -----  ------  ------------\n";*/


    for (int i = 0; i < dataPts.getNPts(); i++) {
	int pointindex = i;
	//panelinstance &curpanel = ;
	mypanels[i].panelindex = pointindex;
	int closestcenterindex = closeCtr[i];
	double distancetocenter = sqDist[i];
	mypanels[i].centerindex = closestcenterindex;
	mypanels[i].distancetocenter = distancetocenter;
    }

 /*   *kmOut << ")\n";*/
    
	delete [] closeCtr;
    delete [] sqDist;

    
}










kilianroof::kilianroof(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setup();
}

kilianroof::~kilianroof()
{
}
void kilianroof::setup()
{
SLIDERMOVED(updatemove)
SLIDERRELEASED(updatereleased)

connect(ui.angleschk,SIGNAL(stateChanged(int)),this,SLOT(updatereleased()));
connect(ui.lengthschk,SIGNAL(stateChanged(int)),this,SLOT(updatereleased()));
connect(ui.areachk,SIGNAL(stateChanged(int)),this,SLOT(updatereleased()));
connect(ui.updatechk,SIGNAL(stateChanged(int)),this,SLOT(updatereleased()));

}

void kilianroof::updatemove()
{
//updatemesh(0);
//loadsoumaya(0);
update(0);
}

void kilianroof::updatereleased()
{
//updatemesh(1);
//loadsoumaya(1);
	//breakshape();
}


void kilianroof::update(int vistate)
{
QTime time;
time.start();

INITPART


double p1x = get(p1x)
double p1y = get(p1y)
double p1z = get(p1z)
double p2x = get(p2x)
double p2y = get(p2y)
double p2z = get(p2z)
double p3x = get(p3x)
double p3y = get(p3y)
double p3z = get(p3z)
int ucount = get(ucount)
int vcount = get(vcount)
gp_Pnt p1(p1x,p1y,0);
gp_Pnt p2(p2x,p2y,0);
gp_Pnt p3(p3x,p3y,0);
pointlist(splinedata)
splinedata << p1 << p2 << p3;
TopoDS_Shape spline = hsf::AddNewSplineSShape(splinedata);
TopoDS_Shape cross1 = makesplinesection(spline,0.0,p1z/2,p1z);
TopoDS_Shape cross2 = makesplinesection(spline,0.5,p2z/2,p2z);
TopoDS_Shape cross3 = makesplinesection(spline,1.0,p3z/2,p3z);
shapelist(surfacedata)
surfacedata << cross1 << cross2 << cross3;
TopoDS_Shape newsurf = hsf::AddNewLoftSurface(surfacedata);
if (vistate == 0) 
{

if (!mastersurface.IsEqual(newsurf))
{
	mastersurface = newsurf;
	BRepTools::Clean(mastersurface);
	double res = 1/ (double) ucount;
	BRepMesh::Mesh(mastersurface, res);

}
//vis(mastersurface)



if(!agauss.IsNull()) ic->Remove(agauss,true);	
agauss = new AIS_Gauss(mastersurface,true);

ic->Display(agauss);


} else {

PANELIZE(mastersurface,ucount,vcount)
if(p1.Distance(p3) && p2.Distance(p4))
{
	TopoDS_Shape diag1 = hsf::AddNewLineptpt(p1,p3);
	TopoDS_Shape diag2 = hsf::AddNewLineptpt(p2,p4);
	double diag1length = hsf::GetLength(diag1);
	double diag2length = hsf::GetLength(diag2);
	double avrglength = (diag1length + diag2length)/2;
	double radius = avrglength / 100;

	gp_Vec circvec1 = hsf::getVectorTangentToCurveAtPoint(diag1,0);
	gp_Pnt startp1 = hsf::AddNewPointonCurve(diag1,0);
	TopoDS_Shape circle1 = hsf::AddNewCircle(startp1,circvec1,radius);
	TopoDS_Shape sweep1 = hsf::AddNewExtrude(circle1,circvec1,diag1length);
	
	gp_Vec circvec2 = hsf::getVectorTangentToCurveAtPoint(diag2,0);
	gp_Pnt startp2 = hsf::AddNewPointonCurve(diag2,0);
	TopoDS_Shape circle2 = hsf::AddNewCircle(startp2,circvec2,radius);
	TopoDS_Shape sweep2 = hsf::AddNewExtrude(circle2,circvec2,diag2length);
	if (vistate == 1) 
	{
	vis(sweep1)
	vis(sweep2)
	} else
	{
		vis(diag1)
		vis(diag2)
	}
}


ENDPANELIZE

}
ENDPART

double milliseconds = time.elapsed();
double seconds = (milliseconds/1000);
double minutes = seconds/60;
double hours = minutes/60;


QString timetxt;
QTextStream linestream(&timetxt);
linestream << "h" << hours << ":m " << minutes << ":s " << seconds << ".ms" << milliseconds; 

qDebug() << timetxt;


}
TopoDS_Shape kilianroof::makesplinesection(TopoDS_Shape edge, double percent, double width, double height)
{
	if (width < 2 || height < 2) width = height = 2;

	gp_Pnt curp = hsf::AddNewPointonCurve(edge,percent);
	gp_Pnt uppoint = hsf::MovePointByVector(curp,gp::DZ(),10);
	gp_Vec rotvec(curp,uppoint);
	gp_Vec tanvec = hsf::getVectorTangentToCurveAtPoint(edge,percent);
	TopoDS_Shape line = hsf::AddNewLineptdir(curp,tanvec,0,width);
	TopoDS_Shape Lline = hsf::rotate(line,curp,rotvec,90);
	TopoDS_Shape Rline = hsf::rotate(line,curp,rotvec,-90);
	gp_Pnt leftpoint = hsf::AddNewPointonCurve(Lline,1);
	gp_Pnt middlep = hsf::MovePointByVector(curp,gp::DZ(),height);
	gp_Pnt rightpoint = hsf::AddNewPointonCurve(Rline,1);
	pointlist(curvedata)
	curvedata << leftpoint << middlep << rightpoint;
	TopoDS_Shape spline = hsf::AddNewSplineSShape(curvedata);
	return spline;
}

void kilianroof::updatemesh(int vistate)
{


	QTime time;
time.start();

INITPART


double p1x = get(p1x)
double p1y = get(p1y)
double p1z = get(p1z)
double p2x = get(p2x)
double p2y = get(p2y)
double p2z = get(p2z)
double p3x = get(p3x)
double p3y = get(p3y)
double p3z = get(p3z)
int ucount = get(ucount)
int vcount = get(vcount)
int kval = get(famcount)

gp_Pnt p1(p1x,p1y,0);
gp_Pnt p2(p2x,p2y,0);
gp_Pnt p3(p3x,p3y,0);
pointlist(splinedata)
splinedata << p1 << p2 << p3;
TopoDS_Shape spline = hsf::AddNewSplineSShape(splinedata);
TopoDS_Shape cross1 = makesplinesection(spline,0.0,p1z/2,p1z);
TopoDS_Shape cross2 = makesplinesection(spline,0.5,p2z/2,p2z);
TopoDS_Shape cross3 = makesplinesection(spline,1.0,p3z/2,p3z);
shapelist(surfacedata)
surfacedata << cross1 << cross2 << cross3;
TopoDS_Shape mastersurface = hsf::AddNewLoftSurface(surfacedata);

QList<QList<gp_Pnt>> triangles;


if (vistate == 0) 
{
vis(mastersurface)
} else {






int quadcount = 0;

QList<panelinstance> panelarray;

PANELIZE(mastersurface,ucount,vcount)
if(p1.Distance(p3) && p2.Distance(p4))
{
	quadcount++;
	TopoDS_Shape diag1 = hsf::AddNewLineptpt(p1,p3);
	TopoDS_Shape diag2 = hsf::AddNewLineptpt(p2,p4);
	double diag1length = hsf::GetLength(diag1);
	double diag2length = hsf::GetLength(diag2);
	double avrglength = (diag1length + diag2length)/2;
	double radius = avrglength / 100;
	
	QList<gp_Pnt> triangle1;
	QList<gp_Pnt> triangle2;

	triangle1 << p1 << p2 << p3;
	triangle2 << p3 << p4 << p1;

	panelinstance curpanel;
	curpanel.panelindex = quadcount;
	curpanel.triangles << triangle1 << triangle2;

	// begin atribute measures;

	double toplen,rightlen,botlen,leftlen,diag1len,diag2len;
	double nwangle,neangle,seangle,swangle;
	double panelarea;

	toplen = p1.Distance(p2);
	rightlen = p2.Distance(p3);
	botlen = p3.Distance(p4);
	leftlen = p4.Distance(p1);
	diag1len = p1.Distance(p3);
	diag2len = p2.Distance(p4);
	nwangle = gp_Vec(p1,p2).Angle(gp_Vec(p1,p4));
	neangle = gp_Vec(p2,p1).Angle(gp_Vec(p2,p3));
	seangle = gp_Vec(p3,p2).Angle(gp_Vec(p3,p4));
	swangle = gp_Vec(p4,p1).Angle(gp_Vec(p4,p3));
	panelarea = ((toplen + botlen)/2) *((leftlen + rightlen)/2) ;


	curpanel.parameters << toplen << rightlen << botlen << leftlen << diag1len << diag2len << nwangle << neangle << seangle << swangle << panelarea;

	panelarray << curpanel ;

	
	TopoDS_Shape l1 = hsf::AddNewLineptpt(p1,p2);
	TopoDS_Shape l2 = hsf::AddNewLineptpt(p2,p3);
	TopoDS_Shape l3 = hsf::AddNewLineptpt(p3,p4);
	TopoDS_Shape l4 = hsf::AddNewLineptpt(p4,p1);

	vis(l1)
	vis(l2)
	vis(l3)
	vis(l4)

	//gp_Vec circvec1 = hsf::getVectorTangentToCurveAtPoint(diag1,0);
	//gp_Pnt startp1 = hsf::AddNewPointonCurve(diag1,0);
	//TopoDS_Shape circle1 = hsf::AddNewCircle(startp1,circvec1,radius);
	//TopoDS_Shape sweep1 = hsf::AddNewExtrude(circle1,circvec1,diag1length);
	//
	//gp_Vec circvec2 = hsf::getVectorTangentToCurveAtPoint(diag2,0);
	//gp_Pnt startp2 = hsf::AddNewPointonCurve(diag2,0);
	//TopoDS_Shape circle2 = hsf::AddNewCircle(startp2,circvec2,radius);
	//TopoDS_Shape sweep2 = hsf::AddNewExtrude(circle2,circvec2,diag2length);
	//if (vistate == 1) 
	//{
	//vis(sweep1)
	//vis(sweep2)
	//} else
	//{
	//	vis(diag1)
	//	vis(diag2)
	//}
}


ENDPANELIZE


//kmeanscluster

kmeanscluster(kval,panelarray);

QList<Quantity_Color> centercolors;

for(int i=0 ; i < kval; i++)
{
	double r,g,b;
	r =0; g=0; b=0;
	r = double(rand()) / double(RAND_MAX);
	g = double(rand()) / double(RAND_MAX);
	b = double(rand()) / double(RAND_MAX);
	Quantity_Color curcolor(r,g,b,Quantity_TypeOfColor::Quantity_TOC_RGB);
	centercolors << curcolor;
}



Handle( StlMesh_Mesh ) aSTLMesh = new StlMesh_Mesh();
aSTLMesh->AddDomain();
	
	
/*Handle( MeshVS_Mesh  ) */

if(!aMesh.IsNull()) ic->Remove(aMesh,true);	

aMesh = new MeshVS_Mesh();





//ic->RemoveAll();


  for( int i = 0 ; i < panelarray.count() ; i++ )
  {
	panelinstance curpanel = panelarray.at(i);

	for(int j = 0; j < curpanel.triangles.count(); j++)
	{
    QList<gp_Pnt> curtriangle = curpanel.triangles.at(j);
	gp_Pnt tp1 = curtriangle.at(0);
	gp_Pnt tp2 = curtriangle.at(1);
	gp_Pnt tp3 = curtriangle.at(2);
	
	int i1 = aSTLMesh->AddOnlyNewVertex(tp1.X(),tp1.Y(),tp1.Z());
	int i2 = aSTLMesh->AddOnlyNewVertex(tp2.X(),tp2.Y(),tp2.Z());
	int i3 = aSTLMesh->AddOnlyNewVertex(tp3.X(),tp3.Y(),tp3.Z());
	aSTLMesh->AddTriangle(i1,i2,i3,0,0,0);
	}

  }


Handle( XSDRAWSTLVRML_DataSource ) aDS = new XSDRAWSTLVRML_DataSource( aSTLMesh );

	
aMesh->SetDataSource(aDS);
	//aMesh->AddBuilder( new MeshVS_MeshPrsBuilder( aMesh), Standard_True );//False -> No selection 
MeshVS_ElementalColorPrsBuilder* meshbuilder = new MeshVS_ElementalColorPrsBuilder( aMesh ,MeshVS_DMF_ElementalColorDataPrs | MeshVS_DMF_OCCMask );

meshbuilder->SetDataSource(aDS);



	TColStd_MapIteratorOfPackedMapOfInteger it (aDS->GetAllElements());
	Handle( MeshVS_HArray1OfSequenceOfInteger ) aTopo;
	Standard_Integer NbNodes;

int colorcount = 0;
int panelindex = 0;
int panelcount = panelarray.count();
int facecount = 0;
Quantity_Color curcolor;

  for( ; it.More(); it.Next() )
  {
	
	
    Standard_Integer aKey = it.Key();

    aDS->Get3DGeom( aKey, NbNodes, aTopo ) ;
	
		MeshVS_EntityType elemtype;
		aDS->GetGeomType(aKey,true,elemtype);
		
		if (elemtype == MeshVS_ET_Face)
		{
			facecount++;
			panelinstance curpanel = panelarray.at(panelindex);

			colorcount ++;
			
			if(colorcount % 2)
			{
			curcolor = centercolors.at(curpanel.centerindex);
			panelindex++;
			if (panelindex > panelarray.count()-1) panelindex = panelarray.count()-1;
			}
		
			meshbuilder->SetColor1(aKey,curcolor);
			
			//meshbuilder->SetColor2(aKey,Quantity_Color(r,g,b,Quantity_TypeOfColor::Quantity_TOC_RGB));
		}

	}
  

aMesh->AddBuilder( meshbuilder, Standard_True );//False -> No selection 
	
aMesh->GetDrawer()->SetBoolean(MeshVS_DA_DisplayNodes,Standard_False);
aMesh->GetDrawer()->SetBoolean(MeshVS_DA_ColorReflection,Standard_True);
aMesh->GetDrawer()->SetBoolean(MeshVS_DA_ShowEdges,false);


aMesh->SetDisplayMode (2);


ic->Display(aMesh);





}
ENDPART

double milliseconds = time.elapsed();
double seconds = (milliseconds/1000);
double minutes = seconds/60;
double hours = minutes/60;


QString timetxt;
QTextStream linestream(&timetxt);
linestream << "h" << hours << ":m " << minutes << ":s " << seconds << ".ms" << milliseconds; 

qDebug() << timetxt;






}

void kilianroof::loadsoumaya(int vistate)
{

if(!ui.updatechk->isChecked()) return;


QTime time;
time.start();

INITPART


double p1x = get(p1x)
double p1y = get(p1y)
double p1z = get(p1z)
double p2x = get(p2x)
double p2y = get(p2y)
double p2z = get(p2z)
double p3x = get(p3x)
double p3y = get(p3y)
double p3z = get(p3z)
int ucount = get(ucount)
int vcount = get(vcount)
int kval = get(famcount)

qDebug() << kval;

bool areachk = ui.areachk->isEnabled();
bool lengthschk = ui.lengthschk->isEnabled();
bool angleschk = ui.angleschk->isEnabled();


if (mastersurface.IsNull())
{
QoccInputOutput* io_man = new QoccInputOutput();
QString filename = QFileDialog::getOpenFileName ( this,
							 tr("Import File"),"");


if (!(filename.length() > 0)) return; 


Handle(TopTools_HSequenceOfShape) famshape1 = io_man->importIGES(filename);

mastersurface = famshape1->Value(1);
gp_Pnt cog = hsf::GetCOG(mastersurface);
mastersurface = hsf::movepointopoint(mastersurface,cog,gp::Origin());
}


QList<QList<gp_Pnt>> triangles;


if (vistate == 0) 
{
vis(mastersurface)
} else {



int quadcount = 0;

//QList<panelinstance> panelarray;

if(ucount != prevucount || vcount != prevvcount || areachk != prevareachk || angleschk != prevangleschk || lengthschk != prevlengthschk)
{
if (panelarray.count() >0) panelarray.clear();

PANELIZE(mastersurface,ucount,vcount)
if(p1.Distance(p3) && p2.Distance(p4))
{
	quadcount++;
	TopoDS_Shape diag1 = hsf::AddNewLineptpt(p1,p3);
	TopoDS_Shape diag2 = hsf::AddNewLineptpt(p2,p4);
	double diag1length = hsf::GetLength(diag1);
	double diag2length = hsf::GetLength(diag2);
	double avrglength = (diag1length + diag2length)/2;
	double radius = avrglength / 100;
	
	QList<gp_Pnt> triangle1;
	QList<gp_Pnt> triangle2;

	triangle1 << p1 << p2 << p3;
	triangle2 << p3 << p4 << p1;

	panelinstance curpanel;
	curpanel.panelindex = quadcount;
	curpanel.triangles << triangle1 << triangle2;

	// begin atribute measures;

	double toplen,rightlen,botlen,leftlen,diag1len,diag2len;
	double nwangle,neangle,seangle,swangle;
	double panelarea;

	toplen = p1.Distance(p2);
	rightlen = p2.Distance(p3);
	botlen = p3.Distance(p4);
	leftlen = p4.Distance(p1);
	diag1len = p1.Distance(p3);
	diag2len = p2.Distance(p4);
	nwangle = gp_Vec(p1,p2).Angle(gp_Vec(p1,p4));
	neangle = gp_Vec(p2,p1).Angle(gp_Vec(p2,p3));
	seangle = gp_Vec(p3,p2).Angle(gp_Vec(p3,p4));
	swangle = gp_Vec(p4,p1).Angle(gp_Vec(p4,p3));
	panelarea = ((toplen + botlen)/2) *((leftlen + rightlen)/2) ;


	if (ui.lengthschk->isChecked())
	{
	curpanel.parameters << toplen << rightlen << botlen << leftlen << diag1len << diag2len;
	}

	if(ui.angleschk->isChecked())
	{
	curpanel.parameters << nwangle << neangle << seangle << swangle;
	}

	if(ui.areachk->isChecked())
	{
		curpanel.parameters << panelarea;
	}
	 

	panelarray << curpanel ;

	
	TopoDS_Shape l1 = hsf::AddNewLineptpt(p1,p2);
	TopoDS_Shape l2 = hsf::AddNewLineptpt(p2,p3);
	TopoDS_Shape l3 = hsf::AddNewLineptpt(p3,p4);
	TopoDS_Shape l4 = hsf::AddNewLineptpt(p4,p1);

	vis(l1)
	vis(l2)
	vis(l3)
	vis(l4)

	//gp_Vec circvec1 = hsf::getVectorTangentToCurveAtPoint(diag1,0);
	//gp_Pnt startp1 = hsf::AddNewPointonCurve(diag1,0);
	//TopoDS_Shape circle1 = hsf::AddNewCircle(startp1,circvec1,radius);
	//TopoDS_Shape sweep1 = hsf::AddNewExtrude(circle1,circvec1,diag1length);
	//
	//gp_Vec circvec2 = hsf::getVectorTangentToCurveAtPoint(diag2,0);
	//gp_Pnt startp2 = hsf::AddNewPointonCurve(diag2,0);
	//TopoDS_Shape circle2 = hsf::AddNewCircle(startp2,circvec2,radius);
	//TopoDS_Shape sweep2 = hsf::AddNewExtrude(circle2,circvec2,diag2length);
	//if (vistate == 1) 
	//{
	//vis(sweep1)
	//vis(sweep2)
	//} else
	//{
	//	vis(diag1)
	//	vis(diag2)
	//}
}


ENDPANELIZE
lastfolder = folder;

} else
{
vis(lastfolder)
}


//kmeanscluster
if (panelarray.count() < 1) return;

kmeanscluster(kval,panelarray);

QList<Quantity_Color> centercolors;

for(int i=0 ; i < kval; i++)
{
	double r,g,b;
	r =0; g=0; b=0;
	r = double(rand()) / double(RAND_MAX);
	g = double(rand()) / double(RAND_MAX);
	b = double(rand()) / double(RAND_MAX);
	Quantity_Color curcolor(r,g,b,Quantity_TypeOfColor::Quantity_TOC_RGB);
	centercolors << curcolor;
}



Handle( StlMesh_Mesh ) aSTLMesh = new StlMesh_Mesh();
aSTLMesh->AddDomain();
	
	
/*Handle( MeshVS_Mesh  ) */

if(!aMesh.IsNull()) ic->Remove(aMesh,true);	

aMesh = new MeshVS_Mesh();





//ic->RemoveAll();


  for( int i = 0 ; i < panelarray.count() ; i++ )
  {
	panelinstance curpanel = panelarray.at(i);

	for(int j = 0; j < curpanel.triangles.count(); j++)
	{
    QList<gp_Pnt> curtriangle = curpanel.triangles.at(j);
	gp_Pnt tp1 = curtriangle.at(0);
	gp_Pnt tp2 = curtriangle.at(1);
	gp_Pnt tp3 = curtriangle.at(2);
	
	int i1 = aSTLMesh->AddOnlyNewVertex(tp1.X(),tp1.Y(),tp1.Z());
	int i2 = aSTLMesh->AddOnlyNewVertex(tp2.X(),tp2.Y(),tp2.Z());
	int i3 = aSTLMesh->AddOnlyNewVertex(tp3.X(),tp3.Y(),tp3.Z());
	aSTLMesh->AddTriangle(i1,i2,i3,0,0,0);
	}

  }


Handle( XSDRAWSTLVRML_DataSource ) aDS = new XSDRAWSTLVRML_DataSource( aSTLMesh );

	
aMesh->SetDataSource(aDS);
	//aMesh->AddBuilder( new MeshVS_MeshPrsBuilder( aMesh), Standard_True );//False -> No selection 
MeshVS_ElementalColorPrsBuilder* meshbuilder = new MeshVS_ElementalColorPrsBuilder( aMesh ,MeshVS_DMF_ElementalColorDataPrs | MeshVS_DMF_OCCMask );

meshbuilder->SetDataSource(aDS);



	TColStd_MapIteratorOfPackedMapOfInteger it (aDS->GetAllElements());
	Handle( MeshVS_HArray1OfSequenceOfInteger ) aTopo;
	Standard_Integer NbNodes;

int colorcount = 0;
int panelindex = 0;
int panelcount = panelarray.count();
int facecount = 0;
Quantity_Color curcolor;

  for( ; it.More(); it.Next() )
  {
	
	
    Standard_Integer aKey = it.Key();

    aDS->Get3DGeom( aKey, NbNodes, aTopo ) ;
	
		MeshVS_EntityType elemtype;
		aDS->GetGeomType(aKey,true,elemtype);
		
		if (elemtype == MeshVS_ET_Face)
		{
			facecount++;
			panelinstance curpanel = panelarray.at(panelindex);

			colorcount ++;
			
			if(colorcount % 2)
			{
			curcolor = centercolors.at(curpanel.centerindex);
			panelindex++;
			if (panelindex > panelarray.count()-1) panelindex = panelarray.count()-1;
			}
		
			meshbuilder->SetColor1(aKey,curcolor);
			
			//meshbuilder->SetColor2(aKey,Quantity_Color(r,g,b,Quantity_TypeOfColor::Quantity_TOC_RGB));
		}

	}
  

aMesh->AddBuilder( meshbuilder, Standard_True );//False -> No selection 
	
aMesh->GetDrawer()->SetBoolean(MeshVS_DA_DisplayNodes,Standard_False);
aMesh->GetDrawer()->SetBoolean(MeshVS_DA_ColorReflection,Standard_True);
aMesh->GetDrawer()->SetBoolean(MeshVS_DA_ShowEdges,false);


aMesh->SetDisplayMode (2);


ic->Display(aMesh);





}
ENDPART

double milliseconds = time.elapsed();
double seconds = (milliseconds/1000);
double minutes = seconds/60;
double hours = minutes/60;


QString timetxt;
QTextStream linestream(&timetxt);
linestream << "h" << hours << ":m " << minutes << ":s " << seconds << ".ms" << milliseconds; 

qDebug() << timetxt;


prevucount = ucount;
prevvcount = vcount;
prevareachk = areachk;
prevlengthschk = lengthschk;
prevangleschk = angleschk;



}


void kilianroof::breakshape()
{

INITPART
int kval = get(famcount)

if (mastersurface.IsNull())
{
QoccInputOutput* io_man = new QoccInputOutput();
QString filename = QFileDialog::getOpenFileName ( this,
							 tr("Import File"),"");


if (!(filename.length() > 0)) return; 


Handle(TopTools_HSequenceOfShape) famshape1 = io_man->importIGES(filename);
mastersurface = famshape1->Value(1);
gp_Pnt cog = hsf::GetCOG(mastersurface);
mastersurface = hsf::movepointopoint(mastersurface,cog,gp::Origin());
//mastersurface = hsf::getfacefromshape(mastersurface);

BRepTools::Clean(mastersurface);
BRepMesh::Mesh(mastersurface, 10);

}


Handle(AIS_Gauss) mygauss = new AIS_Gauss(mastersurface,true);
ic->Display(mygauss);



//creates a tool and initializes it by shape. 

//ShapeUpgrade_ShapeDivideContinuity ShapeDivideCont(surface); 
//
////sets the working 3D and 2D precision and the maximum allowed 
////tolerance 
//ShapeDivideCont.SetTolerance(Precision::Confusion()); 
//ShapeDivideCont.SetTolerance2d(Precision::Confusion()); 
//ShapeDivideCont.SetMaxTolerance(Precision::Confusion()); 
////sets the values of criteria for surfaces, 3D curves and 2D curves.  
//ShapeDivideCont.SetBoundaryCriterion(GeomAbs_C2); 
//ShapeDivideCont.SetPCurveCriterion(GeomAbs_C2); 
//ShapeDivideCont.SetSurfaceCriterion(GeomAbs_C2); 
////performs the splitting. 
//ShapeDivideCont.Perform(); 
////checks the status and gets the result 
////if(ShapeDivideCont.Status(ShapeExtend_DONE)) 
////{
//int panelcount = kval+1;
//double panelarea = hsf::GetArea(mastersurface)/ (double) panelcount;
//ShapeUpgrade_ShapeDivideArea tool (mastersurface); 
//tool.MaxArea() = panelarea; 
//tool.Perform(); 
//if(tool.Status(ShapeExtend_DONE)) { 
//  TopoDS_Shape ResultShape = tool.Result(); 
//  ShapeFix::SameParameter ( ResultShape, Standard_False ); 
//  vis(ResultShape)
//} 


	/*TopoDS_Shape result = ShapeDivideCont.Result();
	vis(result)*/
//}



//ENDPART

}