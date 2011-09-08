#include "sghbframefabdata.h"
#include "ui.h"
#include "shapefactory.h"
#include "parametricsfordummies.h"
#include "bash.h"
#include <QFileDialog>
#include "qoccinputoutput.h"
#include <AIS_text.h>


sghbframefabdata::sghbframefabdata(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setup();
	loadhyperbolafamilies();

}

sghbframefabdata::~sghbframefabdata()
{

}

void sghbframefabdata::setup()
{
SLIDERMOVED(update)
connect(ui.loaddata,SIGNAL(pressed()),this,SLOT(loaddatafile()));
connect(ui.gettopo,SIGNAL(pressed()),this,SLOT(gentopology()));
connect(ui.generatehbs,SIGNAL(pressed()),this,SLOT(createhyperbolas()));
connect(ui.generatesplits,SIGNAL(pressed()),this,SLOT(splithyberbolas()));
connect(ui.savetopo,SIGNAL(pressed()),this,SLOT(savetopology()));
connect(ui.genplanes,SIGNAL(pressed()),this,SLOT(generateplanes()));
connect(ui.loadtopo,SIGNAL(pressed()),this,SLOT(loadtopology()));
connect(ui.buildedgemap,SIGNAL(pressed()),this,SLOT(buildedgemap()));
connect(ui.buildflaps,SIGNAL(pressed()),this,SLOT(buildflaps()));

}

void sghbframefabdata::loadhyperbolafamilies()
	{
		
	QoccInputOutput* io_man = new QoccInputOutput();
	QString file0 = QString("H:/Research/PHD/WORKSHOPS/SG2011/Design/GHX_DP_FILESYSTEM/003KWE/pnl0.igs");
	QString file1 = QString("H:/Research/PHD/WORKSHOPS/SG2011/Design/GHX_DP_FILESYSTEM/003KWE/pnl1_extrapolated.igs");
		QString file2 = QString("H:/Research/PHD/WORKSHOPS/SG2011/Design/GHX_DP_FILESYSTEM/003KWE/backsurface.igs");

	
	Handle(TopTools_HSequenceOfShape) famshape1 = io_man->importIGES(file0);
	TopoDS_Shape famcross0 = famshape1->Value(1);

	Handle(TopTools_HSequenceOfShape) famshape2 = io_man->importIGES(file1);
	TopoDS_Shape famcross1 = famshape2->Value(1);

	Handle(TopTools_HSequenceOfShape) backshape3 = io_man->importIGES(file2);
	backsurface = backshape3 ->Value(1);


	gp_Vec upvec(0,0,1);
	gp_Pnt origin(0,0,0);
	gp_Ax3 originsys(origin,upvec);

	TopoDS_Shape fam0 = HSF::AddNewRevolveSurface(famcross0,gp::Origin(),upvec);	
	TopoDS_Shape fam1 = HSF::AddNewRevolveSurface(famcross1,gp::Origin(),upvec);	


	hyperbolicfamilies << fam0 << fam1 ;


	}


void sghbframefabdata::loaddatafile()
	{

QString datafilepath = QFileDialog::getOpenFileName(this,tr("Open Data File for Fab Data"), "H:/Research/PHD/WORKSHOPS/SG2011/Design/GHX_DP_FILESYSTEM/001DATA/", tr("datafiles (*.csv *.CSV *.txt)"));

QList<QStringList> datafile = HSF::ReadDelimitedtoStringList(datafilepath,",");
	
QList<QStringList>::iterator i;

data.clear();

for (i = datafile.begin(); i != datafile.end(); ++i)
	 {
	 QStringList curlist = *i ;
	 if (curlist.count() >1)
		 {
			hbdata curpanel;
			curpanel.Panel_id = curlist.at(0).toInt();
		    curpanel.Panel_u = curlist.at(1).toInt();
            curpanel.Panel_v = curlist.at(2).toInt();
            curpanel.Panel_type =curlist.at(3).toInt();
            curpanel.Panel_x = curlist.at(4).toDouble();
            curpanel.Panel_y = curlist.at(5).toDouble();
            curpanel.Panel_z = curlist.at(6).toDouble();
            curpanel.Panel_normalxdir = curlist.at(7).toDouble();
            curpanel.Panel_normalydir = curlist.at(8).toDouble();
            curpanel.Panel_normalzdir = curlist.at(9).toDouble();
            //curpanel.Panel_planecount = curlist.at(10).toInt();
			data.insert(curpanel.Panel_id,curpanel);
			
		 }

	 }


 ui::getInstance()->Statusbar("loaded file");
	}

void sghbframefabdata::createhyperbolas()
	{
gp_Vec upvec(0,0,1);
gp_Pnt origin(0,0,0);
gp_Ax3 originsys(origin,upvec);


QMap<int, hbdata>::iterator it;
for (it = data.begin(); it != data.end(); ++it)
	{
		hbdata curpanel = it.value() ;
		TopoDS_Shape hbtype = this->hyperbolicfamilies.at(curpanel.Panel_type);
		gp_Pnt panelorigin(curpanel.Panel_x,curpanel.Panel_y,curpanel.Panel_z);
		gp_Vec panelvec(curpanel.Panel_normalxdir,
						curpanel.Panel_normalydir,
						curpanel.Panel_normalzdir);


		gp_Ax3 targetsys(panelorigin,panelvec);
		TopoDS_Shape hb = HSF::axistoaxis(hbtype,originsys,targetsys);
		hyperbolas.insert(curpanel.Panel_id ,hb);
		this->trimmedhyperbolas.insert(curpanel.Panel_id ,hb);
		}



 ui::getInstance()->Statusbar("hyperbolas Done");

	}

void sghbframefabdata::loadtopology()
	{
	
	QString topologypath = QFileDialog::getOpenFileName(this,tr("Open Data File for Fab Data"), "H:/Research/PHD/WORKSHOPS/SG2011/Design/GHX_DP_FILESYSTEM/001DATA/", tr("datafiles (*.csv *.CSV *.txt)"));

	topology.clear();
	  QFile file(topologypath);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;

     QTextStream in(&file);
     while (!in.atEnd()) {
         QString line = in.readLine();
		 QStringList mylist = line.split(QString(","));
		 int panelid = mylist.at(0).toInt();
		 QList<int> subtopology;
		 for(int i=1; i < mylist.count();i++)
			 {
				int childindex = mylist.at(i).toInt();
				subtopology << childindex;

			 }
         topology.insert(panelid,subtopology);
     }



	}
void sghbframefabdata::savetopology()
	{
	
	 QString fileName = QFileDialog::getSaveFileName(this, tr("Save Topology"), "H:\Research\PHD\WORKSHOPS\SG2011\Design\GHX_DP_FILESYSTEM\007TOPOLOGY/topology.csv",tr("csv (*.csv "));

	 QFile file(fileName);
	 if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
     return;

     QTextStream out(&file);
	 
	 QMap<int, QList<int>>::iterator i;
	 for (i = topology.begin(); i != topology.end(); ++i){
     
	 QString line;
	 QTextStream linestream(&line);
	 for (int j=0; j < i.value().count() ; j++)
		 {
		linestream << "," << i.value().at(j);
		 }
	 out << i.key()  << line << "\n";
	 }

	 file.close();

	  ui::getInstance()->Statusbar("Save topology");

	}

void sghbframefabdata::gentopology()
	{

	
double unitradius = 300;
topology.clear();

QMap<int, TopoDS_Shape>::iterator i;
QMap<int, TopoDS_Shape>::iterator j;
for (i = hyperbolas.begin(); i != hyperbolas.end(); ++i){
     int panelid = i.key();
	 TopoDS_Shape currenthb = i.value();
	 hbdata curpanel = data.value(panelid);
	 gp_Pnt curp(curpanel.Panel_x,curpanel.Panel_y,curpanel.Panel_z);
	 QList<int> curtopo;
		 for (j = hyperbolas.begin(); j != hyperbolas.end(); ++j){
		 int otherpanelid = j.key();
		 hbdata otherpanel = data.value(otherpanelid);
		gp_Pnt otherp(otherpanel.Panel_x,otherpanel.Panel_y,otherpanel.Panel_z);
		 TopoDS_Shape otherhb = j.value();
			double mindis = curp.Distance(otherp);
			if (mindis  > 700) continue;
			if (otherpanelid == panelid) continue;
			try{
			if(HSF::isintersecting(currenthb,otherhb)){
				curtopo << otherpanelid;
			} // end of j
			} catch(...){
			qDebug() << "something went wrong";
			continue;
			}
			QString line;
			QTextStream linestream(&line);
			linestream << "Comparing.. Panel" << panelid << "...with panel" << otherpanelid; 
			ui::getInstance()->Statusbar(line);
			qDebug() << line;

		 } // end of i

		 topology.insert(panelid,curtopo);

} // end of i



 ui::getInstance()->Statusbar("topology generated");
		
}

void sghbframefabdata::update()
{
	INITPART // code starts here
	
	 QMap<int, TopoDS_Shape>::iterator k;
	 for (k = trimmedhyperbolas.begin(); k != trimmedhyperbolas.end(); ++k)
		{
			vis(k.value());
		
		}

	 QMap<int, QList<int>>::iterator i;
	 for (i = topology.begin(); i != topology.end(); ++i){


		QList<TopoDS_Shape> intclist = intcrvs.value(i.key());
			
		QList<TopoDS_Shape>::iterator j;
		for (j = intclist.begin(); j != intclist.end(); ++j)
		 {
			TopoDS_Shape curint = *j;
			vis(curint)

		 }
		 }

	
	ENDPART // code ends here
}




void sghbframefabdata::splithyberbolas()
	{

	int counter =0;
	 QMap<int, QList<int>>::iterator i;
	 for (i = topology.begin(); i != topology.end(); ++i){
     
	 counter++;
	 if (counter > 15) continue;
	 //trimmedhyperbolas
	 TopoDS_Shape centercell = hyperbolas.value(i.key());
	
	 //GEOMAlgo_Splitter PS;

	 //PS.AddShape(centercell);
	 TopoDS_Shape theResult =centercell ;
		for (int j=0; j < i.value().count() ; j++)
		 {
		 TopoDS_Shape nbr = hyperbolas.value(i.value().at(j));
		 int nbrid = i.value().at(j);

		 /*BRepAlgoAPI_Section asect(centercell,nbr,Standard_False);
		asect.ComputePCurveOn1(Standard_True);
		asect.Approximation(Standard_True);
		asect.Build();
		TopoDS_Shape intcrv = asect.Shape();*/


		 //TopoDS_Shape intcrv = HSF::AddNewIntersectSrf(centercell,nbr);
		/* gp_Pnt p1 = HSF::AddNewPointonCurve(intcrv,0);
		 gp_Pnt p2 = HSF::AddNewPointonCurve(intcrv,0.5);
		 gp_Pnt p3 = HSF::AddNewPointonCurve(intcrv,1);

		 gp_Pln intpln = HSF::AddNewPlane(p1,p2,p3);*/
		 
		 TopoDS_Shape split1;
		 try{
		 split1= HSF::AddNewSplit(theResult,nbr);
		theResult = HSF::GetBiggestFace(split1);
			 }catch(...)
			 {
				continue;
			 }

		QString line;
		QTextStream linestream(&line);
		linestream << "Splitting:" << i.key() << "with:" << nbrid; 
		ui::getInstance()->Statusbar(line);
		 if (split1.IsNull()) 
			 {
			 //theResult = centercell;
			 continue;
			 }
		 else
			 {
			theResult = HSF::GetBiggestFace(split1);
			
			 }
		 
 		 //PS.AddTool(nbr);
	     }

		//PS.Perform();
	    //TopoDS_Shape theResult = PS.Shape();
		//theResult = HSF::GetBiggestFace(theResult);
		trimmedhyperbolas.insert(i.key(), theResult);
		 }


	 ui::getInstance()->Statusbar("Split Done");

		
	}

void sghbframefabdata::generateplanes()
	{
int loopcounter = 0;
int counter = 0;
QMap<int, QList<int>>::iterator i;
	 for (i = topology.begin(); i != topology.end(); ++i){
     if (loopcounter++ != 33) continue;
	 
	 TopoDS_Shape centercell = hyperbolas.value(i.key());
	
	 TopoDS_Shape theResult =centercell ;
	 QList<gp_Pln> planelist;
	 QList<TopoDS_Shape> intersectionlist;
		for (int j=0; j < i.value().count() ; j++)
		 {
		

		 counter++;
		 TopoDS_Shape nbr = hyperbolas.value(i.value().at(j));
		 int nbrid = i.value().at(j);

		 BRepAlgoAPI_Section asect(centercell,nbr,Standard_False);
		asect.ComputePCurveOn1(Standard_True);
		asect.Approximation(Standard_True);
		asect.Build();
		TopoDS_Shape R = asect.Shape();

		if (!asect.ErrorStatus()== 0) continue;

		TopoDS_Shape intcrv = HSF::AddNewIntersectSrf(centercell,nbr);
		 intersectionlist << R;
		 
		 gp_Pnt p1 = HSF::AddNewPointonCurve(intcrv,0);
		 gp_Pnt p2 = HSF::AddNewPointonCurve(intcrv,0.5);
		 gp_Pnt p3 = HSF::AddNewPointonCurve(intcrv,1);
		 gp_Pln intpln = HSF::AddNewPlane(p1,p2,p3);

		 gp_Vec v1 = HSF::getVectorNormaltoSurfaceatPoint(centercell,p2);
		 TopoDS_Shape l1 = HSF::AddNewLineptdir(p2,v1,0,100);
		 gp_Pnt endp = HSF::AddNewPointonCurve(l1,1.0);
		 gp_Pnt endpproj = HSF::ProjectPoint(endp,intpln);
		 gp_Vec v2(p2,endpproj);
		 TopoDS_Shape l2 = HSF::AddNewLineptdir(p2,v2,0,10000);
		 TopoDS_Shape sw1 = HSF::AddNewSweep(l2,intcrv);
		 TopoDS_Shape backcrv = HSF::AddNewIntersectSrf(backsurface,sw1);
		 
		 		 
		 TopoDS_Shape loft1 = HSF::AddNewBlendSurface(intcrv,backcrv);
		 trimmedhyperbolas.insert(counter,loft1);

		 

		 planelist << intpln;

		  QString line;
		 QTextStream linestream(&line);
		 linestream << "Splitting:" << i.key() << "with:" << nbrid; 
		 ui::getInstance()->Statusbar(line);

		 }

		 intcrvs.insert(i.key(),intersectionlist);
		 intplanes.insert(i.key(),planelist);
 		 }

		 


	 ui::getInstance()->Statusbar("generating planes done");

		






	}

void sghbframefabdata::buildedgemap()
	{
	ui::getInstance()->getWindowContext()->EraseAll();
int loopcounter = 0;
int counter = 0;


QMap<int, QList<int>>::iterator it;
for (it = topology.begin(); it != topology.end(); ++it){
	loopcounter++;
	//if (loopcounter != ui.horizontalSlider->value()) continue;
hbdata curpanel = data.value(it.key());
gp_Pnt panelcenter(curpanel.Panel_x,curpanel.Panel_y,curpanel.Panel_z);
HSF::AddNewAnnotation(QString("center"),panelcenter);
gp_Vec panelvec(curpanel.Panel_normalxdir,curpanel.Panel_normalydir,curpanel.Panel_normalzdir);
gp_Pln panelplane(panelcenter,panelvec);



QList<int> curtopology = it.value();
QList<gp_Pnt> neighbourpoints;
hbdata firstnbr = data.value(curtopology.at(0));
gp_Pnt firstcenter(firstnbr.Panel_x,firstnbr.Panel_y,firstnbr.Panel_z);

for (int j=0;j< curtopology.count();j++)
	{
		hbdata nbr = data.value(curtopology.at(j));
		gp_Pnt nbrcenter(nbr.Panel_x,nbr.Panel_y,nbr.Panel_z);
		neighbourpoints << nbrcenter;

		gp_Pnt txtp(nbrcenter.X(),nbrcenter.Y(),nbrcenter.Z());
		QString line1;
		QTextStream linestream2(&line1);
		linestream2 << "P" << j+1; 
		//HSF::AddNewAnnotation(line1,txtp);	
	}

gp_Pnt curp = neighbourpoints.at(0);

//HSF::AddNewAnnotation(QString("p1"),curp);	

QList<gp_Pnt> sortedpoints;
QList<int> sortedtopology;
sortedpoints << curp;
sortedtopology << curtopology.at(0);
for(int i = 0; i < neighbourpoints.count()-1; i++)
{
gp_Pnt nearestpoint;
double mindis = 1000000000000000;
int minindex ;

for (int k=1;k<neighbourpoints.count();k++)
{
	gp_Pnt nextp = neighbourpoints.at(k);
	nextp = HSF::ProjectPoint(nextp,panelplane);
	gp_Vec curv(panelcenter,nextp);
	nextp = HSF::MovePointByVector(panelcenter,curv,10);

	if (nextp.IsEqual(curp,Precision::Confusion())) continue;
	if (sortedpoints.contains(nextp)) continue;
	if (curp.Distance(nextp) < mindis)
	{
		mindis = curp.Distance(nextp);
		nearestpoint = nextp;
		minindex = curtopology.at(k);
	}

}
QString line;
QTextStream linestream(&line);
linestream << "P" << i+1; 
HSF::AddNewAnnotation(line,nearestpoint);		
curp = nearestpoint;
sortedpoints << curp;
sortedtopology << minindex;

topology.insert(it.key(),sortedtopology);

}

qDebug() << "sortedpoints" << sortedpoints.count();
qDebug() << "sorted topology" << sortedtopology.count();
qDebug() << "before sort" << curtopology;
qDebug() << "after sort" << sortedtopology;


}
//****************************************
//****************************************


ui::getInstance()->Statusbar("generating planes done");

}
void sghbframefabdata::buildflaps()
	{
	
	int counter = 0;
	QMap<int, QList<gp_Pln>> planemap;
	QMap<int, QList<int>>::iterator it;
	for (it = topology.begin(); it != topology.end(); ++it){
	counter++;
	if (counter > 15) return;
	hbdata curpanel = data.value(it.key());
	TopoDS_Shape curhb = hyperbolas.value(it.key());
	QList<int> curtopology = it.value();
	
	QList<gp_Pln> localplanelist;
	for (int j =0;j<curtopology.count();j++)
		{
			hbdata nbr = data.value(curtopology.at(j));
			TopoDS_Shape nbrhb = hyperbolas.value(nbr.Panel_id);
			TopoDS_Shape intcrv1 = HSF::AddNewIntersectSrf(curhb,nbrhb);
			gp_Pnt p1 = HSF::AddNewPointonCurve(intcrv1,0);
			gp_Pnt p2 = HSF::AddNewPointonCurve(intcrv1,0.5);
			gp_Pnt p3 = HSF::AddNewPointonCurve(intcrv1,1);
			gp_Pln plane1 = HSF::AddNewPlane(p1,p2,p3);
			localplanelist << plane1;
			
		}
	planemap.insert(curpanel.Panel_id,localplanelist);
//************ work on the planes now

	if (curtopology.count() < 4) continue;
	for (int k =1;k<curtopology.count()-1;k++)
		{
			gp_Pln prevpl = prevpl = localplanelist.at(k-1);
			gp_Pln currentpl = localplanelist.at(k);
			gp_Pln nextpl = localplanelist.at(k+1); ;
			

			TopoDS_Shape leftline = HSF::AddNewIntersectSrf(currentpl,prevpl);
			TopoDS_Shape rightline = HSF::AddNewIntersectSrf(currentpl,nextpl);
			gp_Pnt intp1 = HSF::AddNewIntersectCrvSrf(leftline,curhb);
			gp_Pnt backintp1 = HSF::AddNewIntersectCrvSrf(leftline,backsurface);

			gp_Vec intv1(intp1,backintp1);
			TopoDS_Shape flapedge1 = HSF::AddNewLineptdir(intp1,intv1,0,100);

			gp_Pnt intp2 = HSF::AddNewIntersectCrvSrf(rightline,curhb);
			gp_Pnt backintp2 = HSF::AddNewIntersectCrvSrf(rightline,backsurface);

			gp_Vec intv2(intp2,backintp2);
			TopoDS_Shape flapedge2 = HSF::AddNewLineptdir(intp2,intv2,0,100);

			if(flapedge1.IsNull()) continue;
			if(flapedge2.IsNull()) continue;

			//TopoDS_Shape blendsrf = HSF::AddNewBlendSurface(leftline,rightline);
			this->trimmedhyperbolas.insert(curpanel.Panel_id,flapedge1);
			this->trimmedhyperbolas.insert(curpanel.Panel_id,flapedge2);
			//this->trimmedhyperbolas.insert(curpanel.Panel_id*-5,rightline);
			

		 QString line;
		 QTextStream linestream(&line);
		 linestream << "building flap:" << curpanel.Panel_id << "with:" << k-1; 
		 ui::getInstance()->Statusbar(line);
		 line = "";
		 linestream << "building flap:" << curpanel.Panel_id << "with:" << k+1; 
		 ui::getInstance()->Statusbar(line);

		}


	}
// *********** done with building the planes now lets intersect the planes


	
	



	}