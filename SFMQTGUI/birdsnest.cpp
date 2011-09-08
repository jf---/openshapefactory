#include "birdsnest.h"
#include "shapefactory.h"
#include "ui.h"


birdsnest::birdsnest(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setup();

	

	/*arduino = new QProcess();
	QStringList arguments;
    arguments << "COM9";
	arduino->start("H:/DEV/FDM/FDM_GEOM/readport.exe",arguments);	
    arduino->setReadChannel(QProcess::StandardOutput);


	connect(arduino, SIGNAL(readyReadStandardOutput()), this, SLOT(updatelcd()));*/
	 



}

birdsnest::~birdsnest()
{

}

void birdsnest::setup()
{

update2();

connect(ui.middlez1,SIGNAL(sliderReleased()),this,SLOT(update2()));
connect(ui.middlez2,SIGNAL(sliderReleased()),this,SLOT(update2()));

//connect(ui.middlez1,SIGNAL(sliderMoved ( int )),this,SLOT(update2()));
//connect(ui.middlez2,SIGNAL(sliderMoved ( int )),this,SLOT(update2()));
}

void birdsnest::update()
{

	Handle_AIS_InteractiveContext ic = ui::getInstance()->getWindowContext();

	TopoDS_Compound aRes;
	BRep_Builder aBuilder;
	aBuilder.MakeCompound(aRes);


	TopoDS_Shape pnt1 = HSF::AddNewPoint(gp_Pnt(500,0,0));
	aBuilder.Add(aRes,pnt1);
	TopoDS_Shape tvec1 = HSF::AddNewLineptdir(gp_Pnt(500,0,0),gp_Vec(1,0,0),0,100);
	aBuilder.Add(aRes,tvec1);

	TopoDS_Shape crs = HSF::AddNewCircle(gp_Pnt(500,0,-200),gp_Vec(1,0,0),50);

	//TopoDS_Shape mytor = HSF::AddNewToroidalSurface(gp_Pnt(500,0,0),gp_Vec(1,0,0),200,50);

	TopoDS_Shape mytor = HSF::AddNewRevolveSurface(crs,gp_Pnt(500,0,0),gp_Vec(1,0,0));

	Handle(User_AIS) torcenterais = new User_AIS(mytor,ic);
	/*ic->SetTransparency(torcenterais,0.6);
	HSF::updateUserAIS(mytor,torcenterais,ic);*/
	//aBuilder.Add(aRes,mytor);



	TopoDS_Shape cnt1 = HSF::AddNewCircle(gp_Pnt(500,0,0),gp_Vec(0,0,1),15);
	aBuilder.Add(aRes,cnt1);

	


	TopoDS_Shape srf1 = HSF::AddNewExtrude(cnt1,gp_Vec(0,0,-1),400);
	//aBuilder.Add(aRes,srf1);

	/*QList<TopoDS_Shape> slist;
	slist << torcenter << torcenter2;
	TopoDS_Shape srf1 = HSF::AddNewLoftSurface(slist);
	aBuilder.Add(aRes,srf1);

	TopoDS_Shape intcrv = HSF::AddNewIntersectSrf(mytor,srf1);*/

	TopoDS_Shape intcrv = HSF::AddNewIntersectSrf(mytor,srf1);
	aBuilder.Add(aRes,intcrv);


	TopoDS_Shape split1 = HSF::AddNewSplit(mytor,srf1);
	//TopoDS_Shape smallpiece = HSF::GetSmallestFace(split1);
	aBuilder.Add(aRes,split1);

	HSF::updateAIS(aRes,aisp,ic);

}

void birdsnest::update2()
{

try
	{
	Handle_AIS_InteractiveContext ic = ui::getInstance()->getWindowContext();

	TopoDS_Compound aRes;
	BRep_Builder aBuilder;
	aBuilder.MakeCompound(aRes);



	double radius = 200;
	double upperdist = ui.middlez1->value();
	double lowerdist = ui.middlez2->value();

	if (upperdist < 20. || lowerdist < 20. ) return;

	qDebug() << upperdist <<"," << lowerdist;
	

	gp_Pnt origin(0,0,0);
	gp_Vec zaxis(0,0,1);

	gp_Pnt p1(-radius,0,origin.Z()+upperdist);
	gp_Pnt p2=origin;
	gp_Pnt p3(radius,0,origin.Z()+upperdist);

	gp_Pnt p4(0,-radius,origin.Z()-lowerdist);
	gp_Pnt p5=origin;
	gp_Pnt p6(0,radius,origin.Z()-lowerdist);

	TopoDS_Shape p1s = HSF::AddNewPoint(p1);
	aBuilder.Add(aRes,p1s);

	TopoDS_Shape p2s = HSF::AddNewPoint(p2);
	aBuilder.Add(aRes,p2s);

	TopoDS_Shape p3s = HSF::AddNewPoint(p3);
	aBuilder.Add(aRes,p3s);

	TopoDS_Shape p4s = HSF::AddNewPoint(p4);
	aBuilder.Add(aRes,p4s);

	TopoDS_Shape p5s = HSF::AddNewPoint(p5);
	aBuilder.Add(aRes,p5s);

	TopoDS_Shape p6s = HSF::AddNewPoint(p6);
	aBuilder.Add(aRes,p6s);

	TopoDS_Shape c1 = HSF::AddNewCircle(p1,p2,p3);
	aBuilder.Add(aRes,c1);

	gp_Pnt cog1 = HSF::GetCenterCircle(c1);
	TopoDS_Shape cogshape = HSF::AddNewPoint(cog1);
	double upperadius = cog1.Distance(origin);
	aBuilder.Add(aRes,cogshape);

	TopoDS_Shape c2 = HSF::AddNewCircle(p4,p5,p6);
	aBuilder.Add(aRes,c2);

	gp_Pnt cog2 = HSF::GetCenterCircle(c2);
	TopoDS_Shape cogshape2 = HSF::AddNewPoint(cog2);
	aBuilder.Add(aRes,cogshape2);
	double loweradius = cog2.Distance(origin);

	gp_Pnt cog1off = HSF::MovePointByVector(cog1,gp_Vec(1,0,0),10);
	gp_Vec rotvec(cog1,cog1off);
	TopoDS_Shape srf1 = HSF::AddNewRevolveSurface(c2,cog1,rotvec);
	//aBuilder.Add(aRes,srf1);

	gp_Pnt originup = origin;
	originup.SetZ(originup.Z() + radius);

	TopoDS_Shape c3 = HSF::AddNewCircle(originup,zaxis,radius);
	aBuilder.Add(aRes,c3);

	TopoDS_Shape extrudec3 = HSF::AddNewExtrude(c3,zaxis,-radius * 2);
	//aBuilder.Add(aRes,extrudec3);

	TopoDS_Shape roofsplit = HSF::AddNewIntersectSrfW(srf1,extrudec3);
	aBuilder.Add(aRes,roofsplit);

	
	
	TopoDS_Shape c1arc = HSF::AddNewArcofCircle(p1,p2,p3);
	aBuilder.Add(aRes,c1arc);

	
	TopoDS_Shape c2arc = HSF::AddNewArcofCircle(p4,p5,p6);
	aBuilder.Add(aRes,c2arc);

	TopoDS_Shape sweep1 = HSF::AddNewSweep(c1arc,c2arc);
	//aBuilder.Add(aRes,sweep1);

	TopoDS_Shape split1 = HSF::AddNewSplit(sweep1,extrudec3);
	

	
	TopoDS_Shape bigsplit = HSF::GetBiggestFace(split1);
	aBuilder.Add(aRes,bigsplit);
	
	

	gp_Pnt origindown = origin;
	origindown.SetZ(origin.Z() - radius);
	TopoDS_Shape c4 = HSF::AddNewCircle(origindown,zaxis,radius);
	aBuilder.Add(aRes,c4);

	TopoDS_Shape extrusion2 = HSF::AddNewExtrude(c4,zaxis, 400);
	//aBuilder.Add(aRes,extrusion2);

	
	gp_Pnt or1(0,15,0);
	gp_Pnt or2(15,15,0);
	gp_Vec orvec(or1,or2);



	
	/*TopoDS_Shape split2 = PS.Shape();*/
	//aBuilder.Add(aRes,split2 );


	HSF::updateAIS(aRes,aisp,ic);

	}
	catch(...)

		{

			qDebug() << "Crashed";
		}

}


void birdsnest::updatelcd()
{
	
	//char* mybuffer = "";
	//if(myserial->IsConnected())
	//{
	//	int value = myserial->ReadData(mybuffer,11);
	//	ui.lcdNumber->display(value);

	//}
//QByteArray value = arduino->read(1);

//double result = c[0];
//ui.lcdNumber->display(result);

  QByteArray msg = arduino->read(100000);
  qDebug() << "Output: "<< (msg.data())<<"\n";
  //ui.lcdNumber->display(msg.data());
	
  QString myvalstr(msg.data());
  QStringList sensorvalues = myvalstr.split(",");
  QString SensorId1 = sensorvalues.at(0);
  QString SensorValue1 = sensorvalues.at(1);

  QString SensorId2 = sensorvalues.at(2);
  QString SensorValue2 = sensorvalues.at(3);
  


  
 
  double arduinovalue1 = SensorValue1.toDouble();
  double arduinovalue2 = SensorValue2.toDouble();

	  ui.middlez1->setValue(arduinovalue1+1);
      ui.lcd1->display(msg.data());
	
	 
	  ui.middlez2->setValue(arduinovalue2+1);
      ui.lcd2->display(msg.data());

	  update2();
	




	


}
