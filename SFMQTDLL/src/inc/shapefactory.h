/************************************************************************************
**
** This file is part of the Flexible Digital Modeller
**
** Copyright (C) 2010,2011 Alexander Peña de Leon
**
**This work is licensed under the Creative Commons 
**Attribution-NonCommercial-ShareAlike 3.0 Australia License. 
**To view a copy of this license, 
**visit http://creativecommons.org/licenses/by-nc-sa/3.0/au/ 
**or send a letter to Creative Commons, 171 Second Street, 
**Suite 300, San Francisco, California, 94105, USA.
**
***********************************************************************************/


#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H


#include <commonlibraries.h>


	
class shapefactory  {

public:

    void* operator new(size_t,void* anAddress) 
      {
        return anAddress;
      }
    void* operator new(size_t size) 
      { 
        return Standard::Allocate(size); 
      }
    void  operator delete(void *anAddress) 
      { 
        if (anAddress) Standard::Free((Standard_Address&)anAddress); 
      }

	

	
	

	// axis systems
	//get axis system from 2 vectors
	//get axis system from 2 vectors and a point
	//get axis system from a point and a plane
	//get axis system from 2 planes and a point

	// point geometry 
	static __declspec(dllexport) const gp_Pnt2d& Get2dPntonSurfacefromPoint(TopoDS_Shape SupportSurface, gp_Pnt point);
		static __declspec(dllexport) const gp_Pnt2d& Get2dPntonSurfacefromPoint(gp_Pln pln1, gp_Pnt point);

	static __declspec(dllexport) QList<gp_Pnt> SortPointListbyDistance(QList<gp_Pnt> points, gp_Pnt origin);
	static __declspec(dllexport) QList<gp_Pnt> SortPointListbyAngle(QList<gp_Pnt> points, gp_Pnt hborigin, gp_Vec hbvec, gp_Pnt curedgep);

	static __declspec(dllexport) gp_Pnt MovePointByVector(gp_Pnt point, gp_Vec vector, double Offset);
	static __declspec(dllexport) const gp_Pnt& AddNewPointonCurve(TopoDS_Shape SupportEdge, Standard_Real uRatio);
	static __declspec(dllexport) TopoDS_Shape AddNewPoint(gp_Pnt point);
	static __declspec(dllexport) TopoDS_Shape AddNewMake2d(TopoDS_Shape obj, gp_Pnt vieworigin, gp_Pnt viewtarget, gp_Vec viewnormal);

	// vector
	static __declspec(dllexport) const gp_Vec& getVectorNormaltoSurfaceatPoint(TopoDS_Shape SupportSurface, gp_Pnt point);
	static __declspec(dllexport) const gp_Vec& getVectorTangentToCurveAtPoint(TopoDS_Shape SupportEdge, Standard_Real uRatio);
	// project a point to a surface
	static __declspec(dllexport) const gp_Pnt& ProjectPoint(gp_Pnt p1 , TopoDS_Shape Surface);
	static __declspec(dllexport) const gp_Pnt& ProjectPoint(gp_Pnt p1 , gp_Pln pln1);
	static __declspec(dllexport) const TopoDS_Shape& AddNewProjectCurve(TopoDS_Shape crv1 , gp_Pln pln1);
	static __declspec(dllexport) const TopoDS_Shape&AddNewProjectCurve(TopoDS_Shape crv1 , TopoDS_Shape surface, gp_Vec direction);

	static __declspec(dllexport) const gp_Pln& AddNewMeanPlane(QList<gp_Pnt> pointlist);

	static __declspec(dllexport) TopoDS_Shape AddNewCorner(gp_Pnt p1, gp_Pnt p2 , gp_Pnt p3, double radius, gp_Pnt target);


	// get center of circle
	static __declspec(dllexport) gp_Pnt GetCenterCircle(TopoDS_Shape circle);

	// get vector perpendicular to two vectors, crossproduct
	// get vector between 2 vectors
	// get vector length or magnitude
	// get vector base relative to another vector , project one vector to another vector
	// get vector parallel to a vector at another point
	// get vector at an angle 
	// get translated vector

	// plane 
	static __declspec(dllexport) const gp_Pln& AddNewPlane(TopoDS_Shape SupportSurface,TopoDS_Shape SupportEdge, Standard_Real uRatio);// Plane Normal to Surface at Point
			// get plane from 3 points
	static __declspec(dllexport) gp_Pln AddNewPlane(gp_Pnt p1,gp_Pnt p2,gp_Pnt p3);
	// get plane offset 
		static __declspec(dllexport) gp_Pln AddNewPlane(gp_Pln Pln, Standard_Real Dist);
			// get plane normal to curve
			// get plane between 2 lines
			// get plane parallel to plane through point
	static __declspec(dllexport) gp_Pln AddNewPlane(gp_Pln Pln, gp_Pnt Point);


			// get plane from a line and point
			// get maximum plane of a surface
			// get minimum plane of a surface
			// get minimum plane of a curve
			// get maximum plane of a curve




	// curve geometry
	static __declspec(dllexport) TopoDS_Edge AddNewLineptdir(gp_Pnt origin, gp_Vec dir,double length1,double length2);
	// AddNewLinePtPt
	static __declspec(dllexport) TopoDS_Edge AddNewLineptpt(gp_Pnt p1, gp_Pnt p2);
	// AddNewLineBisector
	// AddNewLinetangent to curve at point
	// AddNewLineNormaltosurface at Point (use the gettvectortangetn to curve to build a line )
	// AddNewCorner 
	// AddNewCircle (center and point, center and radius, 3 points, 3 tangents, 2 tangents and a point);

	// add new circle passing through 3 points
	static __declspec(dllexport) TopoDS_Edge AddNewCircle(gp_Pnt p1,gp_Pnt p2,gp_Pnt p3);
	// add new arc of circle passing thorugh 3 points
	static __declspec(dllexport) TopoDS_Shape AddNewArcofCircle(gp_Pnt p1,gp_Pnt p2,gp_Pnt p3);
	
	// add new circle passing through 3 points
	static __declspec(dllexport) TopoDS_Edge AddNewCircle(const gp_Pnt& Center,const gp_Dir& Norm,const double Radius);
		static __declspec(dllexport) TopoDS_Edge AddNewHyberbolicCrv(gp_Pnt origin, gp_Vec dir,double maxradius,double minradius);
	// addnewellipse (center maxradius min radius, 3 tangents, 4 tangents, 3points)
		static __declspec(dllexport) TopoDS_Shape AddNewEllipse(gp_Pnt Origin,gp_Vec Normal, gp_Vec MajorVector, double MajorRadius,double MinorRadius);
	// addnewrectangle (center width length, corner width length rel,point vec width length, 2 pt's)
	// addnew triangle (3 points,center vec base height, center length, circumscribed center radius)
	// addnew spline cv ep curve

	static __declspec(dllexport) QMap<QString,gp_Pnt> SpherePack(TopoDS_Shape Rail, 
																  TopoDS_Shape SRF, 
																  int hcount,
																  int vcount);

	static __declspec(dllexport) TopoDS_Shape AddNewSplineSShape(QList<gp_Pnt> Points);
	static __declspec(dllexport) TopoDS_Shape AddNewBezierCurve(QList<gp_Pnt> Points,QList<double> weights);
	static __declspec(dllexport) TopoDS_Shape AddNewPolyline(QList<gp_Pnt> Points);

	static __declspec(dllexport) TopoDS_Shape AddNewSplineInterpSShape(QList<gp_Pnt> Points);
	static __declspec(dllexport) QList<Handle(Geom_Curve)> getCurveByHeight(TopoDS_Shape theface,bool direction);
	

// surface geometry 
static __declspec(dllexport) TopoDS_Shape AddNewSymmetry(TopoDS_Shape shape, gp_Pln pln1);
// add new sphere
static __declspec(dllexport) TopoDS_Shape AddNewSphereSurface(gp_Pnt center, double radius);

	// add new torus surface
    static __declspec(dllexport) TopoDS_Shape AddNewToroidalSurface(gp_Pnt center, gp_Vec upvector, double circumference, double radius);
	//add new cylinder
	//add new cone
	//add new extrude

	static __declspec(dllexport) TopoDS_Shape AddNewExtrude(TopoDS_Shape crv1 , gp_Vec dir , double length);
	
	// add new constrained surface
	static __declspec(dllexport) TopoDS_Shape AddNewConstrainedSurface(QList<TopoDS_Shape> shapelist);
	//add new revolve
	static __declspec(dllexport) TopoDS_Shape AddNewRevolveSurface(TopoDS_Shape crv1, gp_Pnt origin, gp_Vec dir);
	//add new sweep
    static __declspec(dllexport) TopoDS_Shape AddNewSweep(TopoDS_Shape path, TopoDS_Shape crossection);

	//add new blend surface
	static __declspec(dllexport) TopoDS_Shape AddNewBlendSurface(TopoDS_Shape crv1, TopoDS_Shape crv2);

		//add new multisection surface
	static __declspec(dllexport) TopoDS_Shape AddNewLoftSurface(QList<TopoDS_Shape> sections);
	// add new thicksolid

	static __declspec(dllexport) TopoDS_Shape AddNewThickSolid(TopoDS_Shape theface,double thickness1, double thickness2);

//add new conical surface from 4 points
	static __declspec(dllexport) TopoDS_Shape AddNewConicalSurface(gp_Pnt p1,gp_Pnt p2,gp_Pnt p3,gp_Pnt p4);
	
	//add new fill surface
static __declspec(dllexport) TopoDS_Shape AddNewFillSurface(QList<TopoDS_Shape> shapelist);
static __declspec(dllexport) TopoDS_Shape AddNewFillSurface(TopoDS_Shape edge);

	//add new fillet edge
	//add new chamfer edge
	//add new split surface
	//add new sweet surface
static __declspec(dllexport) TopoDS_Shape getfacefromshape(TopoDS_Shape shape1);
static __declspec(dllexport) TopoDS_Shape getedgefromshape(TopoDS_Shape shape1);

	//by Daniel
	static __declspec(dllexport) TopoDS_Shape AddNewHyperboloidbyFormula(double innerRadius, double height, double heightUnder, double angle,gp_Pnt Origin, gp_Vec Dir);

	static __declspec(dllexport) TopoDS_Shape AddNewConicalSurface(gp_Pnt origin,gp_Dir dir,double angle,double radius);
	static __declspec(dllexport) TopoDS_Face AddNewHyberbolicShell(gp_Pnt origin,gp_Vec dir,double angle,double radius , double length );
	static __declspec(dllexport) TopoDS_Face convertgeomsurfaceface(Handle_Geom_Surface thesrf);
	static __declspec(dllexport) Handle_Geom_Surface convertfacetogeom(TopoDS_Shape aFace);
	static __declspec(dllexport) Handle_Geom_Curve convertshapetocurve(TopoDS_Shape theedge);
	// solids geometry
	static __declspec(dllexport) TopoDS_Shape AddNewRevolvedHyberbolaSolid(gp_Pnt origin,gp_Vec dir,double angle,double radius , double length );

	//shape operations
	static __declspec(dllexport) TopoDS_Shape BooleanCommon(TopoDS_Shape Stock,TopoDS_Shape Tool);
	static __declspec(dllexport) TopoDS_Shape BooleanSubstract(TopoDS_Shape Stock,TopoDS_Shape Tool);
	static __declspec(dllexport) TopoDS_Shape BooleanGlue(TopoDS_Shape Stock,TopoDS_Shape Tool);
	static __declspec(dllexport) TopoDS_Shape BooleanSubstractFiller(TopoDS_Shape Stock,TopoDS_Shape Tool,const BOPTools_DSFiller& filler);
	



	// inntersection point between 2 curves; 
	static __declspec(dllexport) gp_Pnt AddNewIntersectionPoint(TopoDS_Shape crv1,TopoDS_Shape crv2, int nbpoints);
	static __declspec(dllexport) TopoDS_Shape AddNewIntersectionPointMulti(TopoDS_Shape crv1,TopoDS_Shape crv2);

		static __declspec(dllexport) gp_Pnt IntersectCrvtoCrv(TopoDS_Shape crv1, TopoDS_Shape crv2);
		static __declspec(dllexport) gp_Pnt IntersectCrvtoCrv(Handle_Geom_Curve crv1, Handle_Geom_Curve crv2);
		static __declspec(dllexport) gp_Pnt IntersectCrvtoCrv(Handle_Geom_Curve crv1, TopoDS_Edge crv2);
		
	// intersecttion point between a curve and a surface
		static __declspec(dllexport) gp_Pnt IntersectCrvtoSrf(TopoDS_Edge crv1, TopoDS_Face srf1);
		static __declspec(dllexport) gp_Pnt IntersectCrvtoSrf(Handle_Geom_Curve crv1, Handle_Geom_Surface srf1);
		static __declspec(dllexport) gp_Pnt IntersectCrvtoSrf(TopoDS_Edge crv1, Handle_Geom_Surface srf1);

		static __declspec(dllexport) TopoDS_Shape AddNewSplit(TopoDS_Shape Stock, TopoDS_Shape Tool);
		static __declspec(dllexport) TopoDS_Shape AddNewSplit(TopoDS_Shape Stock, gp_Pln plane1);
		static __declspec(dllexport) TopoDS_Shape AddNewSplitW(TopoDS_Shape Stock, TopoDS_Shape Wire);
		static __declspec(dllexport) TopoDS_Shape AddNewSplitGeom(TopoDS_Shape Stock, TopoDS_Shape Tool);
	
	// project a point to a surface
	// project a point to a curve
	// project a curve to a surface
	// project a curve to a curve
	
	// extrude a curve by a length towards a vector
	// extrude a curve towards a vector until it meets a plane
	// extrude a curve by a length towards a plane
	// revolve a curve by another curve
	// revolve a point by a curve

		// intersect a line and a plane
		static __declspec(dllexport) gp_Pnt& AddNewIntersectLinePlane(gp_Pln theplane, gp_Lin theline);

	    // project a point on a curve
	    static __declspec(dllexport) gp_Pnt ProjectPointonCurve(gp_Pnt p1, TopoDS_Shape edge);
		// translate a shape towards a vector by an amount
		static __declspec(dllexport) TopoDS_Shape move(TopoDS_Shape shape, gp_Vec moveVector);
	// translate a shape from point to point
		static __declspec(dllexport) TopoDS_Shape movepointopoint(TopoDS_Shape shape, gp_Pnt p1,gp_Pnt p2);
		// rotate a shape with a point , vector and angle
		static __declspec(dllexport) TopoDS_Shape rotate(TopoDS_Shape shape, gp_Pnt rotorigin,gp_Vec rotvec,double angle);
		// displace from one axis system to another axis system
		static __declspec(dllexport) TopoDS_Shape axistoaxis(TopoDS_Shape shape, gp_Ax3 fromsystem,gp_Ax3 tosystem);
		
	// scale a shape by percentage
	// scale a shape using a reference curve and a targent curve
	// mirror a shape
	// rotate a shape
	// axis to axis a shape
	// array a shape to a curve by a reference point and direction
	// array a shape to a surface by a refefence point and direction



		// spatial analysis

		// getlength of a curve
		// getarea of a surface
		// getcenterfo gravity of a acurve
		// get center of gravity of a surface
		// get min and max distance between 2 points
		// get min and max distance between a point and a curve
		// get min and max distance between a point and a surface
		// get mina nd max distance between 2 curves
		// get min and max distance between a curve and surface
		// get min and max distance between a solid and a point / curve and surface

		// Utility Functions

		// Build a Map of Points on a Surface  upoints by vpoints and name the point key as  U###v### 
		static __declspec(dllexport) QMap<QString,gp_Ax1>  BuildPointGridonSrf(TopoDS_Shape aShape,double ucount, double vcount);

			static __declspec(dllexport) int getshapecount(TopoDS_Shape aShape,TopAbs_ShapeEnum shapetype);
			static __declspec(dllexport) QList<TopoDS_Shape> getshapelist(TopoDS_Shape aShape,TopAbs_ShapeEnum shapetype);

		static __declspec(dllexport) QMap<QString,gp_Ax1>  BuildRandPointsonSrf(TopoDS_Shape aShape,double numberofpoints);
		static __declspec(dllexport) QString  GetNextUvName(QString Name, int uoffset, int voffset);
		static __declspec(dllexport) QString  GetNextUvNameCiclic(QString Name, int uoffset, int voffset, int count);
		
		static __declspec(dllexport) QList<QStringList> ReadDelimitedtoStringList(QString Filename,QString Delimeter);
	
		static __declspec(dllexport) TopoDS_Shape applyuvcurve(TopoDS_Shape& shape2d, TopoDS_Shape& surface);

        static __declspec(dllexport) TopoDS_Shape GetBiggestSolid(TopoDS_Shape& solid);
		static __declspec(dllexport) TopoDS_Shape GetBiggestFace(TopoDS_Shape& face);
		static __declspec(dllexport) TopoDS_Shape GetSmallestFace(TopoDS_Shape& face);
		static __declspec(dllexport) TopoDS_Shape GetLowestVertex(TopoDS_Shape& Shape);
		
		static __declspec(dllexport) TopoDS_Shape GetBiggestShell(TopoDS_Shape& shell);

		static __declspec(dllexport) double GetVolume(TopoDS_Shape &shape);

		static __declspec(dllexport) double GetArea(TopoDS_Shape &shape);
		static __declspec(dllexport) double GetMaxDis(TopoDS_Shape s1, TopoDS_Shape s2);

		static __declspec(dllexport) gp_Pnt GetCOG(TopoDS_Shape &shape);
		static __declspec(dllexport) gp_Pnt GetAveragePoint(QList<gp_Pnt> plist);
		static __declspec(dllexport) double GetLength(TopoDS_Shape &shape);
		static __declspec(dllexport) TopoDS_Shape AddNewBox(gp_Pnt center, gp_Vec orient, double width, double height, double depth);
		static __declspec(dllexport) TopoDS_Shape AddNewRectangle(gp_Pnt center, gp_Vec orient, double width, double height);

		static __declspec(dllexport) Handle_Geom_Curve intersectsrf(const Handle(Geom_Surface)& S1,const Handle(Geom_Surface)& S2);
		static __declspec(dllexport) bool isintersecting(TopoDS_Shape srf1,TopoDS_Shape srf2);
		static __declspec(dllexport) TopoDS_Shape AddNewIntersectSrf(TopoDS_Shape srf1,TopoDS_Shape srf2);
		static __declspec(dllexport) TopoDS_Shape AddNewIntersectSrf(gp_Pln plane1,TopoDS_Shape srf);
		static __declspec(dllexport) TopoDS_Shape AddNewIntersectSrf(gp_Pln pln1,gp_Pln pln2);
		static __declspec(dllexport) gp_Pnt AddNewIntersectCrvPln(TopoDS_Shape crv1,gp_Pln pln1);
		static __declspec(dllexport) gp_Pnt AddNewIntersectCrvSrf(TopoDS_Shape crv1,TopoDS_Shape srf1);
				static __declspec(dllexport) TopoDS_Shape AddNewIntersectCrvSrfMulti(TopoDS_Shape crv1,TopoDS_Shape srf1,double &nbcount);
		static __declspec(dllexport) TopoDS_Shape AddNewJoinEdges(QList<TopoDS_Shape> shapelist);
		static __declspec(dllexport) TopoDS_Shape AddNewIntersectSrfW(TopoDS_Shape srf1,TopoDS_Shape srf2); 
		static __declspec(dllexport) TopoDS_Shape AddNewIntersectFast(TopoDS_Shape srf1,TopoDS_Shape srf2);
		static __declspec(dllexport) gp_Pnt2d AddNewIntersectCrv2d(TopoDS_Shape crv1,TopoDS_Shape crv2,int &nbpoints);

		

		static __declspec(dllexport) void updateUserAIS(TopoDS_Shape aShape, Handle_User_AIS &anAIS, Handle_AIS_InteractiveContext ic);
				static __declspec(dllexport) void updateAIS(TopoDS_Shape aShape, Handle_AIS_Shape &anAIS, Handle_AIS_InteractiveContext ic);

	static __declspec(dllexport) void AddNewAnnotation(QString intext, gp_Pnt loc);

	// new function made by ben coorey in smart geo
		static __declspec(dllexport) double map(double value, double low, double high, double tlow, double thigh);
	static __declspec(dllexport) double getGray(QImage theImg, double u, double v, bool percent);

	static __declspec(dllexport) gp_Pnt AddNewUVPt(TopoDS_Shape srf,double u,double v);
	static __declspec(dllexport) gp_Vec AddNewUVVec(TopoDS_Shape srf,double u,double v);
	static __declspec(dllexport) TopoDS_Shape AddNewIsoCurve(TopoDS_Shape srf,bool orientation, double uv);
	static __declspec(dllexport) gp_Pnt getpointfromshape(TopoDS_Shape point);



static __declspec(dllexport) TopoDS_Shape lineptarc(gp_Pnt p1,gp_Pnt p2,gp_Vec t1,double maxrad);
static __declspec(dllexport) QList<TopoDS_Shape> biarcjunctiondomain(gp_Pnt p1,gp_Pnt p2,gp_Vec t1,gp_Vec t2, double arcratio);

static __declspec(dllexport) QList<TopoDS_Shape> biarcptdir(gp_Pnt p1,gp_Pnt p2,gp_Vec t1,gp_Vec t2, double arcratio,double infinite);
static __declspec(dllexport) QList<TopoDS_Shape> biarconcurve(TopoDS_Shape crv, double ratio1,double ratio2,double arcratio,double infinite);

static __declspec(dllexport) TopoDS_Shape biarcoptimized(TopoDS_Shape crv, double ratio1,double ratio2,double percentinf,double percentsup,double infinite);



		}; // donot delete this by mistake end of class must have a seicolon at the end, tricks for young players

	
	typedef shapefactory HSF;
	typedef shapefactory hsf;
	
#endif   // END OF MANTIS_SHAPEFACTORY_H


