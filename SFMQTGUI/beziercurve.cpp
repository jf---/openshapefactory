#include "beziercurve.h"
#include "ui.h"
#include "shapefactory.h"
#include "cowbellprogress.h"
#include "parametricsfordummies.h"
#include "bash.h"
#include <QTime>
#include <QFileDialog>
#include "qoccinputoutput.h"


beziercurve::beziercurve(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setup();
}

beziercurve::~beziercurve()
{

}

void beziercurve::setup()
{


connect(ui.pushButton,SIGNAL(pressed()),this,SLOT(loadcsvpoints()));

}

void beziercurve::update()
{

INITPART//
//ic->RemoveAll();

if(textlist.count()>0)
	{
	for(int i=0;i < textlist.count();++i)
		{
			ic->Remove(textlist.at(i));
		}
	}

QList<gp_Pnt> splinepoles;
QList<double> splineweights;

for (int i = 0; i < splinepointlist.size(); ++i) {
     splinepoint splp = splinepointlist.at(i);
	 splinepoles << splp.pole ;
	 visgeo(p1,Point(splp.pole))
	 splineweights << splp.weight;    

	  QString curano;
	  QTextStream in(&curano);
	  in << splp.index;
	  
	  	 TCollection_AsciiString mynewtext = TCollection_AsciiString(curano.toAscii().data());
		 Handle(AIS_Text) AIStext = new AIS_Text(mynewtext,splp.pole);
         AIStext->SetColor(Quantity_Color(0,0,0,Quantity_TypeOfColor::Quantity_TOC_RGB));
         ic->Display(AIStext,0);
		 textlist << AIStext;

	 
 }

visgeo(pline, Polyline(splinepoles))
//visgeo(spline1,SplineInterpSShape(splinepoles))
visgeo(spline2,BezierCurve(splinepoles,splineweights))

//FOR(i,1,100)
//pgeo(p1,PointonCurve(spline1,i/100.0))
//pgeo(p2,PointonCurve(spline2,i/100.0))
//visgeo(line1,Lineptpt(p1,p2))
//END
//
//
//visgeo(blend1,BlendSurface(spline1,spline2))
//


ENDPART//
updatecount++;
}

void beziercurve::loadcsvpoints()
{

QString csvfilepath = QFileDialog::getOpenFileName(this,tr("Open Pole + Weight Data"));

	splinepointlist.clear();
	ui.pointlistdata->clear();

	


	
	  QFile file(csvfilepath);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;

     QTextStream in(&file);
     while (!in.atEnd()) {
         QString line = in.readLine();
		 if(line.count() < 5) continue; // check for count of fields
		 QStringList mylist = line.split(QString(","));
		 int pindex = mylist.at(0).toInt();
		 double pX = mylist.at(1).toDouble();
		 double pY = mylist.at(2).toDouble();
		 double pZ = mylist.at(3).toDouble();
		 double pWeight = mylist.at(4).toDouble();

		
		/* QTableWidgetItem *newItem = new QTableWidgetItem(line);
		 ui.pointlistdata->setItem(pindex-1, 0, newItem);*/
		 new QListWidgetItem(line, ui.pointlistdata);
		
		
		 splinepoint curp;
		 curp.index = pindex;
		 curp.pole = gp_Pnt(pX,pY,pZ);
		 curp.weight = pWeight;
		
         splinepointlist << curp;

	
     }


	 update();
}




