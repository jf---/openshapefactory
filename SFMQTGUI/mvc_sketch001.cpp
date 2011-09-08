#include "mvc_sketch001.h"
#include "shapefactory.h"
#include "BRep_Builder.hxx"
#include "qoccinputoutput.h"
#include <QTime>

#include "ui.h"



mvc_sketch001::mvc_sketch001(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	mycontroller = ui::getInstance()->getWindowController();
	setup();
}

mvc_sketch001::~mvc_sketch001()
{

}


void mvc_sketch001::setup()
{

	
	connect(ui.pushButton,SIGNAL(pressed()),this,SLOT(update2()));
	connect(ui.p1x,SIGNAL(sliderMoved (int)),this,SLOT(update3()));
	connect(ui.p1y,SIGNAL(sliderMoved (int)),this,SLOT(update3()));
	connect(ui.p1angle,SIGNAL(sliderMoved (int)),this,SLOT(update3()));
	connect(ui.p1radius,SIGNAL(sliderMoved (int)),this,SLOT(update3()));

	connect(ui.p2x,SIGNAL(sliderMoved (int)),this,SLOT(update3()));
	connect(ui.p2y,SIGNAL(sliderMoved (int)),this,SLOT(update3()));
	connect(ui.p2angle,SIGNAL(sliderMoved (int)),this,SLOT(update3()));
	connect(ui.p2radius,SIGNAL(sliderMoved (int)),this,SLOT(update3()));

}
void mvc_sketch001::update()
{

	
	QTime t;
	
	const Handle(AIS_InteractiveContext)& ic = mycontroller->getContext();
	QoccInputOutput* io_man = new QoccInputOutput();

	/*TopoDS_Compound A1;
	BRep_Builder aBuilder;
	aBuilder.MakeCompound (A1);*/



	
	QString hb1filename = QString("H:/DEV/FDM/igesoutput/sg/splitshapetests/2hyberboloidscloseenough.igs");
			
			Handle(TopTools_HSequenceOfShape) myshapes = io_man->importIGES(hb1filename);

			TopoDS_Shape hb1shape = myshapes->Value(1);
			QList<TopoDS_Shape> facelist;
			TopExp_Explorer Ex;
			for (Ex.Init(hb1shape,TopAbs_FACE); Ex.More(); Ex.Next())
					{
						facelist << Ex.Current();
					}
			
			if (facelist.count() > 1)
				{
					TopoDS_Shape f1 = facelist.at(0);
					TopoDS_Shape f2 = facelist.at(1);
					 //draw(f1);
					 //draw(f2);
					
					//// t.start();
					// TopoDS_Shape resultshape1 = HSF::AddNewSplit(f1,f2);
					//// qDebug() << k << ":" << "Split1" << t.elapsed();
					//// t.start();
					// TopoDS_Shape bigface1 = HSF::GetBiggestFace(resultshape1);
					// //qDebug() << k << ":" << "GebigFace1 :" << t.elapsed();
					//// t.start();
					// TopoDS_Shape resultshape2 = HSF::AddNewSplit(f2,f1);
					//// qDebug() << k << ":" << "Split2 :" << t.elapsed();
					// t.start();
					// TopoDS_Shape bigface2 = HSF::GetBiggestFace(resultshape2);
					//// qDebug() << k << ":" << "GetBigFace2 :" << t.elapsed();
					// t.start();
					// draw(bigface1);
					// //qDebug() << k << ":" << "DrawFace1 :" << t.elapsed();
					// //t.start();
					// draw(bigface2);
					// //qDebug() << k << ":" << "DrawFace2:" << t.elapsed();

						 // t.start();
					 TopoDS_Shape resultshape1 = HSF::AddNewSplit(f1,f2);
					 TopoDS_Shape bigface1 = HSF::GetBiggestFace(resultshape1);
					 TopoDS_Shape resultshape2 = HSF::AddNewSplit(f2,f1);
					 TopoDS_Shape bigface2 = HSF::GetBiggestFace(resultshape2);
					 draw(bigface1);
					 draw(bigface2);
					 
						
				}


	
			
		

 //draw(aRes);
}
void mvc_sketch001::update2()
{

	
	QTime t;
	
	const Handle(AIS_InteractiveContext)& ic = mycontroller->getContext();
	QoccInputOutput* io_man = new QoccInputOutput();

	TopoDS_Compound A1;
	BRep_Builder aBuilder;
	aBuilder.MakeCompound (A1);



	
	QString hb1filename = QString("H:/DEV/FDM/igesoutput/sg/splitshapetests/splitest2.igs");
	Handle(TopTools_HSequenceOfShape) myshapes = io_man->importIGES(hb1filename);

			TopoDS_Shape hb1shape = myshapes->Value(1);
			QList<TopoDS_Shape> facelist;
			TopExp_Explorer Ex;
			for (Ex.Init(hb1shape,TopAbs_SHELL); Ex.More(); Ex.Next())
					{
						facelist << Ex.Current();
					}
			int facecount = facelist.count();
			if (facecount > 10)
				{
				
				
				for (int i=2;i<10;i++)
					{
					TopoDS_Shape f1 = facelist.at(i);
					
					for (int j=2;j<10;j++)
					{
					 TopoDS_Shape f2 = facelist.at(j);
					 TopoDS_Shape resultshape1 = HSF::AddNewSplit(f1,f2);
					 if(resultshape1.IsNull()) continue;
					 TopoDS_Shape bigface1 = HSF::GetBiggestShell(resultshape1);
					 HSF::updateUserAIS(bigface1,aisp,ic);
					 qDebug() << "comparing:" << i << "with" << j;
					}
					
					}
						
				}

				mycontroller->update();
	
			
		

 //draw(aRes);
}
void mvc_sketch001::update3()
{

	
	QTime t;
	
	const Handle(AIS_InteractiveContext)& ic = mycontroller->getContext();
	QoccInputOutput* io_man = new QoccInputOutput();

	TopoDS_Compound A1;
	BRep_Builder aBuilder;
	aBuilder.MakeCompound (A1);



	gp_Pnt p1;
	p1.SetX(ui.p1x->value());
	p1.SetY(ui.p1y->value());

	gp_Pnt p2;
	p2.SetX(ui.p2x->value());
	p2.SetY(ui.p2y->value());

	double p1angle = ui.p1angle->value();
	double p1radius = ui.p1radius->value();

	double p2angle = ui.p2angle->value();
	double p2radius = ui.p2radius->value();

	gp_Vec upvec(0,0,1);

	TopoDS_Shape hb1 = HSF::AddNewHyberbolicShell(p1,upvec,p1angle,p1radius,200);
	TopoDS_Shape hb2 = HSF::AddNewHyberbolicShell(p2,upvec,p2angle,p2radius,200);
	
	//aBuilder.Add(A1,hb1);
	//aBuilder.Add(A1,hb2);

	
	hb1 = HSF::GetBiggestFace(hb1);
	hb2 = HSF::GetBiggestFace(hb2);

	TopoDS_Shape split1 = HSF::AddNewSplit(hb1,hb2);
	TopoDS_Shape cut1 = HSF::GetBiggestFace(split1);
		TopoDS_Shape split2 = HSF::AddNewSplit(hb2,hb1);
		TopoDS_Shape cut2 = HSF::GetBiggestFace(split2);
		if (split1.IsNull()) return;
		if (split2.IsNull()) return;
		aBuilder.Add(A1,cut1);
	    aBuilder.Add(A1,cut2);
		


	if (A1.IsNull()) return;
	
	HSF::updateUserAIS(A1,aisp,ic);
	//draw(A1);
	//mycontroller->update();
	//ic->UpdateCurrentViewer();



}
void mvc_sketch001::draw(TopoDS_Shape theshape)
{

		
		if(!theshape.IsNull())
		{
		Handle_AIS_InteractiveContext ic = mycontroller->getContext();
		ic->RemoveAll();
		
		Handle(User_AIS) aisp1 = new User_AIS(theshape,ic);
		ic->SetColor(aisp1,Quantity_NameOfColor::Quantity_NOC_BLACK);
		ic->SetMaterial(aisp1,Graphic3d_NameOfMaterial::Graphic3d_NOM_PEWTER);
		
		ic->SetDisplayMode(aisp1,2);
		ic->Display(aisp1,true);
		}

}
