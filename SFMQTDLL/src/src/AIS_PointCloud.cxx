// AIS_PointCloud.cpp: implementation of the AIS_PointCloud class.
//
//////////////////////////////////////////////////////////////////////


//// following the code on this link:
//http://www.opencascade.org/org/forum/thread_1125/


#include "AIS_PointCloud.hxx"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif
IMPLEMENT_STANDARD_HANDLE(AIS_PointCloud,AIS_InteractiveObject)
IMPLEMENT_STANDARD_RTTI(AIS_PointCloud,AIS_InteractiveObject)
//
// Foreach ancestors, we add a IMPLEMENT_STANDARD_SUPERTYPE and 
// a IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY macro.
// We must respect the order: from the direct ancestor class
// to the base class.
//
IMPLEMENT_STANDARD_TYPE(AIS_PointCloud)
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
IMPLEMENT_STANDARD_TYPE_END(AIS_PointCloud)


//#include <Graphic2d_Text.hxx>
//#include <Select2D_SensitiveBox.hxx>
//#include <Graphic2d_Segment.hxx>
#include <OSD_Environment.hxx>
//#include <Graphic2d_View.hxx>
//#include <Graphic2d_Drawer.hxx>
//#include "PrsMgr_PresentationManager2d.hxx"
#include "SelectMgr_Selection.hxx"
#include <AIS_Drawer.hxx>
#include <Graphic3d_AspectMarker3d.hxx>
#include <Prs3d_PointAspect.hxx>
#include <Graphic3d_ArrayOfPrimitives.hxx>
#include <Aspect_AspectMarker.hxx>
#include <Graphic3d_ArrayOfPoints.hxx>




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//AIS_PointCloud::AIS_PointCloud(const Graphic3d_Array1OfVertex &points)
//:cloud_data(points)
//{
//
//}

AIS_PointCloud::AIS_PointCloud(QList<gp_Pnt> vlist)
:vertlist(vlist)
{




}




AIS_PointCloud::~AIS_PointCloud()
{

}

void AIS_PointCloud::Compute(const Handle(PrsMgr_PresentationManager3d)& aPresentationManager,
                             const Handle(Prs3d_Presentation)& aPresentation,
                             const Standard_Integer aMode)
{


	
Handle(Graphic3d_Structure) theStructure = Handle(Graphic3d_Structure)::DownCast(aPresentation);



Handle(Graphic3d_Group) theGroup= new Graphic3d_Group(theStructure);

double pcount =  vertlist.length();
Handle_Graphic3d_ArrayOfPoints thepointcloud2= new Graphic3d_ArrayOfPoints(pcount);

int index =0;



index =0;
for(int i=0 ; i < pcount ; i++)
	{
	index++;
	thepointcloud2->AddVertex(vertlist.at(i));
	//thepointcloud2->SetVertexColor(vertindx,colorlist.at(i));
	

	}


theGroup->AddPrimitiveArray(thepointcloud2);



}

void AIS_PointCloud::Compute(const Handle(Prs3d_Projector)& aProjector,
                             const Handle(Prs3d_Presentation)& aPresentation) 
 {
 }

//void AIS_PointCloud::Compute(const Handle(PrsMgr_PresentationManager2d)& aPresentationManager, 
//                         const Handle(Graphic2d_GraphicObject)& aGrObj, 
//                         const Standard_Integer unMode)
//{
//}

void AIS_PointCloud::ComputeSelection(const Handle(SelectMgr_Selection)& aSelection, 
				      const Standard_Integer unMode)
{

}

