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
#include <Graphic3d_Array1OfVertexNC.hxx>
#include <Aspect_Array1OfEdge.hxx>
#include <Quantity_Color.hxx>
#include <BRepBuilderAPI_NurbsConvert.hxx>
#include <Prs3d_LineAspect.hxx>
#include <Prs3d_PointAspect.hxx>
#include <BRepTools.hxx>
#include <AIS_GraphicTool.hxx>
#include <Graphic3d_Group.hxx>
#include "Prs3d_Presentation.hxx"
#include <TopAbs.hxx>

// Constructors implementation
//

User_AIS::User_AIS(TopoDS_Shape theshape, Handle_AIS_InteractiveContext thectx) :
AIS_InteractiveObject(PrsMgr_TOP_ProjectorDependant)
{
	
	myShape = theshape;
	SetHilightMode(0);
	
	SetSelectionMode(0);
	myDrawer->SetShadingAspect(new Prs3d_ShadingAspect());

	myPlanarFaceColor = Quantity_NOC_FIREBRICK3;
	myCylindricalFaceColor = Quantity_NOC_GRAY;
	mycontext = thectx;
	myDrawer->WireAspect()->SetColor(Quantity_NOC_BLACK);
	myDrawer->PointAspect()->SetScale(2);
	


	


				this->SetDisplayMode(3);




}



void User_AIS::Compute(const Handle_PrsMgr_PresentationManager3d& aPresentationManager,
							const Handle_Prs3d_Presentation& aPresentation,
							const Standard_Integer aMode ) 
{
	Quantity_NameOfColor aColor;
	Standard_Real aTransparency = Transparency();
	Graphic3d_NameOfMaterial aMaterial = Material();
	TopExp_Explorer Ex;
		
						
	switch (aMode) {
	case 0:
		aColor = AIS_InteractiveObject::Color();
		SetColor(aColor);
		//myDrawer->WireAspect()->SetColor (Quantity_NameOfColor::Quantity_NOC_WHITE);
		StdPrs_WFDeflectionShape::Add(aPresentation,myShape, myDrawer );
		break;
	case 1:
			
				
					myDrawer->SetShadingAspectGlobal(Standard_False);
					myDrawer->WireAspect()->SetColor(myOwnColor);
					myDrawer->WireAspect()->SetWidth(2);
					myDrawer->PointAspect()->SetTypeOfMarker(Aspect_TypeOfMarker::Aspect_TOM_O);
					StdPrs_WFDeflectionShape::Add(aPresentation,myShape, myDrawer );




	

	  break;

	  case 2:
			
				

				myDrawer->SetShadingAspectGlobal(Standard_False);
				//TopExp_Explorer Ex;
				//Handle(Geom_Surface) Surface; 
		
				/*for (Ex.Init(myShape,TopAbs_FACE); Ex.More(); Ex.Next())
				{
					*/
					//Surface = BRep_Tool::Surface(TopoDS::Face(Ex.Current()));
				    //mycontext->SetTransparency(this,0);
					myDrawer->ShadingAspect()->SetMaterial(aMaterial);	
					myDrawer->ShadingAspect()->SetTransparency (aTransparency);
					StdPrs_ShadedShape::Add(aPresentation,myShape, myDrawer);
					myDrawer->PointAspect()->SetTypeOfMarker(Aspect_TypeOfMarker::Aspect_TOM_O_X );				

					

				//}

				

				for (Ex.Init(myShape,TopAbs_WIRE); Ex.More(); Ex.Next())
					{
						
								
						//myDrawer->WireAspect()->SetColor(Quantity_NOC_BLACK);
						myDrawer->WireAspect()->SetColor(myOwnColor);
						myDrawer->WireAspect()->SetWidth(2);	
						myDrawer->PointAspect()->SetTypeOfMarker		(Aspect_TypeOfMarker::Aspect_TOM_O_X );
						StdPrs_WFDeflectionShape::Add(aPresentation,Ex.Current(), myDrawer );

						//StdPrs_WFDeflectionShape::Add(aPresentation,myShape, myDrawer );


					}
				

	  break;

	  case 3:


				

				


					myDrawer->SetShadingAspectGlobal(Standard_False);
								

					myDrawer->SetShadingAspectGlobal(Standard_False);
					myDrawer->WireAspect()->SetColor(myOwnColor);
					//myDrawer->WireAspect()->SetColor(Quantity_NOC_BLACK);
					myDrawer->WireAspect()->SetWidth(2);
					myDrawer->PointAspect()->SetTypeOfMarker(Aspect_TypeOfMarker::Aspect_TOM_O_X );
					StdPrs_ShadedShape::Add(aPresentation,myShape, myDrawer);
					
					if (myShape.IsNull()) return;
					if (myShape.ShapeType() == TopAbs_FACE || 
						myShape.ShapeType() == TopAbs_SHELL || 
						myShape.ShapeType() == TopAbs_SOLID ||
						myShape.ShapeType() == TopAbs_COMPOUND)
						{
							
							for (Ex.Init(myShape,TopAbs_WIRE); Ex.More(); Ex.Next())
							{
								
										
								//myDrawer->WireAspect()->SetColor(Quantity_NOC_BLACK);
							    myDrawer->WireAspect()->SetColor(myOwnColor);
								myDrawer->WireAspect()->SetWidth(2);
								myDrawer->PointAspect()->SetTypeOfMarker(Aspect_TypeOfMarker::Aspect_TOM_O_X );
								StdPrs_WFDeflectionShape::Add(aPresentation,Ex.Current(), myDrawer );
								//StdPrs_WFDeflectionShape::Add(aPresentation,myShape, myDrawer );


							}
						}
					else
						{
						myDrawer->PointAspect()->SetTypeOfMarker(Aspect_TypeOfMarker::Aspect_TOM_O_X );
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

void User_AIS::ComputeSelection(const Handle_SelectMgr_Selection& aSelection,
									 const Standard_Integer aMode)
{
	switch(aMode){
	case 0:
		StdSelect_BRepSelectionTool::Load(aSelection,myShape,TopAbs_SHAPE,0.01,5);
		break;
	case 1:
		StdSelect_BRepSelectionTool::Load(aSelection,myShape,TopAbs_VERTEX,0.01,5);
		break;
	case 2:
		StdSelect_BRepSelectionTool::Load(aSelection,myShape,TopAbs_EDGE,0.01,5);
		break;
	case 3:
		StdSelect_BRepSelectionTool::Load(aSelection,myShape,TopAbs_WIRE,0.01,5);
		break;
	case 4:
		StdSelect_BRepSelectionTool::Load(aSelection,myShape,TopAbs_FACE,0.01,5);
		break;
	}
}

Standard_Integer User_AIS::NbPossibleSelection() const
{
	return 5;
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

Quantity_Color User_AIS::Color(gp_Pnt& thePoint,Standard_Real AltMin,Standard_Real AltMax,
									const Standard_Integer ColorizationMode) 
{
	red =1;   //initializing colors parameters
	green=1;
	blue =1;
	switch ( ColorizationMode)
	{
		case 0 : //normal, vert/maron
		{
			Standard_Real Alt= thePoint.Z();

			Standard_Real AltDelta;

			AltDelta = AltMax-AltMin;

			red = 0.5- ((0.5*(AltMax-Alt))/(AltDelta));
		//	red = 1/(((1000/(AltMax-AltMin))*Alt)+1000*(1-(AltMin/(AltMax-AltMin))));
		
		//	green = (3*AltMax-AltMin)/(3*AltMax-8*AltMin + 7*Alt);
		//	green = 1/(((7/(3*AltMax-AltMin))*Alt) + 1-(7*AltMin/(3*AltMax-AltMin)));
			Standard_Real A = 7*Alt-7*AltMin;
			green = (3*AltMax-AltMin)/(3*AltMax-AltMin+(7*Alt-7*AltMin));
//			AfxMessageBox(green);
		//	green =(0.30- (((0.3-1)*(AltMax-Alt))/(AltMax-AltMin)));
		
			blue = 0 ;
/*
			red = 0.5;
			green = 0.2222;
			blue = 0;
*/	
			Quantity_Color color;
			color.SetValues(red,green,blue, Quantity_TOC_RGB);
			return color;
			break;
		}//end case 0

		case 1 :	//mer-neige
		{		
			Standard_Real Alt= thePoint.Z();

			Standard_Real b =AltMax-AltMin;
			Standard_Real a= AltMax-thePoint.Z();

			red =1;
			green=1;
			blue =1;
			if (0<a && a <= (b/5))
			{
				red = 1;
				green = 1;
				blue = 1;
			}
			else if ((b/5)<a  && a <= (2*b/5))
			{
				 red = .55;
				 green = 0.3;
				 blue = 0;
			}
			else if ((2*b/5)<a  && a <= (18*b/20))
			{
				 green =1/(((7/(3*AltMax-AltMin))*Alt)+(1-(7*AltMin/(3*AltMax-AltMin))));
				 red = 1/(((1000/(AltMax-AltMin))*Alt)+1000*(1-(AltMin/(AltMax-AltMin))));
				 blue = 0;
			}
			else if ((18*b/20)<a  && a <= (18.5*b/20))
			{
				 red = 0.75;
				 green = 0.66;
				 blue = 0;
			}

			else if ((18.5*b/20)<a  && a <= b)
			{
				red = 0.25;
				green = .66;
				blue = 1;
			}
			Quantity_Color color;
			color.SetValues(red,green,blue, Quantity_TOC_RGB);
			return color;
			break;
		}//end case 1

		case 2 :
		{
			gp_Pnt P (85.,0.,-105.);
			gp_Vec TheVect ( P, thePoint);
			Standard_Real CoordX;
			Standard_Real CoordY;
			Standard_Real CoordZ;

			CoordX = TheVect.X();
			CoordY = TheVect.Y();
			CoordZ = TheVect.Z();
/*
			Standard_Real maxixy = Max(fabs(CoordX),fabs(CoordY));
			Standard_Real maxiyz = Max(fabs(CoordY),fabs(CoordZ));
			Standard_Real Maxi = Max(maxixy,maxiyz);

*/
			Standard_Real Distance = BAR.Distance ( P);

			//red = (abs(CoordX))/(1*Distance) ;
			//green =(abs(CoordY))/(1*Distance)	;
			//blue = (abs(CoordZ))/(1*Distance)	;

			Standard_Real a =fabs(CoordX);
			Standard_Real b =fabs(CoordY);
			Standard_Real c =fabs(CoordZ);

Standard_Real xx = a / Max(Distance,a); //(Max (Distance, Maxi));
Standard_Real yy = b / Max(Distance,b); //(Max (Distance, Maxi));	
Standard_Real zz = c / Max(Distance,c); //(Max (Distance, Maxi));			
			
	
			if (myXRedOnOff)
			red = xx;
			else if (myXGreenOnOff)
			green =xx;
			else if (myXBlueOnOff)
			blue=xx;

			if (myYRedOnOff)
			red = yy;
			else if (myYGreenOnOff)
			green = yy;
			else if (myYBlueOnOff)
			blue = yy;
			
			if (myZRedOnOff)
			red = zz;
			else if (myZGreenOnOff)
			green = zz;
			else if (myZBlueOnOff)
			blue = zz;

		/*	if (myX1OnOff)
			if (myY1OnOff)
			if (myZ1OnOff)*/


			Quantity_Color color;
			color.SetValues(red,green,blue, Quantity_TOC_RGB);
			return color;
			break;
		}//end case 2
	}//end switch

	Quantity_Color c;
	return c;
}

void User_AIS::SetColor(const Quantity_Color &aColor)
{
	AIS_InteractiveObject::SetColor(aColor);
	SetPlanarFaceColor(aColor);
	SetCylindricalFaceColor(aColor);
}
