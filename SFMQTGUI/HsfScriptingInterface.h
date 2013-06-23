#pragma once

#ifndef HSFSCRIPTINGINTERFACE_H
#define HSFSCRIPTINGINTERFACE_H

#include <QDesktopWidget>
#include <QtNetwork\qftp.h>
#include <QBuffer>
#include <QProcess>

#include <QScriptable>
#include <QObject>
#include <QScriptValue>
#include <QScriptContext>
#include <QScriptEngine>

#include "shapefactory.h" // HybridShapeFactory factory middleware for OpenCascade
#include "ui.h"  // user interface singleton 
#include "User_AIS.hxx" //ais object for displaying TopoDS_Shape
#include "AIS_Gauss.hxx" // ais object for displaying TopoDS_Shape with curvature analysis
#include "Handle_MeshVS_Mesh.hxx"
#include "Handle_StlMesh_Mesh.hxx"




#include <QFileSystemWatcher>

#include <QtNetwork\QNetworkRequest>
#include <QtNetwork\QNetworkAccessManager>
#include <QtNetwork\QNetworkReply>

  struct panelinstance {
        int panelindex;
        QList<double> parameters;
		double distancetocenter;
		int centerindex;
		QList<QList<gp_Pnt>> triangles;
    };



#include <MeshVS_ElementalColorPrsBuilder.hxx>
#include <Handle_MeshVS_ElementalColorPrsBuilder.hxx>
#include <StlMesh_Mesh.hxx>


//kinect

//#include "cio.h"
#include <stdio.h>

#include "QKinectWrapper.h"
#include <QSqlDatabase>


typedef QMap<QString,QVariant> variantmap;
typedef QMap<QString,gp_Pnt> pointmap;
typedef QList<gp_Pnt> pointlist;

 Q_DECLARE_METATYPE(shapefactory)
 Q_DECLARE_METATYPE(gp_Pnt)
 Q_DECLARE_METATYPE(gp_Vec)
 Q_DECLARE_METATYPE(TopoDS_Shape)
 Q_DECLARE_METATYPE(TopoDS_Compound)
 Q_DECLARE_METATYPE(QList<TopoDS_Shape>)
 Q_DECLARE_METATYPE(QList<gp_Pnt>)
 Q_DECLARE_METATYPE(QList<QList<gp_Pnt>>)
 Q_DECLARE_METATYPE(QList<QStringList>)
 Q_DECLARE_METATYPE(QStringList)
 Q_DECLARE_METATYPE(gp_Pln)
 Q_DECLARE_METATYPE(QImage)
 Q_DECLARE_METATYPE(panelinstance)
 Q_DECLARE_METATYPE(QList<panelinstance>)
 Q_DECLARE_METATYPE(QList<Quantity_Color>)
 Q_DECLARE_METATYPE(Handle_MeshVS_ElementalColorPrsBuilder)
 Q_DECLARE_METATYPE(Handle_StlMesh_Mesh )
 Q_DECLARE_METATYPE(QList<double>)
 Q_DECLARE_METATYPE(QList<QString>)
 Q_DECLARE_METATYPE(QList<int>)
 Q_DECLARE_METATYPE(MeshVS_ElementalColorPrsBuilder*)
 Q_DECLARE_METATYPE(QList<QVariant>)
 Q_DECLARE_METATYPE(variantmap)
 Q_DECLARE_METATYPE(gp_Ax1)
 Q_DECLARE_METATYPE(gp_Ax3)
 Q_DECLARE_METATYPE(gp_Pnt2d)
 Q_DECLARE_METATYPE(QList<gp_Pnt2d>)
 Q_DECLARE_METATYPE(pointmap)
 Q_DECLARE_METATYPE(Quantity_Color)
// Q_DECLARE_METATYPE(QImage)
 

 

 

class scriptwidget;
class QoccInputOutput;


 class HsfScriptingInterface: public QObject, protected QScriptable
 {
     Q_OBJECT
 public:
 HsfScriptingInterface();
 ~HsfScriptingInterface();

 private:

	Handle_AIS_InteractiveContext ic ;
	int viscount;
	int gaussviscount;
	int meshviscount;

	QMap<QString,QString> objectproperties;

	TopoDS_Compound folder;
	TopoDS_Compound gaussfolder;
	BRep_Builder B;

	QSqlDatabase *db;


	Handle_User_AIS aisp;
	Handle(AIS_Gauss) mygauss;
	//Handle( StlMesh_Mesh ) aSTLMesh;

	
	Handle( MeshVS_Mesh  ) aMesh;

	QFtp *ftp;

	bool showgaussedges;

	scriptwidget* parentwidget;

	// view system
	bool needstofitall;

	

	//do once system
	int runoncecounter;
	int runoncefilecounter;
	int runoncedircountercounter;
	QString LastSavedrunonceFilename;
	


	//filesystem
	QoccInputOutput* io_man;
	QString LastImportFilename;
	TopoDS_Shape LastImportShape;

	//slider system
	QMap<QString,QSlider*> slidermap;
	QMap<QString,QSlider*> currentslidermap;

	//mouse enviroment
	gp_Pnt CurrentMousePoint;

	//file system watcher
	QFileSystemWatcher* filewatch;

	QKinect::QKinectWrapper kinect;

	



 public slots:


	 void setmousepos(gp_Pnt pos);
	 QScriptValue getmousepos();

	QScriptValue mysqlconnect();
	QScriptValue sqlquery(QScriptValue query);

	 QScriptValue publishproject();
     void setparentwidget(scriptwidget* thewidget);
	 void setuprunonce();
	 void finishrunonce();
	 QScriptValue getdistance();
	 QScriptValue getcurvelength();
	 void savecode();

	 void print(QScriptValue msg);
	 void printlist(QScriptValue list);
	 QScriptValue makepointfromlist(QScriptValue list);
	 QScriptValue savescreenshot();

	 QScriptValue readcsv(QScriptValue filename);
	 QScriptValue getcsvrow(QScriptValue csv,QScriptValue row , QScriptValue column);
	 QScriptValue getcsvrowcount(QScriptValue csv);
	 QScriptValue getcsvcolumncount(QScriptValue csv,QScriptValue row);
	

	 QScriptValue kinectstart();
	 QScriptValue kinectgetimg();
	 QScriptValue kinectpoints();

	 QScriptValue makepolyline();
	

	 QScriptValue kinectprocessdata();

	 QScriptValue makebiarcoptimized();
	 QScriptValue makespherepack();
	 QScriptValue getcenterofgravity();
	 QScriptValue makegroup();
     QScriptValue makemap();
	 QScriptValue mapinsert();
	 QScriptValue getmapcount();
	 QScriptValue getmapkeys();
	 QScriptValue getmapvalues();
	 QScriptValue mapcontains();
	 QScriptValue mapgetvalue();

	 QScriptValue make2ddrawing();
	 QScriptValue make2ddrawingpoly();

	 QScriptValue getmapkeysvalue();

	 QScriptValue getaxis1vec();
	 QScriptValue getaxis1pt();
	 QScriptValue getquadpointmap();

	 QScriptValue getnextuvname();
	 QScriptValue getpointinsideshape();
	 QScriptValue getpointsinsideshape();

	 QScriptValue makepointgrid();



	 QScriptValue crossproduct();
	 QScriptValue makeuvvec();
	 QScriptValue makeuvpt();
	 QScriptValue watchfile();
	 QScriptValue makenormal();

	 QScriptValue makesegmentbyparms();
	 //gui
	 QScriptValue getval();
	 QString getlinefromcontext(QStringList mylist);
	 void checksliders();
	 /// view functions
	  QScriptValue fitall();

	//// utility functions
      QScriptValue vis();
	  QScriptValue vispointmap();
	  QScriptValue vislist();
	  QScriptValue vis(QScriptValue obj);
	  QScriptValue viscurvature();
	  QScriptValue initpart();
	  QScriptValue endpart();
	  QScriptValue panelize();
	  QScriptValue shapeready();
	  QScriptValue shapelistready();
 //    
	  //listopertation
	  QScriptValue makeshapelist();
	  QScriptValue makepointlist();
	  QScriptValue addshapetolist();
	  QScriptValue addpointtolist();
	  QScriptValue makecompound();

 //     //object factory

	//  //points
		QScriptValue makepoint();
		QScriptValue makepoint(double x, double y, double z);
		QScriptValue makeplane();
	    QScriptValue makepointoncurve();
		QScriptValue makepointmovebyvector();

	//  QScriptValue makeprojectpointonsurface();
	//  QScriptValue makeprojectpointoncurve();
	//  QScriptValue makecenterofgravitypoint();
	//  QScriptValue makepointbetween();
	//  QScriptValue makeuvpoint();
	//  QScriptValue makeintersectionpoint();
	//  QScriptValue makeintersectionpointmulti();
	//  
	//  //curves
	    QScriptValue makelineptpt();
		QScriptValue makelineptdir();
	//  QScriptValue makelinenormal();
		QScriptValue makebspline();
	//  QScriptValue makeinterpolatespline();
	//  QScriptValue makebeziercurve();
	//  QScriptValue makecorner();
	    QScriptValue makecircle();
		QScriptValue makehyberbolabyradius();
		QScriptValue makehyberbola();
		QScriptValue makerevolve();
		QScriptValue makeintsrfplane();
	//  QScriptValue makeellipse();
	//  QScriptValue makearc();
	//  QScriptValue makearcofellipse();
	//  QScriptValue makehyberbola();
	//  QScriptValue makeparabola;
	//  QScriptValue makeoffsetcurve();
		QScriptValue make2dpoint();
		QScriptValue make2dpointlist();
		QScriptValue insert2dpointtolist();
		QScriptValue makesplineonsurface();
		QScriptValue make2dpointonsurface();
		QScriptValue make2dlineonsurface();
		QScriptValue makewire();
		QScriptValue makeface();
		QScriptValue makecorner();

		QScriptValue makesolid();
		QScriptValue makeshell();

		QScriptValue makerandomvoronoi();
		QScriptValue getshapelist();
		QScriptValue getshapelistcount();
		QScriptValue getshapelistitem();
		QScriptValue joinshapelists();


	

	//  //mesh
	//  QScriptValue makemesh();
	//  QScriptValue makecoloredmesh();

	// surfaces
		QScriptValue makeloft();
		QScriptValue makeextrude();
		QScriptValue makeintsrfsrf();
		QScriptValue makesweep();
		QScriptValue makesweepbrep();
		QScriptValue makesweepgeom();

		QScriptValue makesweep2sec();
		QScriptValue makefillsrf();
		QScriptValue volmesh();
		QScriptValue makeoffsetsurface();
		QScriptValue makeprojectcrvtosrf();
		QScriptValue makecontrainedsrf();
		QScriptValue makeconicalsrf();
		QScriptValue maketrimmedsurface();

		QScriptValue makeapplyuvcurve();
		QScriptValue makesphericalsurface();

		QScriptValue makeaffinity();
		QScriptValue makeaxistoaxis();

		// build panels
		QScriptValue buildpanelarray();
		QScriptValue buildpanel();
		QScriptValue insertpaneltoarray();
		QScriptValue inserttriangletopanel();
		QScriptValue insertparametertopanel();
		QScriptValue getpanelcount();


		QScriptValue getpanelcolorlist();
		QScriptValue makekmeanscluster();
		QScriptValue makerandomcolorlist();
		QScriptValue makemesh();
		QScriptValue colorclustermesh();
		QScriptValue vismesh();
		QScriptValue makecolor();
		QScriptValue makecolorlist();




	
		

	//  // vectors	
        QScriptValue makeaxis();
		QScriptValue makevector();
	    QScriptValue makevectortangenttocurve();
		QScriptValue getvectorangle();
	// 
	//  // operations
	//  QScriptValue movepointtopoint();
	    QScriptValue makerotate();
        QScriptValue makesymmetry();
		QScriptValue maketranslatebylength();
	//  QScriptValue mirror();
	//  QScriptValue movebyvector();
	//  QScriptValue axistoaxis();

	//  QScriptValue intersect();
	//  QScriptValue split();
	//  QScriptValue booleancut();
	//  QScriptValue booleancommon();
	//  QScriptValue booleanglue();

	//  // get info
	//  QScriptValue getlength();
	//  QScriptValue getarea();
	//  QScriptValue getmindistance();
	//  QScriptValue getmaxdistance();
	//  QScriptValue getuvpointonsurface();
	//  QScriptValue getbiggestface();
	//  QScriptValue getbiggestshell();
	//  QScriptValue getbiggestsolid();

	//  // filesystem operations
		QScriptValue getfile();
		QScriptValue getdir();
		
		QScriptValue getfileonce();
		QScriptValue getdironce();

	//  QScriptValue loadtextfile();
	//  QScriptValue loadbinaryfile();
	    QScriptValue importigs();
	    QScriptValue importstp();
	//  QScriptValue importstl();
	    QScriptValue exportigs();
	    QScriptValue exportstp();
		QScriptValue exportbrep();

		QScriptValue mcosh(QScriptValue parameter);
		QScriptValue msinh(QScriptValue parameter);

		QScriptValue getcoord();
		


		
		

 };

#endif




 
