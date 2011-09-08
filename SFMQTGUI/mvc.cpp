#include "mvc.h"
#include "shapefactory.h"
#include "BRep_Builder.hxx"


#include "hyperboloid.h"



#include "ui.h"
#include <stdio.h>
#include <time.h>





mvc::mvc(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	mycontroller = ui::getInstance()->getWindowController();
	setup();
	//update();
	//myserial = new Serial("COM9");

	//arduino = new QProcess();
	//arduino->start("H:/DEV/FDM/FDM_GEOM/readport.exe");	
 //   arduino->setReadChannel(QProcess::StandardOutput);


	// mytimer = new QTimer(this);
 //    //connect(mytimer, SIGNAL(timeout()), this, SLOT(updatelcd()));
	//// connect(arduino, SIGNAL(readyReadStandardOutput()), this, SLOT(updatelcd()));
	// 
 //    mytimer->start(10);


   
	
	

}

mvc::~mvc()
{

}

void mvc::setup()
{
	/*connect(ui.angle,SIGNAL(sliderMoved ( int )),this,SLOT(update()));
	connect(ui.radius,SIGNAL(sliderMoved ( int )),this,SLOT(update()));
	connect(ui.ucount,SIGNAL(sliderMoved ( int )),this,SLOT(update()));
	connect(ui.vcount,SIGNAL(sliderMoved ( int )),this,SLOT(update()));*/
	
	connect(ui.angle,SIGNAL(sliderReleased()),this,SLOT(update()));
	connect(ui.radius,SIGNAL(sliderReleased()),this,SLOT(update()));
	connect(ui.ucount,SIGNAL(sliderReleased()),this,SLOT(update()));
	connect(ui.vcount,SIGNAL(sliderReleased()),this,SLOT(update()));
}



void mvc::update()
{
	Handle_AIS_InteractiveContext ic = mycontroller->getContext();
	
	
	ic->SetIsoNumber(0);

	TopoDS_Compound aRes;
	BRep_Builder aBuilder;
	aBuilder.MakeCompound (aRes);

	
	

	gp_Pnt Origin(0,0,0);
	gp_Vec updir(0,0,1);
	gp_Vec majorvec(1,0,0);
	gp_Vec minorvec(0,1,0);

	gp_Pnt p2(0,0,60);
	int count = 0;
	QList<gp_Pnt> myplist;
	for (int i = 0 ; i< ui.ucount->value(); i++)
	{
	for (int j = 0 ; j< ui.vcount->value(); j++)
	{
		count++;


	
		double percent = (double)count / (double)(ui.ucount->value() * ui.vcount->value());
		TopoDS_Shape hb = HSF::AddNewHyperboloidbyFormula(percent, ui.radius->value(), percent *2 , 0.5 ,gp_Pnt(0,0,0),updir);
		hb = HSF::move(hb,gp_Vec (j * 20, i * 20 ,100));
		aBuilder.Add(aRes,hb);

		QString status;
		QTextStream out(&status);
		out << "running i:" << i << ",j:" << j;
		ui::getInstance()->Statusbar(status);
	}
	}

	if (aRes.IsNull()) return;

			
	mySurface.setShape(aRes);
	mySurface.draw();


	//example of the object oriented langague
	//hyperboloid myHyperboloid(1, ui.ucount->value(), 10, 1);
	//myHyperboloid.move(gp_Vec(10,10,0));
	//myHyperboloid.draw();


	//must end drawing block
	ic->UpdateCurrentViewer();


	
 
}
void mvc::draw(TopoDS_Shape theshape)
{

		
		if(!theshape.IsNull())
		{
		Handle_AIS_InteractiveContext ic = mycontroller->getContext();
		//ic->RemoveAll();
		
		aisp = new AIS_Shape(theshape);
		ic->SetColor(aisp,Quantity_NameOfColor::Quantity_NOC_BLACK);
		ic->SetMaterial(aisp,Graphic3d_NameOfMaterial::Graphic3d_NOM_PEWTER);
		
		ic->SetDisplayMode(aisp,1);
		ic->Display(aisp,true);
		}

}

void mvc::updatelcd()
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

  QByteArray msg = arduino->read(1000);
  qDebug() << "Output: "<< (msg.data())<<"\n";
  ui.lcdNumber->display(msg.data());
	
  QString myvalstr(msg.data());
  myvalstr = myvalstr.left(4);
  
  double arduinovalue = myvalstr.toDouble();
  qDebug() <<"string value =" << myvalstr << "converted:" << arduinovalue;
  this->ui.radius->setValue(arduinovalue);
  update();

	


}