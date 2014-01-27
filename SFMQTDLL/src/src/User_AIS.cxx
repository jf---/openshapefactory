//#include <stdafx.h>

#include <User_AIS.hxx>

// Implementation of Handle and type mgt
//
IMPLEMENT_STANDARD_HANDLE(User_AIS,AIS_InteractiveObject)
IMPLEMENT_STANDARD_RTTI(User_AIS,AIS_InteractiveObject)
//
// Foreach ancestors, we add a IMPLEMENT_STANDARD_SUPERTYPE and 
// a IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY macro.
// We must respect the order: from the direct ancestor class
// to the base class.
//

IMPLEMENT_STANDARD_TYPE(User_AIS)
IMPLEMENT_STANDARD_SUPERTYPE(AIS_InteractiveObject) 
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY()
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY(AIS_InteractiveObject)
IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_END()
IMPLEMENT_STANDARD_TYPE_END(User_AIS)

//#include "ColoredMeshDlg.h"

#include <Graphic3d_StructureManager.hxx>
#include <PrsMgr_PresentationManager3d.hxx>
#include <BRepMesh.hxx>
#include <StdPrs_ToolShadedShape.hxx>
#include <Poly_Connect.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <Poly_Triangulation.hxx>
#include <TColgp_Array1OfDir.hxx>
#include <GProp_PGProps.hxx>
//#include <Graphic3d_Array1OfVertexNC.hxx>
#include <Aspect_Array1OfEdge.hxx>
#include <Quantity_Color.hxx>
#include <BRepBuilderAPI_NurbsConvert.hxx>
#include <Prs3d_LineAspect.hxx>
#include <BRepTools.hxx>
#include <AIS_GraphicTool.hxx>
#include <Graphic3d_Group.hxx>
#include "Prs3d_Presentation.hxx"
#include <TopAbs.hxx>
#include <GeomLProp.hxx>
#include <GeomLProp_SLProps.hxx>
//#include <shapefactory.h>
#include <Prs3d_PointAspect.hxx>
// Constructors implementation
//

User_AIS::User_AIS():
AIS_InteractiveObject(PrsMgr_TOP_ProjectorDependant)
	{
	int inside=34;
	int afterinside=0;
	}

User_AIS::User_AIS(TopoDS_Shape theshape, Handle_AIS_InteractiveContext thectx) :
AIS_InteractiveObject(PrsMgr_TOP_ProjectorDependant)

{
	
	myShape = theshape;
	SetHilightMode(0);
	
	SetSelectionMode(1);
	myDrawer->SetShadingAspect(new Prs3d_ShadingAspect());

	myPlanarFaceColor = Quantity_NOC_FIREBRICK3;
	myCylindricalFaceColor = Quantity_NOC_GRAY;
	mycontext = thectx;
	myDrawer->WireAspect()->SetColor(Quantity_NOC_BLACK);
	

//	thectx->SetHilightColor(Quantity_NOC_YELLOW);
	
	
	


				this->SetDisplayMode(3);




}



void User_AIS::Compute(const Handle_PrsMgr_PresentationManager3d& aPresentationManager,
							const Handle_Prs3d_Presentation& aPresentation,
							const Standard_Integer aMode ) 
{
	Quantity_NameOfColor aColor;
	Standard_Real aTransparency = Transparency();
	Graphic3d_NameOfMaterial aMaterial = Graphic3d_NOM_UserDefined ;//Material();
	double colcount = 0.01;
	
	TopExp_Explorer Ex;

	//TopoDS_Shape isoface = hsf::getfacefromshape(myShape);
	
	//TopoDS_Shape uiso,viso;
	//if (!isoface.IsNull())
	//{
	//	uiso= hsf::AddNewIsoCurve(isoface,false,0.5);
	//    viso = hsf::AddNewIsoCurve(isoface,true,0.5);
	//}

						
	switch (aMode) {
	case 0:
		aColor = AIS_InteractiveObject::Color();
		SetColor(aColor);
		myDrawer->WireAspect()->SetColor (Quantity_NameOfColor::Quantity_NOC_YELLOW);
		StdPrs_WFDeflectionShape::Add(aPresentation,myShape, myDrawer );
		break;
	case 1:
			
				
					myDrawer->SetShadingAspectGlobal(Standard_False);
					myDrawer->WireAspect()->SetColor(myOwnColor);
					//myDrawer->WireAspect()->SetWidth(4);	
					StdPrs_WFDeflectionShape::Add(aPresentation,myShape, myDrawer );




	

	  break;

	  case 2:
			
					//myDrawer->SetTypeOfDeflection( Aspect_TOD_ABSOLUTE ); // just to ensure relative deflection is used
					//myDrawer->SetDeviationAngle(75);
					//myDrawer->SetDeviationCoefficient(200);
					//myDrawer->SetDiscretisation(0);
					//myDrawer->SetHLRDeviationCoefficient(200);
					//myDrawer->SetHLRAngle(70);
					//myDrawer->SetMaximalChordialDeviation(200);

					//

					myDrawer->SetShadingAspectGlobal(Standard_False);
		
					//TopoDS_Shape current = Ex.Current();

					//TopoDS_Shape current = myShape;

					myDrawer->PointAspect()->SetColor(Quantity_NameOfColor::Quantity_NOC_BLACK);
					myDrawer->PointAspect()->SetScale(1);
					myDrawer->PointAspect()->SetTypeOfMarker(Aspect_TypeOfMarker::Aspect_TOM_O_STAR);

					//here we set the line color
					myDrawer->WireAspect()->SetColor(Quantity_NameOfColor::Quantity_NOC_BLACK);
					//myDrawer->WireAspect()->SetWidth(4);	
					StdPrs_ShadedShape::Add(aPresentation,myShape, myDrawer);

					
					
					
				for (Ex.Init(myShape,TopAbs_WIRE); Ex.More(); Ex.Next())
					{
						
								
						//myDrawer->WireAspect()->SetColor(Quantity_NOC_BLACK);
						//myDrawer->WireAspect()->SetColor(myOwnColor);
						//myDrawer->WireAspect()->SetWidth(4.0);	
						StdPrs_WFDeflectionShape::Add(aPresentation,Ex.Current(), myDrawer );
						//StdPrs_WFDeflectionShape::Add(aPresentation,myShape, myDrawer );


					}

				// draw isocurves
				//if ( !uiso.IsNull())
				//{
				//myDrawer->WireAspect()->SetColor(Quantity_NameOfColor::Quantity_NOC_BLACK);
				//myDrawer->WireAspect()->SetWidth(1.0);	
				//myDrawer->SetLineArrowDraw(true);
				//myDrawer->SetIsoOnPlane(true);
				////StdPrs_WFDeflectionShape::Add(aPresentation,uiso, myDrawer );
				////StdPrs_WFDeflectionShape::Add(aPresentation,viso, myDrawer );
				//}

	  break;

	  case 3:


				

				


					myDrawer->SetShadingAspectGlobal(Standard_False);
								

					myDrawer->SetShadingAspectGlobal(Standard_False);
					myDrawer->WireAspect()->SetColor(myOwnColor);
					//myDrawer->WireAspect()->SetColor(Quantity_NOC_BLACK);
					//myDrawer->WireAspect()->SetWidth(4);	
					StdPrs_ShadedShape::Add(aPresentation,myShape, myDrawer);
					
					if (myShape.IsNull()) return;
					if (myShape.ShapeType() == TopAbs_FACE || 
						myShape.ShapeType() == TopAbs_SHELL || 
						myShape.ShapeType() == TopAbs_SOLID ||
						myShape.ShapeType() == TopAbs_COMPOUND)
						{
							
							for (Ex.Init(myShape,TopAbs_WIRE); Ex.More(); Ex.Next())
							{
								
										
								myDrawer->WireAspect()->SetColor(Quantity_NOC_BLACK);
							    //myDrawer->WireAspect()->SetColor(myOwnColor);
								//myDrawer->WireAspect()->SetWidth(4);	
								StdPrs_WFDeflectionShape::Add(aPresentation,Ex.Current(), myDrawer );
								//StdPrs_WFDeflectionShape::Add(aPresentation,myShape, myDrawer );


							}
						}
					else
						{
						StdPrs_WFDeflectionShape::Add(aPresentation,myShape, myDrawer );
						}
				

		

	  break;
	}



	}



void User_AIS::Compute(const Handle_Prs3d_Projector& aProjector,
							const Handle_Prs3d_Presentation& aPresentation)
{
	myDrawer->EnableDrawHiddenLine();
	StdPrs_HLRPolyShape::Add(aPresentation,myShape,myDrawer,aProjector);
}

//=======================================================================
//function : SelectionType
//purpose  : gives the type according to the Index of Selection Mode
//=======================================================================

TopAbs_ShapeEnum User_AIS::SelectionType(const Standard_Integer aMode)
{
  switch(aMode){
  case 1:
    return TopAbs_VERTEX;
  case 2:
    return TopAbs_EDGE;
  case 3:
    return TopAbs_WIRE;
  case 4:
    return TopAbs_FACE;
  case 5:
    return TopAbs_SHELL;
  case 6:
    return TopAbs_SOLID;
  case 7:
    return TopAbs_COMPSOLID;
  case 8:
    return TopAbs_COMPOUND;
  case 0:
  default:
    return TopAbs_SHAPE;
  }
  
}
//=======================================================================
//function : SelectionType
//purpose  : gives the SelectionMode according to the Type od Decomposition...
//=======================================================================
Standard_Integer User_AIS::SelectionMode(const TopAbs_ShapeEnum aType)
{
  switch(aType){
  case TopAbs_VERTEX:
    return 1;
  case TopAbs_EDGE:
    return 2;
  case TopAbs_WIRE:
    return 3;
  case  TopAbs_FACE:
    return 4;
  case TopAbs_SHELL:
    return 5;
  case TopAbs_SOLID:
    return 6;
  case TopAbs_COMPSOLID:
    return 7;
  case TopAbs_COMPOUND:
    return 8;
  case TopAbs_SHAPE:
  default:
    return 0;
  }
}

void User_AIS::ComputeSelection(const Handle(SelectMgr_Selection)& aSelection,
                                              const Standard_Integer aMode)
{
  if(myShape.IsNull()) return;
  if (myShape.ShapeType() == TopAbs_COMPOUND) {
    TopoDS_Iterator anExplor (myShape);

    if (!anExplor.More()) // empty Shape -> empty Assembly.
      return;
  }

  static TopAbs_ShapeEnum TypOfSel;
  TypOfSel = User_AIS::SelectionType(aMode);
  TopoDS_Shape shape = myShape;
  if( HasTransformation() ) {
    Handle(Geom_Transformation) trsf = Transformation();
    shape = shape.Located(TopLoc_Location(trsf->Trsf())*shape.Location());
  }

// POP protection against crash in low layers

  Standard_Real aDeflection = GetDeflection(shape, myDrawer);
  Standard_Boolean autoTriangulation = Standard_True;
  try {  
    //OCC_CATCH_SIGNALS
    StdSelect_BRepSelectionTool::Load(aSelection,
                                      this,
                                      shape,
                                      TypOfSel,
                                      aDeflection,
                                      myDrawer->HLRAngle(),
                                      autoTriangulation); 
  } catch ( Standard_Failure ) {
//    cout << "a Shape should be incorrect : A Selection on the Bnd  is activated   "<<endl;
    if ( aMode == 0 ) {
      Bnd_Box B = BoundingBox();
      Handle(StdSelect_BRepOwner) aOwner = new StdSelect_BRepOwner(shape,this);
      Handle(Select3D_SensitiveBox) aSensitiveBox = new Select3D_SensitiveBox(aOwner,B);
      aSelection->Add(aSensitiveBox);
    }
  }

  // insert the drawer in the BrepOwners for hilight...
  StdSelect::SetDrawerForBRepOwner(aSelection,myDrawer);
}


Standard_Real User_AIS::GetDeflection(const TopoDS_Shape& aShape,
                                       const Handle(Prs3d_Drawer)& aDrawer)
{
  // WARNING: this same piece of code appears several times in Prs3d classes
  Standard_Real aDeflection = aDrawer->MaximalChordialDeviation();
  if (aDrawer->TypeOfDeflection() == Aspect_TOD_RELATIVE) {
    Bnd_Box B;
    BRepBndLib::Add(aShape, B, Standard_False);
    if ( ! B.IsVoid() )
    {
      Standard_Real aXmin, aYmin, aZmin, aXmax, aYmax, aZmax;
      B.Get(aXmin, aYmin, aZmin, aXmax, aYmax, aZmax);
      aDeflection = Max( aXmax-aXmin, Max(aYmax-aYmin, aZmax-aZmin)) *
                    aDrawer->DeviationCoefficient() * 4;
    }
  }
  return aDeflection;
}



//void User_AIS::ComputeSelection(const Handle_SelectMgr_Selection& aSelection,
//									 const Standard_Integer aMode)
//{
//	switch(aMode){
//	case 0:
//		StdSelect_BRepSelectionTool::Load(aSelection,myShape,TopAbs_COMPOUND,Precision::Confusion(),Precision::Angular());
//		break;
//	case 1:
//		StdSelect_BRepSelectionTool::Load(aSelection,myShape,TopAbs_VERTEX,Precision::Confusion(),Precision::Angular());
//		break;
//	case 2:
//		StdSelect_BRepSelectionTool::Load(aSelection,myShape,TopAbs_EDGE,Precision::Confusion(),Precision::Angular());
//		break;
//	case 3:
//		StdSelect_BRepSelectionTool::Load(aSelection,myShape,TopAbs_WIRE,Precision::Confusion(),Precision::Angular());
//		break;
//	case 4:
//		StdSelect_BRepSelectionTool::Load(aSelection,myShape,TopAbs_FACE,Precision::Confusion(),Precision::Angular());
//		break;
//	}
//}

Standard_Integer User_AIS::NbPossibleSelection() const
{
	return 5;
}

const Bnd_Box& User_AIS::BoundingBox()  
{
  if (myShape.ShapeType() == TopAbs_COMPOUND) {
    TopoDS_Iterator anExplor (myShape);

    if (!anExplor.More()) { // empty Shape  -> empty Assembly.
      myBB.SetVoid();
      return myBB;
    }
  }

  if(myCompBB) {
    BRepBndLib::AddClose(myShape, myBB);
    myCompBB = Standard_False;
  }
  return myBB;
}

Standard_Boolean User_AIS::AcceptShapeDecomposition() const
{
	return Standard_True;
}

void User_AIS::SetPlanarFaceColor(const Quantity_Color acolor)
{
	myPlanarFaceColor = acolor;
}

void User_AIS::SetCylindricalFaceColor(const Quantity_Color acolor)
{
	myCylindricalFaceColor = acolor;
}

Standard_Boolean User_AIS::TriangleIsValid(const gp_Pnt& P1, const gp_Pnt& P2, const gp_Pnt& P3) const
{ 
  gp_Vec V1(P1,P2);								// V1=(P1,P2)
  gp_Vec V2(P2,P3);								// V2=(P2,P3)
  gp_Vec V3(P3,P1);								// V3=(P3,P1)
  
  if ((V1.SquareMagnitude() > 1.e-10) && (V2.SquareMagnitude() > 1.e-10) && (V3.SquareMagnitude() > 1.e-10))
    {
      V1.Cross(V2);								// V1 = Normal	
      if (V1.SquareMagnitude() > 1.e-10)
	return Standard_True;
      else
	return Standard_False;
    }
  else
    return Standard_False;
  
}

///*Quantity_Color User_AIS::Color(gp_Pnt& thePoint,Standard_Real AltMin,Standard_Real AltMax,
//	*/								const Standard_Integer ColorizationMode) 
//{
//	red =1;   //initializing colors parameters
//	green=1;
//	blue =1;
//	switch ( ColorizationMode)
//	{
//		case 0 : //normal, vert/maron
//		{
//			Standard_Real Alt= thePoint.Z();
//
//			Standard_Real AltDelta;
//
//			AltDelta = AltMax-AltMin;
//
//			red = 0.5- ((0.5*(AltMax-Alt))/(AltDelta));
//		//	red = 1/(((1000/(AltMax-AltMin))*Alt)+1000*(1-(AltMin/(AltMax-AltMin))));
//		
//		//	green = (3*AltMax-AltMin)/(3*AltMax-8*AltMin + 7*Alt);
//		//	green = 1/(((7/(3*AltMax-AltMin))*Alt) + 1-(7*AltMin/(3*AltMax-AltMin)));
//			Standard_Real A = 7*Alt-7*AltMin;
//			green = (3*AltMax-AltMin)/(3*AltMax-AltMin+(7*Alt-7*AltMin));
////			AfxMessageBox(green);
//		//	green =(0.30- (((0.3-1)*(AltMax-Alt))/(AltMax-AltMin)));
//		
//			blue = 0 ;
///*
//			red = 0.5;
//			green = 0.2222;
//			blue = 0;
//*/	
//			Quantity_Color color;
//			color.SetValues(red,green,blue, Quantity_TOC_RGB);
//			return color;
//			break;
//		}//end case 0
//
//		case 1 :	//mer-neige
//		{		
//			Standard_Real Alt= thePoint.Z();
//
//			Standard_Real b =AltMax-AltMin;
//			Standard_Real a= AltMax-thePoint.Z();
//
//			red =1;
//			green=1;
//			blue =1;
//			if (0<a && a <= (b/5))
//			{
//				red = 1;
//				green = 1;
//				blue = 1;
//			}
//			else if ((b/5)<a  && a <= (2*b/5))
//			{
//				 red = .55;
//				 green = 0.3;
//				 blue = 0;
//			}
//			else if ((2*b/5)<a  && a <= (18*b/20))
//			{
//				 green =1/(((7/(3*AltMax-AltMin))*Alt)+(1-(7*AltMin/(3*AltMax-AltMin))));
//				 red = 1/(((1000/(AltMax-AltMin))*Alt)+1000*(1-(AltMin/(AltMax-AltMin))));
//				 blue = 0;
//			}
//			else if ((18*b/20)<a  && a <= (18.5*b/20))
//			{
//				 red = 0.75;
//				 green = 0.66;
//				 blue = 0;
//			}
//
//			else if ((18.5*b/20)<a  && a <= b)
//			{
//				red = 0.25;
//				green = .66;
//				blue = 1;
//			}
//			Quantity_Color color;
//			color.SetValues(red,green,blue, Quantity_TOC_RGB);
//			return color;
//			break;
//		}//end case 1
//
//		case 2 :
//		{
//			gp_Pnt P (85.,0.,-105.);
//			gp_Vec TheVect ( P, thePoint);
//			Standard_Real CoordX;
//			Standard_Real CoordY;
//			Standard_Real CoordZ;
//
//			CoordX = TheVect.X();
//			CoordY = TheVect.Y();
//			CoordZ = TheVect.Z();
///*
//			Standard_Real maxixy = Max(fabs(CoordX),fabs(CoordY));
//			Standard_Real maxiyz = Max(fabs(CoordY),fabs(CoordZ));
//			Standard_Real Maxi = Max(maxixy,maxiyz);
//
//*/
//			Standard_Real Distance = BAR.Distance ( P);
//
//			//red = (abs(CoordX))/(1*Distance) ;
//			//green =(abs(CoordY))/(1*Distance)	;
//			//blue = (abs(CoordZ))/(1*Distance)	;
//
//			Standard_Real a =fabs(CoordX);
//			Standard_Real b =fabs(CoordY);
//			Standard_Real c =fabs(CoordZ);
//
//Standard_Real xx = a / Max(Distance,a); //(Max (Distance, Maxi));
//Standard_Real yy = b / Max(Distance,b); //(Max (Distance, Maxi));	
//Standard_Real zz = c / Max(Distance,c); //(Max (Distance, Maxi));			
//			
//	
//			if (myXRedOnOff)
//			red = xx;
//			else if (myXGreenOnOff)
//			green =xx;
//			else if (myXBlueOnOff)
//			blue=xx;
//
//			if (myYRedOnOff)
//			red = yy;
//			else if (myYGreenOnOff)
//			green = yy;
//			else if (myYBlueOnOff)
//			blue = yy;
//			
//			if (myZRedOnOff)
//			red = zz;
//			else if (myZGreenOnOff)
//			green = zz;
//			else if (myZBlueOnOff)
//			blue = zz;
//
//		/*	if (myX1OnOff)
//			if (myY1OnOff)
//			if (myZ1OnOff)*/
//
//
//			Quantity_Color color;
//			color.SetValues(red,green,blue, Quantity_TOC_RGB);
//			return color;
//			break;
//		}//end case 2
//	}//end switch
//
//	Quantity_Color c;
//	return c;
//}

//void User_AIS::SetColor(const Quantity_Color &aColor)
//{
//	AIS_InteractiveObject::SetColor(aColor);
//	SetPlanarFaceColor(aColor);
//	SetCylindricalFaceColor(aColor);
//}
