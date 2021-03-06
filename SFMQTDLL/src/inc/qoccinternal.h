
#ifndef QOCCINTERNAL_H
#define QOCCINTERNAL_H

#include <AIS_Drawer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>
#include <AIS_ListOfInteractive.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>
#include <AIS_Shape.hxx>
#include <AIS_SequenceOfInteractive.hxx>
#include <AIS_Trihedron.hxx>

#include <Aspect_Background.hxx>
#include <Aspect_ColorMapEntry.hxx>
#include <Aspect_GenericColorMap.hxx>
#include <Aspect_GridDrawMode.hxx>
#include <Aspect_GridType.hxx>
#include <Aspect_LineStyle.hxx>
#include <Aspect_MarkMap.hxx>
#include <Aspect_RectangularGrid.hxx>
#include <Aspect_TypeMap.hxx>
#include <Aspect_TypeMapEntry.hxx>
#include <Aspect_TypeOfLine.hxx>
#include <Aspect_TypeOfText.hxx>
#include <Aspect_WidthMapEntry.hxx>
#include <Aspect_WidthOfLine.hxx>
#include <Aspect_Window.hxx>

#include <Bnd_Box2d.hxx>
#include <BndLib_Add2dCurve.hxx>
#include <BRep_Builder.hxx>
#include <BRep_Tool.hxx>

#include <BRepBuilderAPI.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>

#include <BRepAlgo.hxx>

#include <BRepTools.hxx>
#include <Standard_DefineHandle.hxx>
#include <DsgPrs_LengthPresentation.hxx>
#include <GCE2d_MakeSegment.hxx>
#include <GCPnts_TangentialDeflection.hxx>
#include <Geom_Axis2Placement.hxx>
#include <Geom_CartesianPoint.hxx>
#include <Geom_Line.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>
#include <Geom2d_BezierCurve.hxx>
#include <Geom2d_BSplineCurve.hxx>
#include <Geom2d_Curve.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <Geom2dAdaptor_Curve.hxx>
#include <GeomAbs_CurveType.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <GeomTools_Curve2dSet.hxx>
#include <Geom_SurfaceOfRevolution.hxx>
#include <BRepBuilderAPI_MakeShell.hxx> 
#include <BRepPrimAPI_MakeRevol.hxx>

#include <IntAna_IntConicQuad.hxx>

#include <gp_Ax2d.hxx>
#include <gp_Circ2d.hxx>
#include <gp_Dir2d.hxx>
#include <gp_Lin2d.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Vec.hxx>
#include <gp_Vec2d.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pln.hxx>
#include <gp_Lin.hxx>
#include <gp_Dir.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <GC_MakeEllipse.hxx>

#include <Geom2d_Ellipse.hxx>
//#include <Graphic2d_Array1OfVertex.hxx>
//#include <Graphic2d_Buffer.hxx>
//#include <Graphic2d_CircleMarker.hxx>
//#include <Graphic2d_DisplayList.hxx>
//#include <Graphic2d_Drawer.hxx>
//#include <Graphic2d_Polyline.hxx>
//#include <Graphic2d_Segment.hxx>
//#include <GGraphic2d_SetOfCurves.hxx>
//#include <Graphic2d_SetOfSegments.hxx>
//#include <Graphic2d_Text.hxx>
//#include <Graphic2d_Vertex.hxx>
//#include <Graphic2d_View.hxx>
#include <Graphic3d_AspectMarker3d.hxx>
#include <Graphic3d_NameOfMaterial.hxx>
#include <MMgt_TShared.hxx>
#include <OSD_Environment.hxx>
#include <Precision.hxx>
#include <Prs3d_IsoAspect.hxx>
#include <Prs3d_LineAspect.hxx>
#include <Prs3d_Projector.hxx>
#include <Prs3d_Text.hxx>
//#include <PrsMgr_PresentationManager2d.hxx>
#include <Quantity_Factor.hxx>
#include <Quantity_Length.hxx>
#include <Quantity_NameOfColor.hxx>
#include <Quantity_PhysicalQuantity.hxx>
#include <Quantity_PlaneAngle.hxx>
#include <Quantity_TypeOfColor.hxx>
//#include <Select2D_SensitiveArc.hxx>
//#include <Select2D_SensitiveBox.hxx>
//#include <Select2D_SensitiveEntity.hxx>
//#include <Select2D_SensitiveSegment.hxx>
#include <SelectBasics_BasicTool.hxx>
#include <SelectBasics_ListOfBox2d.hxx>
#include <SelectMgr_EntityOwner.hxx>
#include <SelectMgr_SelectableObject.hxx>
#include <SelectMgr_Selection.hxx>
#include <SelectMgr_SelectionManager.hxx>
#include <ShapeSchema.hxx>
#include <Standard_Boolean.hxx>
#include <Standard_CString.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Integer.hxx>
#include <Standard_IStream.hxx>
#include <Standard_Macro.hxx>
#include <Standard_NotImplemented.hxx>
#include <Standard_OStream.hxx>
#include <Standard_Real.hxx>
#include <StdPrs_Curve.hxx>
#include <StdPrs_Point.hxx>
#include <StdPrs_PoleCurve.hxx>
//#include <StdSelect_SensitiveText2d.hxx>
//#include <StdSelect_TextProjector2d.hxx>
//#include <StdSelect_ViewerSelector2d.hxx>
#include <TCollection_AsciiString.hxx>
//#include <TColgp_Array1OfPnt2d.hxx>
//#include <TColgp_HArray1OfPnt2d.hxx>
#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TColStd_HSequenceOfTransient.hxx>
#include <TColStd_MapIteratorOfMapOfTransient.hxx>
#include <TColStd_MapOfTransient.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_ListIteratorOfListOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>




#include <TopExp.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopTools_HSequenceOfShape.hxx>

#include <TPrsStd_AISViewer.hxx>

#include <UnitsAPI.hxx>
//#include <V2d_View.hxx>
//#include <V2d_Viewer.hxx>
#include <V3d_ColorScale.hxx>
#include <V3d_RectangularGrid.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include <Visual3d_View.hxx>
#include <Visual3d_ViewMapping.hxx>
#include <Visual3d_ViewManager.hxx>

// specific to ISession2D_Shape
#include <TopTools_ListOfShape.hxx>
#include <HLRAlgo_Projector.hxx>
#include <HLRBRep_Algo.hxx>
#include <HLRBRep_PolyAlgo.hxx>
#include <HLRBRep_PolyHLRToShape.hxx>
#include <HLRBRep_HLRToShape.hxx>
#include <HLRAlgo_Projector.hxx>

// specific IGES, STEP or DXF
#include <XSControl_WorkSession.hxx>
#include <STEPControl_StepModelType.hxx>
#include <IFSelect_ReturnStatus.hxx>

// specific IGES STEP
//#include <Interface_DT.hxx>
// specific IGES, STEP or DXF
#include <Interface_Static.hxx>

// specific STEP
#include <STEPControl_StepModelType.hxx>
#include <STEPControl_Controller.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>

// specific IGES
#include <Interface_InterfaceModel.hxx>
#include <IGESControl_Controller.hxx>
#include <IGESControl_Writer.hxx>
#include <IGESControl_Reader.hxx>
#include <IGESToBRep_Actor.hxx>
#include <IGESToBRep_Reader.hxx>

// specific CSFDB
#include <FSD_File.hxx>
#include <MgtBRep.hxx>
#include <PTColStd_PersistentTransientMap.hxx>
#include <PTColStd_TransientPersistentMap.hxx>
#include <PTopoDS_HShape.hxx>
#include <Storage_Data.hxx>
#include <Storage_Error.hxx>
#include <Storage_HSeqOfRoot.hxx>
#include <Storage_Root.hxx>

// Visual AIS

#include <User_AIS.hxx>

// tdf

#include <TDF_LabelSequence.hxx>


// specific STL
#include <StlAPI_Writer.hxx>

// specific VRML
#include <VrmlAPI_Writer.hxx>

// The OpenCASCADE includes...
#include <AIS_InteractiveContext.hxx>
#ifdef WNT
//#include <WNT_WDriver.hxx>
#include <WNT_Window.hxx>
//#include <Graphic3d_WNTGraphicDevice.hxx>
#endif

#endif // QOCCINTERNAL_H

