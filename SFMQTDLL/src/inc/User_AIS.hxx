#ifndef _User_AIS_HeaderFile
	#define _User_AIS_HeaderFile
	#ifndef _Standard_Macro_HeaderFile
	#include <Standard_Macro.hxx>
#endif

#include <AIS_InteractiveObject.hxx>
#include <Standard_DefineHandle.hxx>
#include <Standard_Macro.hxx>
#include <Graphic3d_AspectFillArea3d.hxx>
#include <AIS_Drawer.hxx>
#include <TopoDS_Shape.hxx>
#include <Geom_Surface.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <gp_Pnt.hxx>
#include <StdPrs_HLRPolyShape.hxx>
#include <StdPrs_ShadedShape.hxx>
#include <StdPrs_WFDeflectionShape.hxx>
#include <StdSelect_BRepSelectionTool.hxx>
#include <Geom_Plane.hxx>

// Handle definition
//
DEFINE_STANDARD_HANDLE(User_AIS,AIS_InteractiveObject)

class User_AIS: public AIS_InteractiveObject {
public:
	Standard_EXPORT User_AIS();
	Standard_EXPORT User_AIS(TopoDS_Shape theshape, Handle_AIS_InteractiveContext thectx);
	//Standard_EXPORT User_AIS(const gp_Ax2 CylAx2, const Standard_Real R, const Standard_Real H);

	~User_AIS(){};
	TopoDS_Shape myShape;
	Standard_Boolean AcceptShapeDecomposition() const;
	void Standard_EXPORT SetCylindricalFaceColor(const Quantity_Color acolor);
	void Standard_EXPORT SetPlanarFaceColor(const Quantity_Color);
	Standard_Integer NbPossibleSelection() const;
	//void SetColor(const Quantity_Color &aColor);
	void Set(const TopoDS_Shape theshape) {myShape = theshape;};
	TopoDS_Shape Shape() { return myShape;}

private:
	void Compute(const Handle_PrsMgr_PresentationManager3d& aPresentationManager,
				 const Handle_Prs3d_Presentation& aPresentation,
				 const Standard_Integer aMode = 0) ;
	void ComputeSelection(const Handle_SelectMgr_Selection& aSelection,
						  const Standard_Integer aMode) ;
	void Compute(const Handle_Prs3d_Projector& aProjector,const Handle_Prs3d_Presentation& aPresentation);
	Standard_Boolean TriangleIsValid(const gp_Pnt& P1,const gp_Pnt& P2,const gp_Pnt& P3) const;
	/*Quantity_Color Color() ;*/
						
// some methods like DynamicType() or IsKind()


	Handle_AIS_InteractiveContext      mycontext;
//
DEFINE_STANDARD_RTTI(User_AIS)
private:
//	Quantity_NameOfColor myCylindricalFaceColor;
//	Quantity_NameOfColor myPlanarFaceColor;
	Quantity_Color myCylindricalFaceColor;
	Quantity_Color myPlanarFaceColor;
	

	Handle_Graphic3d_AspectFillArea3d myAspect;
	Standard_Real myDeflection;

	Standard_Boolean myX1OnOff;
	Standard_Boolean myXBlueOnOff;
	Standard_Boolean myXGreenOnOff;
	Standard_Boolean myXRedOnOff;
	Standard_Boolean myY1OnOff;
	Standard_Boolean myYBlueOnOff;
	Standard_Boolean myYGreenOnOff;
	Standard_Boolean myYRedOnOff;
	Standard_Boolean myZ1OnOff;
	Standard_Boolean myZBlueOnOff;
	Standard_Boolean myZGreenOnOff;
	Standard_Boolean myZRedOnOff;

	gp_Pnt BAR;

	Standard_Real Umin;
	Standard_Real Umax;
	Standard_Real Vmin;
	Standard_Real Vmax;
	Standard_Real dUmax;
	Standard_Real dVmax;

	Standard_Real red;
	Standard_Real green;
	Standard_Real blue;



//Handle_Graphic3d_Texture2Dmanual mytexture;

};
#endif