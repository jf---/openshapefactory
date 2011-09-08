#include "slicer.h"
#include "ui.h"
#include "shapefactory.h"
#include "parametricsfordummies.h"
#include "bash.h"
#include <QFileDialog>
#include <QProgressDialog>
#include <QTime>
#include <QDebug>
#include <QFuture>
#include <QtConcurrentMap>
#include <QFutureWatcher>
#include <QMutex>
#include <QThread>
#include <booleanthread.h>

 

 
TopoDS_Shape mainshape = TopoDS_Shape();
QList<TopoDS_Shape> shapes;
//QMutex mutex;

void addToCompound(TopoDS_Shape compound,  TopoDS_Shape shape)
 {
     BRep_Builder B;
	 B.Add(compound,shape);
 }

TopoDS_Shape fuse(const TopoDS_Shape &shape1)
 {
 //if(mainshape.IsNull()) return TopoDS_Shape();
try{
	TopoDS_Shape result =hsf::AddNewIntersectSrfW(mainshape,shape1);
 return result;
} catch(...){
return TopoDS_Shape();
}
 

  
}

//void displayshapes()
//
//	{
//		INITPART
//		for(int i=0;i<shapes.count();i++)
//			{
//				vis(shapes.at(i));
//			}
//		ENDPART
//
//	}

 
using namespace QtConcurrent;


slicer::slicer(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setup();
}

slicer::~slicer()
{

}

void slicer::setup()
	{

	SLIDERRELEASED(loadfaces2)

	}

void slicer::update()
	{

INITPART

if(EOS.IsNull())

{
QoccInputOutput* io_man = new QoccInputOutput();
QString filename = QFileDialog::getOpenFileName ( this,
							 tr("Import File"),"");
//QString filename2 = QFileDialog::getOpenFileName ( this,
//							 tr("Import File"),"");

if (!(filename.length() > 0)) return; 
//if (!(filename2.length() > 0)) return; 

Handle(TopTools_HSequenceOfShape) famshape1 = io_man->importSTEP(filename);
EOS = famshape1->Value(1);

//Handle(TopTools_HSequenceOfShape) famshape2 = io_man->importSTEP(filename2);
//EOS2 = famshape2->Value(1);

}

mainshape = EOS;

//vis(EOS);

double Xmin, Ymin, Zmin, Xmax, Ymax, Zmax;
Bnd_Box Bnd;
BRepBndLib::Add(EOS, Bnd);
Bnd.Get(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);

double width = abs(Xmax -Xmin);
double height = abs(Ymax -Ymin);
double depth = abs(Zmax -Zmin);

double slicecount = get(slicecount);

double sliceheight =(double) depth / slicecount;
gp_Pnt centerp((Xmin+Xmax)/2,(Ymin+Ymax)/2,(Zmin+Zmax)/2);


TopoDS_Compound slices;
BRep_Builder sliceB;
sliceB.MakeCompound(slices);

QTime time;
time.start();
shapecount = 0;

QList<booleanthread*> threadlist;
for(int i= 0;i<slicecount;i++) {
shapecount++;
gp_Pnt slicecenter = centerp;
//slicecenter.SetZ((double) i * sliceheight * 0.99 + Zmin);

TopoDS_Shape rect = hsf::AddNewRectangle(slicecenter,gp::DZ(),width*2,height*2);
//TopoDS_Shape circle = hsf::AddNewCircle(slicecenter,gp::DZ(),width);
//TopoDS_Shape line = hsf::AddNewLineptdir(slicecenter,gp::DX(),0,width*2);

TopoDS_Shape rectfill = hsf::AddNewFillSurface(rect);
shapelist << rectfill;
TopoDS_Shape &rectref = shapelist[0];
TopoDS_Shape &eosref = EOS;

//TopoDS_Shape boolobj = hsf::BooleanCommon(EOS,rectfill);
if(EOS.IsNull()) continue;
if(rectfill.IsNull()) continue;
booleanthread* bt = new booleanthread(this,shapelist[0],EOS);
threadlist << bt;

connect(threadlist.last(),SIGNAL(finishedshape( const TopoDS_Shape& )),this,SLOT(appendtolist( const TopoDS_Shape& )));
threadlist.last()->start();

}

//for(int i= 0;i<slicecount;i++) {
//gp_Pnt slicecenter = centerp;
//slicecenter.SetZ((double) i * sliceheight);
//
//TopoDS_Shape rect = hsf::AddNewRectangle(slicecenter,gp::DZ(),width*2,height*2);
//TopoDS_Shape rectfill = hsf::AddNewFillSurface(rect);
//boolthread* bt = new boolthread(0,EOS,rectfill);
//connect(bt,SIGNAL(finishedshape(TopoDS_Shape)),this,SLOT(appendtolist(TopoDS_Shape)));
//bt->start();
//}


//QFuture<TopoDS_Shape> collage = QtConcurrent::mapped(planelist,fuse);


double milliseconds = time.elapsed();
double seconds = (milliseconds/1000);
double minutes = seconds/60;
double hours = minutes/60;


QString timetxt;
QTextStream linestream(&timetxt);
linestream << "h" << hours << ":m " << minutes << ":s " << seconds << ".ms" << milliseconds; 

qDebug() << timetxt;

ENDPART

	}

void slicer::appendtolist(const TopoDS_Shape& shape1 )
		{

	    	resultlist << shape1;
			if (resultlist.count() == shapecount)
				{
					INITPART
						for(int i=0;i<resultlist.count();i++)
							{
								vis(resultlist.at(i))
							}

					ENDPART

				}
			
		}




void slicer::loadfaces1()
{

	INITPART
	QoccInputOutput* io_man = new QoccInputOutput();
	QString filename = QFileDialog::getOpenFileName ( this,
							 tr("Import File"),"");

	if (!(filename.length() > 0)) return; 
	Handle(TopTools_HSequenceOfShape) famshape1 = io_man->importIGES(filename);
	TopoDS_Shape catfile = famshape1->Value(1);

	QList<TopoDS_Shape> facelist;
	QMap<QString,TopoDS_Shape> facemap;

	TopExp_Explorer EX;
	for (EX.Init(catfile,TopAbs_FACE); EX.More(); EX.Next())
	{
	TopoDS_Face aF1 = TopoDS::Face(EX.Current());
	facelist << aF1;

		TopExp_Explorer EX2;
		for (EX2.Init(aF1,TopAbs_VERTEX); EX2.More(); EX2.Next())
		{
			gp_Pnt corner = BRep_Tool::Pnt(TopoDS::Vertex(EX2.Current()));
			QString ptext;
			QTextStream out(&ptext);
			out << corner.X() << "," << corner.Y() << "," << corner.Z();

			facemap.insert(ptext,aF1);
			
		}
	}


	TopoDS_Shape initialface = facelist.at(0);
	TopoDS_Shape nextface;
	TopoDS_Shape firstface = initialface;
	gp_Pnt ffcog = hsf::GetCOG(firstface);
	gp_Pnt nfcog = ffcog;
	nfcog.SetX(ffcog.X() * 200);


	int count = 0;
	while (!ffcog.IsEqual(nfcog,Precision::Confusion()))
	{
		count ++;
		if (count > facelist.count()) break;
	TopoDS_Shape topedge;
	double maxval = -1000000000000000000;
		TopExp_Explorer EX3;
		for (EX3.Init(firstface,TopAbs_EDGE); EX3.More(); EX3.Next())
		{
			TopoDS_Shape curedge= EX3.Current();
			gp_Pnt cpt = hsf::AddNewPointonCurve(curedge,0.5);

			if (cpt.Z() > maxval)
			{
				maxval = cpt.Z();
				topedge = curedge;
			}
						
		}
	
		vis(topedge);

		gp_Pnt anypoint = hsf::AddNewPointonCurve(topedge,1);
		
			QString ptext;
			QTextStream out(&ptext);
			out << anypoint.X() << "," << anypoint.Y() << "," << anypoint.Z();


		QList<TopoDS_Shape> faces = facemap.values(ptext);
		

		for (int i=0;i< faces.count(); i++)
		{
			TopoDS_Shape curface = faces.at(i);
			gp_Pnt cfcog = hsf::GetCOG(curface);

			if(!ffcog.IsEqual(cfcog,Precision::Confusion()))
			{
				nextface = curface;
			}
		}

		gp_Pnt cog = hsf::GetCOG(nextface);
		QString numtext = QString("F") + QString::number(count);
		hsf::AddNewAnnotation(numtext,cog);

		firstface = nextface;
		nfcog = cog;
	}




	//vis(catfile)

	ENDPART
}
void slicer::loadfaces2()
{

	INITPART
	QoccInputOutput* io_man = new QoccInputOutput();
	QString filename = QFileDialog::getOpenFileName ( this,
							 tr("Import File"),"");

	if (!(filename.length() > 0)) return; 
	Handle(TopTools_HSequenceOfShape) famshape1 = io_man->importIGES(filename);
	TopoDS_Shape catfile = famshape1->Value(1);

	QList<TopoDS_Shape> facelist;
	QMap<QString,TopoDS_Shape> facemap;
	
	int count = 0;
	TopoDS_Shape prevface;

	TopExp_Explorer EX;
	TopExp_Explorer EX2;
	for (EX.Init(catfile,TopAbs_FACE); EX.More(); EX.Next())
	{
	count++;
	TopoDS_Face aF1 = TopoDS::Face(EX.Current());
	gp_Pnt cog = hsf::GetCOG(aF1);
	QString facename = QString("F") + QString::number(count);
	hsf::AddNewAnnotation(facename,cog);

	
	if ( count > 1)
	{
		QList<TopoDS_Shape> edgelist1 = hsf::getshapelist(aF1,TopAbs_EDGE);
		//QList<TopoDS_Shape> edgelist2 = hsf::getshapelist(prevface,TopAbs_EDGE);
		
		TopoDS_Shape edge1 = edgelist1.at(0);
		TopoDS_Shape biarc1 = hsf::biarcoptimized(edge1,0.05,0.95,0.05,0.95,0);
		/*TopoDS_Shape edge2 = edgelist1.at(2);
		TopoDS_Shape biarc2 = hsf::biarcoptimized(edge2,0,1,0,1,0);*/
		
		vis(biarc1)
		//vis(biarc2)
		//TopoDS_Shape prevf1 = edgelist.at(0);
	}

	//facelist << aF1;

	//prevface = aF1;
	}




	ENDPART
}