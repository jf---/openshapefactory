#ifndef UTILITYFUNCTIONS_H
#define UTILITYFUNCTIONS_H

//gp_Pnt ConvertClickToPoint(Standard_Real x, Standard_Real y, Handle(V3d_View) aView)
//{
//     
//     V3d_Coordinate XEye,YEye,ZEye,XAt,YAt,ZAt;
//     aView->Eye(XEye,YEye,ZEye);
//     aView->At(XAt,YAt,ZAt);
//     gp_Pnt EyePoint(XEye,YEye,ZEye);
//     gp_Pnt AtPoint(XAt,YAt,ZAt);
//
//     gp_Vec EyeVector(EyePoint,AtPoint);
//     gp_Dir EyeDir(EyeVector);
//
//     gp_Pln PlaneOfTheView = gp_Pln(AtPoint,EyeDir);
//     Standard_Real X,Y,Z;
//     aView->Convert(x,y,X,Y,Z);
//     gp_Pnt ConvertedPoint(X,Y,Z);
//     gp_Pnt2d ConvertedPointOnPlane = ProjLib::Project(PlaneOfTheView,ConvertedPoint);
//     
//     gp_Pnt ResultPoint = ElSLib::Value(ConvertedPointOnPlane.X(),
//                                             ConvertedPointOnPlane.Y(),
//                                             PlaneOfTheView);
//     return ResultPoint;
//}






	//gp_Vec inverteAxis = majorvector.Normalized();
	//			inverteAxis.Multiply(mymajorradius * -1);
	//			

	//			gp_Vec positiveAxis = majorvector.Normalized();
	//			//positiveAxis.Multiply(mymajorradius);

	//			gp_Pnt invertedPoint = myStartPoint.Translated(inverteAxis);
	//			gp_Pnt PositivePoint = myStartPoint.Translated( positiveAxis);
	//			

	//			//qGeomApp->curvesOps( doc_id )->StartOperation();
	//			//Handle(GEOM_Object) myvector = qGeomApp->basicOps( doc_id )->MakeVectorDXDYDZ( myDirection.X(), 
	//			//														                     myDirection.Y(), 
	//			//														                     myDirection.Z() );

	//			//Handle(GEOM_Object) mycentre = qGeomApp->basicOps( doc_id )->MakePointXYZ( myStartPoint.X(), 
	//			//                                                                         myStartPoint.Y(), 
	//			//																		 myStartPoint.Z() );
	//			//Handle(GEOM_Object) myendpoint = qGeomApp->basicOps( doc_id )->MakePointXYZ( myCurrentPoint.X(), 
	//			//                                                                             myCurrentPoint.Y(), 
	//			//																		     myCurrentPoint.Z() );

	//			//Handle(GEOM_Object) p1 = qGeomApp->basicOps( doc_id )->MakePointXYZ( invertedPoint.X(), 
	//			//                                                                             invertedPoint.Y(), 
	//			//																		     invertedPoint.Z() );

	//			//Handle(GEOM_Object) p2 = qGeomApp->basicOps( doc_id )->MakePointXYZ( PositivePoint.X(), 
	//			//                                                                             PositivePoint.Y(), 
	//			//																		     PositivePoint.Z() );
	//			//
	//			//Handle(GEOM_Object) myMajorVector = qGeomApp->basicOps( doc_id )->MakeLineTwoPnt(p1,p2);
	//			//Handle(GEOM_Object) UPplane = qGeomApp->basicOps( doc_id )->MakePlane2Vec(myvector,myMajorVector,3000);


	//			
	//				
	//			//Handle(GEOM_Object) ellipse = qGeomApp->curvesOps( doc_id )->MakeEllipse(mycentre, myvector, mymajorradius,myminorradius,myMajorVector);
	//			
	//			const gp_Ax1 &axis1d = gp_Ax1(myStartPoint,gp_Dir(positiveAxis));
	//			const gp_Dir2d &mydir2d = gp_Dir2d(axis1d.Direction().X(),axis1d.Direction().Y());
	//			const gp_Pnt2d myp2d = gp_Pnt2d(myStartPoint.X(),myStartPoint.Y());
	//			gp_Ax2d &myaxis = gp_Ax2d();
	//			myaxis.SetDirection(mydir2d);
	//			myaxis.SetLocation(myp2d);

	//			Handle(Geom2d_Ellipse) anEllipse1 = new Geom2d_Ellipse(myaxis,mymajorradius,myminorradius);
	//			Handle(Geom2d_TrimmedCurve) aArc1 = new Geom2d_TrimmedCurve(anEllipse1 , 0.0 , PI);
	//			const Handle(Geom_Curve) &ArcCurve = Handle(Geom_Curve)::DownCast(aArc1);

	//			//Handle(Geom_SurfaceOfRevolution) myrevolve = new Geom_SurfaceOfRevolution(ArcCurve,axis1d);
	//			
	//			
	//			//BRepBuilderAPI_MakeShell myshellB = BRepBuilderAPI_MakeShell(myrevolve,0,PI,0.1,0.9,false);
	//			gp_Pln mysupport = gp_Pln(myStartPoint,gp_Dir(0,0,1));

	//		    BRepBuilderAPI_MakeEdge myEdge = BRepBuilderAPI_MakeEdge(ArcCurve,mysupport);
	//			const TopoDS_Shape &theShape = myEdge.Edge();
	//			Handle(User_AIS) aisEllipsoid = new User_AIS(theShape,vc->getContext());
	//            
	//			vc->getContext()->SetMaterial(aisEllipsoid,Graphic3d_NameOfMaterial::Graphic3d_NOM_PEWTER);
	//            vc->getContext()->SetColor(aisEllipsoid,Quantity_NameOfColor::Quantity_NOC_PURPLE4,Standard_True);
 //               vc->getContext()->SetDisplayMode(aisEllipsoid,2,Standard_False);
 //               vc->getContext()->Display(aisEllipsoid, Standard_False);


#include <BRep_Tool.hxx>
#include <BRepAlgoAPI_Fuse.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>

#include <BRepLib.hxx>

#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>

#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakePrism.hxx>

#include <GC_Root.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>

#include <GCE2d_MakeSegment.hxx>

#include <gp.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Dir.hxx>
#include <gp_Dir2d.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>

#include <Geom_CylindricalSurface.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GeomAPI_IntSS.hxx>

#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>

#include <TopExp_Explorer.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>

#include <TopTools_ListOfShape.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>

#include "qocc.h"
#include "qoccinternal.h"
#include <QString>
#include <QTextStream>
#include <QAxObject>
#include <QList>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>


bool GetBiggestSolid(TopoDS_Shape& solid);
double GetVolume(TopoDS_Shape shape);


bool GetBiggestSolid(TopoDS_Shape& solid)
{
     TopExp_Explorer solidExp;
     TopoDS_Shape     aSolid;
     double maxVolume = 0;
     double volume;
     for (solidExp.Init(solid,TopAbs_SOLID); solidExp.More(); solidExp.Next())
     {
          aSolid = solidExp.Current();
          volume = GetVolume(aSolid);
          if (maxVolume < volume)
          {
               maxVolume = volume;
               solid = aSolid;
          }          
     }
     return true;
}

double GetVolume(TopoDS_Shape shape)
{
     GProp_GProps System;                     
	 BRepGProp::VolumeProperties(shape, System);
     return System.Mass();     
}

gp_Pnt GetCOG(TopoDS_Shape shape)
{
     GProp_GProps System;                     
	 BRepGProp::SurfaceProperties (shape, System);
     return System.CentreOfMass();     
}

Handle(Geom_Curve) intersectsrf(const Handle(Geom_Surface)& S1,const Handle(Geom_Surface)& S2) 
{
//This class is instantiated as follows:
	GeomAPI_IntSS Intersector(S1, S2, Precision::Intersection());

//Once the GeomAPI_IntSS object has been created, it can be interpreted.
//Calling the number of intersection curves
Standard_Integer nb = Intersector. NbLines();

//Calling the intersection curves
Handle(Geom_Curve) C = Intersector.Line(1);

//where Index is an integer between 1 and Nb.
return C;
}

#endif






//void qoccdlgeditobject::dosineobjects()
//{
//
//
//	
//
//	QoccController* vc = mywindow->myController;
//	QoccDocument* doc = vc->getDocument();
//	GEOM_Solver* thesolver = qGeomApp->getSolver();
//    int doc_id = doc->id();
//
//				
//				QListIterator<Handle_GEOM_Object> glistIT(tobedeleted);
//				int count =0;
//				while (glistIT.hasNext())
//				{
//					count +=1; 
//					doc->removeObject( glistIT.next());
//					
//				}
//				
//				
//
//				Handle(GEOM_Object) prevpoint;
//				for(int i=1;i<sincount;i++)
//				{
//					double amplitude = 200;
//					double frequency1 = sincount;
//					double phase1 = sincount + 1000;
//					double sinwave = amplitude + sin(i*frequency1 + phase1); 
//					Handle(GEOM_Object) point = qGeomApp->basicOps( doc_id )->MakePointXYZ(i*10,sinwave,20);
//					tobedeleted << point;
//					doc->registerPresentation(point);
//
//					if (i>1)
//					{
//					Handle(GEOM_Object) line = qGeomApp->basicOps( doc_id )->MakeLineTwoPnt(point,prevpoint);
//					tobedeleted << line;
//					doc->registerPresentation(line);
//
//					}
//
//					prevpoint = point;
//
//				}
//				vc->update();
//
//
//
//}
//
//void qoccdlgeditobject::dosimplesine()
//{
//
//				QoccController* vc = mywindow->myController;
//				QoccDocument* doc = vc->getDocument();
//
//				QListIterator<Handle_GEOM_Object> glistIT2(tobedeleted);
//				while (glistIT2.hasNext())
//				{
//				doc->removeObject( glistIT2.next());
//				}
//
//
//				QListIterator<Handle_AIS_Shape> glistIT(delaislist);
//				while (glistIT.hasNext())
//				{
//					
//					vc->getContext()->Remove(glistIT.next(),false);
//				}
//				
//
//				
//
//
//			
//				BRep_Builder aBuilder;
//				TopoDS_Compound aRes;
//				aBuilder.MakeCompound (aRes);
//				gp_Pnt prevpoint;
//				for(int i=1;i<sincount;i++)
//				{
//					double amplitude = 200;
//					double phase1 = gphase + 1000;
//					double sinwave = amplitude + sin(i*freq + phase1); 
//					
//					gp_Pnt aPnt = gp_Pnt(10 * i, sinwave, 20);
//					BRepBuilderAPI_MakeVertex mkVertex (aPnt);
//					TopoDS_Shape p1 = mkVertex.Shape();
//					if (i>1)
//					{
//					TopoDS_Shape line = BRepBuilderAPI_MakeEdge(prevpoint,aPnt ).Shape();
//					aBuilder.Add (aRes, line);
//					}
//					aBuilder.Add (aRes, p1);
//			
//					prevpoint = aPnt;
//				}
//
//					Handle(AIS_Shape) AISBottle=new AIS_Shape(aRes);
//					
//					//vc->getContext()->SetMaterial(AISBottle,Graphic3d_NOM_GOLD);
//					vc->getContext()->SetColor(AISBottle, Quantity_NOC_BLACK);
//					vc->getContext()->SetDisplayMode(AISBottle,1,Standard_False);
//					
//					//theContext->SetTransparency(AISBottle,0.6);
//					vc->getContext()->Display(AISBottle, 0,0,false,false);
//					//theContext->SetCurrentObject(AISBottle, Standard_False);
//					delaislist << AISBottle;
//
//
//				vc->update();
//				
//
//}



//getting selections 

//Handle(User_AIS) theais = Handle(User_AIS)::DownCast(getContext()->Current()); 
//TopoDS_Shape S = theais->myShape;