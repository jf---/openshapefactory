// AIS_PointCloud.h: interface for the AIS_PointCloud class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AIS_PointCloud_H__A9B277C4_A69E_11D1_8DA4_0800369C8A03__INCLUDED_)
#define AFX_AIS_PointCloud_H__A9B277C4_A69E_11D1_8DA4_0800369C8A03__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include <Standard_Macro.hxx>
#include <Standard_DefineHandle.hxx>

#include <TCollection_AsciiString.hxx>
#include <Aspect_TypeOfText.hxx>
#include <Standard_Real.hxx>
#include <Standard_Integer.hxx>
#include <Quantity_Factor.hxx>
#include <Quantity_PlaneAngle.hxx>
#include <Handle_PrsMgr_PresentationManager2d.hxx>
#include <Handle_Graphic2d_GraphicObject.hxx>
#include <Handle_SelectMgr_Selection.hxx>
#include <Standard_OStream.hxx>
#include <Standard_IStream.hxx>
#include <Standard_CString.hxx>
#include <SelectMgr_SelectableObject.hxx>
#include <Graphic2d_Text.hxx>
#include <AIS_InteractiveObject.hxx>
#include <Graphic2d_PolylineMarker.hxx>
#include <Graphic3d_Group.hxx>
//#include <Graphic2d_Group.hxx>
#include <Graphic2d_Array1OfVertex.hxx>
#include <Graphic3d_Array1OfVertex.hxx>
#include <QList>
#include <gp_Pnt.hxx>
#include <Graphic3d_ArrayOfPoints.hxx>




class TCollection_AsciiString;
class PrsMgr_PresentationManager2d;
class Graphic2d_GraphicObject;
class SelectMgr_Selection;

DEFINE_STANDARD_HANDLE(AIS_PointCloud,AIS_InteractiveObject)
class AIS_PointCloud : public AIS_InteractiveObject  
{
public:
	//AIS_PointCloud(const Graphic3d_Array1OfVertex &points);
	AIS_PointCloud(QList<gp_Pnt> vertlist);
	

    virtual ~AIS_PointCloud();



DEFINE_STANDARD_RTTI(AIS_PointCloud)


protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 

void Compute          (const Handle(PrsMgr_PresentationManager3d)& aPresentationManager,
                       const Handle(Prs3d_Presentation)& aPresentation,
                       const Standard_Integer aMode);
void Compute          (const Handle(Prs3d_Projector)& aProjector,
                       const Handle(Prs3d_Presentation)& aPresentation);
void Compute          (const Handle(PrsMgr_PresentationManager2d)& aPresentationManager,
                       const Handle(Graphic2d_GraphicObject)& aGrObj,
                       const Standard_Integer unMode = 0) ;
void ComputeSelection (const Handle(SelectMgr_Selection)& aSelection,
                       const Standard_Integer unMode) ;


 // Fields PRIVATE
 //
//const Graphic3d_Array1OfVertex &cloud_data;
QList<gp_Pnt> vertlist;


};



#endif // !defined(AFX_AIS_PointCloud_H__A9B277C4_A69E_11D1_8DA4_0800369C8A03__INCLUDED_)
