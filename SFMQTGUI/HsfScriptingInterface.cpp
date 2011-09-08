#include "HsfScriptingInterface.h"

#include "shapefactory.h"
#include "ui.h"
#include "User_AIS.hxx"

#include "SGMGUI_COMMON.h"
# include <QHBoxLayout>
#include "AIS_Gauss.hxx" 

HsfScriptingInterface::HsfScriptingInterface()
{
	UnitsAPI::SetLocalSystem(UnitsAPI_MDTV);
	Handle_AIS_InteractiveContext ic = ui::getInstance()->getWindowContext();
	viscount = 0;	 
 }

HsfScriptingInterface::~HsfScriptingInterface()
 {
 }

QScriptValue HsfScriptingInterface::makepoint()
 {
	 
	 TopoDS_Shape myptshape;
	 if (context()->argumentCount() == 3)
	 {
		 double x,y,z;

		 if (context()->argument(0).toString().toLower() == "infinity") {x = 0;} else { x = context()->argument(0).toNumber();}
		 if (context()->argument(1).toString().toLower() == "infinity") {y = 0;} else { y = context()->argument(1).toNumber();}
		 if (context()->argument(2).toString().toLower() == "infinity") {z = 0;} else { z = context()->argument(2).toNumber();}

    	 gp_Pnt mypt = gp_Pnt(x,y,z);
		 myptshape = hsf::AddNewPoint(mypt);
		 
	 }
	      
     return engine()->toScriptValue(myptshape);
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


QScriptValue HsfScriptingInterface::viscurvature ()
 {
     
	 if(context()->argumentCount() == 3)
	 {
	 TopoDS_Shape myshape = context()->argument(0).toVariant().value<TopoDS_Shape>();
	 bool showedges = context()->argument(1).toBool();
	 double accuracy = context()->argument(2).toNumber();
	 if (accuracy == 0) accuracy = 0.1;
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
				 myvec = gp_Vec(p1,p2);
			 }

		 }
		
	 }
	      
     return engine()->toScriptValue(myvec);
 }

QScriptValue HsfScriptingInterface::makecircle()
 {
     TopoDS_Shape myshape;
	 if (context()->argumentCount() == 3)
	 {
		 gp_Pnt origin = hsf::getpointfromshape(context()->argument(0).toVariant().value<TopoDS_Shape>());
		 gp_Vec upvec  = context()->argument(1).toVariant().value<gp_Vec>();
		 double radius = context()->argument(2).toNumber();
		 if (radius == 0) radius = 0.1;
		 myshape = hsf::AddNewCircle(origin,upvec,radius);
	 
	 }
	      
     return engine()->toScriptValue(myshape);
 }
QScriptValue HsfScriptingInterface::makeshapelist()
 {
    QList<TopoDS_Shape> shapelist;
	return engine()->toScriptValue(shapelist);
 }

QScriptValue HsfScriptingInterface::addshapetolist()
 {
    QList<TopoDS_Shape> shapelist;
	if (context()->argumentCount() == 2)
	{
		shapelist = context()->argument(0).toVariant().value<QList<TopoDS_Shape>>();
		TopoDS_Shape valuetoadd = context()->argument(1).toVariant().value<TopoDS_Shape>();
		shapelist << valuetoadd; 
	}
	return engine()->toScriptValue(shapelist);
 }

QScriptValue HsfScriptingInterface::makeloft()
 {
	 TopoDS_Shape myshape;
    if (context()->argumentCount() == 1)
	{
		QList<TopoDS_Shape> myshapelist = context()->argument(0).toVariant().value<QList<TopoDS_Shape>>();
		if (myshapelist.count() > 0)
		{
		myshape = hsf::AddNewLoftSurface(myshapelist);
		}
	}
	return engine()->toScriptValue(myshape);
 }

QScriptValue HsfScriptingInterface::makeline()
 {
     TopoDS_Shape myshape;
	 if (context()->argumentCount() == 2)
	 {
		 gp_Pnt p1 = hsf::getpointfromshape(context()->argument(0).toVariant().value<TopoDS_Shape>());
		 gp_Pnt p2 = hsf::getpointfromshape(context()->argument(1).toVariant().value<TopoDS_Shape>());

		 myshape = hsf::AddNewLineptpt(p1,p2);
		 	 
	 }
	      
     return engine()->toScriptValue(myshape);
 }

QScriptValue HsfScriptingInterface::initpart()
 {
 	viscount=0;
	gaussviscount =0;
	B.MakeCompound(folder);
	B.MakeCompound(gaussfolder);
	return engine()->toScriptValue(true);
 }

QScriptValue HsfScriptingInterface::endpart()
 {
     if (viscount>0){
	 ic = ui::getInstance()->getWindowContext();
	 HSF::updateUserAIS(folder,aisp,ic);}
	 
	 if (gaussviscount>0){
		 if(!mygauss.IsNull())
		 {
		    mygauss->setshape(gaussfolder);
			mygauss->setshowedges(showgaussedges);
			ic->Redisplay(mygauss);
		 } else 
		 {
            mygauss = new AIS_Gauss(gaussfolder,showgaussedges);
			ic = ui::getInstance()->getWindowContext();
			ic->Display(mygauss);
			
		 }
	 }




     return engine()->toScriptValue(true);
 }