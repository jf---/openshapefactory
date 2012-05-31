#pragma once

#ifndef HSFSCRIPTINGINTERFACE_H
#define HSFSCRIPTINGINTERFACE_H


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

#include <QFileSystemWatcher>

 Q_DECLARE_METATYPE(shapefactory)
 Q_DECLARE_METATYPE(gp_Pnt)
 Q_DECLARE_METATYPE(gp_Vec)
 Q_DECLARE_METATYPE(TopoDS_Shape)
 Q_DECLARE_METATYPE(TopoDS_Compound)
 Q_DECLARE_METATYPE(QList<TopoDS_Shape>)
 Q_DECLARE_METATYPE(QList<gp_Pnt>)
 Q_DECLARE_METATYPE(QList<QStringList>)
 Q_DECLARE_METATYPE(QStringList)
 Q_DECLARE_METATYPE(gp_Pln)

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
	TopoDS_Compound folder;
	TopoDS_Compound gaussfolder;
	BRep_Builder B;

	Handle_User_AIS aisp;
	Handle(AIS_Gauss) mygauss;

	//Handle(MeshVS_Mesh) aMeshVS;

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
	



 public slots:


	 void setmousepos(gp_Pnt pos);
	 QScriptValue getmousepos();

     void setparentwidget(scriptwidget* thewidget);
	 void setuprunonce();
	 void finishrunonce();
	 QScriptValue getdistance();

	 void print(QScriptValue msg);
	 void printlist(QScriptValue list);
	 QScriptValue makepointfromlist(QScriptValue list);

	 QScriptValue readcsv(QScriptValue filename);
	 QScriptValue getcsvrow(QScriptValue csv,QScriptValue row , QScriptValue column);
	 QScriptValue getcsvrowcount(QScriptValue csv);
	 QScriptValue getcsvcolumncount(QScriptValue csv,QScriptValue row);
	
	
	 QScriptValue crossproduct();
	 QScriptValue makeuvvec();
	 QScriptValue makeuvpt();
	 QScriptValue watchfile();
	 QScriptValue makenormal();
	 //gui
	 QScriptValue getval();
	 void checksliders();
	 /// view functions
	  QScriptValue fitall();

	//// utility functions
      QScriptValue vis();
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
	//  QScriptValue makeellipse();
	//  QScriptValue makearc();
	//  QScriptValue makearcofellipse();
	//  QScriptValue makehyberbola();
	//  QScriptValue makeparabola;
	//  QScriptValue makeoffsetcurve();
	

	//  //mesh
	//  QScriptValue makemesh();
	//  QScriptValue makecoloredmesh();

	// surfaces
		QScriptValue makeloft();
		QScriptValue makeextrude();
		QScriptValue makeintsrfsrf();
		QScriptValue makesweep();
		QScriptValue makefillsrf();
		QScriptValue volmesh();
		QScriptValue makeoffsetsurface();
		QScriptValue makecontrainedsrf();
		

	//  // vectors	
		QScriptValue makevector();
	    QScriptValue makevectortangenttocurve();
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




 
