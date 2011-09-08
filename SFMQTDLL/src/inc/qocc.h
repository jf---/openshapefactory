

#ifndef SGM_H
#define SGM_H

// OpenCASCADE includes

#include <AIS_StatusOfDetection.hxx>
#include <AIS_StatusOfPick.hxx>
#include <Aspect_Drawable.hxx>
#include <Aspect_GridDrawMode.hxx>
#include <Aspect_GridType.hxx>
#include <Aspect_GraphicCallbackProc.hxx>
#include <AIS_InteractiveContext.hxx>
#include <NIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include <Handle_TopTools_HSequenceOfShape.hxx>
#include <Standard_TypeDef.hxx>
#include <Quantity_Factor.hxx>
#include <Quantity_Length.hxx>
#include <Quantity_NameOfColor.hxx>
#include <V3d_Coordinate.hxx>
#include <Handle_AIS_Shape.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Pln.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TColStd_SequenceOfExtendedString.hxx>
#include <TDocStd_Document.hxx>


#ifdef WNT
#include <Handle_WNT_Window.hxx>
#else
#include <Handle_Xw_Window.hxx>
#endif

//#define SIGN(X) ((X) < 0. ? -1 : ((X) > 0. ? 1 : 0.))
//#define INITIAL_PRECISION 0.001



#endif // SGM_H

