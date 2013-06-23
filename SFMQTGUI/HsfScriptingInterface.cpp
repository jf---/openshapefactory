#include "HsfScriptingInterface.h"

#include "shapefactory.h"
#include "ui.h"
#include "User_AIS.hxx"

#include "SGMGUI_COMMON.h"
# include <QHBoxLayout>
#include "AIS_Gauss.hxx" 
#include <QFileDialog>
#include <scriptwidget.h>
#include <QRadioButton>
#include <QoccInputOutput.h>
#include <Prs3d_Presentation.hxx>
#include <Qsci/qsciscintilla.h>
#include <QLabel>

#include "volumemeshsettings.h"#include <Handle_MeshVS_Drawer.hxx>
#include <MeshVS_Mesh.hxx>
#include <MeshVS_DrawerAttribute.hxx>
#include <MeshVS_MeshPrsBuilder.hxx>
//#include <MeshVS_ElementalColorPrsBuilder.hxx>
#include <MeshVS_TextPrsBuilder.hxx> 
#include <MeshVS_Drawer.hxx>

#include <XSDRAWSTLVRML_DataSource.hxx>
#include <Graphic3d_MaterialAspect.hxx>
#include <Handle_Prs3d_BasicAspect.hxx>

#include <Graphic3d_MaterialAspect.hxx>
#include <Quantity_Color.hxx> //voronoi needs#include <iostream>
#include <math.h>
#include <algorithm>
#include <time.h>
#include "Voronoi.h"
#include "VPoint.h"
//end voronoi

#include "QSqlQuery"
#include "QSqlRecord"
#include "gradients.h"class MeshVS_Drawer;



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
















HsfScriptingInterface::HsfScriptingInterface()
:ftp(0)
{
	UnitsAPI::SetLocalSystem(UnitsAPI_MDTV);
	Handle_AIS_InteractiveContext ic = appui::getInstance()->getWindowContext();
	viscount = 0;	 
	setuprunonce();

	io_man = new QoccInputOutput();

	needstofitall = false;

	filewatch = new QFileSystemWatcher( this ); // pass this (QObject) as parent


	ftp = new QFtp(0);
	

	//aMeshVS = new MeshVS_Mesh(Standard_True);
 }

HsfScriptingInterface::~HsfScriptingInterface()
 {
 }




QScriptValue HsfScriptingInterface::mcosh(QScriptValue parameter)
	{
	return engine()->toScriptValue(cosh(parameter.toNumber()));
	}
QScriptValue HsfScriptingInterface::msinh(QScriptValue parameter)
	{
	return engine()->toScriptValue(sinh(parameter.toNumber()));
	}



void HsfScriptingInterface::setmousepos(gp_Pnt pos)
{
CurrentMousePoint = pos;
}

QScriptValue HsfScriptingInterface::readcsv(QScriptValue filename)
{

	QList<QStringList> csvfile = hsf::ReadDelimitedtoStringList(filename.toString(),",");
	QScriptValue returnval = engine()->toScriptValue(csvfile);
	return returnval;


}

QScriptValue HsfScriptingInterface::mysqlconnect()
	{

		if (context()->argumentCount() == 4)
		 {

			QString hostname = context()->argument(0).toString();
			QString database = context()->argument(1).toString();
			QString user = context()->argument(2).toString();
			QString password = context()->argument(3).toString();


			

			db = new QSqlDatabase();
			db->addDatabase("QMYSQL" , "alexcon" );
			db->setHostName(hostname);
			db->setDatabaseName(database);
			db->setUserName(user);
			db->setPassword(password);
			bool ok = db->open();

			return engine()->toScriptValue(ok);

		}


	}

QScriptValue HsfScriptingInterface::sqlquery(QScriptValue querycommand)
{
 QSqlQuery query;
 query.exec(querycommand.toString());

 QString resultstring;
  while (query.next()) {
        QSqlRecord rec = query.record();

		QString recordstring;
		for(int i; i < rec.count();i++)
			{
				QString fieldstring;

				QString fieldname = rec.fieldName(i);
				QString fieldval = rec.value(i).toString();
				fieldstring = fieldname + ":" + fieldval;
				
				recordstring = recordstring + "," + fieldstring;
			}
		
      resultstring = resultstring + recordstring + "\n";

       
    }

return engine()->toScriptValue(resultstring);
 


}



void HsfScriptingInterface::print(QScriptValue msg)
{

	QString themsg = msg.toString();
	if (themsg.count()>0) 
	{
	qDebug() << themsg << "\n";
	fprintf(stderr, "%s", themsg.toAscii().data());
	//fprintf("%c[34;43G", 0x1b);
		parentwidget->ui.listener->addItem(msg.toString());
	}

}

QScriptValue HsfScriptingInterface::getnextuvname()
	{

	 if (context()->argumentCount() == 3)
	 {
		QString name = context()->argument(0).toString();
		int uoff = context()->argument(1).toNumber();
		int voff = context()->argument(2).toNumber();

		QString newname = HSF::GetNextUvName(name,uoff,voff);

		return engine()->toScriptValue(newname);

	 }

	 return engine()->toScriptValue(false);


	}


QScriptValue HsfScriptingInterface::make2dpoint()
 {
	 
	 gp_Pnt2d mypt ;
	 if (context()->argumentCount() == 2)
	 {
		 double x,y;

		 if (context()->argument(0).toString().toLower() == "infinity") {x = 0;} else { x = context()->argument(0).toNumber();}
		 if (context()->argument(1).toString().toLower() == "infinity") {y = 0;} else { y = context()->argument(1).toNumber();}
		

    	 mypt = gp_Pnt2d(x,y);
		 //qDebug() << "2dpoint:" << x << "," << y;
				 
	 }
	     
	 QScriptValue myptval = engine()->toScriptValue(mypt);
	 myptval.setProperty(QString("x"),engine()->toScriptValue(mypt.X() ));
	 myptval.setProperty(QString("y"),engine()->toScriptValue(mypt.Y()));
	 
     return myptval;
 }


QScriptValue HsfScriptingInterface::makebiarcoptimized()
	{
	

	TopoDS_Shape shape;
	 
	 if (context()->argumentCount() == 5)
	 {
		 
		 TopoDS_Shape crv = context()->argument(0).toVariant().value<TopoDS_Shape>();
		 double ratio1 = context()->argument(1).toNumber();
		 double ratio2 = context()->argument(2).toNumber();
		 double percentinf = context()->argument(3).toNumber();
		 double percentsup = context()->argument(4).toNumber();
         
		 if ( ratio2 > ratio1 && percentsup > percentinf)
			 {
			 try
				 {
			      shape = hsf::biarcoptimized(crv,ratio1,ratio2,percentinf,percentsup,100000);
				 }
			 catch(...)
				 {
					QScriptValue result = engine()->toScriptValue(QString("biarc failed"));
				 }
     		 }

		 

		
		
         QScriptValue result = engine()->toScriptValue(shape);

		 return result;
	 }
	     
	return engine()->toScriptValue(false);

	}

QScriptValue HsfScriptingInterface::makecorner()
	{
	

	TopoDS_Shape shape;
	 
	 if (context()->argumentCount() == 5)
	 {
		 
		 gp_Pnt p1 = hsf::getpointfromshape(context()->argument(0).toVariant().value<TopoDS_Shape>());
		 gp_Pnt p2 = hsf::getpointfromshape(context()->argument(1).toVariant().value<TopoDS_Shape>());
		 gp_Pnt p3 = hsf::getpointfromshape(context()->argument(2).toVariant().value<TopoDS_Shape>());
		 gp_Pnt target = hsf::getpointfromshape(context()->argument(3).toVariant().value<TopoDS_Shape>());
		 double radius = context()->argument(4).toNumber();

		 qDebug() << p1.X() << p1.Y() << p1.Z() << radius;
		 qDebug() << p2.X() << p2.Y() << p2.Z() << radius;
		 qDebug() << p3.X() << p3.Y() << p3.Z() << radius;
		 qDebug() << target.X() << target.Y() << target.Z() << radius;

		shape = hsf::AddNewCorner(p1,p2,p3,radius,target);
		
         QScriptValue result = engine()->toScriptValue(shape);

		 return result;
	 }
	     
	return engine()->toScriptValue(false);

	}


QScriptValue HsfScriptingInterface::make2dpointonsurface()
 {
	 TopoDS_Shape pointshape;
	 
	 if (context()->argumentCount() == 2)
	 {
		 double x,y;
		 TopoDS_Shape surface = context()->argument(0).toVariant().value<TopoDS_Shape>();
		 gp_Pnt2d p1 = context()->argument(1).toVariant().value<gp_Pnt2d>();

		 gp_Pnt thept = hsf::AddNewUVPt(surface,p1.X(),p1.Y());
		 pointshape = hsf::AddNewPoint(thept);
         QScriptValue myptval = engine()->toScriptValue(pointshape);

		 return myptval;
	 }
	     
	return engine()->toScriptValue(false);
	
 }


QScriptValue HsfScriptingInterface::make2dlineonsurface()
{
	 if (context()->argumentCount() == 3)
	 {
		 
		 TopoDS_Shape surface = context()->argument(0).toVariant().value<TopoDS_Shape>();
		 gp_Pnt2d p1 = context()->argument(1).toVariant().value<gp_Pnt2d>();
		 gp_Pnt2d p2 = context()->argument(2).toVariant().value<gp_Pnt2d>();

		 TopoDS_Shape theline = hsf::AddNew2dLineOnSurface(surface,p1,p2);
		 return engine()->toScriptValue(theline);
	 }
	     
	return engine()->toScriptValue(false);
	
 }

QScriptValue HsfScriptingInterface::getshapelistcount()
	{
	
	if (context()->argumentCount() == 1)
	{
	    QList<TopoDS_Shape> shapelist = context()->argument(0).toVariant().value<QList<TopoDS_Shape>>();
		return shapelist.count();

	}

	return engine()->toScriptValue(false);

	}
QScriptValue HsfScriptingInterface::getshapelistitem()
	{
    
	if (context()->argumentCount() == 2)
			{
			QList<TopoDS_Shape> keys = context()->argument(0).toVariant().value<QList<TopoDS_Shape>>();
			int pos = context()->argument(1).toNumber();
			if (pos < keys.count())
				{
					return engine()->toScriptValue(keys.at(pos));
				}
			}
	return engine()->toScriptValue(false); 

	}
QScriptValue HsfScriptingInterface::joinshapelists()
	{
    
	if (context()->argumentCount() == 2)
			{
			QList<TopoDS_Shape> list1 = context()->argument(0).toVariant().value<QList<TopoDS_Shape>>();
			QList<TopoDS_Shape> list2 = context()->argument(1).toVariant().value<QList<TopoDS_Shape>>();

			QList<TopoDS_Shape> result = list1 + list2;
			
			return engine()->toScriptValue(result);
				
			}
	return engine()->toScriptValue(false); 

	}

QScriptValue HsfScriptingInterface::getshapelist()
	{

	if (context()->argumentCount() == 2)
	 {
	    TopoDS_Shape theshape = context()->argument(0).toVariant().value<TopoDS_Shape>();
		 QString shapetype  = context()->argument(1).toString();

		 TopAbs_ShapeEnum shapesel;

		 if (shapetype.length() < 1) return  engine()->toScriptValue(false);

		 if (shapetype == "compsolid")
			 {
			 shapesel = TopAbs_COMPOUND;
			 } else if(shapetype == "solid"){
				shapesel = TopAbs_SOLID;
			 } else if(shapetype == "shell"){
				shapesel = TopAbs_SHELL;
			 } else if(shapetype == "face"){
				shapesel = TopAbs_FACE;
			 } else if(shapetype == "wire"){
				shapesel = TopAbs_WIRE;
			 } else if(shapetype == "edge"){
				shapesel = TopAbs_EDGE;
			 } else if(shapetype == "vertex"){
				shapesel = TopAbs_VERTEX;
			 } else if(shapetype == "shape"){
				shapesel = TopAbs_SHAPE;
		     }
			 

		 QList<TopoDS_Shape> shapelist = hsf::getshapelist(theshape, shapesel);

	  return  engine()->toScriptValue(shapelist);
	  }

	 return  engine()->toScriptValue(false);
	}



QScriptValue HsfScriptingInterface::make2dpointlist()
	{
	QList<gp_Pnt2d> mypointlist;
    return engine()->toScriptValue(mypointlist);
	}


QScriptValue HsfScriptingInterface::insert2dpointtolist()
	{

	//QList<gp_Pnt2d> mypointlist;
   
	 if (context()->argumentCount() > 1)
	 {
		 QList<gp_Pnt2d> mypointlist = context()->argument(0).toVariant().value<QList<gp_Pnt2d>>();
		 for(int i=1;i<context()->argumentCount();i++)
			{
			gp_Pnt2d valuetoadd = context()->argument(i).toVariant().value<gp_Pnt2d>();
			mypointlist << valuetoadd; 
			//qDebug() << "inserpoint2dtolist" << valuetoadd.X() << "," << valuetoadd.Y();
			}
	  return  engine()->toScriptValue(mypointlist);
	  }

	 return  engine()->toScriptValue(false);


	}

QScriptValue HsfScriptingInterface::makewire()
	{

	//QList<gp_Pnt2d> mypointlist;
   
	 if (context()->argumentCount() == 1)
	 {
		 QList<TopoDS_Shape> shapelist = context()->argument(0).toVariant().value<QList<TopoDS_Shape>>();
		 TopoDS_Shape wire = hsf::AddNewWire(shapelist); 
		
	     return  engine()->toScriptValue(wire);
	  }

	 return  engine()->toScriptValue(false);


	}

QScriptValue HsfScriptingInterface::makeshell()
	{

	//QList<gp_Pnt2d> mypointlist;
   
	 if (context()->argumentCount() == 1)
	 {
		 QList<TopoDS_Shape> shapelist = context()->argument(0).toVariant().value<QList<TopoDS_Shape>>();
		 TopoDS_Shape shell = hsf::AddNewShell(shapelist); 
		
	     return  engine()->toScriptValue(shell);
	  }

	 return  engine()->toScriptValue(false);


	}

QScriptValue HsfScriptingInterface::makecompound()
	{

	//QList<gp_Pnt2d> mypointlist;
   TopoDS_Compound resultofoperation;
   BRep_Builder builder;
   builder.MakeCompound(resultofoperation);

	 if (context()->argumentCount() == 1)
	 {
		 QList<TopoDS_Shape> shapelist = context()->argument(0).toVariant().value<QList<TopoDS_Shape>>();
		 for(int i=0;i<shapelist.count();i++)
			 {
				TopoDS_Shape curshape = shapelist.at(i);
				builder.Add(resultofoperation,curshape);
				
			 }
		 
		TopoDS_Shape resultshape = resultofoperation;
	     return  engine()->toScriptValue(resultshape);
	  }

	 return  engine()->toScriptValue(false);


	}


QScriptValue HsfScriptingInterface::makesolid()
	{

	//QList<gp_Pnt2d> mypointlist;
   
	 if (context()->argumentCount() == 1)
	 {
		 TopoDS_Shape shell = context()->argument(0).toVariant().value<TopoDS_Shape>();
		 TopoDS_Shape solid = hsf::AddNewSolid(shell); 
		
	     return  engine()->toScriptValue(solid);
	  }

	 return  engine()->toScriptValue(false);


	}


QScriptValue HsfScriptingInterface::makerandomvoronoi()
	{

if (context()->argumentCount() == 2)
	 {
		 TopoDS_Shape face = context()->argument(0).toVariant().value<TopoDS_Shape>();
		 int  num = context()->argument(1).toNumber();

		 TopoDS_Compound voronoifolder;
		 BRep_Builder voronoiB;
         voronoiB.MakeCompound(voronoifolder);


	using namespace vor;

	vor::Voronoi * voronoi;
	vor::Vertices * ver; // vrcholy
	vor::Edges * edg;	 // hrany diagramu

   double voronoiwidth = 0.5;

	voronoi = new Voronoi();
	ver = new Vertices();
	
	
	srand(static_cast<unsigned int>(clock()));
	int randval ;

	for(int i=0; i<num; i++) 
	{

	randval = rand();
	//qDebug() << "the current random value is" << randval;
	

	double px = randval / double(RAND_MAX) ;
	px = px * (voronoiwidth);
	px = px / voronoiwidth;

	randval = (double)rand();
	

	double py = randval / double(RAND_MAX) ; 
	py = py * (voronoiwidth);
	py = py / voronoiwidth;

	ver->push_back(new VPoint( px , py )); 
		
	}

	edg = voronoi->GetEdges(ver, voronoiwidth, voronoiwidth);
	std::cout << "voronois done!\n";

	int curedge=0;
	for(vor::Edges::iterator i = edg->begin(); i!= edg->end(); ++i)
	{
	curedge ++;
			if( (*i)->start == 0 )
			{
				std::cout << "chybi zacatek hrany!\n";
				continue;
			}
			if( (*i)->end == 0 )
			{
				std::cout << "chybi konec hrany!\n";
				continue;
			}	

			double x1 = (*i)->start->x;
			double y1 = (*i)->start->y;
			double x2 = (*i)->end->x;
			double y2 = (*i)->end->y;
			bool startin , endin;
			startin = endin = false;
			double thew = 1;
			if (x1 > 0 && x1 < thew && y1 > 0 && y1 < thew) startin = true;
			if (x2 > 0 && x2 < thew && y2 > 0 && y2 < thew) endin = true;

			if (startin == true && endin == true)
				{

			/*	x1 = x1/4;
				y1 = y1/4;
				x2 = x2/4;
				y2 = -y2/4;*/

				gp_Pnt2d p1(x1,y1);
				gp_Pnt2d p2(x2,y2);


				

				if (p1.Distance(p2) > 0 && p1.Distance(p2) < 0.1 )
					{
					qDebug() <<"x1:" << x1 << "y1:" << y1;
					qDebug() <<"x2:" << x2 << "y2:" << y2;
				TopoDS_Shape line = hsf::AddNew2dLineOnSurface(face,p1,p2);
				voronoiB.Add(voronoifolder,line);
					}
				

				}
			

				
			


	}

	delete voronoi;
	delete ver; // vrcholy
	delete edg;	 // hrany diagramu

	TopoDS_Shape result = voronoifolder;
	return  engine()->toScriptValue(result);
	
	}

   return  engine()->toScriptValue(false);

  

	}
QScriptValue HsfScriptingInterface::make2ddrawingpoly()
	{

	

	if (context()->argumentCount() == 5)
	 {
		 TopoDS_Shape shape = context()->argument(0).toVariant().value<TopoDS_Shape>();

		 gp_Pnt target = hsf::getpointfromshape(context()->argument(2).toVariant().value<TopoDS_Shape>());
		 gp_Pnt camera = hsf::getpointfromshape(context()->argument(1).toVariant().value<TopoDS_Shape>());
		 gp_Vec up = context()->argument(3).toVariant().value<gp_Vec>();
		double deflect = context()->argument(4).toNumber();
		 
		 TopoDS_Shape drawing = hsf::AddNewMake2dpoly(shape, camera, target, up,deflect);
		 
		 if (!drawing.IsNull())
			 {
			 return  engine()->toScriptValue(drawing);
			 } else {
				 return  engine()->toScriptValue(false); 
			 }

	  }

	 return  engine()->toScriptValue(false);


	

	}

QScriptValue HsfScriptingInterface::make2ddrawing()
	{

	

	if (context()->argumentCount() == 4)
	 {
		 TopoDS_Shape shape = context()->argument(0).toVariant().value<TopoDS_Shape>();

		 gp_Pnt target = hsf::getpointfromshape(context()->argument(2).toVariant().value<TopoDS_Shape>());
		 gp_Pnt camera = hsf::getpointfromshape(context()->argument(1).toVariant().value<TopoDS_Shape>());
		 gp_Vec up = context()->argument(3).toVariant().value<gp_Vec>();
		 
		 TopoDS_Shape drawing = hsf::AddNewMake2d(shape, camera, target, up);
		 
		 if (!drawing.IsNull())
			 {
			 return  engine()->toScriptValue(drawing);
			 } else {
				 return  engine()->toScriptValue(false); 
			 }

	  }

	 return  engine()->toScriptValue(false);


	

	}

QScriptValue HsfScriptingInterface::makegroup()
	{

	//QList<gp_Pnt2d> mypointlist;
	TopoDS_Compound group;
	B.MakeCompound(group);
	

   
	 if (context()->argumentCount() == 1)
	 {
		 QList<TopoDS_Shape> shapelist = context()->argument(0).toVariant().value<QList<TopoDS_Shape>>();
		
		 for(int i=0;i<shapelist.count();i++)
			 {
				TopoDS_Shape shape = shapelist.at(i);

				if (!shape.IsNull()){
				B.Add(group,shape);
				}

			 }
		 		
		 TopoDS_Shape result = group;
	     return  engine()->toScriptValue(result);
	  }

	 return  engine()->toScriptValue(false);


	}


QScriptValue HsfScriptingInterface::kinectstart()
	{

	try {
		 kinect.setDisplayInfoDepth (false);
		kinect.start();
		//connect(&kinect,SIGNAL(dataNotification()),parentwidget, SLOT( evaluatetext() ));
		connect(&kinect,SIGNAL(dataNotification()),this, SLOT( kinectprocessdata() ));
		
		} catch(...) {
		return QScriptValue(false);
		}
		return QScriptValue(true);
	}

QScriptValue HsfScriptingInterface::kinectprocessdata()
	{
		if (kinect.isRunning())
			{
			
			QTime thet = QTime::currentTime();
			int secs = thet.second();

			if (secs % 3 == 0) 
				{
				parentwidget->evaluatetext();
				}

			return QScriptValue(false);

			}

		return QScriptValue(false);

	}



QScriptValue HsfScriptingInterface::kinectgetimg()
	{
		if (kinect.isRunning())
			{
			
			QImage img = kinect.getDepth();
			return  engine()->toScriptValue(img);
			}


		return  engine()->toScriptValue(false);

	}

QScriptValue HsfScriptingInterface::kinectpoints()
	{

		 
		 
		QList<TopoDS_Shape> plist;
		 if (kinect.isRunning())
			 {
				QImage img = kinect.getDepth();

				for (int i = 0 ; i< img.width();i+=50)
					{
						for (int j=0;j<img.height();j+=50)
							{
								int value = qGray(img.pixel(i,j));

								if (value)
									{
								gp_Pnt p1(i ,j ,value);
								TopoDS_Shape pshape = HSF::AddNewPoint(p1);
								vis(engine()->toScriptValue(pshape));
								plist << pshape;
									}
								
								
					

							}

					}

				

				return engine()->toScriptValue(plist);


			 }
		
		
	}






QScriptValue HsfScriptingInterface::getcenterofgravity()
	{

	if (context()->argumentCount() == 1)
	 {
		 TopoDS_Shape shape = context()->argument(0).toVariant().value<TopoDS_Shape>();
		 gp_Pnt pt1 = hsf::GetCOG(shape);
		 
		 TopoDS_Shape ptshape = hsf::AddNewPoint(pt1);
				
		 
	     return  engine()->toScriptValue(ptshape);
	  }

	 return  engine()->toScriptValue(false);


	}



QScriptValue HsfScriptingInterface::makeface()
	{

	//QList<gp_Pnt2d> mypointlist;
   
	 if (context()->argumentCount() == 1)
	 {
		 //TopoDS_Shape face = context()->argument(0).toVariant().value<TopoDS_Shape>();
		 TopoDS_Shape  wire = context()->argument(0).toVariant().value<TopoDS_Shape>();
		 //bool  dir = context()->argument(2).toBool();

		 //face = hsf::getfacefromshape(face);

		 TopoDS_Shape trim = hsf::AddNewFace(wire,true);
		 		
	     return  engine()->toScriptValue(trim);
	  }

	 return  engine()->toScriptValue(false);


	}


QScriptValue HsfScriptingInterface::makesplineonsurface()
	{

	
   
	 if (context()->argumentCount() == 2)
	 {
		TopoDS_Shape surface = context()->argument(0).toVariant().value<TopoDS_Shape>();
		if (surface.IsNull()) return engine()->toScriptValue(false);

		 QList<gp_Pnt2d> mypointlist = context()->argument(1).toVariant().value<QList<gp_Pnt2d>>();
		 TopoDS_Shape bspline = hsf::AddNewSplineOnSurface(surface,mypointlist);

		 return  engine()->toScriptValue(bspline);
		 

	  
	 }
	return  engine()->toScriptValue(false);
	}

QScriptValue HsfScriptingInterface::makepointgrid()
	{
	 // xnum,ynum,xspace,yspace,origin ; 
     if (context()->argumentCount() == 5)
		 {
		 gp_Pnt origin = hsf::getpointfromshape(context()->argument(0).toVariant().value<TopoDS_Shape>());
		 double xnum = context()->argument(1).toNumber();
		 double ynum = context()->argument(2).toNumber();
	     double xspace = context()->argument(3).toNumber();
		 double yspace = context()->argument(4).toNumber();

		 double curx = origin.X();
		 double cury = origin.Y();
		 double curz = origin.Z();
		 
		 double newx;
		 double newy;

		 QList<gp_Pnt> plist;

		 for(int i=0; i < xnum; i++)
			 {
				for(int j= 0 ; j < ynum ; j++)
					{
					  newx= curx + (i* xspace);
					  newy = cury + (j* yspace);
					  gp_Pnt curpoint(newx,newy,curz);
					  plist << curpoint;

					}
			 }

		 return  engine()->toScriptValue(plist);

		 }
		return  engine()->toScriptValue(false);
	}

QScriptValue HsfScriptingInterface::getpointinsideshape()
	{

     if (context()->argumentCount() == 2)
	 {
	 
	  TopoDS_Shape obj1 = context()->argument(0).toVariant().value<TopoDS_Shape>();
	  if(obj1.IsNull()) {engine()->toScriptValue(false);}

	  gp_Pnt normalPoint = hsf::getpointfromshape(context()->argument(1).toVariant().value<TopoDS_Shape>());

      BRepClass3d_SolidClassifier* solidClassifier = new BRepClass3d_SolidClassifier(obj1);
      //normalPoint[1] = -1.0;
      solidClassifier->Perform(normalPoint, 0.5);
	  bool normalPointInside = !(solidClassifier->State() == TopAbs_State::TopAbs_OUT);
      
	  return  engine()->toScriptValue(normalPointInside);

	 }

	 return  engine()->toScriptValue(false);



	}
QScriptValue HsfScriptingInterface::getpointsinsideshape()
	{

     if (context()->argumentCount() == 3)
	 {
	 
	  TopoDS_Shape obj1 = context()->argument(0).toVariant().value<TopoDS_Shape>();
	  if(obj1.IsNull()) {engine()->toScriptValue(false);}

	  pointlist plist = context()->argument(1).toVariant().value<pointlist>();

	  bool direction = context()->argument(2).toNumber();


      pointlist resultlist;
      BRepClass3d_SolidClassifier* solidClassifier = new BRepClass3d_SolidClassifier(obj1);
      //normalPoint[1] = -1.0;
	  for (int i; i < plist.count(); i ++)
		  {
			gp_Pnt normalPoint = plist.at(i);
			solidClassifier->Perform(normalPoint, 0.5);
			bool normalPointInside = !(solidClassifier->State() == TopAbs_State::TopAbs_OUT);
			if (normalPointInside == direction) resultlist << normalPoint;
				
		  }
      
	  return  engine()->toScriptValue(resultlist);

	 }

	 return  engine()->toScriptValue(false);



	}

QScriptValue HsfScriptingInterface::crossproduct()
	{

	 if (context()->argumentCount() == 2)
	 {
		QScriptValue arg1 = context()->argument(0);
		QScriptValue arg2 = context()->argument(1);

		gp_Vec obj1 = context()->argument(0).toVariant().value<gp_Vec>();
		gp_Vec obj2  = context()->argument(1).toVariant().value<gp_Vec>();

		gp_Vec perpvec = obj1.Crossed(obj2);

		return engine()->toScriptValue(perpvec);

	 }


	}

QScriptValue HsfScriptingInterface::maketrimmedsurface()
	{

	 if (context()->argumentCount() == 5)
	 {
		TopoDS_Shape face = context()->argument(0).toVariant().value<TopoDS_Shape>();;
		double u1 = context()->argument(1).toNumber();
		double u2 = context()->argument(2).toNumber();
		double v1 = context()->argument(3).toNumber();
		double v2 = context()->argument(4).toNumber();
		
		if((u1!=u2)&&(v1!=v2))
			{
		TopoDS_Shape surface = HSF::AddNewTrimmedSrf(face,u1,u2,v1,v2);
		return engine()->toScriptValue(surface);
			}else{
			return engine()->toScriptValue(false);
			}

	 }


	}



QScriptValue HsfScriptingInterface::makeapplyuvcurve()
	{

	 if (context()->argumentCount() == 2)
	 {
		TopoDS_Shape face = context()->argument(0).toVariant().value<TopoDS_Shape>();
		TopoDS_Shape curve = context()->argument(1).toVariant().value<TopoDS_Shape>();;
		
		try{
			TopoDS_Shape curve2d = HSF::AddNew2dLineOnSurface(face,curve);
			if (curve2d.IsNull()) return engine()->toScriptValue(false);
			return engine()->toScriptValue(curve2d);

		}catch(...)
		{
				qDebug() << "something went wrong with apply2dcurve";
				return engine()->toScriptValue(false);
		}
		

	  }else{
		
		  return engine()->toScriptValue(false);
		
	  }

	 


	}



QScriptValue HsfScriptingInterface::getcsvrowcount(QScriptValue csv)
	{
	QList<QStringList> mylist = csv.toVariant().value<QList<QStringList>>();
	return engine()->toScriptValue(mylist.count());

	}

QScriptValue HsfScriptingInterface::getcsvcolumncount(QScriptValue csv,QScriptValue row)
	{
	QList<QStringList> mylist = csv.toVariant().value<QList<QStringList>>();
	if( (mylist.count()-1) > row.toNumber())
	{
		QStringList currow = mylist.at(row.toNumber());
		return engine()->toScriptValue(currow.count());
	}
	return engine()->toScriptValue(false);

	}


QScriptValue HsfScriptingInterface::getcsvrow(QScriptValue csv,QScriptValue row , QScriptValue column)
	{
	QList<QStringList> mylist = csv.toVariant().value<QList<QStringList>>();
	if( (mylist.count()) > row.toNumber())
	{
		QStringList currow = mylist.at(row.toNumber());
		if ((currow.count()) > column.toNumber())
		{
		QString curvalue = currow.at(column.toNumber());
		return engine()->toScriptValue(curvalue);
		}
		return engine()->toScriptValue(false);
	}

	return engine()->toScriptValue(false);
	}



void HsfScriptingInterface::printlist(QScriptValue list)
{

	QList<QStringList> mylist = list.toVariant().value<QList<QStringList>>();
	if (mylist.count()>0) 
	{
		for(int i=0;i < mylist.count(); i++)
		{
			QStringList mycurstringlist = mylist.at(i);
			
			QString outval;
			for(int j=0;j < mycurstringlist.count(); j++)
			{
				outval += QString(",") + mycurstringlist.at(j) ;
			}
			parentwidget->ui.listener->addItem(outval);

			


		}
	}

}

QScriptValue HsfScriptingInterface::makepointfromlist(QScriptValue list)
{

TopoDS_Compound pointfolder;
BRep_Builder pointB;

pointB.MakeCompound(pointfolder);


	QList<QStringList> mylist = list.toVariant().value<QList<QStringList>>();
	if (mylist.count()>0) 
	{
		for(int i=0;i < mylist.count(); i++)
		{
			QStringList mycurstringlist = mylist.at(i);
			
			//QString outval;
			double x = mycurstringlist.at(0).toDouble();
			double y = mycurstringlist.at(1).toDouble();
			double z = mycurstringlist.at(2).toDouble();

			gp_Pnt curpoint(x,y,z);
			TopoDS_Shape myshape = hsf::AddNewPoint(curpoint);
			pointB.Add(pointfolder,myshape);
			
			


		}
	}
	TopoDS_Shape resultshape = pointfolder;
	return engine()->toScriptValue(resultshape);
}


QScriptValue HsfScriptingInterface::getdistance()
{
     double distanceval = 0;
	 if (context()->argumentCount() == 2)
	 {
		QScriptValue arg1 = context()->argument(0);
		QScriptValue arg2 = context()->argument(1);
	

		QVariant var1,var2;

		if (arg1.isVariant() && arg2.isVariant())
		{

		 var1 = arg1.toVariant(); 
		 var2 = arg2.toVariant(); 
		 
		 QString var1typename = var1.typeName();
		 QString var2typename = var2.typeName();
		
		 if (var1typename == "TopoDS_Shape" && var2typename == "TopoDS_Shape")
		 {
		 TopoDS_Shape obj1 = context()->argument(0).toVariant().value<TopoDS_Shape>();
		 TopoDS_Shape obj2  = context()->argument(1).toVariant().value<TopoDS_Shape>();
		 
		 distanceval = hsf::GetMaxDis(obj1,obj2);


		 }

		}
	 
	 } 


	      
     return engine()->toScriptValue(distanceval);
}


QScriptValue HsfScriptingInterface::getcurvelength()
{
     double distanceval = 0;
	 if (context()->argumentCount() == 1)
	 {
		QScriptValue arg1 = context()->argument(0);
		
	

		QVariant var1;

		if (arg1.isVariant())
		{

		 var1 = arg1.toVariant(); 
	
		 
		 QString var1typename = var1.typeName();
				
		 if (var1typename == "TopoDS_Shape" )
		 {
		 TopoDS_Shape obj1 = context()->argument(0).toVariant().value<TopoDS_Shape>();
				 
		 distanceval = hsf::GetLength(obj1);


		 }

		}
	 
	 } 


	      
     return engine()->toScriptValue(distanceval);
}


void HsfScriptingInterface::setuprunonce()
{
	runoncecounter =0;
	runoncefilecounter=0;
	runoncedircountercounter=0;
}

void HsfScriptingInterface::finishrunonce()
{

if (runoncecounter > 0)
{

} else 
{
	runoncefilecounter = 0;
	if (parentwidget->ui.runonceset->count())
	{
		QLayoutItem* widgetitem = parentwidget->ui.runonceset->layout()->takeAt(0);
		delete widgetitem->widget();
		delete widgetitem;
	}



	
}
runoncecounter =0;
}


void HsfScriptingInterface::setparentwidget(scriptwidget* thewidget)
{
parentwidget = thewidget;


	 // Connect signals to our slots
 connect( filewatch, SIGNAL( directoryChanged ( const QString& ) ), 
          parentwidget, SLOT( evaluatetext() ) );
 
 connect( filewatch, SIGNAL( fileChanged ( const QString& ) ), 
          parentwidget, SLOT( evaluatetext() ) );


}
QScriptValue HsfScriptingInterface::getmousepos()
{	
	return engine()->toScriptValue(hsf::AddNewPoint(CurrentMousePoint));
}
QScriptValue HsfScriptingInterface::getfileonce()
{	runoncecounter++;

	QScriptValue filename ; 
	if (runoncefilecounter > 0)
	{
		if (parentwidget->ui.runonceset->layout()->count()> 0)
		{
			QRadioButton* widgetitem = qobject_cast<QRadioButton*> (parentwidget->ui.runonceset->layout()->itemAt(0)->widget());
			if (widgetitem && widgetitem->isChecked()) 
			{
			  LastSavedrunonceFilename = getfile().toString(); 
			} else if (widgetitem && !widgetitem->isChecked())
		    {
			 // LastSavedrunonceFilename.clear();
			}
		} else {
		LastSavedrunonceFilename.clear();
		}

	} else {
		
		LastSavedrunonceFilename = getfile().toString();
		if ( LastSavedrunonceFilename.length() > 0 ) 
		{
		QRadioButton *button = new QRadioButton("GetFile", 0);
		button->setObjectName( "getfileonce");
		QString curobjname = button->objectName();
		parentwidget->ui.runonceset->addWidget(button);

		connect(button,SIGNAL(toggled()),parentwidget,SLOT(evaluatetext()));

		runoncefilecounter++;
		}
	}

	return engine()->toScriptValue(LastSavedrunonceFilename);
}
QScriptValue HsfScriptingInterface::getdironce()
{
	return getdir();
}

// mesh the shape using the volumetric mesher from salome
QScriptValue HsfScriptingInterface::volmesh()
{
	 if(context()->argumentCount() == 1)
	 {
	     TopoDS_Shape aShape = context()->argument(0).toVariant().value<TopoDS_Shape>();
		 if (aShape.IsNull()) 
		 {
			 return engine()->toScriptValue(false);
		 }

//****************************************************************************
//***************************CREATE SMESH OBJECTS*****************************
SMESH_Gen *aMeshGen = new SMESH_Gen();
SMESH_Mesh *aMesh = aMeshGen->CreateMesh(0,true);


//NETGENPlugin_Mesher* netgensettings = new NETGENPlugin_Mesher(0,0,aMeshGen);
//NETGENPlugin_NETGEN_2D3D * netgenhypo = new NETGENPlugin_NETGEN_2D3D (1,0, aMeshGen);
//NETGENPlugin_SimpleHypothesis_3D* netgensettings = new NETGENPlugin_SimpleHypothesis_3D(0,0,aMeshGen);

StdMeshers_Arithmetic1D *an1DHypothesis = new StdMeshers_Arithmetic1D(0,0,aMeshGen);
an1DHypothesis->SetLength(100, false);
an1DHypothesis->SetLength(500., true);

StdMeshers_Regular_1D *an1DAlgo = new StdMeshers_Regular_1D(1,0,aMeshGen);


StdMeshers_QuadranglePreference *a2DHypothesis = new StdMeshers_QuadranglePreference(2,0,aMeshGen);

StdMeshers_Quadrangle_2D *a2DAlgo = new StdMeshers_Quadrangle_2D(3,0,aMeshGen);


StdMeshers_Prism_3D* a3dHypothesis = new StdMeshers_Prism_3D(4,0,aMeshGen);StdMeshers_MaxElementVolume* maxvol = new StdMeshers_MaxElementVolume(5,0,aMeshGen);
//StdMeshers_Hexa_3D* a3dHypothesis = new StdMeshers_Hexa_3D(4,0,aMeshGen);//a3dHypothesis->SetMaxVolume(1000000000/30);
//#a3dHypothesis = StdMeshers_CompositeHexa_3D(4,0,aMeshGen) #Ok
//#a3dHypothesis = StdMeshers_RadialPrism_3D(4,0,aMeshGen) # Don't work?


//****************************************************************************
//**************************CALCULATE MESH************************************
//ADD SHAPE TO MESH
aMesh->ShapeToMesh(aShape);

//ASSIGN FIRST EDGE WITH HYPOTHESIS //Somehow smesh needs 1d algo to be assigned directly to an edge ... ???
TopExp_Explorer Exp3 (aMesh->GetShapeToMesh(), TopAbs_EDGE);
while ( Exp3.More() ) {
aMesh->AddHypothesis( Exp3.Current() , 0 );
//aMesh->RebuildPropagationChains();
break;
}

//ASSIGN HYPOTHESIS TO MESH
aMesh->AddHypothesis( aShape , 0 );
aMesh->AddHypothesis( aShape , 1 );
aMesh->AddHypothesis( aShape , 2 );
aMesh->AddHypothesis( aShape , 3 );
aMesh->AddHypothesis( aShape , 4 );

//COMPUTE THE DATA
aMeshGen->Compute(*aMesh, aMesh->GetShapeToMesh() );

//****************************************************************************
//****************************EXPORT THE DATA*********************************

//int groupindex=0;
//TopExp_Explorer Exp4;
//
//for (Exp4.Init(aMesh->GetShapeToMesh(),TopAbs_FACE); Exp4.More(); Exp4.Next())
//     {
//          TopoDS_Shape curshape = Exp4.Current();
//
// 
//  char* groupname = QString::number(groupindex).toAscii().data();
// SMESHDS_GroupOnGeom* group_load = new SMESHDS_GroupOnGeom(groupindex,aMesh->GetMeshDS(),SMDSAbs_Face, curshape);// aMesh->AddGroup( SMDSAbs_Face,groupname,groupindex,group_load->GetShape());// groupindex +=1;//// break;//
//}


aMesh->ExportDAT("_TEST.DAT");
aMesh->ExportUNV("_TEST.UNV");
//aMesh->ExportSTL("_TEST.stl",true);


//print 'Done.'
return engine()->toScriptValue(QString("smeshworked"));


	  } else { return engine()->toScriptValue(false);}

	}


 QScriptValue HsfScriptingInterface::importstp()
 {

	 TopoDS_Shape curimport;

	  if(context()->argumentCount() == 1)
	 {
		 QString filename = context()->argument(0).toString();
		 QFileInfo curfile(filename);
		 if (filename == LastImportFilename)
		 {
			curimport = LastImportShape;
		 }else if (curfile.isFile() && (filename.contains("stp")) || (filename.contains("step")) )
		 {
			 Handle(TopTools_HSequenceOfShape) importedsequence = io_man->importSTEP(filename);
			 if (importedsequence->Length() > 0)
			 {
				curimport = importedsequence->Value(1);
				LastImportShape = curimport;
			 } // end of check something inside file
			LastImportFilename  = filename;
		 } // end of check filename

		 
	 } // end of check argument


return engine()->toScriptValue(curimport);



 }


 QScriptValue HsfScriptingInterface::watchfile()
{

if(context()->argumentCount() == 1)
	 {
		 QString filename = context()->argument(0).toString();
		 try{
			 if (!filewatch->files().contains(filename)) {
					filewatch->addPath( filename );
				 }
			 }
		 catch(...)
			 {
				return engine()->toScriptValue(false);
			 }
		 // Constructing QFileSystemWatcher
		return engine()->toScriptValue(true);

	}
return engine()->toScriptValue(false); 
}

QScriptValue HsfScriptingInterface::exportigs()
	{


	Handle(TopTools_HSequenceOfShape) exportshapes = new TopTools_HSequenceOfShape();
	if(context()->argumentCount() == 2)
	{
		 QList<TopoDS_Shape> shapestoexport = context()->argument(0).toVariant().value<QList<TopoDS_Shape>>();
		 QString filename = context()->argument(1).toString();
		
		 for (int i=0; i < shapestoexport.count();i++)
		 {				
		  exportshapes->Append( shapestoexport.at(i) );
		 }
		if (exportshapes->Length() > 0)
		{
		io_man->exportIGES(filename,exportshapes);
		return engine()->toScriptValue(true);
		} // end of check something inside file
	} // end of check argument


return engine()->toScriptValue(false);
}


QScriptValue HsfScriptingInterface::exportstp()
	{


	Handle(TopTools_HSequenceOfShape) exportshapes = new TopTools_HSequenceOfShape();
	if(context()->argumentCount() == 2)
	{
		 QList<TopoDS_Shape> shapestoexport = context()->argument(0).toVariant().value<QList<TopoDS_Shape>>();
		 QString filename = context()->argument(1).toString();
		
		 for (int i=0; i < shapestoexport.count();i++)
		 {				
		  exportshapes->Append( shapestoexport.at(i) );
		 }
		if (exportshapes->Length() > 0)
		{
		io_man->exportSTEP(filename,exportshapes);
		return engine()->toScriptValue(true);
		} // end of check something inside file
	} // end of check argument


return engine()->toScriptValue(false);
}

QScriptValue HsfScriptingInterface::exportbrep()
	{


	Handle(TopTools_HSequenceOfShape) exportshapes = new TopTools_HSequenceOfShape();
	if(context()->argumentCount() == 2)
	{
		 QList<TopoDS_Shape> shapestoexport = context()->argument(0).toVariant().value<QList<TopoDS_Shape>>();
		 QString filename = context()->argument(1).toString();
		
		 for (int i=0; i < shapestoexport.count();i++)
		 {				
		  exportshapes->Append( shapestoexport.at(i) );
		 }
		if (exportshapes->Length() > 0)
		{
		io_man->exportBREP(filename,exportshapes);
		return engine()->toScriptValue(true);
		} // end of check something inside file
	} // end of check argument


return engine()->toScriptValue(false);
}


 QScriptValue HsfScriptingInterface::importigs()
 {

	 TopoDS_Shape curimport;

	  if(context()->argumentCount() == 1)
	 {
		 QString filename = context()->argument(0).toString();
		 QFileInfo curfile(filename);
		// if (filename == LastImportFilename)
		 //{
		//	curimport = LastImportShape;
		 //}else if (curfile.isFile() && (filename.contains("igs")) || (filename.contains("iges")) )
		 //{
			 Handle(TopTools_HSequenceOfShape) importedsequence = io_man->importIGES(filename);
			 if (importedsequence->Length() > 0)
			 {
				curimport = importedsequence->Value(1);
				LastImportShape = curimport;
			 } // end of check something inside file
			LastImportFilename  = filename;
		 //} // end of check filename

		 
	 } // end of check argument


return engine()->toScriptValue(curimport);



 }




QScriptValue HsfScriptingInterface::fitall()
{

	needstofitall = true;
	qGeomApp->myview->fitAll();
	return engine()->toScriptValue(true);

}

QScriptValue HsfScriptingInterface::getdir()
{
 
	 QString myfilename;
	 myfilename = QFileDialog::getExistingDirectory ( parentwidget->parentWidget(),
							 tr("Open Folder"),"");
	      
     return engine()->toScriptValue(myfilename);

}

QScriptValue HsfScriptingInterface::getfile()
 {
	 
	 QString myfilename;
	 myfilename = QFileDialog::getOpenFileName ( parentwidget->parentWidget(),
							 tr("Import File"),"");
	      
     return engine()->toScriptValue(myfilename);
 }


QScriptValue HsfScriptingInterface::makepointoncurve()
 {
	 
	 TopoDS_Shape myptshape;
	 gp_Pnt p1;
	 if (context()->argumentCount() == 2)
	 {
		 TopoDS_Shape crv1 = context()->argument(0).toVariant().value<TopoDS_Shape>();
		 if (crv1.IsNull()) 
		 {
			 return engine()->toScriptValue(false);
		 }
		 double ratio = context()->argument(1).toNumber();
		 p1 = hsf::AddNewPointonCurve(crv1,ratio);
		 myptshape = hsf::AddNewPoint(p1);
		 
	 }
	 
	 QScriptValue myptval = engine()->toScriptValue(myptshape);
	 double x,y,z;
	 x = p1.X();y = p1.Y();z = p1.Z();

	 myptval.setProperty(QString("x"),engine()->toScriptValue(x));
	 myptval.setProperty(QString("y"),engine()->toScriptValue(y));
	 myptval.setProperty(QString("z"),engine()->toScriptValue(z));

	QScriptValue xvaltest = myptval.property(QString("x"));
	double xval = xvaltest.toNumber();


     return myptval;
 }



QScriptValue HsfScriptingInterface::makeprojectcrvtosrf()
 {
	 
	 TopoDS_Shape myshape;
	
	 if (context()->argumentCount() == 3)
	 {
		 TopoDS_Shape curve = context()->argument(0).toVariant().value<TopoDS_Shape>();
		  TopoDS_Shape surface = context()->argument(1).toVariant().value<TopoDS_Shape>();
		 gp_Vec direction = context()->argument(2).toVariant().value<gp_Vec>();
		 if (surface.IsNull() && curve.IsNull()) 
		 {
			 return engine()->toScriptValue(false);
		 }
		 TopoDS_Shape projcurve = hsf::AddNewProjectCurve(surface,curve,direction);
		 myshape = projcurve;
		 
	 }
	 
	 QScriptValue myshapeval = engine()->toScriptValue(myshape);
	 return myshapeval;
 }

QScriptValue HsfScriptingInterface::makeoffsetsurface()
 {
	 
	 TopoDS_Shape myshape;
	
	 if (context()->argumentCount() == 2)
	 {
		 TopoDS_Shape surface = context()->argument(0).toVariant().value<TopoDS_Shape>();
		 surface = hsf::getfacefromshape(surface);
		 if (surface.IsNull()) 
		 {
			 return engine()->toScriptValue(false);
		 }
		 double offset = context()->argument(1).toNumber();
		 myshape = hsf::AddNewOffsetSurface(surface,offset);
		 
	 }
	 
	 QScriptValue myshapeval = engine()->toScriptValue(myshape);
	 return myshapeval;
 }

QScriptValue HsfScriptingInterface::makecontrainedsrf()
 {
	 
	 TopoDS_Shape myshape;
	
	 if (context()->argumentCount() == 1)
	 {
		 QList<TopoDS_Shape> shapelist = context()->argument(0).toVariant().value<QList<TopoDS_Shape>>();
		 if (shapelist.count() < 1) 
		 {
			 return engine()->toScriptValue(false);
		 }
		 
		 myshape = hsf::AddNewConstrainedSurface(shapelist);
		 
	 }
	 
	 QScriptValue myshapeval = engine()->toScriptValue(myshape);
	 return myshapeval;
 }

QScriptValue HsfScriptingInterface::makeconicalsrf()
{


	if (context()->argumentCount() == 4)
	{
		 gp_Pnt p1 = context()->argument(0).toVariant().value<gp_Pnt>();
		 gp_Pnt p2 = context()->argument(1).toVariant().value<gp_Pnt>();
		 gp_Pnt p3 = context()->argument(2).toVariant().value<gp_Pnt>();
		 gp_Pnt p4 = context()->argument(3).toVariant().value<gp_Pnt>();

		 TopoDS_Shape conic = hsf::AddNewConicalSurface(p1,p2,p3,p4);
		 if ( conic.IsNull()) return engine()->toScriptValue(false);
		 engine()->toScriptValue(conic);
	}

	return engine()->toScriptValue(false);

}


void HsfScriptingInterface::savecode()
	{
if (context()->argumentCount() == 1)
{
	QString path = context()->argument(0).toString();

	QString codefilename = path;

	QFile data(codefilename);
	if (data.open(QFile::ReadWrite)) {
     QTextStream out(&data);
     out << parentwidget->gettext();
	}
	data.close();
}


	}

QScriptValue HsfScriptingInterface::publishproject()
	{

			if (context()->argumentCount() == 3)
			{
			

			/*QString path = context()->argument(0).toString();
			parentwidget->myuploader->setpath(path);
			parentwidget->myuploader->show();
			return QScriptValue(true);*/
			
			QString path = context()->argument(0).toString();
			QString projectname = context()->argument(1).toString();
			QString author = context()->argument(2).toString();

			QString program =  qApp->applicationDirPath() + tr("\\ftpuploader.exe");
			

			QString ftpbasedir("ftp://penadeleon.com/workshop_occ_pentaho/useruploads/");
				ftpbasedir = ftpbasedir +  projectname + tr("_") + author ;
				//QString ftpbasedir("ftp://ftp.penadeleon.com/workshop_occ_pentaho/useruploads/");

			QString ftpnewfolder = ftpbasedir;

				QDir currentDir = QDir(path);
				QString fileName;
			    
				QStringList files;
				 if (fileName.isEmpty()) fileName = "*";
				 files = currentDir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);

				 if (files.isEmpty())
				 return QScriptValue(false);

				 for (int i=0;i<files.count();i++)
					{

						QString curfilepath = files.at(i);
						QFileInfo myfileinfo(curfilepath);
						fileName = myfileinfo.fileName(); 
						

						fileName = ftpnewfolder +  tr("/") +fileName;

						QStringList arguments;
						qDebug() << "debug:" << currentDir.absolutePath()+ tr("/") + curfilepath  << fileName;
						arguments << currentDir.absolutePath()+ tr("/") + curfilepath << ftpnewfolder <<  fileName;

						QProcess *myProcess = new QProcess(appui::getInstance()->getWindow());
						myProcess->start(program, arguments);
						myProcess->waitForFinished();
						
						QString uploadresult(myProcess->readAll());
						parentwidget->ui.listener->addItem(uploadresult);

						//return QScriptValue(true);
					} // end of for
				return QScriptValue(true);
				}
				
				
	return QScriptValue(false);

	}

 QScriptValue HsfScriptingInterface::savescreenshot()
 {
	if (context()->argumentCount() == 1)
		{
	QString filepath =  context()->argument(0).toString();
	
	QFileInfo myfileinfo(filepath);
	myfileinfo.absoluteDir().mkpath(myfileinfo.absolutePath());

	//QPixmap originalPixmap = QPixmap::grabWindow(ui::getInstance()->getWindow()->mySketchFrame->winId());
	QDesktopWidget* desktop = QApplication::desktop();


	
	QPixmap originalPixmap = QPixmap::grabWindow(desktop->winId());
    

     QString format = "png";
     QString initialPath = filepath + tr(".") + format;

     
     if (!initialPath.isEmpty())
		 {
         originalPixmap.save(initialPath, format.toAscii());
		return QScriptValue(true);
		 }
		}

	return QScriptValue(false);
 }



QString HsfScriptingInterface::getlinefromcontext(QStringList mylist)
{

  //qDebug() << "backtrace values" << mylist;
  QString comand = mylist.at(1);
  int linenumber = comand.split("at").at(1).toInt()-1;

  //qDebug() << "line number" << linenumber;

  QString lineat;
  lineat  = parentwidget->textEdit->text(linenumber-1);

  //qDebug() << "line at " << lineat;

  return lineat;

}

QScriptValue HsfScriptingInterface::getval()
{

//qDebug() << "getval started";
	if (context()->argumentCount() == 4)
	{
//	qDebug() << "getval entered if statement arg count > 3";
	
	 QStringList mylist = context()->backtrace();
	 
	 QString lineat = getlinefromcontext(mylist);

//	 qDebug() << "lineat worked" << lineat;
	 QString varname = lineat.split("=").at(0).trimmed();

//	 qDebug() << "getval varname = " << varname;
	 
	 int pos = context()->argument(0).toInteger();
	 QString name = varname;
	 QString namelbl = varname +QString("txt");

//	 qDebug() << "getval varname namelbl= " << namelbl;



	 int minval = context()->argument(1).toInteger();
	 int maxval = context()->argument(2).toInteger();

	 QString colorname;
	 /*if (context()->argumentCount() < 4 )
		 {
		 colorname = "green";
		 } else {*/
	 	 colorname = context()->argument(3).toString();
		 /*}*/
	 QColor slidercolor(colorname);

	 double r = slidercolor.red();
	 double g = slidercolor.green();
	 double b = slidercolor.blue();

	 QString colorstring = tr("rgb(") + QString::number(r) + tr(", ") + QString::number(g) + tr(", ") + QString::number(b) + tr(")");

		 if (slidermap.contains(name)) // if slider is already in the map ge the value
	{
//	    qDebug() << "getval slidermap contains the slider = " ;

		QSlider* curslider = slidermap[name];
		int index = parentwidget->ui.sliderset->layout()->indexOf(curslider);

//		qDebug() << "getval slider pointer found = " ;
		 
		if (pos != index)
		{
//		qDebug() << "getval slider if pos ! index = " ;

		QLayoutItem* curitem = parentwidget->ui.sliderset->layout()->takeAt(index);
//		qDebug() << "getval slider curitem worked" ;
		QLayoutItem* curitemlbl = parentwidget->ui.labelset->layout()->takeAt(index);
//		qDebug() << "getval slider curitemlbl worked" ;
		parentwidget->ui.sliderset->insertWidget(pos,curitem->widget());
//		qDebug() << "getval slider insertwidget worked" ;
		parentwidget->ui.labelset->insertWidget(pos,curitemlbl->widget());
//		qDebug() << "getval slider insert label worked" ;
		}
		
		curslider->setMinimum(minval);
//		qDebug() << "getval slider set minimum" ;
		curslider->setMaximum(maxval);
//		qDebug() << "getval slider set maximum" ;
		double val = curslider->value();
//		qDebug() << "getval slider get value worked" ;
		currentslidermap.insert(name,curslider);
//		qDebug() << "getval slidermap insert name worked" ;
		return engine()->toScriptValue(val);
	} else // else create a new slider
	{
//	    qDebug() << "gettval new slider creation" ;

		QGraphicsDropShadowEffect* dropeffect1 = new QGraphicsDropShadowEffect();
		dropeffect1->setColor(QColor(0, 0, 0));
		dropeffect1->setBlurRadius(20);
		dropeffect1->setOffset(4,4);

		
		QGraphicsDropShadowEffect* dropeffect2 = new QGraphicsDropShadowEffect();
		dropeffect2->setColor(QColor(0, 0, 0));
		dropeffect2->setBlurRadius(20);
		dropeffect2->setOffset(4,4);

		QSlider* curslider = new QSlider(Qt::Orientation::Horizontal );
		curslider->setTickPosition(QSlider::TicksAbove);
        curslider->setTickInterval(2/100 * maxval );

		
		 QString stylesheet = QString(" QSlider { \n");
		 //stylesheet += tr("border: 1px solid #999999;\n");
		 stylesheet += tr("padding: 1px ;}\n");
		stylesheet += tr(" QSlider::groove:horizontal { \n");
        stylesheet += tr("height: 8px; \n");
		stylesheet += tr("padding: 3px; \n");
        stylesheet += tr("background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);\n");
		stylesheet += tr("margin: 2px 0;}\n");
        stylesheet += tr(" QSlider::handle:horizontal {\n");
        stylesheet += tr("background-color: ");
	    stylesheet += colorstring + tr(";\n");
		stylesheet += tr("border-style: outset;\n");
		stylesheet += tr("border-color: black;\n");
        stylesheet += tr(" border-width: 0.5px;\n");
        stylesheet += tr("width: 18px;\n");
        stylesheet += tr("margin: -2px 0;\n");
		stylesheet += tr("border-radius: 3px;}\n");

		curslider->setStyleSheet(stylesheet);

		//curslider->setGraphicsEffect(dropeffect1);
 

		
//		qDebug() << "getval curslider worked" ;
		curslider->setObjectName(name);
//		qDebug() << "getval curslider set name" ;
		curslider->setMinimum(minval);
//		qDebug() << "getval curslider set minimum" ;
		curslider->setMaximum(maxval);
//		qDebug() << "getval curslider set maximum" ;
		connect(curslider,SIGNAL(sliderMoved(int)),parentwidget,SLOT(evaluatetext()));
		
		slidermap.insert(name,curslider);
		parentwidget->ui.sliderset->insertWidget(pos,curslider);

		QLabel* curlabel = new QLabel(name);

		
		//curlabel->setGraphicsEffect(dropeffect2);
		curlabel->setObjectName(namelbl);

		stylesheet = QString::fromUtf8("background-color: ") + colorstring + tr(";\n");
            stylesheet +=tr("color: rgb(0, 0, 0);\n");
			stylesheet +=tr("border-style: outset;\n");
			stylesheet +=tr(" border-width: 0.5px;\n");
			stylesheet +=tr("border-radius: 4.5px;\n");
			stylesheet +=tr("border-color: black;\n");
			stylesheet +=tr("padding: 1px;");

		curlabel->setStyleSheet(stylesheet);

		curlabel->setMaximumHeight(16);
		

		parentwidget->ui.labelset->insertWidget(pos,curlabel);
		currentslidermap.insert(name,curslider);

        QSpinBox* curspinbox = new QSpinBox();
		curspinbox->setRange(minval, maxval);
        curspinbox->setSingleStep(1);
		curspinbox->setButtonSymbols(QAbstractSpinBox::PlusMinus);
		curspinbox->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);"));
		curspinbox->setSingleStep(1);

		connect(curspinbox, SIGNAL(valueChanged(int)),curslider, SLOT(setValue(int)));
		connect(curspinbox, SIGNAL(valueChanged(int)),parentwidget,SLOT(evaluatetext()));
		connect(curslider,SIGNAL(valueChanged(int)),curspinbox, SLOT(setValue(int)));

		curspinbox->setMaximumHeight(16+2);
		
		
		parentwidget->ui.numberset->insertWidget(pos,curspinbox);
		

		
 
		return QScriptValue(false);
	}
	}

}

void HsfScriptingInterface::checksliders()
{

QList<QLayoutItem*> tobedeleted;

QList<QSlider*> sliders = slidermap.values();

	for (int i=0;i<sliders.count();i++)
	{
		QSlider* curslider = slidermap[sliders[i]->objectName()];
		if (!currentslidermap.contains(curslider->objectName()))
		{
			slidermap.remove(sliders[i]->objectName());
			int index = parentwidget->ui.sliderset->layout()->indexOf(sliders[i]);
			QWidget* curlabel = parentwidget->ui.labelset->layout()->itemAt(index)->widget();
			QWidget* curspinbox = parentwidget->ui.numberset->layout()->itemAt(index)->widget();
			delete curlabel;
			delete curslider;
			delete curspinbox;
		}
	}


	//for (int i=0;i<tobedeleted.count();i++)
	//{
	//	QLayoutItem* curitem = tobedeleted[i];
	//	//tobedeleted.removeLast();
	//	
	//	if (curitem)
	//	{
	//	if (curitem->isEmpty()) continue;
	//	QSlider* slideritem = qobject_cast<QSlider*>(curitem->widget());
	//	QLabel* labelitem = qobject_cast<QLabel*>(curitem->widget());
	//	QLayoutItem* takenitem;
	//	if (slideritem)
	//	{
	//	int index = parentwidget->ui.sliderset->layout()->indexOf(curitem->widget());
	//	takenitem = parentwidget->ui.sliderset->layout()->takeAt(index);
	//	}
	//	if (labelitem)
	//	{
	//	int index = parentwidget->ui.labelset->layout()->indexOf(curitem->widget());
	//	takenitem = parentwidget->ui.sliderset->layout()->takeAt(index);
	//	}

	//	QString objname = takenitem->widget()->objectName();
	//	delete takenitem->widget();
	//	
	//	//delete takenitem;
	//	}
	//}



}


QScriptValue HsfScriptingInterface::makeplane()
 {
	 
	 

	 if (context()->argumentCount() == 2)
	 {
		
		 gp_Pnt orig = hsf::getpointfromshape(context()->argument(0).toVariant().value<TopoDS_Shape>());
    	 gp_Vec vector = context()->argument(1).toVariant().value<gp_Vec>();
		 
		 Handle_Geom_Plane myplane =  GC_MakePlane(orig, vector);
		 gp_Pln mypln = myplane->Pln();

		 return engine()->toScriptValue(mypln);
		 
	 }
	     
		
     return engine()->toScriptValue(false);
 }

QScriptValue HsfScriptingInterface::makepoint(double x, double y, double z)
{
	 TopoDS_Shape myptshape;
	 gp_Pnt mypt ;
	
    	 mypt = gp_Pnt(x,y,z);
		 myptshape = hsf::AddNewPoint(mypt);
		 
	 
	     
	 QScriptValue myptval = engine()->toScriptValue(myptshape);
	 myptval.setProperty(QString("x"),engine()->toScriptValue(mypt.X() ));
	 myptval.setProperty(QString("y"),engine()->toScriptValue(mypt.Y()));
	 myptval.setProperty(QString("z"),engine()->toScriptValue(mypt.Z()));


     return engine()->toScriptValue(myptshape);
 }

QScriptValue HsfScriptingInterface::getcoord()
	{
	
	double curval = 0;
	 if (context()->argumentCount() == 2)
	 {
		 gp_Pnt dapoint = hsf::getpointfromshape(context()->argument(0).toVariant().value<TopoDS_Shape>());
		 int sel = context()->argument(1).toNumber();

		 if (sel == 0) curval = dapoint.X();
		 if (sel == 1) curval = dapoint.Y();
		 if (sel == 2) curval = dapoint.Z();

		 return curval;

      }

	}

QScriptValue HsfScriptingInterface::getvectorangle()
	{
	
	double curval = 0;
	 if (context()->argumentCount() == 2)
	 {
		 gp_Vec v1 = context()->argument(0).toVariant().value<gp_Vec>();
		 gp_Vec v2 = context()->argument(1).toVariant().value<gp_Vec>();

		 int angle = v1.Angle(v2);
		 return angle;

      }

	  

	  return engine()->toScriptValue(false);

	}

QScriptValue HsfScriptingInterface::makepoint()
 {
	 TopoDS_Shape myptshape;
	 gp_Pnt mypt ;
	 if (context()->argumentCount() == 3)
	 {
		 double x,y,z;

		 if (context()->argument(0).toString().toLower() == "infinity") {x = 0;} else { x = context()->argument(0).toNumber();}
		 if (context()->argument(1).toString().toLower() == "infinity") {y = 0;} else { y = context()->argument(1).toNumber();}
		 if (context()->argument(2).toString().toLower() == "infinity") {z = 0;} else { z = context()->argument(2).toNumber();}

    	 mypt = gp_Pnt(x,y,z);
		 myptshape = hsf::AddNewPoint(mypt);
		 
	 }
	     
	 QScriptValue myptval = engine()->toScriptValue(myptshape);
	 myptval.setProperty(QString("x"),engine()->toScriptValue(mypt.X() ));
	 myptval.setProperty(QString("y"),engine()->toScriptValue(mypt.Y()));
	 myptval.setProperty(QString("z"),engine()->toScriptValue(mypt.Z()));


     return engine()->toScriptValue(myptshape);
 }

QScriptValue HsfScriptingInterface::panelize()
{

	
if(context()->argumentCount() == 4)
	 {
	 TopoDS_Shape surface1 = context()->argument(0).toVariant().value<TopoDS_Shape>();
	 int x = context()->argument(1).toNumber();
	 int y = context()->argument(2).toNumber();
	 QScriptValue panelfunc = context()->argument(3);
	 
	 if (panelfunc.isFunction())
	 {
	
	TopoDS_Compound folder;
	BRep_Builder B;
	B.MakeCompound(folder);
	int viscount =0;

	 QMap<QString,QVariant> plist = HSF::BuildPointGridonSrf(surface1,x,y);
		QMapIterator<QString,QVariant> i(plist);
		while (i.hasNext()){
		i.next();
			QString currentname = i.key();
			QString epn = HSF::GetNextUvName(currentname,1,0);
			QString sepn = HSF::GetNextUvName(currentname,1,1);
			QString swpn = HSF::GetNextUvName(currentname,0,1);
			if(plist.contains(epn) && plist.contains(sepn) && plist.contains(swpn)){


				gp_Ax1 p1val = i.value().value<gp_Ax1>();
				gp_Ax1 p2val = plist.value(epn).value<gp_Ax1>();
				gp_Ax1 p3val = plist.value(sepn).value<gp_Ax1>();
				gp_Ax1 p4val = plist.value(swpn).value<gp_Ax1>();
				
				
				gp_Pnt p1 = p1val.Location();
				gp_Pnt p2 = p2val.Location();
				gp_Pnt p3 = p3val.Location();
				gp_Pnt p4 = p4val.Location();
				gp_Vec v1 = p1val.Direction();
				gp_Vec v2 = p2val.Direction();
				gp_Vec v3 = p3val.Direction();
				gp_Vec v4 = p4val.Direction();


				QScriptValue pp1 = engine()->toScriptValue( hsf::AddNewPoint(p1));
				QScriptValue pp2 = engine()->toScriptValue( hsf::AddNewPoint(p2));
				QScriptValue pp3 = engine()->toScriptValue( hsf::AddNewPoint(p3));
				QScriptValue pp4 = engine()->toScriptValue( hsf::AddNewPoint(p4));



				QScriptValueList args;
				args << pp1 << pp2 << pp3 << pp4;

				QScriptValue result;
				QScriptValue thepanel = panelfunc.call(thisObject(),args);
				QString type = thepanel.toVariant().typeName();
				QList<TopoDS_Shape> panelshape = thepanel.toVariant().value<QList<TopoDS_Shape>>();

				if (!panelshape.isEmpty() ){
					for(int j=0;j<panelshape.count();j++)
					{
						B.Add(folder,panelshape.at(j));
						viscount++;
					}
				    }


			}} // end panelize
				if (viscount>0){
					TopoDS_Shape resultshape = folder;
					return engine()->toScriptValue(resultshape);
				;}
		
	 } // end if its function
		return engine()->toScriptValue(true);
} else { return engine()->toScriptValue(false);}
	 
	
     



}




QScriptValue HsfScriptingInterface::getaxis1pt()
{
if(context()->argumentCount() == 1)
	{
	 gp_Ax1 axis = context()->argument(0).toVariant().value<gp_Ax1>();
	 double x,y,z;
	 x = axis.Location().X();
	 y = axis.Location().Y(); 
     z = axis.Location().Z();

	 qDebug() << "axis1 typename:" << context()->argument(0).toVariant().typeName();
	 qDebug() << " axis1: " << x << y << z;
	 gp_Pnt thepoint(x,y,z);
	  TopoDS_Shape myptshape = hsf::AddNewPoint(thepoint);
	 return engine()->toScriptValue(myptshape);
	}
}

QScriptValue HsfScriptingInterface::getaxis1vec()
{
if(context()->argumentCount() == 1)
	{
	 gp_Ax1 axis = context()->argument(0).toVariant().value<gp_Ax1>();
	 return engine()->toScriptValue(gp_Vec(axis.Direction()));
	}

}

QScriptValue HsfScriptingInterface::getquadpointmap()
{

	
if(context()->argumentCount() == 3)
	 {
	 TopoDS_Shape surface1 = context()->argument(0).toVariant().value<TopoDS_Shape>();
	 int x = context()->argument(1).toNumber();
	 int y = context()->argument(2).toNumber();
	 
	 QMap<QString,QVariant> plist = HSF::BuildPointGridonSrf(surface1,x,y);
	 return engine()->toScriptValue(plist);

	}
	return engine()->toScriptValue(false);
}


QScriptValue HsfScriptingInterface::vis()
 {
     if(folder.IsNull())
	 {
		initpart();
	 }
	 TopoDS_Shape myshape = context()->argument(0).toVariant().value<TopoDS_Shape>();
	 
	 int localvis = viscount;
	 if (!myshape.IsNull()){
	 B.Add(folder,myshape);
	 localvis = localvis + 1;
	 }
	 viscount = localvis;
	
     return engine()->toScriptValue(true);
 }

QScriptValue HsfScriptingInterface::vispointmap()
 {
     if(folder.IsNull())
	 {
		initpart();
	 }
	 int localvis = viscount;

	 if (context()->argumentCount() == 1)
		 {
	 pointmap mymap = context()->argument(0).toVariant().value<pointmap>();

	 for (int i=0; i < mymap.values().count(); i++)
		 {
		
		     gp_Pnt curpoint = mymap.values().at(i);

			 TopoDS_Shape myshape = hsf::AddNewPoint(curpoint);
			 if (!myshape.IsNull()){
			 B.Add(folder,myshape);
			 localvis = localvis + 1;
			 }
		 }

	     viscount = localvis;

		 }
	 
	
	
     return engine()->toScriptValue(true);
 }

QScriptValue HsfScriptingInterface::vislist()
 {
     if(folder.IsNull())
	 {
		initpart();
	 }
	 int localvis = viscount;

	 if (context()->argumentCount() == 1)
		 {
	 pointlist mymap = context()->argument(0).toVariant().value<pointlist>();

	 for (int i=0; i < mymap.count(); i++)
		 {
		
		     gp_Pnt curpoint = mymap.at(i);

			 TopoDS_Shape myshape = hsf::AddNewPoint(curpoint);
			 if (!myshape.IsNull()){
			 B.Add(folder,myshape);
			 localvis = localvis + 1;
			 }
		 }

	     viscount = localvis;

		 }
	 
	
	
     return engine()->toScriptValue(true);
 }


QScriptValue HsfScriptingInterface::vis(QScriptValue obj)
 {
     if(folder.IsNull())
	 {
		initpart();
	 }
	 TopoDS_Shape myshape = obj.toVariant().value<TopoDS_Shape>();
	 
	 int localvis = viscount;
	 if (!myshape.IsNull()){
	 B.Add(folder,myshape);
	 localvis = localvis + 1;
	 }
	 viscount = localvis;
	
     return engine()->toScriptValue(true);
 }





QScriptValue HsfScriptingInterface::viscurvature ()
 {
     
	 if(context()->argumentCount() == 3)
	 {
	 TopoDS_Shape myshape = context()->argument(0).toVariant().value<TopoDS_Shape>();
	 bool showedges = context()->argument(1).toBool();
	 double accuracy = context()->argument(2).toNumber();
	 if (accuracy == 0) accuracy = 10;
	 showgaussedges =showedges;
	 int localvis = gaussviscount;
	 if (!myshape.IsNull()){
	 
		 BRepTools::Clean(myshape);
		 BRepMesh::Mesh(myshape, accuracy);

	 B.Add(gaussfolder,myshape);
	 localvis = localvis + 1;
	 }
	 gaussviscount = localvis;
	 }
	 
	
     return engine()->toScriptValue(true);
 }


QScriptValue HsfScriptingInterface::makevector()
 {
    gp_Vec myvec;
	 if (context()->argumentCount() == 3)
	 {
		 double x,y,z;
		 if (context()->argument(0).toString().toLower() == "infinity") {x = 0;} else { x = context()->argument(0).toNumber();}
		 if (context()->argument(1).toString().toLower() == "infinity") {y = 0;} else { y = context()->argument(1).toNumber();}
		 if (context()->argument(2).toString().toLower() == "infinity") {z = 0;} else { z = context()->argument(2).toNumber();}
    	 myvec = gp_Vec(x,y,z);
		 
	 } else if (context()->argumentCount() == 2)  {

		QScriptValue arg1 =context()->argument(0);
		QScriptValue arg2 =context()->argument(1);

		 if(context()->argument(0).isVariant() && context()->argument(1).isVariant())
		 {
			 QVariant arg1variant = arg1.toVariant();
		     QVariant arg2variant = arg2.toVariant();

			 QString type1(arg1variant.typeName());
			 QString type2(arg2variant.typeName());

			 if( type1 == "gp_Pnt" && type2 == "gp_Pnt" )
			 {
				gp_Pnt p1 = arg1variant.value<gp_Pnt>();
				gp_Pnt p2 = arg2variant.value<gp_Pnt>();

				myvec = gp_Vec(p1,p2);
			 } else if( type1 == "TopoDS_Shape" && type2 == "TopoDS_Shape" )
			 {
				 gp_Pnt p1 = hsf::getpointfromshape(arg1variant.value<TopoDS_Shape>());
				 gp_Pnt p2 = hsf::getpointfromshape(arg2variant.value<TopoDS_Shape>());
				 if (p1.Distance(p2) >0) { myvec = gp_Vec(p1,p2); }else {myvec = gp_Vec(1,0,0);}
			 }

		 }
		
	 }
	 
	 QScriptValue myvecval = engine()->toScriptValue(myvec);
	myvecval.setProperty(QString("x"),engine()->toScriptValue(myvec.X()));
	myvecval.setProperty(QString("y"),engine()->toScriptValue(myvec.Y()));
	myvecval.setProperty(QString("z"),engine()->toScriptValue(myvec.Z()));



     return myvecval;
 }

QScriptValue HsfScriptingInterface::makecircle()
 {
     TopoDS_Shape myshape;
	 if (context()->argumentCount() == 3)
	 {
		QScriptValue arg1 = context()->argument(0);
		QScriptValue arg2 = context()->argument(1);
		QScriptValue arg3 = context()->argument(2);

		QVariant var1,var2,var3;

		if (arg1.isVariant() && arg2.isVariant() && arg3.isNumber())
		{

		 gp_Pnt origin = hsf::getpointfromshape(context()->argument(0).toVariant().value<TopoDS_Shape>());
		 gp_Vec upvec  = context()->argument(1).toVariant().value<gp_Vec>();
		 double radius = context()->argument(2).toNumber();
		 if (radius == 0) radius = 0.1;
		 myshape = hsf::AddNewCircle(origin,upvec,radius);
		} else if ( arg1.isVariant() && arg2.isVariant() && arg3.isVariant())
		{
		 var1 = arg1.toVariant(); 
		 var2 = arg2.toVariant(); 
		 var3 = arg3.toVariant();
		 QString var1typename = var1.typeName();
		 QString var2typename = var2.typeName();
		 QString var3typename = var3.typeName();


		 if (var1typename == "TopoDS_Shape" && var2typename == "TopoDS_Shape" && var3typename == "TopoDS_Shape")
		 {
		 gp_Pnt p1 = hsf::getpointfromshape(context()->argument(0).toVariant().value<TopoDS_Shape>());
		 gp_Pnt p2  = hsf::getpointfromshape(context()->argument(1).toVariant().value<TopoDS_Shape>());
		 gp_Pnt p3 = hsf::getpointfromshape(context()->argument(2).toVariant().value<TopoDS_Shape>());

		 double dis1 = p1.Distance(p2);
		 double dis2 = p2.Distance(p3);
		 double dis3 = p3.Distance(p1);

		 if ((dis1>0) && (dis2>0) && (dis3>0) ) 
		 {
			 myshape = hsf::AddNewCircle(p1,p2,p3);
		 }
		 
		 }

		}
	 
	 } 
	      
     return engine()->toScriptValue(myshape);
 }
QScriptValue HsfScriptingInterface::makeshapelist()
 {
    QList<TopoDS_Shape> shapelist;
	
	
	if (context()->argumentCount() > 0)
	{
			for(int i=0;i<context()->argumentCount();i++)
			{
			TopoDS_Shape valuetoadd = context()->argument(i).toVariant().value<TopoDS_Shape>();
			shapelist << valuetoadd; 
			}
	}
	return engine()->toScriptValue(shapelist);

 }

QScriptValue HsfScriptingInterface::makepointlist()
 {
    QList<gp_Pnt> pointlist;
	
	if (context()->argumentCount() > 0)
	{
			for(int i=0;i<context()->argumentCount();i++)
			{
			gp_Pnt valuetoadd = context()->argument(i).toVariant().value<gp_Pnt>();
			pointlist << valuetoadd; 
			}
	}
	return engine()->toScriptValue(pointlist);

 }

QScriptValue HsfScriptingInterface::makecolorlist()
 {
    QList<Quantity_Color> colorlist;
	
	if (context()->argumentCount() > 0)
	{
			for(int i=0;i<context()->argumentCount();i++)
			{
			Quantity_Color valuetoadd = context()->argument(i).toVariant().value<Quantity_Color>();
			colorlist << valuetoadd; 
			}
	}
	return engine()->toScriptValue(colorlist);

 }


QScriptValue HsfScriptingInterface::shapeready()
 {
    TopoDS_Shape myshape;
	if (context()->argumentCount() == 2)
	{
		myshape = context()->argument(0).toVariant().value<TopoDS_Shape>();
		
		if (myshape.IsNull())
			{
			return engine()->toScriptValue(false);
			} else {
			return engine()->toScriptValue(true);
			}
	}
	return engine()->toScriptValue(false);
 }


QScriptValue HsfScriptingInterface::shapelistready()
 {
    QList<TopoDS_Shape> myshapelist;
	if (context()->argumentCount() == 2)
	{
		myshapelist = context()->argument(0).toVariant().value<QList<TopoDS_Shape>>();
		
		for (int i=0;i < myshapelist.count();i++)
			{
			TopoDS_Shape myshape = myshapelist.at(i);
		if (myshape.IsNull())
			{
			return engine()->toScriptValue(false);
			} else {
			return engine()->toScriptValue(true);
			}
		}
	}
	return engine()->toScriptValue(false);
 }


	  



QScriptValue HsfScriptingInterface::addshapetolist()
 {
    QList<TopoDS_Shape> shapelist;
	if (context()->argumentCount() > 1)
	{
		shapelist = context()->argument(0).toVariant().value<QList<TopoDS_Shape>>();
		for(int i=1;i<context()->argumentCount();i++)
			{
			TopoDS_Shape valuetoadd = context()->argument(i).toVariant().value<TopoDS_Shape>();
			shapelist << valuetoadd; 
			}
	}
	return engine()->toScriptValue(shapelist);
 }

QScriptValue HsfScriptingInterface::addpointtolist()
 {
    QList<gp_Pnt> pointlist;
	if (context()->argumentCount() > 1)
	{
		pointlist = context()->argument(0).toVariant().value<QList<gp_Pnt>>();

		for(int i=1;i<context()->argumentCount();i++)
			{
			gp_Pnt valuetoadd = hsf::getpointfromshape(context()->argument(i).toVariant().value<TopoDS_Shape>());
			pointlist << valuetoadd; 
			}

		
	}
	return engine()->toScriptValue(pointlist);
 }


QScriptValue HsfScriptingInterface::makefillsrf()
	{

	 TopoDS_Shape myshape;
    if (context()->argumentCount() == 1)
	{
		QList<TopoDS_Shape> myshapelist = context()->argument(0).toVariant().value<QList<TopoDS_Shape>>();
		if (myshapelist.count() > 0)
		{
		myshape = hsf::AddNewFillSurface(myshapelist);

		}
	}
	return engine()->toScriptValue(myshape);

	}




QScriptValue HsfScriptingInterface::makeextrude()
	{
TopoDS_Shape myshape;
    if (context()->argumentCount() == 3)
	{
		TopoDS_Shape curve = context()->argument(0).toVariant().value<TopoDS_Shape>();
		gp_Vec dir = context()->argument(1).toVariant().value<gp_Vec>();
		double magnitude = context()->argument(2).toNumber();

		if (!curve.IsNull() && !magnitude==0)
		{
		myshape = hsf::AddNewExtrude(curve,dir,magnitude);
		}
	}
	return engine()->toScriptValue(myshape);

	}

QScriptValue HsfScriptingInterface::makesphericalsurface()
	{

	TopoDS_Shape myshape;
    if (context()->argumentCount() == 2)
	{
		gp_Pnt origin = context()->argument(0).toVariant().value<gp_Pnt>();
		double radius = context()->argument(1).toNumber();
			
		TopoDS_Shape surface = hsf::AddNewSphereSurfacePatch(origin,radius);
		myshape = surface;
		return engine()->toScriptValue(myshape);

	}

	return QScriptValue(false);
	
	}

QScriptValue HsfScriptingInterface::makeaxis()
	{
	
    if (context()->argumentCount() == 3)
	{
	
		gp_Pnt p1 = hsf::getpointfromshape(context()->argument(0).toVariant().value<TopoDS_Shape>());
		gp_Vec v1 = context()->argument(1).toVariant().value<gp_Vec>();
		gp_Vec v2 = context()->argument(2).toVariant().value<gp_Vec>();
		gp_Ax3 theaxis(p1,v1,v2);
					
		
		return engine()->toScriptValue(theaxis);
	}

	return QScriptValue(false);
	

	}


QScriptValue HsfScriptingInterface::makeaxistoaxis()
	{

	TopoDS_Shape myshape;
    if (context()->argumentCount() == 3)
	{
		TopoDS_Shape obj = context()->argument(0).toVariant().value<TopoDS_Shape>();
		gp_Ax3 origin = context()->argument(1).toVariant().value<gp_Ax3>();
		gp_Ax3 target = context()->argument(2).toVariant().value<gp_Ax3>();
		TopoDS_Shape result = HSF::axistoaxis(obj,origin,target);
					
		if (result.IsNull()) return engine()->toScriptValue(false);
		return engine()->toScriptValue(result);
	}

	return QScriptValue(false);
	
	}

QScriptValue HsfScriptingInterface::makeaffinity()
	{

	TopoDS_Shape myshape;
    if (context()->argumentCount() == 4)
	{
		TopoDS_Shape obj = context()->argument(0).toVariant().value<TopoDS_Shape>();
		gp_Pnt origin = context()->argument(1).toVariant().value<gp_Pnt>();
		gp_Vec vec = context()->argument(2).toVariant().value<gp_Vec>();
		double x = context()->argument(3).toNumber();
					
		TopoDS_Shape surface = hsf::AddNewAffinity(obj,origin,vec,x);
		return engine()->toScriptValue(surface);
	}

	return QScriptValue(false);
	
	}



QScriptValue HsfScriptingInterface::makeintsrfsrf()
	{
TopoDS_Shape myshape;
    if (context()->argumentCount() == 2)
	{
		TopoDS_Shape srf1 = context()->argument(0).toVariant().value<TopoDS_Shape>();
		TopoDS_Shape srf2 = context()->argument(1).toVariant().value<TopoDS_Shape>();
		

		if (!srf1.IsNull() && !srf2.IsNull())
		{
		myshape = hsf::AddNewIntersectSrf(srf1,srf2);
		}
	}
	return engine()->toScriptValue(myshape);

	}


QScriptValue HsfScriptingInterface::makeintsrfplane()
	{
TopoDS_Shape myshape;
    if (context()->argumentCount() == 2)
	{
		TopoDS_Shape srf1 = context()->argument(0).toVariant().value<TopoDS_Shape>();
		gp_Pln pl = context()->argument(1).toVariant().value<gp_Pln>();
		

		if (!srf1.IsNull() )
		{
		myshape = hsf::AddNewIntersectSrf(pl,srf1);
		}
	}
	return engine()->toScriptValue(myshape);

	}




QScriptValue HsfScriptingInterface::makerevolve()
	{
TopoDS_Shape myshape;
    if (context()->argumentCount() == 3)
	{
		TopoDS_Shape crv = context()->argument(0).toVariant().value<TopoDS_Shape>();
		gp_Pnt origin = hsf::getpointfromshape(context()->argument(1).toVariant().value<TopoDS_Shape>());
		gp_Vec vec = context()->argument(2).toVariant().value<gp_Vec>();
		///double angle = context()->argument(2).toNumber();
		
		myshape = hsf::AddNewRevolveSurface(crv,origin,vec);

		
		
		}

	
	 if (!myshape.IsNull())
	 {
		return engine()->toScriptValue(myshape);
	 } else
	 {
		return engine()->toScriptValue(false);
	 }
	}




QScriptValue HsfScriptingInterface::makesweep()
	{
 TopoDS_Shape myshape;
    if (context()->argumentCount() == 2)
	{
		TopoDS_Shape rail = context()->argument(0).toVariant().value<TopoDS_Shape>();
		TopoDS_Shape section = context()->argument(1).toVariant().value<TopoDS_Shape>();

		if (!rail.IsNull() && !section.IsNull())
		{
		myshape = hsf::AddNewSweep(rail,section);
		}
	}
	return engine()->toScriptValue(myshape);

	}


QScriptValue HsfScriptingInterface::makesweepbrep()
	{
 TopoDS_Shape myshape;
    if (context()->argumentCount() == 3)
	{
		TopoDS_Shape rail = context()->argument(0).toVariant().value<TopoDS_Shape>();
		TopoDS_Shape section = context()->argument(1).toVariant().value<TopoDS_Shape>();
		

		if (!rail.IsNull() && !section.IsNull())
		{
		myshape = hsf::AddNewSweepBrep(rail,section);
		}
	}
	return engine()->toScriptValue(myshape);

	}

QScriptValue HsfScriptingInterface::makesweepgeom()
	{
 TopoDS_Shape myshape;
    if (context()->argumentCount() == 3)
	{
		TopoDS_Shape rail = context()->argument(0).toVariant().value<TopoDS_Shape>();
		TopoDS_Shape section = context()->argument(1).toVariant().value<TopoDS_Shape>();
		int transition = context()->argument(2).toNumber();

		if (!rail.IsNull() && !section.IsNull())
		{
		myshape = hsf::AddNewSweepGeom(rail,section,transition);
		}
	}
	return engine()->toScriptValue(myshape);

	}


QScriptValue HsfScriptingInterface::makesweep2sec()
	{
 TopoDS_Shape myshape;
    if (context()->argumentCount() == 3)
	{
		TopoDS_Shape rail = context()->argument(0).toVariant().value<TopoDS_Shape>();
		TopoDS_Shape section1 = context()->argument(1).toVariant().value<TopoDS_Shape>();
		TopoDS_Shape section2 = context()->argument(1).toVariant().value<TopoDS_Shape>();

		if (!rail.IsNull() && !section1.IsNull() && !section2.IsNull())
		{
		myshape = hsf::AddNewSweep2sec(rail,section1,section2);
		}
	}
	return engine()->toScriptValue(myshape);

	}

QScriptValue HsfScriptingInterface::makeloft()
 {
	 TopoDS_Shape myshape;
    if (context()->argumentCount() == 1)
	{
		QList<TopoDS_Shape> myshapelist = context()->argument(0).toVariant().value<QList<TopoDS_Shape>>();
		if (myshapelist.count() > 1)
		{
		myshape = hsf::AddNewLoftSurface(myshapelist);
		}
	}
	return engine()->toScriptValue(myshape);
 }

QScriptValue HsfScriptingInterface::makebspline()
 {
	 TopoDS_Shape myshape;
    if (context()->argumentCount() == 1)
	{
		QList<gp_Pnt> mypointlist = context()->argument(0).toVariant().value<QList<gp_Pnt>>();
		if (mypointlist.count() > 1)
		{
			myshape = hsf::AddNewSplineInterpSShape(mypointlist);
		}
	}
	return engine()->toScriptValue(myshape);
 }

QScriptValue HsfScriptingInterface::makepolyline()
 {
	 TopoDS_Shape myshape;
    if (context()->argumentCount() == 1)
	{
		QList<gp_Pnt> mypointlist = context()->argument(0).toVariant().value<QList<gp_Pnt>>();
		if (mypointlist.count() > 1)
		{
		myshape = hsf::AddNewPolyline(mypointlist);
		}
	}
	return engine()->toScriptValue(myshape);
 }

QScriptValue HsfScriptingInterface::makelineptpt()
 {
     TopoDS_Shape myshape;
	 if (context()->argumentCount() == 2)
	 {
			QScriptValue arg1 = context()->argument(0);
			QScriptValue arg2 = context()->argument(1);
		 if(context()->argument(0).isVariant() && context()->argument(1).isVariant())
		 {
			 QVariant arg1variant = arg1.toVariant();
		     QVariant arg2variant = arg2.toVariant();

			 QString type1(arg1variant.typeName());
			 QString type2(arg2variant.typeName());
			 gp_Pnt p1;
			 gp_Pnt p2;
			 if( type1 == "gp_Pnt" && type2 == "gp_Pnt" )
			 {
				p1 = arg1variant.value<gp_Pnt>();
				p2 = arg2variant.value<gp_Pnt>();

				
			 } else if( type1 == "TopoDS_Shape" && type2 == "TopoDS_Shape" )
			 {
				 p1 = hsf::getpointfromshape(arg1variant.value<TopoDS_Shape>());
				 p2 = hsf::getpointfromshape(arg2variant.value<TopoDS_Shape>());
				
			 }

			 if (p1.Distance(p2) > Precision::Confusion())
			 {
				 myshape = hsf::AddNewLineptpt(p1,p2);
			 }
		 }

	 }

	 if (!myshape.IsNull())
	 {
		return engine()->toScriptValue(myshape);
	 } else
	 {
		return engine()->toScriptValue(false);
	 }

     
 }



QScriptValue HsfScriptingInterface::makehyberbola()
	{


	TopoDS_Shape myshape;
	
	 
	 if (context()->argumentCount() == 5)
	 {
			gp_Pnt p1 = hsf::getpointfromshape(context()->argument(0).toVariant().value<TopoDS_Shape>());
			gp_Vec up =  context()->argument(1).toVariant().value<gp_Vec>();
			gp_Vec v1 =  context()->argument(2).toVariant().value<gp_Vec>();
			double major =  context()->argument(3).toNumber();
			double minor =  context()->argument(4).toNumber();

			gp_Ax2 axis(p1,up,v1);
			gp_Hypr myhb( axis, major, minor);

			myshape = BRepBuilderAPI_MakeEdge(myhb,-2,2);


	  }
	
	/*double fp,lp;
	  fp = SPL1->FirstParameter();
	  lp = SPL1->LastParameter();*/
	 

	  if (!myshape.IsNull())
	 {
		return engine()->toScriptValue(myshape);
	 } else
	 {
		return engine()->toScriptValue(false);
	 }


	


	}


QScriptValue HsfScriptingInterface::makehyberbolabyradius ()
{
     TopoDS_Shape myshape;
	 if (context()->argumentCount() == 7)
	 {
			QScriptValue arg1 = context()->argument(0);
			QScriptValue arg2 = context()->argument(1);
			QScriptValue arg3 = context()->argument(2);
			QScriptValue arg4 = context()->argument(3);
			QScriptValue arg5 = context()->argument(4);
			QScriptValue arg6 = context()->argument(5);
			QScriptValue arg7 = context()->argument(6);

		 if(context()->argument(0).isVariant() && context()->argument(1).isVariant())
		 {
			 QVariant arg1variant = arg1.toVariant();
		     QVariant arg2variant = arg2.toVariant();

			 QString type1(arg1variant.typeName());
			 QString type2(arg2variant.typeName());
			 gp_Pnt origin;
			 gp_Vec dir;

			 double innerradius = arg3.toNumber();
			 double height = arg4.toNumber();
			 double heightunder = arg5.toNumber();
			 double angle  = arg6.toNumber();
			 double detail  = arg7.toNumber();

			 
			 origin = hsf::getpointfromshape(arg1variant.value<TopoDS_Shape>());
			 dir = arg2variant.value<gp_Vec>();

				
			 if (height > 0 && heightunder > 0)
			 {
			 gp_Pnt firstorigin(0,0,0);
			 gp_Vec firstdir(0,0,1);
			 gp_Ax3 axis1(firstorigin,firstdir);
			 gp_Ax3 axis2(origin,dir);
			 

			 myshape = hsf::AddNewHyperboloidbyFormula(innerradius,height,heightunder,angle,firstorigin,firstdir,detail);
			 myshape = hsf::axistoaxis(myshape,axis1,axis2);
			 }
		 }
	}


	 if (!myshape.IsNull())
	 {
		return engine()->toScriptValue(myshape);
	 } else
	 {
		return engine()->toScriptValue(false);
	 }

	 } // end of command
QScriptValue HsfScriptingInterface::makelineptdir()
 {
     TopoDS_Shape myshape;
	 if (context()->argumentCount() > 2)
	 {
			QScriptValue arg1 = context()->argument(0);
			QScriptValue arg2 = context()->argument(1);
			QScriptValue arg3 = context()->argument(2);
			QScriptValue arg4 = context()->argument(3);

			
		 if(context()->argument(0).isVariant() && context()->argument(1).isVariant() && context()->argument(2).isNumber() && context()->argument(3).isNumber())
		 {
			 QVariant arg1variant = arg1.toVariant();
		     QVariant arg2variant = arg2.toVariant();
			 double distance1 = arg3.toNumber();
			 double distance2 = arg4.toNumber();

			 QString type1(arg1variant.typeName());
			 QString type2(arg2variant.typeName());
			 gp_Pnt p1;
			 gp_Vec v1;
			 if( type1 == "gp_Pnt" && type2 == "gp_Vec" )
			 {
				p1 = arg1variant.value<gp_Pnt>();
				v1 = arg2variant.value<gp_Vec>();

				
			 } else if( type1 == "TopoDS_Shape" && type2 == "gp_Vec" )
			 {
				 p1 = hsf::getpointfromshape(arg1variant.value<TopoDS_Shape>());
				 v1 = arg2variant.value<gp_Vec>();
				
			 }

			 if (std::max(abs(distance1),abs(distance2))>0)
			 {
			 qDebug() << distance1 << "," << distance2;
			 myshape = hsf::AddNewLineptdir(p1,v1,distance1,distance2);
			 }
			 
		 }

	 }

	 if (!myshape.IsNull())
	 {
		return engine()->toScriptValue(myshape);
	 } else
	 {
		return engine()->toScriptValue(false);
	 }

     
 }





QScriptValue HsfScriptingInterface::makespherepack()
{
TopoDS_Shape myshape;
	 if (context()->argumentCount() ==4)
	 {
		QScriptValue arg1 = context()->argument(0);
		QScriptValue arg2 = context()->argument(1);
		QScriptValue arg3 = context()->argument(2);
		QScriptValue arg4 = context()->argument(3);

		if(arg1.isVariant() && arg2.isVariant() )
		 {
			TopoDS_Shape sourceshape = arg1.toVariant().value<TopoDS_Shape>();
			TopoDS_Shape rail = arg2.toVariant().value<TopoDS_Shape>();
			double unum,vnum;
			unum = arg3.toNumber();
			vnum = arg4.toNumber();
		
			QMap<QString,gp_Pnt> pointmap = hsf::SpherePack(rail,sourceshape,unum,vnum);
			return engine()->toScriptValue(pointmap);
		 }
	 }

	 return engine()->toScriptValue(false);

	}
QScriptValue HsfScriptingInterface::maketranslatebylength()
{
TopoDS_Shape myshape;
	 if (context()->argumentCount() ==3)
	 {
		QScriptValue arg1 = context()->argument(0);
		QScriptValue arg2 = context()->argument(1);
		QScriptValue arg3 = context()->argument(2);

		if(arg1.isVariant() && arg2.isVariant())
		 {
			TopoDS_Shape sourceshape = arg1.toVariant().value<TopoDS_Shape>();
			gp_Vec movevector;
			
			gp_Pln myplane = arg2.toVariant().value<gp_Pln>();
			movevector = myplane.Axis().Direction();
			/*gp_Pnt cog = hsf::GetCOG(sourceshape);
			gp_Pnt targetpt = hsf::MovePointByVector(cog,movevector,arg3.toNumber());*/
			movevector.Normalize();
			movevector.Multiply(arg3.toNumber());

			myshape = hsf::move(sourceshape,movevector);
			

		

		 }


	 }


	  if (!myshape.IsNull())
	 {
		return engine()->toScriptValue(myshape);
	 } else
	 {
		return engine()->toScriptValue(false);
	 }


}


QScriptValue HsfScriptingInterface::makesymmetry()
{
TopoDS_Shape myshape;
	 if (context()->argumentCount() ==2)
	 {
		QScriptValue arg1 = context()->argument(0);
		QScriptValue arg2 = context()->argument(1);

		if(arg1.isVariant() && arg2.isVariant())
		 {
			TopoDS_Shape sourceshape = arg1.toVariant().value<TopoDS_Shape>();
			gp_Pln myplane = arg2.toVariant().value<gp_Pln>();

			myshape = hsf::AddNewSymmetry(sourceshape,myplane);
		 }


	 }


	  if (!myshape.IsNull())
	 {
		return engine()->toScriptValue(myshape);
	 } else
	 {
		return engine()->toScriptValue(false);
	 }


}


QScriptValue HsfScriptingInterface::makerotate()
 {
     TopoDS_Shape myshape;
	 if (context()->argumentCount() ==4)
	 {
			QScriptValue arg1 = context()->argument(0);
			QScriptValue arg2 = context()->argument(1);
			QScriptValue arg3 = context()->argument(2);
			QScriptValue arg4 = context()->argument(3);
			
			
		 if(context()->argument(0).isVariant() && context()->argument(1).isVariant() && context()->argument(2).isVariant() && context()->argument(3).isNumber() )
		 {
			 QVariant arg1variant = arg1.toVariant();
		     QVariant arg2variant = arg2.toVariant();
			 QVariant arg3variant = arg3.toVariant();
			 double angle = arg4.toNumber();
			

			 QString type1(arg1variant.typeName());
			 QString type2(arg2variant.typeName());
			 QString type3(arg3variant.typeName());
			 gp_Pnt p1;
			 gp_Vec v1;
			
			 if( type1 == "TopoDS_Shape" && type2 == "TopoDS_Shape" && type3 == "gp_Vec" )
			 {
				 TopoDS_Shape shapetorot = arg1variant.value<TopoDS_Shape>();
				 p1 = hsf::getpointfromshape(arg2variant.value<TopoDS_Shape>());
				 v1 = arg3variant.value<gp_Vec>();
				 myshape = hsf::rotate(shapetorot,p1,v1,angle);
				
			 }

			 
			
			 
			 
		 }

	 }

	 if (!myshape.IsNull())
	 {
		return engine()->toScriptValue(myshape);
	 } else
	 {
		return engine()->toScriptValue(false);
	 }

     
 }



// build panels for kmeasn clustering 

QScriptValue HsfScriptingInterface::buildpanelarray()
	{
	QList<panelinstance> panelarray ;

	return engine()->toScriptValue(panelarray);

	}

QScriptValue HsfScriptingInterface::buildpanel()
	{
	panelinstance panel ;

	return engine()->toScriptValue(panel);

	}


     QScriptValue HsfScriptingInterface::makemap()
		 {
		 QMap<QString,QVariant> map;
		 return engine()->toScriptValue(map);

		 }
	 QScriptValue HsfScriptingInterface::mapinsert()
		 {
		  if (context()->argumentCount() > 2)
			{
			QMap<QString,QVariant> map = context()->argument(0).toVariant().value<QMap<QString,QVariant>>();
			QString key = context()->argument(1).toString();
			for(int i=2;i < context()->argumentCount();i++)
			{
				QVariant curval = context()->argument(i).toVariant();
				map.insert(key,curval);

			}
			return engine()->toScriptValue(map);
			}
	
		return engine()->toScriptValue(false);

		 }
	 QScriptValue HsfScriptingInterface::getmapcount()
		 {
		  if (context()->argumentCount() == 1)
			{
			QMap<QString,QVariant> map = context()->argument(0).toVariant().value<QMap<QString,QVariant>>();
			return engine()->toScriptValue(map.count());
			}
		 return engine()->toScriptValue(false); 
		 }
	 QScriptValue HsfScriptingInterface::getmapkeys()
			 {
		  if (context()->argumentCount() == 1)
			{
			QMap<QString,QVariant> map = context()->argument(0).toVariant().value<QMap<QString,QVariant>>();
			return engine()->toScriptValue(map.keys());
			}
		 return engine()->toScriptValue(false); 
		 }
	 
	 QScriptValue HsfScriptingInterface::getmapkeysvalue()
	 {
		  if (context()->argumentCount() == 2)
			{
			QList<QString> keys = context()->argument(0).toVariant().value<QList<QString>>();
			int pos = context()->argument(1).toNumber();
			return engine()->toScriptValue(keys.at(pos));
			}
		 return engine()->toScriptValue(false); 
		 }
	 QScriptValue HsfScriptingInterface::getmapvalues()
	 {
		  if (context()->argumentCount() == 1)
			{
			QMap<QString,QVariant> map = context()->argument(0).toVariant().value<QMap<QString,QVariant>>();
			return engine()->toScriptValue(map.values());
			}
		 return engine()->toScriptValue(false); 
		 }
	 QScriptValue HsfScriptingInterface::mapcontains()
	 {
		  if (context()->argumentCount() == 2)
			{
			QMap<QString,QVariant> map = context()->argument(0).toVariant().value<QMap<QString,QVariant>>();
			QString key = context()->argument(1).toString();
			return engine()->toScriptValue(map.contains(key));
			}
		 return engine()->toScriptValue(false); 
		 }

	 QScriptValue HsfScriptingInterface::mapgetvalue()
		 {
		  if (context()->argumentCount() == 2)
			{
			QMap<QString,QVariant> map = context()->argument(0).toVariant().value<QMap<QString,QVariant>>();
			QString key = context()->argument(1).toString();
			if (map.contains(key))
				{
					return engine()->toScriptValue(map.value(key));
				} else
				{
					return engine()->toScriptValue(false);
				}
			
			}
		 return engine()->toScriptValue(false); 
		 }



QScriptValue HsfScriptingInterface::getpanelcount()
	{
	
	if (context()->argumentCount() == 1)
	 {
	 QList<panelinstance> panelarray = context()->argument(0).toVariant().value<QList<panelinstance>>();
	 return engine()->toScriptValue(panelarray.count());
	 }

	return engine()->toScriptValue(false);
	}

QScriptValue HsfScriptingInterface::insertpaneltoarray()
	{
	
	if (context()->argumentCount() > 1)
	 {
	 QList<panelinstance> panelarray = context()->argument(0).toVariant().value<QList<panelinstance>>();
		for (int i=1 ; i < context()->argumentCount() ; i++)
		{
			panelinstance curval = context()->argument(i).toVariant().value<panelinstance>();
			panelarray << curval;
		}
	 return engine()->toScriptValue(panelarray);
	 }

	return engine()->toScriptValue(false);
	}
QScriptValue HsfScriptingInterface::inserttriangletopanel()
	{
	if (context()->argumentCount() > 1)
	 {
	 panelinstance panel = context()->argument(0).toVariant().value<panelinstance>();
	 for (int i=1 ; i < context()->argumentCount(); i++)
		{
			QList<gp_Pnt> triangle = context()->argument(i).toVariant().value<QList<gp_Pnt>>();
			panel.triangles << triangle;
		}
	 return engine()->toScriptValue(panel);	 
	 }
	return engine()->toScriptValue(false);

	}
QScriptValue HsfScriptingInterface::insertparametertopanel()
	{

	if (context()->argumentCount() > 1)
	 {
	 panelinstance panel = context()->argument(0).toVariant().value<panelinstance>();
	 for (int i=1 ; i < context()->argumentCount(); i++)
		{
			double parameter = context()->argument(i).toNumber();
			panel.parameters << parameter;
		}
	 return engine()->toScriptValue(panel);	 
	 }
	return engine()->toScriptValue(false);

	}



QScriptValue HsfScriptingInterface::makekmeanscluster()
{
if (context()->argumentCount()==2)
	{
	 int knumber = context()->argument(0).toNumber();
	 QList<panelinstance> panelarray = context()->argument(1).toVariant().value<QList<panelinstance>>();
	 try
		 {
		 if (panelarray.count() > knumber)
			 {
				 kmeanscluster(knumber,panelarray);
				 return engine()->toScriptValue(panelarray);
			 } else
			 {
			    qDebug() << "panelarray has less panels than kmeans number";
				return engine()->toScriptValue(false);
			 }
		 }
	 catch(...)
		 {
		 qDebug() << "something went wrong with kmeans caculate clusters";	
		 return engine()->toScriptValue(false);

		 }
	}
return engine()->toScriptValue(false);

}
QScriptValue HsfScriptingInterface::makerandomcolorlist()
{

if (context()->argumentCount() == 1)
	{
	 int knumber = context()->argument(0).toNumber();
		QList<Quantity_Color> centercolors;

		for(int i=0 ; i < knumber; i++)
		{
			double r,g,b;
			r =0; g=0; b=0;
			r = double(rand()) / double(RAND_MAX);
			g = double(rand()) / double(RAND_MAX);
			b = double(rand()) / double(RAND_MAX);
			Quantity_Color curcolor(r,g,b,Quantity_TypeOfColor::Quantity_TOC_RGB);
			centercolors << curcolor;
		}

		return engine()->toScriptValue(centercolors);

	}

return engine()->toScriptValue(false);
}


QScriptValue HsfScriptingInterface::makecolor()
{

if (context()->argumentCount() == 3)
	{
	 double R = context()->argument(0).toNumber();
	 double G = context()->argument(0).toNumber();
	 double B = context()->argument(0).toNumber();

	 Quantity_Color curcolor(R,G,B,Quantity_TypeOfColor::Quantity_TOC_RGB);
	 return engine()->toScriptValue(curcolor);
	}
	return engine()->toScriptValue(0);

	}





QScriptValue HsfScriptingInterface::makemesh()
{

if (context()->argumentCount() == 1)
	{

	 QList<panelinstance> panelarray = context()->argument(0).toVariant().value<QList<panelinstance>>();
     Handle( StlMesh_Mesh ) aSTLMesh = new StlMesh_Mesh();
	 aSTLMesh->AddDomain();
			
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
		return engine()->toScriptValue(aSTLMesh);
	}

return engine()->toScriptValue(false);
}


typedef QList<Quantity_Color> colorlistype;


QScriptValue HsfScriptingInterface::colorclustermesh()
{
if(!aMesh.IsNull()) aMesh->Destroy();

aMesh = new MeshVS_Mesh();
if (context()->argumentCount() == 2)
	{

  Handle_StlMesh_Mesh  stlmesh = context()->argument(0).toVariant().value<Handle_StlMesh_Mesh>();
  
 // int num = context()->argument(1).toNumber();
  
  QList<Quantity_Color> centercolors = context()->argument(1).toVariant().value<QList<Quantity_Color>>();

		//for(int i=0 ; i < num ; i++)
		//{
		//	double r,g,b;
		//	r =0; g=0; b=0;
		//	r = double(rand()) / double(RAND_MAX);
		//	g = double(rand()) / double(RAND_MAX);
		//	b = double(rand()) / double(RAND_MAX);
		//	Quantity_Color curcolor(r,g,b,Quantity_TypeOfColor::Quantity_TOC_RGB);
		//	centercolors << curcolor;
		//}
  //

  //colorlistype panelcolors = centercolors;


Handle_XSDRAWSTLVRML_DataSource aDS = new XSDRAWSTLVRML_DataSource(stlmesh);
aMesh->SetDataSource(aDS);

MeshVS_ElementalColorPrsBuilder * meshbuilder = new MeshVS_ElementalColorPrsBuilder (aMesh ,MeshVS_DMF_ElementalColorDataPrs | MeshVS_DMF_OCCMask );
meshbuilder->SetDataSource(aDS);

TColStd_MapIteratorOfPackedMapOfInteger it (aDS->GetAllElements());
Handle( MeshVS_HArray1OfSequenceOfInteger ) aTopo;
Standard_Integer NbNodes;
qDebug() << "number of nodes" << NbNodes;
int curcolor = 0;
for( ; it.More(); it.Next() )
{

Standard_Integer aKey = it.Key();
qDebug() << "inside triangle loop:" << aKey;
aDS->Get3DGeom( aKey, NbNodes, aTopo ) ;
MeshVS_EntityType elemtype;
aDS->GetGeomType(aKey,true,elemtype);
	if (elemtype == MeshVS_ET_Face)
	{

	Quantity_Color thecolor = centercolors.at(curcolor); // Quantity_Color(1,1,0,Quantity_TypeOfColor::Quantity_TOC_RGB);
	meshbuilder->SetColor1(aKey,thecolor );
	qDebug() << "wroking on node: " << curcolor << "color:" << thecolor.Red() << "," << thecolor.Green() << "," << thecolor.Blue() ;
	}
curcolor +=1;

}

qDebug() << "starting mesh builder";
aMesh->AddBuilder( meshbuilder, Standard_True );//False -> No selection 
meshviscount +=1;
qDebug() << "finished meshbuilder";

if(meshbuilder) 
	{
	try{
	return engine()->toScriptValue(true);
		} catch(...)
		{

		}
	}
return engine()->toScriptValue(false);

} // end of if context

return engine()->toScriptValue(false);
} // end of function


QScriptValue HsfScriptingInterface::getpanelcolorlist()
{

if (context()->argumentCount() ==2)
	{

	QList<panelinstance> panelarray = context()->argument(0).toVariant().value<QList<panelinstance>>();
	QList<Quantity_Color> centercolors = context()->argument(1).toVariant().value<QList<Quantity_Color>>();
	QList<Quantity_Color> trianglecolors;

	for(int i =0;i<panelarray.count();i++)
	{

	panelinstance curpanel = panelarray.at(i);
	Quantity_Color curcolor = centercolors.at(curpanel.centerindex);

		for (int j=0;j < curpanel.triangles.count(); j++)
		{
			//curtriangle repeat color for every triangle
			trianglecolors << curcolor;
			qDebug() << "generating color at panel:" << i << "on triangle" << j;

		} // end of j loop


	} // end of i loop
	
	return engine()->toScriptValue(trianglecolors);

}// end of if context

return engine()->toScriptValue(false);

} // end of function


QScriptValue HsfScriptingInterface::vismesh()
{
//meshviscount
return engine()->toScriptValue(false);
}





QScriptValue HsfScriptingInterface::makepointmovebyvector()
 {
     TopoDS_Shape myshape;
	 gp_Pnt resultp;
	 if (context()->argumentCount() == 3)
	 {
			QScriptValue arg1 = context()->argument(0);
			QScriptValue arg2 = context()->argument(1);
			QScriptValue arg3 = context()->argument(2);
		 if(context()->argument(0).isVariant() && context()->argument(1).isVariant() && context()->argument(2).isNumber())
		 {
			 QVariant arg1variant = arg1.toVariant();
		     QVariant arg2variant = arg2.toVariant();
			 double distance = arg3.toNumber();

			 QString type1(arg1variant.typeName());
			 QString type2(arg2variant.typeName());
			 gp_Pnt p1;
			 gp_Vec vec;
			 if( type1 == "gp_Pnt" && type2 == "gp_Vec" )
			 {
				p1 = arg1variant.value<gp_Pnt>();
				vec = arg2variant.value<gp_Vec>();

				
			 } else if( type1 == "TopoDS_Shape" && type2 == "gp_Vec" )
			 {
				 p1 = hsf::getpointfromshape(arg1variant.value<TopoDS_Shape>());
				 vec = arg2variant.value<gp_Vec>();
				
			 }
			 resultp = hsf::MovePointByVector(p1,vec,distance);
			 myshape = hsf::AddNewPoint(resultp);
				

		 }

	 }

	 if (!myshape.IsNull())
	 {
		QScriptValue myptval = engine()->toScriptValue(myshape);
	    myptval.setProperty(QString("x"),engine()->toScriptValue(resultp.X()));
		myptval.setProperty(QString("y"),engine()->toScriptValue(resultp.Y()));
		myptval.setProperty(QString("z"),engine()->toScriptValue(resultp.Z()));

		return myptval;
	 } else
	 {
		return engine()->toScriptValue(false);
	 }

     
 }




QScriptValue HsfScriptingInterface::makevectortangenttocurve() 
 {
     gp_Vec myvec;
	 if (context()->argumentCount() == 2)
	 {
			QScriptValue arg1 = context()->argument(0);
			QScriptValue arg2 = context()->argument(1);
			
			
		 if(context()->argument(0).isVariant() &&  context()->argument(1).isNumber())
		 {
			 QVariant arg1variant = arg1.toVariant();
		     double ratio = arg2.toNumber();

			 QString type1(arg1variant.typeName());
			
			if( type1 == "TopoDS_Shape" )
			 {
				 
				TopoDS_Shape crv1 = arg1variant.value<TopoDS_Shape>();
				if (crv1.IsNull()) return engine()->toScriptValue(false);
				myvec = hsf::getVectorTangentToCurveAtPoint(crv1,ratio);
				//QScriptValue myval = engine()->toScriptValue(myvec);

				QScriptValue myvecval = engine()->toScriptValue(myvec);
				myvecval.setProperty(QString("x"),engine()->toScriptValue(myvec.X()));
				myvecval.setProperty(QString("y"),engine()->toScriptValue(myvec.Y()));
				myvecval.setProperty(QString("z"),engine()->toScriptValue(myvec.Z()));

				return myvecval;

			} else { return engine()->toScriptValue(false);}

		 } else { return engine()->toScriptValue(false);}

	 } else { return engine()->toScriptValue(false);}
 

     
 }







QScriptValue HsfScriptingInterface::makeuvvec()
	{

 gp_Vec myvec;
	 if (context()->argumentCount() == 3)
	 {
			QScriptValue arg1 = context()->argument(0);
			QScriptValue arg2 = context()->argument(1);
			QScriptValue arg3 = context()->argument(2);
			
			
		 if(context()->argument(0).isVariant() )
		 {
			 QVariant arg1variant = arg1.toVariant();
			  QVariant arg2variant = arg2.toVariant();
			  QVariant arg3variant = arg3.toVariant();

			  double uval = arg2variant.toDouble();
			  double vval = arg3variant.toDouble();


		    

			 QString type1(arg1variant.typeName());
			 //QString type2(arg2variant.typeName());
			
			if( type1 == "TopoDS_Shape"   )
			 {				 
				TopoDS_Shape srf = arg1variant.value<TopoDS_Shape>();
				myvec = hsf::AddNewUVVec(srf,uval,vval);

				QScriptValue myvecval = engine()->toScriptValue(myvec);
				myvecval.setProperty(QString("x"),engine()->toScriptValue(myvec.X()));
				myvecval.setProperty(QString("y"),engine()->toScriptValue(myvec.Y()));
				myvecval.setProperty(QString("z"),engine()->toScriptValue(myvec.Z()));

				return myvecval;


			 }

			 
			 }

		 }

		return engine()->toScriptValue(false);


	}

QScriptValue HsfScriptingInterface::makesegmentbyparms()
	{
				TopoDS_Shape myshape;
	 if (context()->argumentCount() ==3)
	 {
		
			TopoDS_Shape theedge= context()->argument(0).toVariant().value<TopoDS_Shape>();
			double parm1= context()->argument(1).toNumber();
			double parm2= context()->argument(2).toNumber();

			double aFP,aLP;
			Handle(Geom_Curve) aCurve = BRep_Tool::Curve(TopoDS::Edge(theedge), aFP, aLP);
   				
   
   				double aP1 = aFP + (aLP - aFP) * parm1;
   				double aP2 = aFP + (aLP - aFP) * parm2;
   				gp_Pnt startpoint = aCurve->Value(aP1);
   				gp_Pnt middlepoint = aCurve->Value(aP2);
   				
   				BRepBuilderAPI_MakeEdge ellipseseg(aCurve,startpoint,middlepoint);

				return engine()->toScriptValue(ellipseseg.Shape());

		}

return engine()->toScriptValue(false);
	}

QScriptValue HsfScriptingInterface::makeuvpt()
	{

 gp_Pnt mypt;
 TopoDS_Shape myptshape;
	 if (context()->argumentCount() == 3)
	 {
			QScriptValue arg1 = context()->argument(0);
			QScriptValue arg2 = context()->argument(1);
			QScriptValue arg3 = context()->argument(2);
			
			
		 if(context()->argument(0).isVariant() )
		 {
			 QVariant arg1variant = arg1.toVariant();
			  QVariant arg2variant = arg2.toVariant();
			  QVariant arg3variant = arg3.toVariant();

			  double uval = arg2variant.toDouble();
			  double vval = arg3variant.toDouble();


		    

			 QString type1(arg1variant.typeName());
			 //QString type2(arg2variant.typeName());
			
			if( type1 == "TopoDS_Shape"   )
			 {				 
				TopoDS_Shape srf = arg1variant.value<TopoDS_Shape>();
				mypt = hsf::AddNewUVPt(srf,uval,vval);
				myptshape = hsf::AddNewPoint(mypt);

				QScriptValue myptval = engine()->toScriptValue(myptshape);
				myptval.setProperty(QString("x"),engine()->toScriptValue(mypt.X()));
				myptval.setProperty(QString("y"),engine()->toScriptValue(mypt.Y()));
				myptval.setProperty(QString("z"),engine()->toScriptValue(mypt.Z()));

				return myptval;


			 }

			 
			 }

		 }

		return engine()->toScriptValue(false);


	}




QScriptValue HsfScriptingInterface::makenormal() 
 {
     gp_Vec myvec;
	 if (context()->argumentCount() == 2)
	 {
			QScriptValue arg1 = context()->argument(0);
			QScriptValue arg2 = context()->argument(1);
			
			
		 if(context()->argument(0).isVariant() )
		 {
			 QVariant arg1variant = arg1.toVariant();
			  QVariant arg2variant = arg2.toVariant();
		    

			 QString type1(arg1variant.typeName());
			 QString type2(arg2variant.typeName());
			
			if( type1 == "TopoDS_Shape"   )
			 {
				 
				TopoDS_Shape srf = arg1variant.value<TopoDS_Shape>();
				gp_Pnt pt = hsf::getpointfromshape(arg1variant.value<TopoDS_Shape>());

				if (srf.IsNull()) return engine()->toScriptValue(false);
				//if (pt.IsNull()) return engine()->toScriptValue(false);

				myvec = hsf::getVectorNormaltoSurfaceatPoint(srf,pt);
				//QScriptValue myval = engine()->toScriptValue(myvec);

				QScriptValue myvecval = engine()->toScriptValue(myvec);
				myvecval.setProperty(QString("x"),engine()->toScriptValue(myvec.X()));
				myvecval.setProperty(QString("y"),engine()->toScriptValue(myvec.Y()));
				myvecval.setProperty(QString("z"),engine()->toScriptValue(myvec.Z()));

				return myvecval;

			} else { return engine()->toScriptValue(false);}

		 } else { return engine()->toScriptValue(false);}

	 } else { return engine()->toScriptValue(false);}
 

     
 }







QScriptValue HsfScriptingInterface::initpart()
 {
 	viscount=0;
	gaussviscount =0;
	meshviscount = 0;
	B.MakeCompound(folder);
	B.MakeCompound(gaussfolder);


	objectproperties.clear();

	currentslidermap.clear();

	return engine()->toScriptValue(true);
 }

QScriptValue HsfScriptingInterface::endpart()
 {

	qDebug() <<"End part started" ;
	checksliders();

	//try {if(!aMesh.IsNull()) qDebug() <<"passed mesh not null" ; ic->Remove(aMesh,true); }catch(...) { 	}
	
	
	

	if (meshviscount>0)
		{
		qDebug() <<"begin mesh vis" ;

		ic = appui::getInstance()->getWindowContext();
		if(!aMesh.IsNull()) ic->Remove(aMesh,true);	
		aMesh->GetDrawer()->SetBoolean(MeshVS_DA_DisplayNodes,Standard_False);
		aMesh->GetDrawer()->SetBoolean(MeshVS_DA_ColorReflection,Standard_True);
		aMesh->GetDrawer()->SetBoolean(MeshVS_DA_ShowEdges,false);
		aMesh->SetDisplayMode (2);
        ic->Display(aMesh);
		
		qDebug() <<"end mesh vis" ;

		}



     if (viscount>0)
	 {
		 if (!aisp.IsNull())
		 {
			 if (!aisp->Presentation()->IsVisible())
			 {
				ic->Remove(aisp);
				//delete aisp;
				aisp = new User_AIS(folder,ic);
				ic->SetMaterial(aisp,Graphic3d_NOM_NEON_GNC);
				ic->SetColor(aisp, Quantity_NOC_BLACK);
				ic->SetPreselectionColor(Quantity_NOC_YELLOW);
				ic->SetDisplayMode(aisp,2,Standard_False);
				
				ic->SetHilightColor(Quantity_NOC_YELLOW);
				
				ic->Display(aisp);
				
			 }else{
			   ic = appui::getInstance()->getWindowContext();
		       HSF::updateUserAIS(folder,aisp,ic);
			 }
		 } else {
			   ic = appui::getInstance()->getWindowContext();
		       HSF::updateUserAIS(folder,aisp,ic);
	     }
	    
		
			
	 } else {
		 if (!aisp.IsNull())
		{
			aisp->Presentation()->SetVisible(false);
		}	
	 }
	 
	 if (gaussviscount>0){
		 if(!mygauss.IsNull())
		 {
		    mygauss->setshape(gaussfolder);
			mygauss->setshowedges(showgaussedges);
			ic->Redisplay(mygauss);
		 } else 
		 {
            mygauss = new AIS_Gauss(gaussfolder,showgaussedges);
			ic = appui::getInstance()->getWindowContext();
			ic->Display(mygauss);
			
		 }

	 } else
	 {
		  if(!mygauss.IsNull())
		 {
		    mygauss->setshape(TopoDS_Shape());
			mygauss->setshowedges(showgaussedges);
			ic->Redisplay(mygauss);
		 }

	 }

	
	 // finish the runonce system
	 finishrunonce();

	 if ( needstofitall)
	 {		
	needstofitall = false;
	//qGeomApp->myview->viewAxo();
	qGeomApp->myview->fitAll();
	 }
	 
     return engine()->toScriptValue(true);
 }