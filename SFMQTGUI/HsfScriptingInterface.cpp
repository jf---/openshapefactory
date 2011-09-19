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


HsfScriptingInterface::HsfScriptingInterface()
{
	UnitsAPI::SetLocalSystem(UnitsAPI_MDTV);
	Handle_AIS_InteractiveContext ic = ui::getInstance()->getWindowContext();
	viscount = 0;	 
	setuprunonce();

	io_man = new QoccInputOutput();

	needstofitall = false;
 }

HsfScriptingInterface::~HsfScriptingInterface()
 {
 }


void HsfScriptingInterface::setmousepos(gp_Pnt pos)
{
CurrentMousePoint = pos;
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

 QScriptValue HsfScriptingInterface::importigs()
 {

	 TopoDS_Shape curimport;

	  if(context()->argumentCount() == 1)
	 {
		 QString filename = context()->argument(0).toString();
		 QFileInfo curfile(filename);
		 if (filename == LastImportFilename)
		 {
			curimport = LastImportShape;
		 }else if (curfile.isFile() && (filename.contains("igs")) || (filename.contains("iges")) )
		 {
			 Handle(TopTools_HSequenceOfShape) importedsequence = io_man->importIGES(filename);
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

QScriptValue HsfScriptingInterface::fitall()
{

	needstofitall = true;

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

QScriptValue HsfScriptingInterface::getval()
{

	if (context()->argumentCount() == 3)
	{
	 QStringList mylist = context()->backtrace();
	 QString comand = mylist.at(1);
	 int linenumber = comand.split("at").at(1).toInt()-2;
	 
	 QString lineat  = parentwidget->gettextbyline(linenumber);
	 QString varname = lineat.split("=").at(0).trimmed();
	 
	 int pos = context()->argument(0).toInteger();
	 QString name = varname;
	 QString namelbl = varname +QString("txt");

	 int minval = context()->argument(1).toInteger();
	 int maxval = context()->argument(2).toInteger();
	
	if (slidermap.contains(name)) // if slider is already in the map ge the value
	{
		QSlider* curslider = slidermap[name];
		int index = parentwidget->ui.sliderset->layout()->indexOf(curslider);
		 
		if (pos != index)
		{
		QLayoutItem* curitem = parentwidget->ui.sliderset->layout()->takeAt(index);
		QLayoutItem* curitemlbl = parentwidget->ui.labelset->layout()->takeAt(index);
		parentwidget->ui.sliderset->insertWidget(pos,curitem->widget());
		parentwidget->ui.labelset->insertWidget(pos,curitemlbl->widget());
		}
		
		curslider->setMinimum(minval);
		curslider->setMaximum(maxval);
		double val = curslider->value();
		currentslidermap.insert(name,curslider);
		return engine()->toScriptValue(val);
	} else // else create a new slider
	{
		QSlider* curslider = new QSlider(Qt::Orientation::Horizontal );
		curslider->setObjectName(name);
		curslider->setMinimum(minval);
		curslider->setMaximum(maxval);
		connect(curslider,SIGNAL(sliderMoved(int)),parentwidget,SLOT(evaluatetext()));
		
		slidermap.insert(name,curslider);
		parentwidget->ui.sliderset->insertWidget(pos,curslider);

		QLabel* curlabel = new QLabel(name);
		curlabel->setObjectName(namelbl);
		parentwidget->ui.labelset->insertWidget(pos,curlabel);
		currentslidermap.insert(name,curslider);
 
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
			delete curlabel;
			delete curslider;
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

	 QMap<QString,gp_Ax1> plist = HSF::BuildPointGridonSrf(surface1,x,y);
		QMapIterator<QString,gp_Ax1> i(plist);
		while (i.hasNext()){
		i.next();
			QString currentname = i.key();
			QString epn = HSF::GetNextUvName(currentname,1,0);
			QString sepn = HSF::GetNextUvName(currentname,1,1);
			QString swpn = HSF::GetNextUvName(currentname,0,1);
			if(plist.contains(epn) && plist.contains(sepn) && plist.contains(swpn)){
				gp_Pnt p1 = i.value().Location();
				gp_Pnt p2 = plist.value(epn).Location();
				gp_Pnt p3 = plist.value(sepn).Location();
				gp_Pnt p4 = plist.value(swpn).Location();
				gp_Vec v1 = i.value().Direction();
				gp_Vec v2 = plist.value(epn).Direction();
				gp_Vec v3 = plist.value(sepn).Direction();
				gp_Vec v4 = plist.value(swpn).Direction();


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
	return engine()->toScriptValue(shapelist);
 }

QScriptValue HsfScriptingInterface::makepointlist()
 {
    QList<gp_Pnt> pointlist;
	return engine()->toScriptValue(pointlist);
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

QScriptValue HsfScriptingInterface::addpointtolist()
 {
    QList<gp_Pnt> pointlist;
	if (context()->argumentCount() == 2)
	{
		pointlist = context()->argument(0).toVariant().value<QList<gp_Pnt>>();
		gp_Pnt valuetoadd = hsf::getpointfromshape(context()->argument(1).toVariant().value<TopoDS_Shape>());
		pointlist << valuetoadd; 
	}
	return engine()->toScriptValue(pointlist);
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







QScriptValue HsfScriptingInterface::initpart()
 {
 	viscount=0;
	gaussviscount =0;
	B.MakeCompound(folder);
	B.MakeCompound(gaussfolder);

	currentslidermap.clear();

	return engine()->toScriptValue(true);
 }

QScriptValue HsfScriptingInterface::endpart()
 {
	checksliders();

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
				ic->SetDisplayMode(aisp,2,Standard_False);
				ic->Display(aisp);
				
			 }else{
			   ic = ui::getInstance()->getWindowContext();
		       HSF::updateUserAIS(folder,aisp,ic);
			 }
		 } else {
			   ic = ui::getInstance()->getWindowContext();
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
			ic = ui::getInstance()->getWindowContext();
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