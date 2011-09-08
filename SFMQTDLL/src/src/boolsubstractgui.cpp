#include "boolsubstractgui.h"
#include "qoccapplication.h"
#include <shapefactory.h>

#include <User_AIS.hxx>

#include <GeomConvert.hxx>
#include <Geom_Surface.hxx>
#include <Geom_BezierSurface.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_RectangularTrimmedSurface.hxx>
#include <Geom_ConicalSurface.hxx>
#include <BRep_Builder.hxx>
#include <TopExp_Explorer.hxx>
#include <QProgressDialog>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <TopoDS_Shell.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepFill_Filling.hxx>
#include <BRepOffsetAPI_MakeOffsetShape.hxx> 
#include <BRepOffsetAPI_Sewing.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <gp_GTrsf.hxx>
#include <BRepBuilderAPI_GTransform.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <QDebug>
#include <GCPnts_UniformDeflection.hxx>
#include <GCPnts_UniformAbscissa.hxx>
#include <GeomAPI_IntCS.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <BndLib_Add3dCurve.hxx>
#include <QTime>
#include <BOPTools_DSFiller.hxx>









boolsubstractgui::boolsubstractgui(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	io_man = new QoccInputOutput();
	connect(ui.pushButton,SIGNAL(pressed()), this, SLOT(executeboolean8()) );
	connect(ui.pushButton_2,SIGNAL(pressed()), this, SLOT(executeboolean7()) );
	connect(ui.pushButton_3,SIGNAL(pressed()), this, SLOT(executeboolean3()) );
	connect(ui.pushButton_4,SIGNAL(pressed()), this, SLOT(executestadium()) );
	connect(ui.executerandom1btn,SIGNAL(pressed()), this, SLOT(executeSphereRandom1()) );
	connect(ui.deletebtn,SIGNAL(pressed()), this, SLOT(deleteallgeo()) );
	connect(ui.splittripodbtn,SIGNAL(pressed()), this, SLOT(executesplittripod()) );


	

	


	connect(ui.xslider,SIGNAL(sliderMoved(int)), this, SLOT(setvalues()) );
	connect(ui.yslider,SIGNAL(sliderMoved(int)), this, SLOT(setvalues()) );
	connect(ui.zslider,SIGNAL(sliderMoved(int)), this, SLOT(setvalues()) );
	connect(ui.angleslider,SIGNAL(sliderMoved(int)), this, SLOT(setvalues()) );
	connect(ui.radiusslider,SIGNAL(sliderMoved(int)), this, SLOT(setvalues()) );
	connect(ui.lengthslider,SIGNAL(sliderMoved(int)), this, SLOT(setvalues()) );

	connect(ui.hb_ucountslider,SIGNAL(sliderMoved(int)), this, SLOT(setvalues()) );
	connect(ui.hb_vcountslider,SIGNAL(sliderMoved(int)), this, SLOT(setvalues()) );
	connect(ui.hb_normalscale,SIGNAL(sliderMoved(int)), this, SLOT(setvalues()) );

	connect(ui.bool_check ,SIGNAL(toggled(bool)), this, SLOT(setvalues()) );
	connect(ui.hb_bool_check ,SIGNAL(toggled(bool)), this, SLOT(setvalues()) );

	
	//setvalues();
	
}

void boolsubstractgui::setvalues()
	{
		posx = ui.xslider->value();
		posy = ui.yslider->value();
		posz = ui.zslider->value();
		angle = ui.angleslider->value();
		radius = ui.radiusslider->value();
		length = ui.lengthslider->value();

		hb_ucount = ui.hb_ucountslider->value();
	    hb_vcount= ui.hb_vcountslider->value();
	    hb_zscale= ui.hb_normalscale->value();

		//executeboolean4();
		try{
		executeboolean9();
			}
		catch(...)
			{
			qDebug()<<"crashed";
			}


	}

boolsubstractgui::~boolsubstractgui()
{

}

void boolsubstractgui::executeboolean1()
{
	
	QoccController* vc = myController ;
	const Handle(AIS_InteractiveContext)& ic = vc->getContext();
	ic->EraseAll();

	QString file1 = QString("H:/DEV/FDM/igesoutput/surface_face_001.igs");
	bool myshapes1 = io_man->importModel(	file1,
											io_man->FormatIGES,
											vc->getContext());
}

void boolsubstractgui::executeboolean2()
{
	
	QoccController* vc = myController ;
	const Handle(AIS_InteractiveContext)& ic = vc->getContext();

	ic->EraseAll();

	QString file1 = QString("H:/DEV/FDM/igesoutput/sg/boolsub1.stp");
	QString file2 = QString("H:/DEV/FDM/igesoutput/sg/boolsub2.stp");

	Handle(TopTools_HSequenceOfShape) myshapes1 = io_man->importSTEP(file1);
	Handle(TopTools_HSequenceOfShape) myshapes2 = io_man->importSTEP(file2);

	TopoDS_Shape A = myshapes1->Value(1);
	TopoDS_Shape B = myshapes2->Value(1);

	/*TopoDS_Compound B;
	BRep_Builder aBuilder;
	aBuilder.MakeCompound (B);*/

	TopoDS_Shape S = B;
	QProgressDialog progress("Cheese Factory Boolean:", "Abort Copy", 1,100);
	progress.setValue(0);
	int solidcount = 0;
	TopExp_Explorer Ex;
	for (Ex.Init(myshapes2->Value(1),TopAbs_SOLID); Ex.More(); Ex.Next())
	{
	solidcount +=1;
	}

	int curval = 0;
	for (Ex.Init(myshapes2->Value(1),TopAbs_SOLID); Ex.More(); Ex.Next())
	{
		curval +=1;
		//if (curval == 20) { break;}
		B = Ex.Current();
		S = HSF::BooleanSubstract(A,B);
		A = S;
		progress.setValue(curval*100/solidcount);
		QCoreApplication::instance()->processEvents();
		
		
	}

		Handle(User_AIS) anAISShape = new User_AIS( S,ic );
		ic->SetMaterial( anAISShape, Graphic3d_NameOfMaterial::Graphic3d_NOM_PEWTER);
		ic->SetColor( anAISShape, Quantity_NOC_RED);
		ic->SetDisplayMode( anAISShape, 2, Standard_False );
		ic->Display(anAISShape, Standard_False);

	//const TopoDS_Shape& B = ;
	

  
	

}


void boolsubstractgui::executeboolean3()
{
	
	QoccController* vc = myController ;
	const Handle(AIS_InteractiveContext)& ic = vc->getContext();
	//ic->EraseAll();

	

	
	//double angle = 15;
	//double radius = 20;
	//double length = 300;
	int numx = 3;
	int numy = 3;
	gp_Pnt borigin(posx-(2 * radius),posy-(2 * radius),posz-(length));
	BRepPrimAPI_MakeBox mybox = BRepPrimAPI_MakeBox(borigin,(numx * (radius*2)),(numy * (radius*2)),length);
	mybox.Build();
	TopoDS_Shape stock1 = mybox.Shape();
	TopoDS_Shape stock = mybox.Shape();

	TopoDS_Compound A;
	BRep_Builder aBuilder;
	aBuilder.MakeCompound(A);
	
	for (int i=0;i<numx;i++)
		{
		
		double newx = posx + ( i * 2 * radius);

		for(int j=0;j<numy;j++)
			{

			TopoDS_Compound B;
			aBuilder.MakeCompound (B);

			double newy = posy +( j * 2 * radius);
			gp_Pnt origin(newx,newy,posz);
			gp_Vec dir(0,0,1);
			TopoDS_Face S = HSF::AddNewHyberbolicShell(origin,dir,angle,radius,length/2);
			//aBuilder.Add(B,S);

			BRepOffsetAPI_Sewing sewedObj;
			sewedObj.Add(S);


			TopExp_Explorer Ex;
			for (Ex.Init(S,TopAbs_EDGE); Ex.More(); Ex.Next())
			{
				const TopoDS_Edge& aEdge = TopoDS::Edge (Ex.Current());/*
				Standard_Real aFP, aLP, aP;
				Handle(Geom_Curve) aCurve = BRep_Tool::Curve(aEdge, aFP, aLP);*/
				gp_Pnt startp = HSF::GetgpPntonCurve(aEdge,0.0);
				gp_Pnt endp = HSF::GetgpPntonCurve(aEdge,1.0);
				


				if( startp == endp )
					{
						TopoDS_Wire W = BRepBuilderAPI_MakeWire (aEdge);
						TopoDS_Face topshell = BRepBuilderAPI_MakeFace(W);
						//aBuilder.Add(B,topshell);
						BRepFill_Filling* myfill = new BRepFill_Filling();
						myfill->Add(aEdge,GeomAbs_Shape::GeomAbs_C0);
						myfill->Build();
						sewedObj.Add(myfill->Face());
					
			
					}
					
			}
			sewedObj.Perform();
			TopoDS_Shape newShape = sewedObj.SewedShape();
			TopoDS_Shell newShell ;

			TopExp_Explorer anExp (newShape, TopAbs_SHELL);
			if (anExp.More()) {
			TopoDS_Shape aTmpShape = anExp.Current();
			//newShell = TopoDS::Shell (aTmpShape);
			newShell = BRepBuilderAPI_MakeShell(BRep_Tool::Surface(S));
			BRepBuilderAPI_MakeSolid mySolid(newShell);
			aBuilder.Add(A,newShell);
			stock = HSF::BooleanSubstract(stock,mySolid.Solid());}
			


			
			
			//stock = HSF::BooleanSubstract(stock1,mysolid.Solid());
			

			}

		}

				  //aBuilder.Add(B,stock);
				    aBuilder.Add(A,stock);
	
	

				  if (aisShape.IsNull())
					  {
						aisShape=new User_AIS(A,vc->getContext());								
						vc->getContext()->SetColor(aisShape, Quantity_NOC_BLACK);
					    vc->getContext()->SetMaterial(aisShape,Graphic3d_NameOfMaterial::Graphic3d_NOM_PEWTER);
					    vc->getContext()->SetDisplayMode(aisShape,2,Standard_False);
						aisShape->SetHilightMode(1);
						vc->getContext()->Display(aisShape);
					  }
				  if (aisShape->HasPresentation())
					  {
						aisShape->Set(A);
						vc->getContext()->Deactivate(aisShape);
						vc->getContext()->Redisplay(aisShape);
					  }
				  else
					  {
						vc->getContext()->Display(aisShape);
					  }

		
					 
				     vc->update();

}

void boolsubstractgui::executeboolean4()
{
	
	QoccController* vc = myController ;
	const Handle(AIS_InteractiveContext)& ic = vc->getContext();
	//ic->EraseAll();

	

	
	//double angle = 15;
	//double radius = 20;
	//double length = 300;
	int numx = 3;
	int numy = 3;
	gp_Pnt borigin(posx-(2 * radius),posy-(2 * radius),posz-(length));
	BRepPrimAPI_MakeBox mybox = BRepPrimAPI_MakeBox(borigin,(numx * (radius*2)),(numy * (radius*2)),length);
	mybox.Build();
	TopoDS_Shape stock1 = mybox.Shape();
	TopoDS_Shape stock = mybox.Shape();

	TopoDS_Compound A;
	BRep_Builder aBuilder;
	aBuilder.MakeCompound(A);
	
	for (int i=0;i<numx;i++)
		{
		
		double newx = posx + ( i * 2 * radius);

		for(int j=0;j<numy;j++)
			{

			TopoDS_Compound B;
			aBuilder.MakeCompound (B);

			double newy = posy +( j * 2 * radius);
			gp_Pnt origin(newx,newy,posz);
			gp_Vec dir(0,0,1);
			TopoDS_Shape S = HSF::AddNewRevolvedHyberbolaSolid(origin,dir,angle,radius,length);
			aBuilder.Add(A,S);
			}}

				  //aBuilder.Add(B,stock);
				    
	
	

				  if (aisShape.IsNull())
					  {
						aisShape=new User_AIS(A,vc->getContext());								
						vc->getContext()->SetColor(aisShape, Quantity_NOC_BLACK);
					    vc->getContext()->SetMaterial(aisShape,Graphic3d_NameOfMaterial::Graphic3d_NOM_PEWTER);
					    vc->getContext()->SetDisplayMode(aisShape,2,Standard_False);
						aisShape->SetHilightMode(1);
						vc->getContext()->Display(aisShape);
					  }
				  if (aisShape->HasPresentation())
					  {
						aisShape->Set(A);
						vc->getContext()->Deactivate(aisShape);
						vc->getContext()->Redisplay(aisShape);
					  }
				  else
					  {
						vc->getContext()->Display(aisShape);
					  }

		
					 
				     vc->update();

}


void boolsubstractgui::executeboolean5()
	{
	QoccController* vc = myController ;
	const Handle(AIS_InteractiveContext)& ic = vc->getContext();

	ic->EraseAll();

	QString file1 = QString("H:/DEV/FDM/igesoutput/sg/botcrv.igs");
	QString file2 = QString("H:/DEV/FDM/igesoutput/sg/topcrv.igs");

	Handle(TopTools_HSequenceOfShape) myshapes1 = io_man->importIGES(file1);
	Handle(TopTools_HSequenceOfShape) myshapes2 = io_man->importIGES(file2);

	TopoDS_Shape A = myshapes1->Value(1);
	TopoDS_Shape B = myshapes2->Value(1);

	BRepOffsetAPI_ThruSections aBuilder;
	aBuilder = BRepOffsetAPI_ThruSections(false,true,Precision::Confusion());
	aBuilder.CheckCompatibility(Standard_False);

	QList<TopoDS_Shape> sections;
    sections << A;
    sections << B;

	QListIterator<TopoDS_Shape> glistIT(sections);
	while (glistIT.hasNext())
	{
	
	TopoDS_Shape currentshape = glistIT.next();
	TopoDS_Shape aShapeSection = currentshape;

					TopAbs_ShapeEnum aTypeSect = aShapeSection.ShapeType();
					if(aTypeSect == TopAbs_WIRE)
					  aBuilder.AddWire(TopoDS::Wire(aShapeSection));

					else if(aTypeSect == TopAbs_EDGE) {
					  TopoDS_Edge anEdge = TopoDS::Edge(aShapeSection);
					  TopoDS_Wire aWire = BRepBuilderAPI_MakeWire(anEdge);
					  aBuilder.AddWire(aWire);
					}
					else if(aTypeSect == TopAbs_VERTEX) {
					  TopoDS_Vertex aVert = TopoDS::Vertex(aShapeSection);
					  aBuilder.AddVertex(aVert);
					}
	}

	aBuilder.Build();
	if (aBuilder.Shape().IsNull()) { return; }
	TopoDS_Shape aShape = aBuilder.Shape();

	Handle_Geom_Surface theSurface;
	TopExp_Explorer Ex;
	for (Ex.Init(aShape,TopAbs_FACE); Ex.More(); Ex.Next())
	{
	theSurface = BRep_Tool::Surface(TopoDS::Face(Ex.Current()));
    }
	
	Handle_Geom_BSplineSurface bsplinesurf = GeomConvert::SurfaceToBSplineSurface(theSurface);
	Standard_Real u1, u2, v1, v2;
	theSurface->Bounds(u1,u2,v1,v2);

		TopoDS_Compound pointshape;
		BRep_Builder brb;
		brb.MakeCompound (pointshape);

		TopoDS_Face aFace = HSF::convertgeomsurfaceface(theSurface);

	for(int i=0;i<100;i++)
		{
		for(int j=0;j<56;j++)
		{
		gp_Pnt isopoint;
		theSurface->D0(u1+(((u2-u1)/100)*i),v1+(((v2-v1)/55)*j),isopoint);

		gp_Vec normal = HSF::getVectorNormaltoSurfaceatPoint(aFace,isopoint);
		double nlength = ((v2-v1)/5)*10;
		TopoDS_Edge normalE = HSF::AddNewLineptdir(isopoint,normal,0,nlength);

		BRepBuilderAPI_MakeVertex mkVertex (isopoint);
		TopoDS_Shape aShape = mkVertex.Shape();
		brb.Add(pointshape,aShape);
		brb.Add(pointshape,normalE);

		}
		}


	


		Handle(User_AIS) anAISShape1 = new User_AIS( aShape,ic );
		ic->SetMaterial( anAISShape1, Graphic3d_NameOfMaterial::Graphic3d_NOM_PEWTER);
		ic->SetColor( anAISShape1, Quantity_NOC_RED);
		ic->SetDisplayMode( anAISShape1, 2, Standard_False );
		ic->Display(anAISShape1, Standard_False);
		
		Handle(User_AIS) anAISShape2 = new User_AIS( pointshape,ic );
		ic->SetMaterial( anAISShape2, Graphic3d_NameOfMaterial::Graphic3d_NOM_PEWTER);
		ic->SetColor( anAISShape2, Quantity_NOC_BLACK);
		ic->SetDisplayMode( anAISShape2, 2, Standard_False );
		ic->Display(anAISShape2, Standard_False);
		


	}
void boolsubstractgui::executeboolean6()
	{
	QoccController* vc = myController ;
	const Handle(AIS_InteractiveContext)& ic = vc->getContext();

	ic->EraseAll();

	QString file1 = QString("H:/DEV/FDM/igesoutput/sg/botcrv.igs");
	QString file2 = QString("H:/DEV/FDM/igesoutput/sg/topcrv.igs");

	Handle(TopTools_HSequenceOfShape) myshapes1 = io_man->importIGES(file1);
	Handle(TopTools_HSequenceOfShape) myshapes2 = io_man->importIGES(file2);

	TopoDS_Shape A = myshapes1->Value(1);
	TopoDS_Shape B = myshapes2->Value(1);

	TopoDS_Shape blendsrf = HSF::AddNewBlendSurface(A,B);
	TopoDS_Shape thicksolid = BRepOffsetAPI_MakeThickSolid(blendsrf,TopTools_ListOfShape(),posx,Precision::Confusion());


	
	TopoDS_Compound Bsrf;
	BRep_Builder aBuilder;
	aBuilder.MakeCompound (Bsrf);

	Handle_Geom_Surface theSurface;
	TopExp_Explorer Ex;
	TopoDS_Face face;
	for (Ex.Init(blendsrf,TopAbs_FACE); Ex.More(); Ex.Next())
	{
	face = TopoDS::Face(Ex.Current());
	theSurface = BRep_Tool::Surface(face);
    }

	 //aBuilder.Add(B,face);

	QProgressDialog progress("Cheese Factory Boolean:", "Abort Copy", 1,100);
	progress.setValue(0);

	QMap<QString,gp_Ax1> pointmap = HSF::BuildPointGridonSrf(face,20,5);
	QMapIterator<QString,gp_Ax1> i(pointmap);
	int count = 0;
	while (i.hasNext()) {
		count +=1;
	 progress.setValue((count * 100)/(1000 * 1000));

	 
     gp_Pnt curpoint = i.next().value().Location();
	 gp_Vec curvec = i.next().value().Direction();
	 TopoDS_Edge vecline = HSF::AddNewLineptdir(curpoint,curvec,-5,5);
	 BRepBuilderAPI_MakeVertex mkVertex (curpoint);
	 aBuilder.Add(Bsrf,mkVertex.Shape());
	 //aBuilder.Add(Bsrf,vecline);


 }
	progress.setValue(100);

	
	//gp_Pnt origin;
	//theSurface->D0(0.5,0.5,origin);
	//gp_Vec dir = HSF::getVectorNormaltoSurfaceatPoint(face,origin);
	//TopoDS_Edge hb = HSF::AddNewHyberbolicCrv(origin,dir,300,300);
	//TopoDS_Shape revolvedhb = HSF::AddNewRevolveSurface(hb,origin,dir);

	
	
	
		Handle(AIS_Shape) anAISShape1 = new AIS_Shape( Bsrf);
		ic->SetMaterial( anAISShape1, Graphic3d_NameOfMaterial::Graphic3d_NOM_PLASTIC);
		ic->SetColor( anAISShape1, Quantity_NOC_RED);
		ic->SetDisplayMode( anAISShape1, 1, Standard_False );
		ic->Display(anAISShape1, Standard_False);

		//Handle(User_AIS) anAISShape1 = new User_AIS( revolvedhb,ic );
		//ic->SetMaterial( anAISShape1, Graphic3d_NameOfMaterial::Graphic3d_NOM_PEWTER);
		//ic->SetColor( anAISShape1, Quantity_NOC_RED);
		//ic->SetDisplayMode( anAISShape1, 2, Standard_False );
		//ic->Display(anAISShape1, Standard_False);
		
	
		


	}

void boolsubstractgui::executeboolean7()
{
	
	QoccController* vc = myController ;
	const Handle(AIS_InteractiveContext)& ic = vc->getContext();

	ic->EraseAll();

	QString file1 = QString("H:/DEV/FDM/igesoutput/sg/boolsub1.stp");
	QString file2 = QString("H:/DEV/FDM/igesoutput/sg/boolsub2.stp");

	Handle(TopTools_HSequenceOfShape) myshapes1 = io_man->importSTEP(file1);
	Handle(TopTools_HSequenceOfShape) myshapes2 = io_man->importSTEP(file2);

	TopoDS_Shape A = myshapes1->Value(1);
	TopoDS_Shape B = myshapes2->Value(1);

	TopoDS_Shape S = B;
	TopoDS_Shape firstShape;
	TopoDS_Shape secondShape;
	//QProgressDialog progress("Cheese Factory Boolean:", "Abort Copy", 1,100);
	//progress.setValue(0);
	int solidcount = 0;
	TopExp_Explorer Ex;
	for (Ex.Init(myshapes2->Value(1),TopAbs_SOLID); Ex.More(); Ex.Next())
	{
	solidcount +=1;
	}

	int curval = 0;
	for (Ex.Init(myshapes2->Value(1),TopAbs_SOLID); Ex.More(); Ex.Next())
	{
		curval +=1;
		//if (curval == 20) { break;}
		firstShape = Ex.Current();
		B = Ex.Current();
		if (ui.bool_check->isChecked() == true){
		S = HSF::BooleanSubstract(A,B);
		} else {

		}
		gp_Pnt p1(0,0,0);
		gp_Pnt p2(posx/3,posy/3,posz/3);

		gp_Trsf aTrsf;
        aTrsf.SetTranslation(p1,p2);
		gp_Ax2 anAx2(p1, gp_Dir(1,0,0));

				// Perform Scaling
				gp_GTrsf aGTrsf;
				aGTrsf.SetAffinity(anAx2, radius/100);

		BRepBuilderAPI_Transform aBRepTrsf(B , aTrsf);
		BRepBuilderAPI_GTransform aBRepTrsf2(aBRepTrsf.Shape() , aGTrsf);
		secondShape = aBRepTrsf2.Shape();
		
		if (ui.bool_check->isChecked() == true){
		S = HSF::BooleanSubstract(S,secondShape);
		} else {

		}
		
		//A = S;
		//progress.setValue(curval*100/solidcount);
		//QCoreApplication::instance()->processEvents();
		
		
	}



	
		if (ui.bool_check->isChecked() == true){

			Handle(User_AIS) anAISShape = new User_AIS( S,ic );
			ic->SetMaterial( anAISShape, Graphic3d_NameOfMaterial::Graphic3d_NOM_PEWTER);
			ic->SetColor( anAISShape, Quantity_NOC_RED);
			ic->SetDisplayMode( anAISShape, 2, Standard_False );
			ic->Display(anAISShape, Standard_False);

		} else {

			Handle(User_AIS) anAISShape = new User_AIS( A,ic );
			ic->SetMaterial( anAISShape, Graphic3d_NameOfMaterial::Graphic3d_NOM_PEWTER);
			ic->SetColor( anAISShape, Quantity_NOC_RED);
			ic->SetDisplayMode( anAISShape, 2, Standard_False );
			ic->SetTransparency(anAISShape,0.6);
			ic->Display(anAISShape, Standard_False);




			Handle(User_AIS) anAISShape2 = new User_AIS( secondShape,ic );
			ic->SetMaterial( anAISShape2, Graphic3d_NameOfMaterial::Graphic3d_NOM_NEON_PHC);
			ic->SetColor( anAISShape2, Quantity_NOC_GREEN);
			ic->SetDisplayMode( anAISShape2, 2, Standard_False );
			ic->Display(anAISShape2, Standard_False);

			Handle(User_AIS) anAISShape3 = new User_AIS( firstShape,ic );
			ic->SetMaterial( anAISShape3, Graphic3d_NameOfMaterial::Graphic3d_NOM_NEON_PHC);
			ic->SetColor( anAISShape3, Quantity_NOC_GREEN);
			ic->SetDisplayMode( anAISShape3, 2, Standard_False );
			ic->Display(anAISShape3, Standard_False);

		}


	vc->update();

	//const TopoDS_Shape& B = ;
	

  
	

}

void boolsubstractgui::executeboolean8()
	{
    QoccController* vc = myController ;
	const Handle(AIS_InteractiveContext)& ic = vc->getContext();

	//ic->EraseAll();

	QString topcrvfilename = QString("H:/DEV/FDM/igesoutput/sg/botcrv.igs");
	QString botcrvfilename = QString("H:/DEV/FDM/igesoutput/sg/topcrv.igs");
	QString stockfilename = QString("H:/DEV/FDM/igesoutput/sg/stock.stp");
	QString unitfilename = QString("H:/DEV/FDM/igesoutput/sg/unit.stp");


	Handle(TopTools_HSequenceOfShape) myshapes1 = io_man->importIGES(topcrvfilename);
	Handle(TopTools_HSequenceOfShape) myshapes2 = io_man->importIGES(botcrvfilename);
	Handle(TopTools_HSequenceOfShape) myshapes3 = io_man->importSTEP(stockfilename);
	Handle(TopTools_HSequenceOfShape) myshapes4 = io_man->importSTEP(unitfilename);

	TopoDS_Shape topcrvshape = myshapes1->Value(1);
	TopoDS_Shape botcrvshape = myshapes2->Value(1);
	TopoDS_Shape stockshape = myshapes3->Value(1);
	TopoDS_Shape unitshape = myshapes4->Value(1);

	// Perform Scaling on unit
	gp_GTrsf aGTrsf;
	aGTrsf.SetAffinity(gp_Ax2(gp_Pnt(0,0,0),gp_Vec(0,0,1)), 6.0);
	BRepBuilderAPI_GTransform aBRepTrsf1(unitshape , aGTrsf);
	aGTrsf.SetAffinity(gp_Ax2(gp_Pnt(0,0,0),gp_Vec(0,1,0)), hb_zscale/10);
	BRepBuilderAPI_GTransform aBRepTrsf2(aBRepTrsf1.Shape() , aGTrsf);
	aGTrsf.SetAffinity(gp_Ax2(gp_Pnt(0,0,0),gp_Vec(1,0,0)), hb_zscale/10);
	BRepBuilderAPI_GTransform aBRepTrsf3(aBRepTrsf2.Shape() , aGTrsf);

	
	unitshape = aBRepTrsf3.Shape();

	// new addition testing hyberbolis surface
	unitshape = HSF::AddNewHyberbolicShell(gp_Pnt(0,0,posz),gp_Vec(0,0,1),angle,radius,length * 4);
	
		

	TopoDS_Shape stocksrf= HSF::AddNewBlendSurface(topcrvshape,botcrvshape);
	Handle_Geom_Surface stockgeomsrf = HSF::convertfacetogeom(stocksrf);
	


	
	TopoDS_Compound hbpreviewshape; // load all the hyperbolids to preview
	BRep_Builder aBuilder;
	aBuilder.MakeCompound (hbpreviewshape);

	TopoDS_Shape stockglobkmaterial = stockshape;
	TopoDS_Shape stocklocmaterial;


	QProgressDialog progress("Boolean Api Cut:", "Abort Copy", 1,100);

	if (ui.hb_bool_check->isChecked() == true){
	progress.setValue(0);
	}
	
	int ucount = hb_ucount;
	int vcount = hb_vcount;
	QMap<QString,gp_Ax1> pointmap = HSF::BuildPointGridonSrf(stocksrf,ucount,vcount);

	QMapIterator<QString,gp_Ax1> i(pointmap);
	int count = 0;
	pointmap.count();
	while (i.hasNext()) {
		count +=1;
		if (count > pointmap.count()) { break;}
	 

	 //GETPOINTON SURFACE AND VECTOR
     gp_Pnt curpoint = i.next().value().Location();
	 gp_Dir curvec = i.next().value().Direction();

	 gp_Ax3 axisonsrf(curpoint,curvec);

	 // TRANSFORM THE INCOMING HYBERLOID
	 gp_Pnt p1(0,0,0);
	 gp_Dir v1(0,0,1);
	 gp_Ax3 originaxis(p1,v1);

	 
	 gp_Trsf aTrsf;
     aTrsf.SetDisplacement(originaxis,axisonsrf);
     BRepBuilderAPI_Transform aBRepTrsf(unitshape , aTrsf);
	 

	 TopoDS_Shape secondShape = aBRepTrsf.Shape();
	 

    if (ui.hb_bool_check->isChecked() == true){

	if (!(stockais.IsNull())){ic->Remove(stockais);}
	if (!(hbpreviewais.IsNull())){ic->Remove(hbpreviewais);}

	progress.setValue((count * 100)/(ucount * vcount));
	stocklocmaterial = HSF::BooleanSubstract(stockglobkmaterial,secondShape);
	stockglobkmaterial = stocklocmaterial;
	
	
if (hb_AISShape.IsNull())
					  {
						hb_AISShape=new User_AIS(stocklocmaterial,vc->getContext());								
						vc->getContext()->SetColor(hb_AISShape, Quantity_NOC_BLACK);
					    vc->getContext()->SetMaterial(hb_AISShape,Graphic3d_NameOfMaterial::Graphic3d_NOM_PEWTER);
					    vc->getContext()->SetDisplayMode(hb_AISShape,2,Standard_False);
						hb_AISShape->SetHilightMode(1);
						vc->getContext()->Display(hb_AISShape);
					  }
				  if (hb_AISShape->HasPresentation())
					  {
						hb_AISShape->Set(stocklocmaterial);
						vc->getContext()->Deactivate(hb_AISShape);
						vc->getContext()->Redisplay(hb_AISShape);
					  }
				  else
					  {
						vc->getContext()->Display(hb_AISShape);
					  }

		
					 
				     vc->update();



	QCoreApplication::instance()->processEvents();

	
	} else {
	aBuilder.Add(hbpreviewshape,secondShape);
	}

	 

 }
	

	
	
	if (ui.hb_bool_check->isChecked() == true){
		progress.setValue(100);
	
		ic->Remove(stockais);
	    ic->Remove(hbpreviewais);

		if (hb_AISShape.IsNull())
					  {
						hb_AISShape=new User_AIS(stocklocmaterial,vc->getContext());								
						vc->getContext()->SetColor(hb_AISShape, Quantity_NOC_BLACK);
					    vc->getContext()->SetMaterial(hb_AISShape,Graphic3d_NameOfMaterial::Graphic3d_NOM_PEWTER);
					    vc->getContext()->SetDisplayMode(hb_AISShape,2,Standard_False);
						hb_AISShape->SetHilightMode(1);
						vc->getContext()->Display(hb_AISShape);
					  }
				  if (hb_AISShape->HasPresentation())
					  {
						hb_AISShape->Set(stocklocmaterial);
						vc->getContext()->Deactivate(hb_AISShape);
						vc->getContext()->Redisplay(hb_AISShape);
					  }
				  else
					  {
						vc->getContext()->Display(hb_AISShape);
					  }

		
					 
				     vc->update();


		} else {




		ic->Remove(hb_AISShape);

		
		if (stockais.IsNull())
					  {
		stockais = new AIS_Shape(stockshape);
		ic->SetMaterial( stockais, Graphic3d_NameOfMaterial::Graphic3d_NOM_PLASTER);
		ic->SetColor( stockais, Quantity_NOC_RED);
		ic->SetDisplayMode( stockais, 1, Standard_False );
		ic->SetTransparency(stockais,0.6);
		ic->Display(stockais, Standard_False);
					  }
				  if (stockais->HasPresentation())
					  {
						stockais->Set(stockshape);
						vc->getContext()->Deactivate(stockais);
						vc->getContext()->Redisplay(stockais);
					  }
				  else
					  {
						vc->getContext()->Display(stockais);
					  }

		
					 
				     vc->update();



	 if (hbpreviewais.IsNull())
					  {
				hbpreviewais = new AIS_Shape(hbpreviewshape);
		ic->SetMaterial( hbpreviewais, Graphic3d_NameOfMaterial::Graphic3d_NOM_NEON_GNC);
		ic->SetColor( hbpreviewais, Quantity_NOC_RED);
		ic->SetDisplayMode( hbpreviewais, 1, Standard_False );
		ic->Display(hbpreviewais, Standard_False);
					  }
				  if (hbpreviewais->HasPresentation())
					  {
						hbpreviewais->Set(hbpreviewshape);
						vc->getContext()->Deactivate(hbpreviewais);
						vc->getContext()->Redisplay(hbpreviewais);
					  }
				  else
					  {
						vc->getContext()->Display(hbpreviewais);
					  }

		
					 
				     vc->update();
		
		

		




		}

		//Handle(User_AIS) anAISShape1 = new User_AIS( revolvedhb,ic );
		//ic->SetMaterial( anAISShape1, Graphic3d_NameOfMaterial::Graphic3d_NOM_PEWTER);
		//ic->SetColor( anAISShape1, Quantity_NOC_RED);
		//ic->SetDisplayMode( anAISShape1, 2, Standard_False );
		//ic->Display(anAISShape1, Standard_False);
		

	}



void boolsubstractgui::executeboolean9()
	{
    QoccController* vc = myController ;
	const Handle(AIS_InteractiveContext)& ic = vc->getContext();

	//ic->EraseAll();

	QString surfacefilename = QString("H:/DEV/FDM/igesoutput/surface_face_003.igs");
	QString stockfilename = QString("H:/DEV/FDM/igesoutput/sg/stock4.stp");
	QString unitfilename = QString("H:/DEV/FDM/igesoutput/sg/unit.stp");


	Handle(TopTools_HSequenceOfShape) myshapes1 = io_man->importIGES(surfacefilename);
	
	Handle(TopTools_HSequenceOfShape) myshapes3 = io_man->importSTEP(stockfilename);
	Handle(TopTools_HSequenceOfShape) myshapes4 = io_man->importSTEP(unitfilename);

	TopoDS_Shape srfshape = myshapes1->Value(1);
	
	TopoDS_Shape stockshape = myshapes3->Value(1);
	TopoDS_Shape unitshape = myshapes4->Value(1);

	//// Perform Scaling on unit
	//gp_GTrsf aGTrsf;
	//aGTrsf.SetAffinity(gp_Ax2(gp_Pnt(0,0,0),gp_Vec(0,0,1)), 6.0);
	//BRepBuilderAPI_GTransform aBRepTrsf1(unitshape , aGTrsf);
	//aGTrsf.SetAffinity(gp_Ax2(gp_Pnt(0,0,0),gp_Vec(0,1,0)), hb_zscale/10);
	//BRepBuilderAPI_GTransform aBRepTrsf2(aBRepTrsf1.Shape() , aGTrsf);
	//aGTrsf.SetAffinity(gp_Ax2(gp_Pnt(0,0,0),gp_Vec(1,0,0)), hb_zscale/10);
	//BRepBuilderAPI_GTransform aBRepTrsf3(aBRepTrsf2.Shape() , aGTrsf);

	
	/*unitshape = aBRepTrsf3.Shape();*/

	// new addition testing hyberbolis surface
	/*unitshape = HSF::AddNewHyberbolicShell(gp_Pnt(0,0,posz),gp_Vec(0,0,1),angle,radius,length * 4);
	*/
		

	TopoDS_Shape stocksrf= srfshape;
	Handle_Geom_Surface stockgeomsrf = HSF::convertfacetogeom(stocksrf);
	


	
	TopoDS_Compound hbpreviewshape; // load all the hyperbolids to preview
	BRep_Builder aBuilder;
	aBuilder.MakeCompound (hbpreviewshape);

	TopoDS_Shape stockglobkmaterial = stockshape;
	TopoDS_Shape stocklocmaterial;

	TopoDS_Face aFace = TopoDS::Face(srfshape);


	QProgressDialog progress("Boolean Api Cut:", "Abort Copy", 1,100);

	if (ui.hb_bool_check->isChecked() == true){
	progress.setValue(0);
	}
	
	int ucount = hb_ucount;
	int vcount = hb_vcount;
	QMap<QString,gp_Ax1> pointmap = HSF::BuildRandPointsonSrf(stocksrf,ucount * 3);

	QMapIterator<QString,gp_Ax1> i(pointmap);
	int count = 0;
	pointmap.count();
	while (i.hasNext()) {
		count +=1;
		if (count > pointmap.count()) { break;}
	 

	 //GETPOINTON SURFACE AND VECTOR
     gp_Pnt curpoint = i.next().value().Location();
	 gp_Dir curvec = i.next().value().Direction();

		BRepAdaptor_Surface aSurface(TopoDS::Face(srfshape));
		gp_Pnt2d pUV = HSF::Get2dPntonSurfacefromPoint(srfshape,curpoint);
		gp_Vec ut,vt;
		gp_Pnt pt;
		aSurface.D1(pUV.X(),pUV.Y(),pt,ut,vt);
		gp_Vec V = ut.Crossed(vt);

		Standard_Real mod = V.Magnitude();
		if (mod < Precision::Confusion()) qDebug() << "Vector has no Magnitude" ;
				// consider the face orientation
		if (aFace.Orientation() == TopAbs_REVERSED || 
			aFace.Orientation() == TopAbs_INTERNAL) {
			  V = -V;
		}

	 gp_Ax3 axisonsrf(curpoint,V);

	 // TRANSFORM THE INCOMING HYBERLOID
	 gp_Pnt p1(0,0,0);
	 gp_Dir v1(0,0,1);
	 gp_Ax3 originaxis(p1,v1);

	 
	 gp_Trsf aTrsf;
     aTrsf.SetDisplacement(originaxis,axisonsrf);
     BRepBuilderAPI_Transform aBRepTrsf(unitshape , aTrsf);
	 

	 TopoDS_Shape secondShape = aBRepTrsf.Shape();
	 

    if (ui.hb_bool_check->isChecked() == true){

	if (!(stockais.IsNull())){ic->Remove(stockais);}
	if (!(hbpreviewais.IsNull())){ic->Remove(hbpreviewais);}

	progress.setValue((count * 100)/(ucount * vcount));
	try
		{
	stocklocmaterial = HSF::BooleanSubstract(stockglobkmaterial,secondShape);
		}
	catch(...)
		{
		return;
		}

	stockglobkmaterial = stocklocmaterial;
	
	
if (hb_AISShape.IsNull())
					  {
						hb_AISShape=new User_AIS(stocklocmaterial,vc->getContext());								
						vc->getContext()->SetColor(hb_AISShape, Quantity_NOC_BLACK);
					    vc->getContext()->SetMaterial(hb_AISShape,Graphic3d_NameOfMaterial::Graphic3d_NOM_PEWTER);
					    vc->getContext()->SetDisplayMode(hb_AISShape,2,Standard_False);
						hb_AISShape->SetHilightMode(1);
						vc->getContext()->Display(hb_AISShape);
					  }
				  if (hb_AISShape->HasPresentation())
					  {
						hb_AISShape->Set(stocklocmaterial);
						vc->getContext()->Deactivate(hb_AISShape);
						vc->getContext()->Redisplay(hb_AISShape);
					  }
				  else
					  {
						vc->getContext()->Display(hb_AISShape);
					  }

		
					 
				     vc->update();



	QCoreApplication::instance()->processEvents();

	
	} else {
	aBuilder.Add(hbpreviewshape,secondShape);
	}

	 

 }
	

	
	
	if (ui.hb_bool_check->isChecked() == true){
		progress.setValue(100);
	
		ic->Remove(stockais);
	    ic->Remove(hbpreviewais);

		if (hb_AISShape.IsNull())
					  {
						hb_AISShape=new User_AIS(stocklocmaterial,vc->getContext());								
						vc->getContext()->SetColor(hb_AISShape, Quantity_NOC_BLACK);
					    vc->getContext()->SetMaterial(hb_AISShape,Graphic3d_NameOfMaterial::Graphic3d_NOM_PEWTER);
					    vc->getContext()->SetDisplayMode(hb_AISShape,2,Standard_False);
						hb_AISShape->SetHilightMode(1);
						vc->getContext()->Display(hb_AISShape);
					  }
				  if (hb_AISShape->HasPresentation())
					  {
						hb_AISShape->Set(stocklocmaterial);
						vc->getContext()->Deactivate(hb_AISShape);
						vc->getContext()->Redisplay(hb_AISShape);
					  }
				  else
					  {
						vc->getContext()->Display(hb_AISShape);
					  }

		
					 
				     vc->update();


		} else {




		ic->Remove(hb_AISShape);

		
		if (stockais.IsNull())
					  {
		stockais = new AIS_Shape(stockshape);
		ic->SetMaterial( stockais, Graphic3d_NameOfMaterial::Graphic3d_NOM_PLASTER);
		ic->SetColor( stockais, Quantity_NOC_RED);
		ic->SetDisplayMode( stockais, 1, Standard_False );
		ic->SetTransparency(stockais,0.6);
		ic->Display(stockais, Standard_False);
					  }
				  if (stockais->HasPresentation())
					  {
						stockais->Set(stockshape);
						vc->getContext()->Deactivate(stockais);
						vc->getContext()->Redisplay(stockais);
					  }
				  else
					  {
						vc->getContext()->Display(stockais);
					  }

		
					 
				     vc->update();



	 if (hbpreviewais.IsNull())
					  {
				hbpreviewais = new AIS_Shape(hbpreviewshape);
		ic->SetMaterial( hbpreviewais, Graphic3d_NameOfMaterial::Graphic3d_NOM_NEON_GNC);
		ic->SetColor( hbpreviewais, Quantity_NOC_RED);
		ic->SetDisplayMode( hbpreviewais, 1, Standard_False );
		ic->Display(hbpreviewais, Standard_False);
					  }
				  if (hbpreviewais->HasPresentation())
					  {
						hbpreviewais->Set(hbpreviewshape);
						vc->getContext()->Deactivate(hbpreviewais);
						vc->getContext()->Redisplay(hbpreviewais);
					  }
				  else
					  {
						vc->getContext()->Display(hbpreviewais);
					  }

		
					 
				     vc->update();
		
		

		




		}

		//Handle(User_AIS) anAISShape1 = new User_AIS( revolvedhb,ic );
		//ic->SetMaterial( anAISShape1, Graphic3d_NameOfMaterial::Graphic3d_NOM_PEWTER);
		//ic->SetColor( anAISShape1, Quantity_NOC_RED);
		//ic->SetDisplayMode( anAISShape1, 2, Standard_False );
		//ic->Display(anAISShape1, Standard_False);
		

	}

	void boolsubstractgui::executestadium()
		{

		QoccController* vc = myController ;
		const Handle(AIS_InteractiveContext)& ic = vc->getContext();

		TopoDS_Compound B;
		BRep_Builder aBuilder;
		aBuilder.MakeCompound (B);


		
		//build origin of ellispse
		gp_Pnt aPt(0,0,0);
		gp_Pnt aPt2(0,0,300);
				
		// build axis system that orients ellipse
		gp_Vec upVector(0,0,1);
		gp_Vec upVector2(0,0.5,0.5);
		gp_Vec majorVector(0,1,0);

		TopoDS_Shape ellipseshape1 = HSF::AddNewEllipse(aPt,upVector,majorVector,500,200);
		aBuilder.Add(B,ellipseshape1);

		TopoDS_Shape ellipseshape2 = HSF::AddNewEllipse(aPt2,upVector2,majorVector,375,150);
		aBuilder.Add(B,ellipseshape2);

		
		double st,en;
		
		BRepAdaptor_Curve hC(TopoDS::Edge(ellipseshape2));
		Handle(Geom_Curve) C = BRep_Tool::Curve(TopoDS::Edge(ellipseshape1),st,en);

		

		
		QList<gp_Pnt> myplist;
		GCPnts_UniformAbscissa myAlgo;
		Standard_Real Deflection = 0.1;
		Standard_Real abscissa = 5;
		
		TopoDS_Edge prevedge;
		myAlgo.Initialize( hC , abscissa ) ;
		if ( myAlgo.IsDone() )
		{ Standard_Integer nbr = myAlgo.NbPoints() ;
			Standard_Real param ;
			for ( Standard_Integer i = 1 ; i <= nbr ; i++ )
			{ 
				param = myAlgo.Parameter(i) ;
				gp_Pnt curpoint;
				hC.D0(param,curpoint);

				TopoDS_Shape pvert = BRepBuilderAPI_MakeVertex(curpoint).Shape();
				gp_Vec tngvec;
				hC.D1(param,curpoint,tngvec);
				gp_Pln myplane(curpoint,tngvec);
				TopoDS_Face myplaneface = BRepBuilderAPI_MakeFace(myplane);
				Handle_Geom_Surface S = BRep_Tool::Surface(myplaneface);
				GeomAPI_ProjectPointOnCurve Proj (curpoint, C);
				if (Proj.NbPoints() > 0)
				{
				
				double minangle =0;
				int pindex;
				
				for (int k=1;k < Proj.NbPoints();k++)
					{

					gp_Vec curvec(curpoint,Proj.Point(k));
					if (curvec.Angle(tngvec) > minangle)
						{
							pindex = k;
						}

					}

				//BndLib_Add3dCurve::Add(GAC,Precision::Approximation(),aBox);  
				
		
				TopoDS_Shape pvert1 = BRepBuilderAPI_MakeVertex(Proj.Point(pindex)).Shape();
				aBuilder.Add(B,pvert1);

				TopoDS_Shape isoedge = HSF::AddNewLineptpt(curpoint,Proj.NearestPoint());
				aBuilder.Add(B,isoedge);
				
				

				if (i> 1)
					{
					TopoDS_Shape face = HSF::AddNewBlendSurface(prevedge,isoedge);
					aBuilder.Add(B,face);
					}
						
				prevedge = TopoDS::Edge(isoedge);
				}
				
						
					

				TopoDS_Edge tngedge = HSF::AddNewLineptdir(curpoint,tngvec,0,20);
				aBuilder.Add(B,tngedge);
				aBuilder.Add(B,TopoDS::Vertex(pvert));

			}
		}

		Handle(User_AIS) anAISShape1 = new User_AIS( B,ic );
		ic->SetMaterial( anAISShape1, Graphic3d_NameOfMaterial::Graphic3d_NOM_PEWTER);
		ic->SetColor( anAISShape1, Quantity_NOC_BLACK);
		ic->SetDisplayMode( anAISShape1, 2, Standard_False );
		ic->Display(anAISShape1, Standard_False);
		

		}


void boolsubstractgui::executeSphereRandom1()
{

	QTime t;
	t.start();

	
 


	QoccController* vc = myController ;
	const Handle(AIS_InteractiveContext)& ic = vc->getContext();

	TopoDS_Compound A1;
	TopoDS_Compound A2;
	TopoDS_Compound A3;
	TopoDS_Compound B;
	BRep_Builder aBuilder;
	aBuilder.MakeCompound (A1);
	aBuilder.MakeCompound (A2);
	aBuilder.MakeCompound (A3);
	aBuilder.MakeCompound (B);



	
	QString stockfilename = QString("H:/DEV/FDM/igesoutput/sg/stock.stp");
	//QString surfacefilename = QString("H:/DEV/FDM/igesoutput/sg/sphere_middle.igs");
	QString surfacefilename = QString("H:/DEV/FDM/igesoutput/surface_face_003.igs");
	QString unitfilename = QString("H:/DEV/FDM/igesoutput/sg/unit.stp");
	

	QString exporfilename = QString("H:/DEV/FDM/igesoutput/sg/OutputFileOCC.stp");



	
	Handle(TopTools_HSequenceOfShape) myshapes1 = io_man->importSTEP(stockfilename);
	Handle(TopTools_HSequenceOfShape) myshapes2 = io_man->importIGES(surfacefilename);
	Handle(TopTools_HSequenceOfShape) myshapes4 = io_man->importSTEP(unitfilename);

	
	TopoDS_Shape unitshape = myshapes4->Value(1);

	
	
	

	// Perform Scaling on unit
	//gp_GTrsf aGTrsf;
	//aGTrsf.SetAffinity(gp_Ax2(gp_Pnt(0,0,0),gp_Vec(0,0,1)), 6.0);
	//BRepBuilderAPI_GTransform aBRepTrsf1(unitshape , aGTrsf);
	//aGTrsf.SetAffinity(gp_Ax2(gp_Pnt(0,0,0),gp_Vec(0,1,0)), hb_zscale/10);
	//BRepBuilderAPI_GTransform aBRepTrsf2(aBRepTrsf1.Shape() , aGTrsf);
	//aGTrsf.SetAffinity(gp_Ax2(gp_Pnt(0,0,0),gp_Vec(1,0,0)), hb_zscale/10);
	//BRepBuilderAPI_GTransform aBRepTrsf3(aBRepTrsf2.Shape() , aGTrsf);
	

	TopoDS_Shape stockshape = myshapes1->Value(1);
	int countofshapes = myshapes2->Length();

	TopoDS_Shape stockglobkmaterial = stockshape;
	TopoDS_Shape stocklocmaterial;

	Handle(TopTools_HSequenceOfShape) aSequence =new TopTools_HSequenceOfShape();

	if (countofshapes > 0 )
		{
		TopoDS_Shape srfshape1 = myshapes2->Value(1);
		aBuilder.Add(B,srfshape1);
		
		/*TopoDS_Face face1 = TopoDS::Face(srfshape1);
		TopoDS_Shape srfshape2 = HSF::AddNewThickSolid(face1,30,0);
		aBuilder.Add(A2,srfshape2);*/

		
		


		QMap<QString,gp_Ax1> pointmap = HSF::BuildRandPointsonSrf(srfshape1,100);
		
		QMapIterator<QString,gp_Ax1> i(pointmap);
		int count = 0;
		pointmap.count();
		while (i.hasNext()) {
		count +=1;
		if (count > pointmap.count()) { break;}
		if (count == 15) { continue;}
		QTime t1;
		t1.start();

		gp_Pnt curpoint = i.next().value().Location();

		const TopoDS_Face& aFace = TopoDS::Face (srfshape1);
		gp_Pnt2d pUV = HSF::Get2dPntonSurfacefromPoint(srfshape1,curpoint);

		BRepAdaptor_Surface aSurface(aFace);
		gp_Vec ut,vt;
		gp_Pnt pt;
		aSurface.D1(pUV.X(),pUV.Y(),pt,ut,vt);
		gp_Vec V = ut.Crossed(vt);

		Standard_Real mod = V.Magnitude();
		if (mod < Precision::Confusion()) 
			{
			qDebug() << "Vector has no Magnitude" ;
			continue;
			}
				// consider the face orientation
		if (aFace.Orientation() == TopAbs_REVERSED || 
			aFace.Orientation() == TopAbs_INTERNAL) {
			  V = -V;
		}



		/*TopoDS_Shape secondShape = HSF::AddNewLineptdir(curpoint,ut,0,10);
		aBuilder.Add(A1,secondShape);

		secondShape = HSF::AddNewLineptdir(curpoint,vt,0,10);
		aBuilder.Add(A2,secondShape);*/

		TopoDS_Shape secondShape = HSF::AddNewLineptdir(curpoint,V,0,10);
		aBuilder.Add(A3,secondShape);


		gp_Dir curvec = V;



		gp_Ax3 axisonsrf(curpoint,curvec);

		 // TRANSFORM THE INCOMING HYBERLOID
		 gp_Pnt p1(0,0,0);
		 gp_Dir v1(0,0,1);
		 gp_Ax3 originaxis(p1,v1);

		 
		 gp_Trsf aTrsf;
		 aTrsf.SetDisplacement(originaxis,axisonsrf);
		 BRepBuilderAPI_Transform aBRepTrsf(unitshape , aTrsf);
		 

		 secondShape = aBRepTrsf.Shape();

				
		 //aBuilder.Add(A1,secondShape);
		
		 try{
		 stocklocmaterial = HSF::BooleanSubstract(stockglobkmaterial,secondShape);
			
		 
		 
		 aSequence->Append(stocklocmaterial);
		 io_man->exportModel(QoccInputOutput::FormatSTEP,exporfilename,aSequence);
		 aSequence->Clear();

			 }
		 catch(Standard_Failure)
			 {
			 continue;
			 }
		 stockglobkmaterial = stocklocmaterial;




		aBuilder.Add(B,HSF::AddNewPoint(curpoint));

		
		



		qDebug("Random Hyberbolid Function Time elapsed: %d ms on iteration:%d", t1.elapsed(), count);

		}


		
		

		
			
		Handle(User_AIS) anAISShape1 = new User_AIS( B,ic);
		ic->SetMaterial( anAISShape1, Graphic3d_NameOfMaterial::Graphic3d_NOM_CHROME);
		ic->SetColor( anAISShape1, Quantity_NOC_BLACK);
		ic->SetTransparency(anAISShape1,0.6);
		ic->SetDisplayMode( anAISShape1, 2, Standard_False );
		ic->Display(anAISShape1, Standard_False);

		Handle(User_AIS) anAISShape2 = new User_AIS( stockglobkmaterial,ic);
		ic->SetMaterial( anAISShape2, Graphic3d_NameOfMaterial::Graphic3d_NOM_NEON_GNC);
		ic->SetColor( anAISShape2, Quantity_NOC_RED);
		//anAISShape1->SetHilightMode(1);
		ic->SetDisplayMode( anAISShape2, 2, Standard_False );
		ic->Display(anAISShape2, Standard_False);

	/*	Handle(User_AIS) anAISShape3 = new User_AIS( A2, ic);
		ic->SetMaterial( anAISShape3, Graphic3d_NameOfMaterial::Graphic3d_NOM_NEON_GNC);
		ic->SetColor( anAISShape3, Quantity_NOC_BLUE1);
		ic->SetDisplayMode( anAISShape3, 2, Standard_False );
		ic->Display(anAISShape3, Standard_False);*/


		//Handle(User_AIS) anAISShape4 = new User_AIS( A3,ic);
		//ic->SetMaterial( anAISShape4, Graphic3d_NameOfMaterial::Graphic3d_NOM_NEON_GNC);
		//ic->SetColor( anAISShape4, Quantity_NOC_GREEN);
		////anAISShape1->SetHilightMode(1);
		//ic->SetDisplayMode( anAISShape4, 2, Standard_False );
		//ic->Display(anAISShape4, Standard_False);


		
		vc->update();
		}
	//anAISShape1 = new User_AIS( srfshape,ic );
	//ic->SetMaterial( anAISShape1, Graphic3d_NameOfMaterial::Graphic3d_NOM_NEON_GNC);
	//ic->SetColor( anAISShape1, Quantity_NOC_BLACK);
	//ic->SetDisplayMode( anAISShape1, 2, Standard_False );
	//ic->Display(anAISShape1, Standard_False);

	
	qDebug("Random Hyberbolid Function Time elapsed: %d ms", t.elapsed());


}




void boolsubstractgui::executeSphereRandom1optimized()
{

	QTime t;
	t.start();

	
 


	QoccController* vc = myController ;
	const Handle(AIS_InteractiveContext)& ic = vc->getContext();

	TopoDS_Compound A1;
	TopoDS_Compound A2;
	TopoDS_Compound A3;
	TopoDS_Compound B;
	BRep_Builder aBuilder;
	aBuilder.MakeCompound (A1);
	aBuilder.MakeCompound (A2);
	aBuilder.MakeCompound (A3);
	aBuilder.MakeCompound (B);

	
	BOPTools_DSFiller* aDF = new BOPTools_DSFiller();
	
	
	



	
	QString stockfilename = QString("H:/DEV/FDM/igesoutput/sg/stock.stp");
	//QString surfacefilename = QString("H:/DEV/FDM/igesoutput/sg/sphere_middle.igs");
	QString surfacefilename = QString("H:/DEV/FDM/igesoutput/surface_face_003.igs");
	QString unitfilename = QString("H:/DEV/FDM/igesoutput/sg/unit.stp");
	

	QString exporfilename = QString("H:/DEV/FDM/igesoutput/sg/OutputFileOCC.stp");



	
	Handle(TopTools_HSequenceOfShape) myshapes1 = io_man->importSTEP(stockfilename);
	Handle(TopTools_HSequenceOfShape) myshapes2 = io_man->importIGES(surfacefilename);
	Handle(TopTools_HSequenceOfShape) myshapes4 = io_man->importSTEP(unitfilename);

	
	TopoDS_Shape unitshape = myshapes4->Value(1);

	
	
	

	// Perform Scaling on unit
	//gp_GTrsf aGTrsf;
	//aGTrsf.SetAffinity(gp_Ax2(gp_Pnt(0,0,0),gp_Vec(0,0,1)), 6.0);
	//BRepBuilderAPI_GTransform aBRepTrsf1(unitshape , aGTrsf);
	//aGTrsf.SetAffinity(gp_Ax2(gp_Pnt(0,0,0),gp_Vec(0,1,0)), hb_zscale/10);
	//BRepBuilderAPI_GTransform aBRepTrsf2(aBRepTrsf1.Shape() , aGTrsf);
	//aGTrsf.SetAffinity(gp_Ax2(gp_Pnt(0,0,0),gp_Vec(1,0,0)), hb_zscale/10);
	//BRepBuilderAPI_GTransform aBRepTrsf3(aBRepTrsf2.Shape() , aGTrsf);
	

	TopoDS_Shape stockshape = myshapes1->Value(1);
	int countofshapes = myshapes2->Length();

	TopoDS_Shape stockglobkmaterial = stockshape;
	TopoDS_Shape stocklocmaterial;

	Handle(TopTools_HSequenceOfShape) aSequence =new TopTools_HSequenceOfShape();

	if (countofshapes > 0 )
		{
		TopoDS_Shape srfshape1 = myshapes2->Value(1);
		aBuilder.Add(B,srfshape1);
		
		/*TopoDS_Face face1 = TopoDS::Face(srfshape1);
		TopoDS_Shape srfshape2 = HSF::AddNewThickSolid(face1,30,0);
		aBuilder.Add(A2,srfshape2);*/

		
		


		QMap<QString,gp_Ax1> pointmap = HSF::BuildRandPointsonSrf(srfshape1,100);
		
		QMapIterator<QString,gp_Ax1> i(pointmap);
		int count = 0;
		pointmap.count();
		while (i.hasNext()) {
		count +=1;
		if (count > pointmap.count()) { break;}
		if (count == 15) { continue;}
		QTime t1;
		t1.start();

		gp_Pnt curpoint = i.next().value().Location();

		const TopoDS_Face& aFace = TopoDS::Face (srfshape1);
		gp_Pnt2d pUV = HSF::Get2dPntonSurfacefromPoint(srfshape1,curpoint);

		BRepAdaptor_Surface aSurface(aFace);
		gp_Vec ut,vt;
		gp_Pnt pt;
		aSurface.D1(pUV.X(),pUV.Y(),pt,ut,vt);
		gp_Vec V = ut.Crossed(vt);

		Standard_Real mod = V.Magnitude();
		if (mod < Precision::Confusion()) 
			{
			qDebug() << "Vector has no Magnitude" ;
			continue;
			}
				// consider the face orientation
		if (aFace.Orientation() == TopAbs_REVERSED || 
			aFace.Orientation() == TopAbs_INTERNAL) {
			  V = -V;
		}



		/*TopoDS_Shape secondShape = HSF::AddNewLineptdir(curpoint,ut,0,10);
		aBuilder.Add(A1,secondShape);

		secondShape = HSF::AddNewLineptdir(curpoint,vt,0,10);
		aBuilder.Add(A2,secondShape);*/

		TopoDS_Shape secondShape = HSF::AddNewLineptdir(curpoint,V,0,10);
		aBuilder.Add(A3,secondShape);


		gp_Dir curvec = V;



		gp_Ax3 axisonsrf(curpoint,curvec);

		 // TRANSFORM THE INCOMING HYBERLOID
		 gp_Pnt p1(0,0,0);
		 gp_Dir v1(0,0,1);
		 gp_Ax3 originaxis(p1,v1);

		 
		 gp_Trsf aTrsf;
		 aTrsf.SetDisplacement(originaxis,axisonsrf);
		 BRepBuilderAPI_Transform aBRepTrsf(unitshape , aTrsf);
		 

		 secondShape = aBRepTrsf.Shape();

				
		 //aBuilder.Add(A1,secondShape);
		
		 

		 aDF->SetShapes(stockglobkmaterial, secondShape);
		 
		 if (aDF->IsDone()) {

			 aDF->Perform();

	
			try{
			 stocklocmaterial = HSF::BooleanSubstractFiller(stockglobkmaterial,secondShape,*aDF);
			 aSequence->Append(stocklocmaterial);
			 io_man->exportModel(QoccInputOutput::FormatSTEP,exporfilename,aSequence);
			 aSequence->Clear();
			 }
		 catch(Standard_Failure)
			 {
			 continue;
			 }
		 
		  
		}
		 
		 
		
		 stockglobkmaterial = stocklocmaterial;




		aBuilder.Add(B,HSF::AddNewPoint(curpoint));

		
		



		qDebug("Random Hyberbolid Function Time elapsed: %d ms on iteration:%d", t1.elapsed(), count);

		}


		
		

		
			
		Handle(User_AIS) anAISShape1 = new User_AIS( B,ic);
		ic->SetMaterial( anAISShape1, Graphic3d_NameOfMaterial::Graphic3d_NOM_CHROME);
		ic->SetColor( anAISShape1, Quantity_NOC_BLACK);
		ic->SetTransparency(anAISShape1,0.6);
		ic->SetDisplayMode( anAISShape1, 2, Standard_False );
		ic->Display(anAISShape1, Standard_False);

		Handle(User_AIS) anAISShape2 = new User_AIS( stockglobkmaterial,ic);
		ic->SetMaterial( anAISShape2, Graphic3d_NameOfMaterial::Graphic3d_NOM_NEON_GNC);
		ic->SetColor( anAISShape2, Quantity_NOC_RED);
		//anAISShape1->SetHilightMode(1);
		ic->SetDisplayMode( anAISShape2, 2, Standard_False );
		ic->Display(anAISShape2, Standard_False);

	/*	Handle(User_AIS) anAISShape3 = new User_AIS( A2, ic);
		ic->SetMaterial( anAISShape3, Graphic3d_NameOfMaterial::Graphic3d_NOM_NEON_GNC);
		ic->SetColor( anAISShape3, Quantity_NOC_BLUE1);
		ic->SetDisplayMode( anAISShape3, 2, Standard_False );
		ic->Display(anAISShape3, Standard_False);*/


		//Handle(User_AIS) anAISShape4 = new User_AIS( A3,ic);
		//ic->SetMaterial( anAISShape4, Graphic3d_NameOfMaterial::Graphic3d_NOM_NEON_GNC);
		//ic->SetColor( anAISShape4, Quantity_NOC_GREEN);
		////anAISShape1->SetHilightMode(1);
		//ic->SetDisplayMode( anAISShape4, 2, Standard_False );
		//ic->Display(anAISShape4, Standard_False);


		
		vc->update();
		}
	//anAISShape1 = new User_AIS( srfshape,ic );
	//ic->SetMaterial( anAISShape1, Graphic3d_NameOfMaterial::Graphic3d_NOM_NEON_GNC);
	//ic->SetColor( anAISShape1, Quantity_NOC_BLACK);
	//ic->SetDisplayMode( anAISShape1, 2, Standard_False );
	//ic->Display(anAISShape1, Standard_False);

	
	qDebug("Random Hyberbolid Function Time elapsed: %d ms", t.elapsed());


}






void boolsubstractgui::deleteallgeo()
	{
	QoccController* vc = myController ;
	const Handle(AIS_InteractiveContext)& ic = vc->getContext();

	ic->EraseAll();

	}

void boolsubstractgui::executesplittripod()
	{

	QTime t;
	t.start();

	
 


	QoccController* vc = myController ;
	const Handle(AIS_InteractiveContext)& ic = vc->getContext();

	TopoDS_Compound A1;
	TopoDS_Compound A2;
	TopoDS_Compound A3;
	TopoDS_Compound B;
	BRep_Builder aBuilder;
	aBuilder.MakeCompound (A1);
	aBuilder.MakeCompound (A2);
	aBuilder.MakeCompound (A3);
	aBuilder.MakeCompound (B);



	
	QString hb1filename = QString("H:/DEV/FDM/igesoutput/sg/tripod/hb1.igs");
	QString hb2filename = QString("H:/DEV/FDM/igesoutput/sg/tripod/hb2.igs");
	QString hb3filename = QString("H:/DEV/FDM/igesoutput/sg/tripod/hb3.igs");
	

		
	Handle(TopTools_HSequenceOfShape) myshapes1 = io_man->importIGES(hb1filename);
	Handle(TopTools_HSequenceOfShape) myshapes2 = io_man->importIGES(hb2filename);
	Handle(TopTools_HSequenceOfShape) myshapes3 = io_man->importIGES(hb3filename);

	
	TopoDS_Shape hb1shape = myshapes1->Value(1);
	TopoDS_Shape hb2shape = myshapes2->Value(1);
	TopoDS_Shape hb3shape = myshapes3->Value(1);

	TopoDS_Shape resultshape = HSF::AddNewSplitGeom(hb1shape,hb2shape);
	if(resultshape.IsNull()) return;


	Handle(User_AIS) anAISShape1 = new User_AIS( resultshape,ic);
	ic->SetMaterial( anAISShape1, Graphic3d_NameOfMaterial::Graphic3d_NOM_CHROME);
	ic->SetColor( anAISShape1, Quantity_NOC_BLACK);
	//ic->SetTransparency(anAISShape1,0.6);
	ic->SetDisplayMode( anAISShape1, 2, Standard_False );
	ic->Display(anAISShape1, Standard_False);





	}