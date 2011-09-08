// AIS_Text.cpp: implementation of the AIS_Text class.
//
//////////////////////////////////////////////////////////////////////

#include "AIS_Text.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif
IMPLEMENT_STANDARD_HANDLE(AIS_Text,AIS_InteractiveObject)
IMPLEMENT_STANDARD_RTTI(AIS_Text,AIS_InteractiveObject)
//
// Foreach ancestors, we add a IMPLEMENT_STANDARD_SUPERTYPE and 
// a IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY macro.
// We must respect the order: from the direct ancestor class
// to the base class.
//
IMPLEMENT_STANDARD_TYPE(AIS_Text)
        IMPLEMENT_STANDARD_SUPERTYPE(AIS_InteractiveObject)
        IMPLEMENT_STANDARD_SUPERTYPE(SelectMgr_SelectableObject)
        IMPLEMENT_STANDARD_SUPERTYPE(PrsMgr_PresentableObject)
        IMPLEMENT_STANDARD_SUPERTYPE(MMgt_TShared)
        IMPLEMENT_STANDARD_SUPERTYPE(Standard_Transient)
        IMPLEMENT_STANDARD_SUPERTYPE_ARRAY()
                IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY(AIS_InteractiveObject)
                IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY(SelectMgr_SelectableObject)
                IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY(PrsMgr_PresentableObject)
                IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY(MMgt_TShared)
                IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY(Standard_Transient)
        IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_END()
IMPLEMENT_STANDARD_TYPE_END(AIS_Text)


#include <Graphic2d_Text.hxx>
#include <Select2D_SensitiveBox.hxx>
#include <Graphic2d_Segment.hxx>
#include <OSD_Environment.hxx>
#include <Graphic2d_View.hxx>
#include <Graphic2d_Drawer.hxx>
#include "PrsMgr_PresentationManager2d.hxx"
#include "SelectMgr_Selection.hxx"
#include "Graphic2d_Array1OfVertex.hxx"
#include "Graphic2d_Polyline.hxx"
#include "Graphic2d_Vertex.hxx"
#include "Graphic2d_DisplayList.hxx"
#include "Prs3d_Text.hxx"
#include <Prs3d_LengthPresentation.hxx> 
#include "Prs3d_Presentation.hxx"
#include <Prs3d_AnglePresentation.hxx> 



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AIS_Text::AIS_Text()
{
SetHilightMode(1);
}


AIS_Text::AIS_Text
                 (const TCollection_AsciiString& aText, 
                  const Standard_Real            anX ,        // = 0
                  const Standard_Real            anY ,        // = 0
                  const Standard_Real            aZ  ,        // = 0
			      const Aspect_TypeOfText        aType,       // = SOLID,
			      const Quantity_PlaneAngle      anAngle,     // = 0.0
			      const Standard_Real            aslant,      // = 0.0
			      const Standard_Integer         aColorIndex, // = 0
			      const Standard_Integer         aFontIndex,  // = 1
			      const Quantity_Factor          aScale)      // = 1
                  :AIS_InteractiveObject(),MyText(aText),MyX(anX),MyY(anY),MyZ(aZ),
                  MyTypeOfText(aType),MyAngle(anAngle),MySlant(aslant),MyFontIndex(aFontIndex),
                  MyColorIndex(aColorIndex),MyScale(aScale),MyWidth(0),MyHeight(0)
{
	SetHilightMode(1);
}

AIS_Text::AIS_Text
                 (const TCollection_AsciiString& aText, 
                  gp_Pnt&                        aPoint,
			      const Aspect_TypeOfText        aType,       // = SOLID,
			      const Quantity_PlaneAngle      anAngle,     // = 0.0
			      const Standard_Real            aslant,      // = 0.0
			      const Standard_Integer         aColorIndex, // = 0
			      const Standard_Integer         aFontIndex,  // = 1
			      const Quantity_Factor          aScale)      // = 1
                  :AIS_InteractiveObject(),MyText(aText),MyX(aPoint.X()),MyY(aPoint.Y()),MyZ(aPoint.Z()),
                  MyTypeOfText(aType),MyAngle(anAngle),MySlant(aslant),MyFontIndex(aFontIndex),
                  MyColorIndex(aColorIndex),MyScale(aScale),MyWidth(0),MyHeight(0)
{
SetHilightMode(1);

}



AIS_Text::~AIS_Text()
{

}

void AIS_Text::Compute(const Handle(PrsMgr_PresentationManager3d)& aPresentationManager,
                             const Handle(Prs3d_Presentation)& aPresentation,
                             const Standard_Integer aMode)
{


	
	aPresentation->Color(Quantity_NameOfColor::Quantity_NOC_BLACK);
	Prs3d_Text::Draw(aPresentation,myDrawer,MyText,gp_Pnt(  MyX ,MyY,MyZ ));
	int textwidth = MyText.Length() * 5;

	//aPresentation->BoundBox();
	//aPresentation->SetPickable();
	
	  //Graphic3d_Array1OfVertex V(1,5);

	  //V(1).SetCoord(MyX-(textwidth/2) ,MyY,MyZ +10 );
	  //V(2).SetCoord(MyX+(textwidth/2),MyY,MyZ +10);
	  //V(3).SetCoord(MyX+(textwidth/2) ,MyY,MyZ -10);
	  //V(4).SetCoord(MyX -(textwidth/2) ,MyY,MyZ -10 );
	  //V(5).SetCoord(MyX-(textwidth/2) ,MyY,MyZ +10);


	//Handle_Graphic3d_AspectFillArea3d myfillaspect = new Graphic3d_AspectFillArea3d();
	//myfillaspect->SetBackInteriorColor(Quantity_Color(Quantity_NameOfColor::Quantity_NOC_BLACK));

	 // Prs3d_Root::NewGroup(aPresentation);
	//Prs3d_Root::CurrentGroup(aPresentation)->SetGroupPrimitivesAspect(myfillaspect);
	//Prs3d_Root::CurrentGroup(aPresentation)->Polyline(V);

	//Handle(Graphic2d_CircleMarker) rc1 = new Graphic2d_CircleMarker(aGrObj, 0.04, 0.03, 0.0, 0.0, 0.01);

	




		
	

	//Prs3d_LengthPresentation::Draw(aPresentation,myDrawer," ",gp_Pnt(  0,0,0),gp_Pnt(  MyX ,MyY,MyZ ),gp_Pnt(  MyX ,MyY,MyZ ) );
	//Prs3d_AnglePresentation::Draw(aPresentation,myDrawer,"a:",gp_Pnt(  0,0,0),gp_Pnt(  MyX/2 ,MyY/2,0 ), gp_Pnt(  MyX ,MyY,MyZ ),gp_Pnt(  MyX/2 ,MyY/2,MyZ/2 ));
	//
}

void AIS_Text::Compute(const Handle(Prs3d_Projector)& aProjector,
                             const Handle(Prs3d_Presentation)& aPresentation) 
 {
 }

void AIS_Text::Compute(const Handle(PrsMgr_PresentationManager2d)& aPresentationManager, 
                         const Handle(Graphic2d_GraphicObject)& aGrObj, 
                         const Standard_Integer unMode)
{

		Handle(Graphic2d_Text) text;


		text = new Graphic2d_Text(aGrObj, MyText, MyX, MyY, MyAngle,MyTypeOfText,MyScale);
		
		
		Quantity_Length anXoffset,anYoffset;

	switch(unMode)
	{

	case 0:
	  
	  
	  text->SetFontIndex(MyFontIndex);
	 
	  text->SetColorIndex(MyColorIndex);
	  //text->SetColorIndex(4);
	 
	  text->SetSlant(MySlant);
	  text->SetUnderline(Standard_False);
	  text->SetZoomable(Standard_True);
	  aGrObj->Display();
	 
	  text->TextSize(MyWidth, MyHeight,anXoffset,anYoffset);
  break;
  	case 1:
	  
	  
	  text->SetFontIndex(MyFontIndex);
	 
	  text->SetColorIndex(MyColorIndex);
	  
	 
	  text->SetSlant(MySlant);
	  text->SetUnderline(Standard_False);
	  text->SetZoomable(Standard_True);
	  aGrObj->Display();
	  MyWidth *=1.1;
      MyHeight *=1.1;
	  text->TextSize(MyWidth, MyHeight,anXoffset,anYoffset);
  break;

	}

}

void AIS_Text::ComputeSelection(const Handle(SelectMgr_Selection)& aSelection, 
				      const Standard_Integer unMode)
{

}

