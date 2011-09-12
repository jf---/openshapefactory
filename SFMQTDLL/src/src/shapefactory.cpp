#include <shapefactory.h>
#include <stdio.h>
#include <time.h>
#include <ui.h>


 
	static const double Pi = 3.14159265358979323846264338327950288419717;
	static double TwoPi = 2.0 * Pi;
	static Standard_Boolean fixParam(Standard_Real& theParam);

	gp_Pnt globalorigin; // glopal point used in the sorting function
	
	gp_Pnt globalhbcenter;// center of current hb
	gp_Pnt globalrefedgemidp;// center of current hb
	gp_Pln globalhbplane; // plane of hyperbola

	//distance sorting function for using with QMap 
	bool sortbydistance(const gp_Pnt p1, const gp_Pnt p2)
	{
	double dis1 = globalorigin.Distance(p1);
	double dis2 = globalorigin.Distance(p2);
	return dis1 < dis2;
	}

	bool sortbyangle(const gp_Pnt p1, const gp_Pnt p2)
	{
	
	gp_Pnt2d p12d = HSF::Get2dPntonSurfacefromPoint(globalhbplane,p1);
	gp_Pnt2d p22d = HSF::Get2dPntonSurfacefromPoint(globalhbplane,p2);

	
	gp_Lin refline(globalhbcenter,gp_Vec(globalhbcenter,globalrefedgemidp));
	gp_Lin l1(globalorigin,gp_Vec(globalorigin,p1));
	gp_Lin l2(globalorigin,gp_Vec(globalorigin,p1));

	double angle1 = refline.Angle(l1);
	double angle2 = refline.Angle(l2);

	if (p12d.X() < 0 && p12d.Y() < 0 ) angle1 += 180;
	if (p22d.X() < 0 && p22d.Y() < 0)  angle2 += 180;

	return angle1 < angle2;
	}





const gp_Pln& HSF::AddNewPlane(TopoDS_Shape SupportSurface, TopoDS_Shape SupportEdge, Standard_Real uRatio)

	{
	
			const TopoDS_Face& aFace = TopoDS::Face (SupportSurface);
			const TopoDS_Edge& aEdge = TopoDS::Edge (SupportEdge);
			Handle(Geom_Surface) aSurf = BRep_Tool::Surface(aFace);
			

			gp_Pnt normalpoint = AddNewPointonCurve(SupportEdge,uRatio);
			gp_Vec normalVector = getVectorNormaltoSurfaceatPoint(SupportSurface,normalpoint);
			gp_Vec TangentVector = getVectorTangentToCurveAtPoint(SupportEdge,uRatio);
			gp_Vec PerpendicularVector = normalVector.Crossed(TangentVector);

			gp_Dir PerpendicularDir(PerpendicularVector);
			gp_Pln aPlane(normalpoint,PerpendicularDir);
			 
			return aPlane;

			


}



const gp_Pnt& HSF::AddNewPointonCurve(TopoDS_Shape SupportEdge, Standard_Real uRatio)

	{
			const TopoDS_Edge& aEdge = TopoDS::Edge (SupportEdge);
			Standard_Real aFP, aLP, aP;
			Handle(Geom_Curve) aCurve = BRep_Tool::Curve(aEdge, aFP, aLP);
			aP = aFP + (aLP - aFP) * uRatio;
			gp_Pnt p1 = aCurve->Value(aP);

			return p1;
	}

const gp_Pnt2d& HSF::Get2dPntonSurfacefromPoint(TopoDS_Shape SupportSurface, gp_Pnt point)
	{
			
			const TopoDS_Face& aFace = TopoDS::Face (SupportSurface);
			Handle(Geom_Surface) aSurf = BRep_Tool::Surface(aFace);
			Handle(ShapeAnalysis_Surface) aSurfAna = new ShapeAnalysis_Surface (aSurf);
			gp_Pnt2d pUV = aSurfAna->ValueOfUV(point, Precision::Confusion());
			return pUV;

	}

const gp_Pnt2d& HSF::Get2dPntonSurfacefromPoint(gp_Pln pln1, gp_Pnt point)
	{
			gp_Pnt projp = HSF::ProjectPoint(point,pln1);
			Handle_Geom_Plane myplane =  GC_MakePlane (pln1); 
			Handle(Geom_Surface) aSurf = myplane;
			Handle(ShapeAnalysis_Surface) aSurfAna = new ShapeAnalysis_Surface (aSurf);
			gp_Pnt2d pUV = aSurfAna->ValueOfUV(projp, Precision::Confusion());
			return pUV;

	}

const gp_Vec&  HSF::getVectorNormaltoSurfaceatPoint(TopoDS_Shape SupportSurface, gp_Pnt point)
	{
			
			if(SupportSurface.ShapeType() != TopAbs_ShapeEnum::TopAbs_FACE) 
				{
				return gp_Vec(0,0,0);
				}

			gp_Pnt projp = HSF::ProjectPoint(point,SupportSurface);
			const TopoDS_Face& aFace = TopoDS::Face (SupportSurface);
			// Get 2d UV data
			gp_Pnt2d pUV = Get2dPntonSurfacefromPoint(SupportSurface,projp);

			/*gp_Pnt pointonsrf;
			gp_Vec V;

			BRepGProp_Face gprops(aFace);
			gprops.Normal(pUV.X(),pUV.Y(),pointonsrf,V);
	*/

			BRepAdaptor_Surface aSurface(aFace);
			gp_Vec ut,vt;
			gp_Pnt pt;
			aSurface.D1(pUV.X(),pUV.Y(),pt,ut,vt);
			gp_Vec V = ut.Crossed(vt);
			
			
			//// find the first derivative in both u and v and then do the cross product
			//gp_Vec Vec1,Vec2;
			//Handle(Geom_Surface) SF = BRep_Tool::Surface(aFace);
			//SF->D1(pUV.X(), pUV.Y(), point, Vec1, Vec2);
			//gp_Vec& V = Vec1.Crossed(Vec2);



			Standard_Real mod = V.Magnitude();
			if (mod < Precision::Confusion()) qDebug() << "Vector has no Magnitude" ;
				// consider the face orientation
			if (aFace.Orientation() == TopAbs_REVERSED ||
				aFace.Orientation() == TopAbs_INTERNAL) {
			  V = -V;
			}



			return V;


	}





const gp_Pnt& HSF::ProjectPoint(gp_Pnt p1 , TopoDS_Shape Surface)
	{
			gp_Pnt resultpoint;
			if(Surface.ShapeType() != TopAbs_ShapeEnum::TopAbs_FACE) 
				{
				return resultpoint;
				}
			TopoDS_Face aFace = TopoDS::Face(Surface);
			
			Handle_Geom_Surface aSurf = BRep_Tool::Surface(aFace);
			GeomAPI_ProjectPointOnSurf Proj (p1, aSurf);
			if (Proj.NbPoints() > 0)
				{
					resultpoint = Proj.NearestPoint();
				}

			return resultpoint;

	}

const gp_Pln& HSF::AddNewMeanPlane(QList<gp_Pnt> pointlist)
	{

Handle(TColgp_HArray1OfPnt) anArrayofPnt = new TColgp_HArray1OfPnt(1,pointlist.count()); // sizing array           
for (int i = 1 ; i <= pointlist.count() ; i++)
	{
	gp_Pnt curpoint = pointlist.at(i-1);
	anArrayofPnt->SetValue(i,curpoint);                          
	}                                                            

Standard_Real Tolerance = 1000;                                     
                                                                 
gp_Pln P;             

GeomPlate_BuildAveragePlane myplane(anArrayofPnt,pointlist.count(),Precision::Confusion(),1,1);
//GProp_PEquation PE(anArrayofPnt,Tolerance); 

if (myplane.IsPlane()) { P = myplane.Plane()->Pln();}  

return P;

}

const TopoDS_Shape& HSF::AddNewProjectCurve(TopoDS_Shape crv1 , TopoDS_Shape surface, gp_Vec direction)
	{
		
	TopoDS_Shape result;
	double area = HSF::GetArea(surface);
	
	gp_Pnt midp = HSF::AddNewPointonCurve(crv1,0.5);
	BRepProj_Projection proj(crv1 ,surface,direction);
	
    if (!proj.IsDone()){
          return result;
     }

	 result = proj.Shape();

	 return result;

	}


const TopoDS_Shape& HSF::AddNewProjectCurve(TopoDS_Shape crv1 , gp_Pln pln1)
	{
		
	TopoDS_Shape result;
	GC_MakePlane makePlane(pln1);
    Handle(Geom_Plane) geomPlane = makePlane.Value();
    if (geomPlane.IsNull())
          return result;
	double infinite = 100000000000000;
    TopoDS_Shape aPlaneShape = BRepBuilderAPI_MakeFace(geomPlane,-infinite,infinite,-infinite,infinite);
	BRepProj_Projection proj(crv1 ,aPlaneShape, pln1.Axis().Direction());
    if (!proj.IsDone()){
          return result;
     }

	 result = proj.Shape();

	 return result;

	}


const gp_Pnt& HSF::ProjectPoint(gp_Pnt p1 , gp_Pln pln1)
	{
			gp_Pnt resultpoint;
						
			Handle_Geom_Plane aSurf = new Geom_Plane(pln1);
			GeomAPI_ProjectPointOnSurf Proj (p1, aSurf);
			if (Proj.NbPoints() > 0)
				{
					resultpoint = Proj.NearestPoint();
				}

			return resultpoint;

	}

const gp_Vec& HSF::getVectorTangentToCurveAtPoint(TopoDS_Shape SupportEdge, Standard_Real uRatio)
	{
	if(SupportEdge.IsNull()) qDebug() << "supportedge null";
	const TopoDS_Edge& aEdge = TopoDS::Edge (SupportEdge);
	Standard_Real aFP, aLP, aP;
	Handle(Geom_Curve) aCurve = BRep_Tool::Curve(aEdge, aFP, aLP);
	aP = aFP + (aLP - aFP) * uRatio;
	gp_Vec& V1 = gp_Vec() ;
	gp_Pnt p1;
	aCurve->D1(aP,p1,V1);

	return V1;



	}




QList<gp_Pnt> HSF::SortPointListbyDistance(QList<gp_Pnt> points, gp_Pnt origin)
	{

	 QList<gp_Pnt> SortedList;
	 SortedList = points;
	 //gp_Pnt randompoint = gp_Pnt(-100000000,0,0);
	 globalorigin = origin;
     qStableSort(SortedList.begin(), SortedList.end(), sortbydistance);

	 //double mindist = 0;
	 //QListIterator<gp_Pnt> i(points);
	 //while (i.hasNext())
		// {
		// gp_Pnt curpoint = i.next();

		// double curdistance = origin.Distance(curpoint);
		// //double curdistance =  randompoint.Distance(curpoint);
		//
		// if (curdistance >= mindist)
		//	 {
		//		SortedList.append(curpoint);
		//	 }
		// else
		//	 {
		//		SortedList.insert(SortedList.size()-2,curpoint);
		//	 }
		// mindist = curdistance;

		// }


	  //QListIterator<gp_Pnt> iT(SortedList);
	  //int count = 0;
	  //while (iT.hasNext())
		 //{
			//count += 1;
			//gp_Pnt curpoint = iT.next();
			//qDebug() << "Point:" << count << ",Distance" << origin.Distance(curpoint);
		 //}

	  return SortedList;


     
	}


QList<gp_Pnt> HSF::SortPointListbyAngle(QList<gp_Pnt> points, gp_Pnt hborigin, gp_Vec hbvec, gp_Pnt curedgep)
	{

	 QList<gp_Pnt> SortedList;
	 SortedList = points;
	 //gp_Pnt randompoint = gp_Pnt(-100000000,0,0);
	globalhbcenter = hborigin; 
	globalrefedgemidp = curedgep;
	globalhbplane = gp_Pln(hborigin,hbvec);

     qStableSort(SortedList.begin(), SortedList.end(), sortbyangle);

	 return SortedList;


     
	}

QMap<QString,gp_Pnt> HSF::SpherePack(TopoDS_Shape Rail, TopoDS_Shape SRF, int hcount,int vcount)
	{

QMap<QString,gp_Pnt> pointmap;
TopoDS_Shape startcrv = Rail;

Handle(Geom_Surface) S1 = BRep_Tool::Surface(TopoDS::Face(SRF));

//This class is instantiated as follows:
GeomAPI_IntCS Intersector;






	

TopoDS_Shape startedge = HSF::getedgefromshape(startcrv);

double crvlen = HSF::GetLength(startedge);
crvlen = crvlen;
double minrad = floor(crvlen / hcount);
QList<gp_Pnt> spherelist1;

int rowcount =0;
double avrgradius=0;
int radiuscount =0;
for (int i = 1; i < hcount; i++){
try
{
rowcount++;
gp_Pnt p1 = HSF::AddNewPointonCurve(startedge,(double) (i-1) / (hcount));
if (i>1)
	{
	radiuscount++;
	avrgradius += p1.Distance(spherelist1.last());
	//TopoDS_Shape sphere1 = HSF::AddNewSphereSurface(p1,avrgradius/radiuscount);
	//vis(sphere1)
	}

QString key = "U" + QString::number(i-1,char(103),3)+ "_"+"V" + QString::number(0,char(103),3);
pointmap.insert(key,p1);
spherelist1 << p1;
QString line;
QTextStream linestream(&line);
linestream << "intersecting sphere:" << i; 
ui::getInstance()->Statusbar(line);
}
catch(...)
{

}
}
avrgradius = avrgradius / radiuscount;


QList<gp_Pnt> prevspheres;
prevspheres = spherelist1;
int evencount = -1;
bool iseven = false;
for(int j =0 ; j < vcount ; j++){

evencount++;
if(evencount % 2 ==0) iseven = true; else iseven = false;

QList<gp_Pnt> nextspheres;

if (!iseven)
	{
//get firstpoint
gp_Pnt p1 = prevspheres.at(0);
gp_Pnt p2 = prevspheres.at(1);

////////first sphere point
gp_Pnt midp((p1.X()+ p2.X())/2,(p1.Y()+ p2.Y())/2,(p1.Z()+ p2.Z())/2);
	double b = p1.Distance(p2)/2;
	double c = avrgradius;
	double bsq = b*b;
	double csq = c*c;
	double a = sqrt(abs(csq-bsq));
	gp_Vec midvec(midp,p2);
	Handle(Geom_Circle) circ = new Geom_Circle(gp_Ax2(midp,midvec),a);
	Intersector.Perform(circ,S1);
	Standard_Integer nb = Intersector.NbPoints();
	gp_Pnt low;
	double minz = 1000000000000000;
	if (nb > 0) {
		for (int i = 1 ; i<=nb;i++)
			{
				gp_Pnt intp = Intersector.Point(i);
				if(intp.Z() < minz)
					{
						minz = intp.Z();
						low = intp;
					}
			}

	}

gp_Pnt lowpt1 = low;

////////second sphere point
p2 = lowpt1;
	midp = gp_Pnt((p1.X()+ p2.X())/2,(p1.Y()+ p2.Y())/2,(p1.Z()+ p2.Z())/2);
	b = p1.Distance(p2)/2;
	c = avrgradius;
	bsq = b*b;
	csq = c*c;
	a = sqrt(abs(csq-bsq));
	midvec = gp_Vec(midp,p2);
	circ = new Geom_Circle(gp_Ax2(midp,midvec),a);
	Intersector.Perform(circ,S1);
	nb = Intersector.NbPoints();
	//gp_Pnt low;
	minz = 1000000000000000;
	if (nb > 0) {
		for (int i = 1 ; i<=nb;i++)
			{
				gp_Pnt intp = Intersector.Point(i);
				if(intp.Z() < minz)
					{
						minz = intp.Z();
						low = intp;
					}
			}

	}

gp_Pnt first = low;



nextspheres << first;
	}

for(int i = 0 ; i < prevspheres.size()-1; i++) {
try {
gp_Pnt p1 = prevspheres.at(i);
gp_Pnt p2 = prevspheres.at(i+1);

////////middle sphere point
gp_Pnt midp((p1.X()+ p2.X())/2,(p1.Y()+ p2.Y())/2,(p1.Z()+ p2.Z())/2);
	double b = p1.Distance(p2)/2;
	double c = avrgradius;
	double bsq = b*b;
	double csq = c*c;
	double a = sqrt(abs(csq-bsq));
	gp_Vec midvec(midp,p2);
	Handle(Geom_Circle) circ = new Geom_Circle(gp_Ax2(midp,midvec),a);
	Intersector.Perform(circ,S1);
	Standard_Integer nb = Intersector.NbPoints();
	gp_Pnt low;
	double minz = 1000000000000000;
	if (nb > 0) {
		for (int i = 1 ; i<=nb;i++)
			{
				gp_Pnt intp = Intersector.Point(i);
				if(intp.Z() < minz)
					{
						minz = intp.Z();
						low = intp;
					}
			}

	}

gp_Pnt lowpt = low;
//gp_Pnt lowpt = spherepoint(SRF,p1,p2,avrgradius);
TopoDS_Shape lowpoint = HSF::AddNewPoint(lowpt);

if(lowpoint.IsNull()) continue;


QString key = "U" + QString::number(i,char(103),3)+ "_"+"V" + QString::number(j+1,char(103),3);
pointmap.insert(key,lowpt);

nextspheres << lowpt;

QString line;
QTextStream linestream(&line);
linestream << "row "<< j << ":" << i ; 
ui::getInstance()->Statusbar(line);


//HSF::AddNewAnnotation(line,lowpt);
} catch (...) {}
}

if(!iseven)
	{
	gp_Pnt p3 = prevspheres.at(prevspheres.size()-2);
	gp_Pnt p4 = prevspheres.at(prevspheres.size()-1);

	////////first sphere point
	gp_Pnt p1 = p3;
	gp_Pnt p2 = p4;
gp_Pnt midp((p1.X()+ p2.X())/2,(p1.Y()+ p2.Y())/2,(p1.Z()+ p2.Z())/2);
	double b = p1.Distance(p2)/2;
	double c = avrgradius;
	double bsq = b*b;
	double csq = c*c;
	double a = sqrt(abs(csq-bsq));
	gp_Vec midvec(midp,p2);
	Handle(Geom_Circle) circ = new Geom_Circle(gp_Ax2(midp,midvec),a);
	Intersector.Perform(circ,S1);
	Standard_Integer nb = Intersector.NbPoints();
	gp_Pnt low;
	double minz = 1000000000000000;
	if (nb > 0) {
		for (int i = 1 ; i<=nb;i++)
			{
				gp_Pnt intp = Intersector.Point(i);
				if(intp.Z() < minz)
					{
						minz = intp.Z();
						low = intp;
					}
			}

	}

gp_Pnt lowpt3 = low;

	//gp_Pnt lowpt3 = spherepoint(SRF,p3,p4,avrgradius);
////////second sphere point
p1 = p4;
p2 = lowpt3;
	midp = gp_Pnt((p1.X()+ p2.X())/2,(p1.Y()+ p2.Y())/2,(p1.Z()+ p2.Z())/2);
	b = p1.Distance(p2)/2;
	c = avrgradius;
	bsq = b*b;
	csq = c*c;
	a = sqrt(abs(csq-bsq));
	midvec = gp_Vec(midp,p2);
	circ = new Geom_Circle(gp_Ax2(midp,midvec),a);
	Intersector.Perform(circ,S1);
	nb = Intersector.NbPoints();
	//gp_Pnt low;
	minz = 1000000000000000;
	if (nb > 0) {
		for (int i = 1 ; i<=nb;i++)
			{
				gp_Pnt intp = Intersector.Point(i);
				if(intp.Z() < minz)
					{
						minz = intp.Z();
						low = intp;
					}
			}

	}

gp_Pnt last = low;

	//gp_Pnt last = spherepoint(SRF,p4,lowpt3,avrgradius);

	nextspheres << last;
	}

prevspheres = nextspheres;
}




	return pointmap;

	}



TopoDS_Shape HSF::AddNewBezierCurve(QList<gp_Pnt> Points,QList<double> weights)
	{
     TopoDS_Shape aShape;
	 if(Points.size() != weights.size()) return aShape;
	  TColgp_Array1OfPnt pointarray (1,Points.count()); // sizing point array 
	  TColStd_Array1OfReal weightarray (1,Points.count()); // sizing weights array 

	int count = 0;
	for (int i = 0; i < Points.size(); ++i) {
     
	  gp_Pnt curpoint = Points.at(i);
	  double curweight = 0; 
	  if(!weights.isEmpty()) curweight = weights.at(i);

	  pointarray.SetValue(count+1,curpoint);
	  weightarray.SetValue(count+1,curweight);
	  count +=1;
	  }
	
	  Handle(Geom_BezierCurve) SPL1 = new Geom_BezierCurve(pointarray, weightarray);
	/*   Handle(Geom_BSplineCurve) SPL1 = new Geom_BSplineCurve(pointarray, weightarray,*/
	 /* for (int i = 1; i < Points.size()-1; ++i) {
     	SPL1->SetWeight(i+1,1);
	  }*/

	  //SPL1->SetWeight(2,20);


	  double curdeg = SPL1->Weight(1) ;
	  double curweight = weightarray.Value(1);
	  //SPL1->Increase(3);

	  double fp,lp;
	  fp = SPL1->FirstParameter();
	  lp = SPL1->LastParameter();
	  aShape = BRepBuilderAPI_MakeEdge(SPL1,fp,lp);
	  return aShape;

	}
TopoDS_Shape HSF::AddNewSplineSShape(QList<gp_Pnt> Points)
	{
     
	  TColgp_Array1OfPnt arrayval (1,Points.count()); // sizing array 

	  QListIterator<gp_Pnt> iT(Points);
	  int count = 0;
	  while (iT.hasNext())
	  {
	  gp_Pnt curpoint = iT.next();
	  arrayval.SetValue(count+1,curpoint);
	  count +=1;
	  }
	
	  Handle(Geom_BSplineCurve) SPL1 =  GeomAPI_PointsToBSpline(arrayval).Curve();
	  
	/*  GeomConvert_ApproxCurve aApprox(SPL1,Precision::Confusion(),GeomAbs_C1,3,3);
      SPL1 = aApprox.Curve();*/


	  double fp,lp;
	  fp = SPL1->FirstParameter();
	  lp = SPL1->LastParameter();
	  TopoDS_Shape aShape = BRepBuilderAPI_MakeEdge(SPL1,fp,lp);
	  return aShape;

	}

TopoDS_Shape HSF::AddNewSplineInterpSShape(QList<gp_Pnt> Points)
	{
     
	TopoDS_Shape aShape;
	  Handle(TColgp_HArray1OfPnt) arrayval = new TColgp_HArray1OfPnt (1,Points.count()); // sizing harray

	  QListIterator<gp_Pnt> iT(Points);
	  int count = 0;
	  while (iT.hasNext())
	  {
	  gp_Pnt curpoint = iT.next();
	  arrayval->SetValue(count+1,curpoint);
	  count +=1;
	  }


	  Handle(Geom_BoundedCurve) SPL2;

	  try
		  {
	  GeomAPI_Interpolate anInterpolation(arrayval,Standard_False,Precision::Approximation());  
	  anInterpolation.Perform();   
		 

	  if (anInterpolation.IsDone() == true)                                                           
       SPL2 = anInterpolation.Curve(); 
	  //Handle(Geom_Curve) thecurve = Handle_Geom_Curve::DownCast(SPL2);
	  double fp,lp;
	 //gp_Pnt mystart = anInterpolation.Curve()->StartPoint();
	    
		aShape = BRepBuilderAPI_MakeEdge(SPL2);
		return aShape;
		  }
	  catch(...)
		  {
		return aShape;
		  }
                                                                                        
	 return aShape;


	}

    
gp_Pnt& HSF::AddNewIntersectLinePlane(gp_Pln theplane, gp_Lin theline)
	{

	gp_Pnt theSolution;

	IntAna_IntConicQuad theIntersection( theline, theplane, Precision::Angular() );
	if (theIntersection.IsDone())
	{
		if (!theIntersection.IsParallel())
		{
			if (theIntersection.NbPoints() > 0)
			{
				theSolution = theIntersection.Point(1);
								
			}
		}

	}

	return theSolution;

	}

gp_Pnt HSF::MovePointByVector(gp_Pnt point, gp_Vec vector, double Offset)
	{
			
			gp_Vec aV = vector.Normalized();
			aV = aV.Multiplied(Offset);

			gp_Pnt newpoint;
			newpoint.SetXYZ(point.XYZ() + aV.XYZ());

			return newpoint;
	}



QList<Handle(Geom_Curve)> HSF::getCurveByHeight(TopoDS_Shape theface,bool direction)
{
QList<TopoDS_Shape> edgelist;
QList<Handle(Geom_Curve)> curvelist;

TopExp_Explorer Ex;
for (Ex.Init(theface,TopAbs_EDGE); Ex.More(); Ex.Next())
{
	TopoDS_Shape curedge = Ex.Current();
	edgelist << curedge;
}// end of edge traversal

TopoDS_Shape lowedge;
TopoDS_Shape highedge;
if (edgelist.count()> 1) 
	{

	gp_Pnt poncrv1  = AddNewPointonCurve(edgelist.at(0),0.5);
	gp_Pnt poncrv2  = AddNewPointonCurve(edgelist.at(1),0.5);
	gp_Pnt middle = gp_Pnt((poncrv1.XYZ() + poncrv2.XYZ())/2);

	double zval = middle.Z();
	if(!direction) zval = zval *-1;

	if (poncrv1.Z() > zval) 
		{lowedge = edgelist.at(0);}
	else
		{highedge = edgelist.at(1);}

	Standard_Real aFP, aLP, aP;
	Handle(Geom_Curve) lCurve = BRep_Tool::Curve(TopoDS::Edge(lowedge), aFP, aLP);
	Handle(Geom_Curve) hCurve = BRep_Tool::Curve(TopoDS::Edge(highedge), aFP, aLP);

	curvelist << lCurve;
	curvelist << hCurve;

	return curvelist;

	}
else
	{
	Standard_Real aFP, aLP, aP;
	Handle(Geom_Curve) lCurve = BRep_Tool::Curve(TopoDS::Edge(edgelist.at(0)), aFP, aLP);
	curvelist << lCurve;

	}






	


}


TopoDS_Shape HSF::BooleanGlue(TopoDS_Shape Stock,TopoDS_Shape Tool)
	{
	TopoDS_Shape Result;
			try
				{
				Result = BRepAlgoAPI_Fuse(Stock,Tool);
				 
				}
			catch(...)
				{
				
				}

		return Result;

		}

 TopoDS_Shape HSF::BooleanCommon(TopoDS_Shape Stock,TopoDS_Shape Tool)
	{
	int count = 0;
	if(Stock.IsNull()) count++;
	if(Tool.IsNull()) count++;

	if(count > 0) return TopoDS_Shape();

	TopoDS_Shape Result ;
			try
				{
				Result = BRepAlgoAPI_Common(Stock,Tool);

					
				 
				}
			catch(...)
				{
				
				}

		return Result;

		}


TopoDS_Shape HSF::BooleanSubstract(TopoDS_Shape Stock,TopoDS_Shape Tool)
	{
		
	TopoDS_Shape Result;
			try
				{
					if (!Stock.IsNull() && !Tool.IsNull())
					{
						Result = BRepAlgoAPI_Cut(Stock,Tool);
					}
				}
			catch(Standard_Failure)
				{
				
				}

		return Result;

		}


TopoDS_Shape HSF::BooleanSubstractFiller(TopoDS_Shape Stock,TopoDS_Shape Tool,const BOPTools_DSFiller& filler)
	{
	TopoDS_Shape Result;
			try
				{

				BRepAlgoAPI_Cut aBCt(Stock,Tool,filler);
				if (aBCt.IsDone()) {
					Result = aBCt.Shape();
					}
				}
			catch(...)
				{
				
				}

		return Result;

		}


TopoDS_Shape HSF::AddNewConicalSurface(gp_Pnt origin,gp_Dir dir,double angle,double radius  )
	{

// creating an axis parallel to Z axis
  gp_Ax3 anAx(origin, dir); 
  // creating a conical surface with base radius = 10 and angle = 20 deg
  Handle(Geom_ConicalSurface) aConicalSurface = new Geom_ConicalSurface(anAx,angle, radius);

  // only finit surfaces can be converted to BSpline surfaces, 
  // conical surface is infinite, it must be trimmed


  Handle(Geom_RectangularTrimmedSurface) aSurface = 
    new Geom_RectangularTrimmedSurface(aConicalSurface, 0, 2*PI, -1000, 1000, Standard_True, Standard_True);

  Handle_Geom_BSplineSurface bsplinesurf = 
  GeomConvert::SurfaceToBSplineSurface(aSurface);
  const Handle_Geom_Surface& theSurface = Handle_Geom_Surface::DownCast(bsplinesurf);

  Standard_Real u1, u2, v1, v2;
  theSurface->Bounds(u1,u2,v1,v2);
  fixParam(u1);
  fixParam(u2);
  fixParam(v1);
  fixParam(v2);

  TopoDS_Shape Result = BRepBuilderAPI_MakeFace (theSurface, u1, u2, v1, v2);
  return Result;


	}





TopoDS_Shape HSF::AddNewConicalSurface(gp_Pnt p1,gp_Pnt p2,gp_Pnt p3,gp_Pnt p4)
	{
	
  TopoDS_Shape Result;
  Handle(Geom_Surface) aConicalSurface = GC_MakeConicalSurface(p1,p2,p3,p4);

  if (aConicalSurface.IsNull()) return Result;
  Standard_Real u1, u2, v1, v2;
  aConicalSurface->Bounds(u1,u2,v1,v2);
  fixParam(u1);
  fixParam(u2);
  fixParam(v1);
  fixParam(v2);

  Result = BRepBuilderAPI_MakeFace (aConicalSurface/*, u1, u2, v1, v2*/);
  return Result;

	

	}

	TopoDS_Shape HSF::AddNewThickSolid(TopoDS_Shape theface,double thickness1, double thickness2)
		{
		
		TopoDS_Shape aShape ;
		try{
			BRepOffset_MakeOffset myoffset(theface, thickness1,0.0001);
			myoffset.MakeThickSolid();
			aShape = myoffset.Shape();
			}catch(...)
			{

			}

		

		return aShape;

}

	/**
	* Generate a hyperboloid using the formula from Wolfram alpha
	* http://mathworld.wolfram.com/Hyperboloid.html
	* Currently revolves a line about an axis,
	* Ideally this formula could be used to make a fully paramatised surface.
	*
	* @param innerRadius
	* @param height
	* @param heightUnder
	* @param angle
	*/
	TopoDS_Shape HSF::AddNewHyperboloidbyFormula(double innerRadius, double height, double heightUnder, double angle,gp_Pnt Origin, gp_Vec Dir)
	{
		int detail = 40;

		int uCount = detail;
		double a = innerRadius;
		double c = angle;
		double totalHeight = height + heightUnder;
		TColgp_Array1OfPnt array (0,uCount - 1);

		for (double u = 0; u < uCount; u++)
		{	
			double uValue = ((totalHeight * u / (uCount - 1)) - heightUnder) / c;
			double vValue = 0;

			double sqrMe = 1 + uValue * uValue;

			double x = a * sqrt(sqrMe) * cos(vValue);
			double y = a * sqrt(sqrMe) * sin(vValue);
			double z = c * uValue;
			gp_Pnt P1(x,y,z);   
			array.SetValue(u,P1); 
		}
	                          
		Handle(Geom_BSplineCurve) hyperbola = GeomAPI_PointsToBSpline(array).Curve();
		TopoDS_Edge hyperbolaTopoDS = BRepBuilderAPI_MakeEdge(hyperbola); 

		gp_Ax1 axis = gp_Ax1(Origin,Dir); 
		TopoDS_Shape hyperboloid = BRepPrimAPI_MakeRevol(hyperbolaTopoDS, axis); 

		return hyperboloid;
	}


	/**
	* Moves a shape by a vector.
	* Should probably return void and just manipulate the shape???
	*/
	TopoDS_Shape HSF::move(TopoDS_Shape shape, gp_Vec moveVector)
	{
		gp_Trsf translation; 
		translation.SetTranslation(moveVector);
		shape.Move(TopLoc_Location(translation));

		return shape;
	}

		/**
	* Moves a shape from a point to a point
	* 
	*/
	TopoDS_Shape HSF::movepointopoint(TopoDS_Shape shape, gp_Pnt p1,gp_Pnt p2)
	{
		gp_Trsf translation; 
		translation.SetTranslation(p1,p2);
		shape.Move(TopLoc_Location(translation));

		return shape;
	}


	gp_Pnt HSF::ProjectPointonCurve(gp_Pnt p1, TopoDS_Shape edge)
		{
		double fp,lp;
		gp_Pnt curpoint;
		edge = HSF::getedgefromshape(edge);
		Handle(Geom_Curve) C = BRep_Tool::Curve(TopoDS::Edge(edge),fp,lp);

			GeomAPI_ProjectPointOnCurve Proj (p1, C);
				if (Proj.NbPoints() > 0)
				{
					curpoint = Proj.NearestPoint();
					
				}
					

		return curpoint;

		}
	
	TopoDS_Shape HSF::rotate(TopoDS_Shape shape, gp_Pnt rotorigin,gp_Vec rotvec,double angle)
	{
		angle = (Pi/180) * angle;
		gp_Trsf translation; 
		translation.SetRotation(gp_Ax1(rotorigin,rotvec),angle);
		BRepBuilderAPI_Transform aBRepTrsf(shape , translation,true);
		

		return aBRepTrsf.Shape() ;
	}

		TopoDS_Shape HSF::axistoaxis(TopoDS_Shape shape, gp_Ax3 fromsystem,gp_Ax3 tosystem)
	{
		gp_Trsf translation; 
		translation.SetDisplacement(fromsystem,tosystem);
		BRepBuilderAPI_Transform aBRepTrsf(shape , translation,true);
		return aBRepTrsf.Shape() ;


	}
TopoDS_Face HSF::AddNewHyberbolicShell(gp_Pnt origin,gp_Vec dir,double angle,double radius , double length )
	{

  angle = (Pi/180) * angle;
  gp_Ax1 A1(origin,dir);
  gp_Ax2 A2(origin,dir);
  Geom_Circle circle = Geom_Circle(A2, radius);
  gp_Pnt pointaway;
  circle.D0(0.5,pointaway);
  

  //rotation axis
  gp_Vec rotdir(origin,pointaway);
  gp_Ax1 rotaxis(pointaway,gp_Dir(rotdir));


  // line to rotate about rotation axis
  Handle(Geom_Curve) spinaxis = new Geom_Line (pointaway,dir);
  // Perform rotation
  gp_Trsf trf;
  trf.SetRotation(rotaxis,angle);
  spinaxis->Transform(trf);
  //trim infinite line
  spinaxis = new Geom_TrimmedCurve (spinaxis, -length, length);

  Handle(Geom_SurfaceOfRevolution) revolved  = new Geom_SurfaceOfRevolution(spinaxis,A1);
  
  

  //Handle_Geom_BSplineSurface bsplinesurf = 
  //GeomConvert::SurfaceToBSplineSurface(aSurface);
  const Handle_Geom_Surface& theSurface = Handle_Geom_Surface::DownCast(revolved);

  Standard_Real u1, u2, v1, v2;
  theSurface->Bounds(u1,u2,v1,v2);
  fixParam(u1);
  fixParam(u2);
  fixParam(v1);
  fixParam(v2);

  TopoDS_Face Result = BRepBuilderAPI_MakeFace(theSurface, u1, u2, v1, v2);
  return Result;


	}


TopoDS_Shape HSF::AddNewRevolvedHyberbolaSolid(gp_Pnt origin,gp_Vec dir,double angle,double radius , double length )
	{

  BRep_Builder aBB;
  TopoDS_Solid aNewSolid;
  TopoDS_Shell aNewShell;
  aBB.MakeSolid(aNewSolid);
  aBB.MakeShell(aNewShell);

  
  TopoDS_Face aFace = AddNewHyberbolicShell(origin,dir,angle,radius,length);
  aBB.Add(aNewShell, aFace);
  aBB.Add(aNewSolid,aNewShell);

  
  return  aNewSolid;


	}

TopoDS_Face HSF::convertgeomsurfaceface(Handle_Geom_Surface thesrf)
	{
	
	TopoDS_Face Result = BRepBuilderAPI_MakeFace(thesrf);
	//TopoDS_Shape Result = revolved.Solid();
	return Result;


	}

Handle_Geom_Curve HSF::convertshapetocurve(TopoDS_Shape theedge)
	{
	double fp,lp;
	Handle_Geom_Curve Result = BRep_Tool::Curve(TopoDS::Edge(theedge),fp,lp);
	//TopoDS_Shape Result = revolved.Solid();
	return Result;


	}

Handle_Geom_Surface HSF::convertfacetogeom(TopoDS_Shape aFace)
	{
	
	Handle_Geom_Surface theSurface;
	TopExp_Explorer Ex;
	 
	for (Ex.Init(aFace,TopAbs_FACE); Ex.More(); Ex.Next())
	{
	TopoDS_Face face = TopoDS::Face(Ex.Current());
	theSurface = BRep_Tool::Surface(face);
    }

	return theSurface;
	}




TopoDS_Edge HSF::AddNewLineptdir(gp_Pnt origin, gp_Vec dir, double length1, double length2)
	{

	Handle(Geom_Curve) spinaxis = new Geom_Line (origin,dir);
    spinaxis = new Geom_TrimmedCurve (spinaxis, length1, length2);
	TopoDS_Edge Result = BRepBuilderAPI_MakeEdge(spinaxis);

	return Result;

	}
TopoDS_Edge HSF::AddNewLineptpt(gp_Pnt p1, gp_Pnt p2)
	{

		TopoDS_Edge aShape;
		if ( !(p1.Distance(p2) > 0)) return aShape; 
			
		aShape = BRepBuilderAPI_MakeEdge(p1, p2);

		return aShape;

	}

QMap<QString,gp_Ax1>  HSF::BuildPointGridonSrf(TopoDS_Shape aShape,double ucount, double vcount)
	{
	
	QMap<QString,gp_Ax1> pointmap;
	Handle_Geom_Surface theSurface;
	TopExp_Explorer Ex;
	for (Ex.Init(aShape,TopAbs_FACE); Ex.More(); Ex.Next())
	{
	theSurface = BRep_Tool::Surface(TopoDS::Face(Ex.Current()));
    }

	if (theSurface.IsNull()) return pointmap;
	
	Standard_Real u1, u2, v1, v2;
	theSurface->Bounds(u1,u2,v1,v2);
	

	TopoDS_Face aFace = HSF::convertgeomsurfaceface(theSurface);
	/*QProgressDialog progress("Generating Grid Points", "Abort Copy", 1,100);
	progress.setValue(0);*/
	int count = 0;
	for(int i=0;i<ucount;i++)
		{
		for(int j=0;j<vcount;j++)
		{
		count +=1;
		
		gp_Pnt isopoint;
		theSurface->D0(u1+(((u2-u1)/(ucount-1))*i),v1+(((v2-v1)/(vcount-1))*j),isopoint);
		QString key = "U" + QString::number(i,char(103),3)+ "_"+"V" + QString::number(j,char(103),3);
		gp_Vec isovec = getVectorNormaltoSurfaceatPoint(aFace,isopoint);
		gp_Ax1 value(isopoint,gp_Dir(isovec));
		pointmap.insert(key,value);
		
		/*progress.setValue((count * 100) / (ucount * vcount));
		QCoreApplication::instance()->processEvents();*/
		}
		}

	return pointmap;

	}






QMap<QString,gp_Ax1>  HSF::BuildRandPointsonSrf(TopoDS_Shape aShape,double numberofpoints)
	{
	
	QMap<QString,gp_Ax1> pointmap;
	Handle_Geom_Surface theSurface;
	TopExp_Explorer Ex;
	for (Ex.Init(aShape,TopAbs_FACE); Ex.More(); Ex.Next())
	{
	theSurface = BRep_Tool::Surface(TopoDS::Face(Ex.Current()));
    }
	
	Standard_Real u1, u2, v1, v2;
	theSurface->Bounds(u1,u2,v1,v2);
	

	TopoDS_Face aFace = HSF::convertgeomsurfaceface(theSurface);

	srand(static_cast<unsigned int>(clock()));
	/*QProgressDialog progress("Generating Grid Points", "Abort Copy", 1,100);
	progress.setValue(0);*/
	int count = 0;
	for(int i=0;i<numberofpoints;i++)
		{
	
		gp_Pnt isopoint;
		double uval = double(rand()) / double(RAND_MAX) ;
		uval = uval * (u2-u1);
		double vval = double(rand()) / double(RAND_MAX) ; 
		vval = vval * (v2-v1);

		theSurface->D0(uval,vval,isopoint);
		QString key = "P" + QString::number(i,char(103),3);
		gp_Vec isovec = getVectorNormaltoSurfaceatPoint(aFace,isopoint);
		gp_Ax1 value(isopoint,gp_Dir(isovec));
		pointmap.insert(key,value);
		ui::getInstance()->Statusbar(key);
		}
		

	return pointmap;

	}










QString  HSF::GetNextUvName(QString Name, int uoffset, int voffset)
	{
		
	 QString mykey = Name;
	 QString upart = mykey.split("_").at(0);
	 upart = upart.right(upart.length() -1);

	 QString vpart = mykey.split("_").at(1);
	 vpart = vpart.right(vpart.length() -1);

	 int NewUpart = upart.toInt() + uoffset;
	 int NewVpart = vpart.toInt() + voffset;

	 	QString NewName;
		QTextStream out(&NewName);
		out << "U" << NewUpart << "_" << "V" << NewVpart;
		

	

	 return NewName;


	}

QString  HSF::GetNextUvNameCiclic(QString Name, int uoffset, int voffset, int count)
	{
		
	 QString mykey = Name;
	 QString upart = mykey.split("_").at(0);
	 upart = upart.right(upart.length() -1);

	 QString vpart = mykey.split("_").at(1);
	 vpart = vpart.right(vpart.length() -1);

	 int NewUpart = upart.toInt() + uoffset;
	 int NewVpart = vpart.toInt() + voffset;

	 if (NewUpart < 1)
		 {
		  NewUpart = count + (uoffset + 1);
		 }else if(NewUpart > count)
		 {
		  NewUpart = 0 + (uoffset - (count - upart.toInt()));
		 }

	  if (NewVpart < 1)
		 {
		  NewVpart = count + (voffset + 1);
		 }else if(NewVpart > count)
		 {
		  NewVpart = 0 + (voffset - (count - vpart.toInt()));
		 }




	 	QString NewName;
		QTextStream out(&NewName);
		out << "U" << NewUpart << "_" << "V" << NewVpart;
		
	 return NewName;


	}




TopoDS_Shape HSF::AddNewLoftSurface(QList<TopoDS_Shape> sections)
	{

	BRepOffsetAPI_ThruSections aBuilder;
	aBuilder = BRepOffsetAPI_ThruSections(false,false,Precision::Confusion());
	aBuilder.CheckCompatibility(Standard_False);
	
	int buildcount = 0;
	QListIterator<TopoDS_Shape> glistIT(sections);
	while (glistIT.hasNext())
	{
	
	TopoDS_Shape currentshape = glistIT.next();
	TopoDS_Shape aShapeSection = currentshape;
	if(currentshape.IsNull()) continue;

					TopAbs_ShapeEnum aTypeSect = aShapeSection.ShapeType();
					if(aTypeSect == TopAbs_WIRE){
					  aBuilder.AddWire(TopoDS::Wire(aShapeSection));
					buildcount++;
					}	
					else if(aTypeSect == TopAbs_EDGE) {
					  TopoDS_Edge anEdge = TopoDS::Edge(aShapeSection);
					  double edgelength = hsf::GetLength(anEdge);
					  if (edgelength > 0)
					  {
					  TopoDS_Wire aWire = BRepBuilderAPI_MakeWire(anEdge);
					  aBuilder.AddWire(aWire);
					  buildcount++;
					  }
					
					}else if(aTypeSect == TopAbs_VERTEX) {
					  TopoDS_Vertex aVert = TopoDS::Vertex(aShapeSection);
					  aBuilder.AddVertex(aVert);
					  buildcount++;
					}
	}
	
	if (buildcount > 0) 
	{
		aBuilder.Build();
		if (aBuilder.Shape().IsNull()) { 
		TopoDS_Shape nullshape;
		return nullshape; }
		TopoDS_Shape aShape = aBuilder.Shape();
		return aShape;
	}else {
		return TopoDS_Shape();
	}

	}

TopoDS_Shape HSF::AddNewBox(gp_Pnt center, gp_Vec Orient, double width, double height, double depth)
	{
		gp_Pnt origin(center.X()-(width/2),center.Y()-(height/2),center.Z()-(depth/2));
		TopoDS_Solid thebox = BRepPrimAPI_MakeBox(origin,width,height,depth);
		return thebox;



	}

TopoDS_Shape HSF::AddNewPolyline(QList<gp_Pnt> Points)
	{

	  BRepBuilderAPI_MakePolygon W;
	
	
	  QListIterator<gp_Pnt> iT(Points);
	  int count = 0;
	  while (iT.hasNext())
	  {
	  gp_Pnt curpoint = iT.next();
	  W.Add(curpoint);
	  count +=1;
	  }

	 return W.Shape();

	}


TopoDS_Shape HSF::AddNewRectangle(gp_Pnt center, gp_Vec orient, double width, double height)
	{

	Geom_Plane myplane(center,orient);
	gp_Pnt cp;
	gp_Vec v1;
	gp_Vec v2;
	myplane.D1(0,0,cp,v1,v2);

	gp_Pnt p1 = center; 
	p1 = HSF::MovePointByVector(p1,v1,-(width/2));
	p1 = HSF::MovePointByVector(p1,v2,(height/2));

	
	gp_Pnt p2 = center; 
	p2 = HSF::MovePointByVector(p2,v1,(width/2));
	p2 = HSF::MovePointByVector(p2,v2,(height/2));

	gp_Pnt p3 = center; 
	p3 = HSF::MovePointByVector(p3,v1,(width/2));
	p3 = HSF::MovePointByVector(p3,v2,-(height/2));

	gp_Pnt p4 = center; 
	p4 = HSF::MovePointByVector(p4,v1,-(width/2));
	p4 = HSF::MovePointByVector(p4,v2,-(height/2));

	//gp_Pnt p1(center.X()-(width/2),center.Y()+(height/2),0);
	//gp_Pnt p2(center.X()+(width/2),center.Y()+(height/2),0);
	//gp_Pnt p3(center.X()+(width/2),center.Y()-(height/2),0);
	//gp_Pnt p4(center.X()-(width/2),center.Y()-(height/2),0);


	BRepBuilderAPI_MakePolygon W;
	
	W.Add(p1);
	W.Add(p2);
	W.Add(p3);
	W.Add(p4);
	W.Add(p1);

	double angle = orient.Angle(gp::DY());
	angle = (Pi*180) / angle;

	

	//gp_Pnt uppoint = MovePointByVector(center,gp::DZ(),10);
	//gp_Vec rotvec(center,uppoint);

	TopoDS_Shape orientedshape;
	//orientedshape = rotate(W,center,rotvec,angle);
	orientedshape = W;


	return orientedshape;


	


	}
TopoDS_Shape HSF::AddNewBlendSurface(TopoDS_Shape crv1, TopoDS_Shape crv2)
	{

	if (crv1.IsNull() || crv2.IsNull()) return TopoDS_Shape();


	BRepOffsetAPI_ThruSections aBuilder;
	aBuilder = BRepOffsetAPI_ThruSections(false,true,Precision::Confusion());
	aBuilder.CheckCompatibility(Standard_False);

	QList<TopoDS_Shape> sections;
    sections << crv1;
    sections << crv2;

	QListIterator<TopoDS_Shape> glistIT(sections);
	while (glistIT.hasNext())
	{
	
	TopoDS_Shape currentshape = glistIT.next();
	TopoDS_Shape aShapeSection = currentshape;

					TopAbs_ShapeEnum aTypeSect = aShapeSection.ShapeType();
					if(aTypeSect == TopAbs_WIRE)
					  aBuilder.AddWire(TopoDS::Wire(aShapeSection));

					else if(aTypeSect == TopAbs_EDGE) {
					  TopoDS_Edge anEdge = TopoDS::Edge(aShapeSection);
					  TopoDS_Wire aWire = BRepBuilderAPI_MakeWire(anEdge);
					  aBuilder.AddWire(aWire);
					}
					else if(aTypeSect == TopAbs_VERTEX) {
					  TopoDS_Vertex aVert = TopoDS::Vertex(aShapeSection);
					  aBuilder.AddVertex(aVert);
					}
	}

	aBuilder.Build();
	if (aBuilder.Shape().IsNull()) { 
		TopoDS_Shape nullshape;
		return nullshape; }
	TopoDS_Shape aShape = aBuilder.Shape();
	
	TopExp_Explorer Ex;
	for (Ex.Init(aShape,TopAbs_FACE); Ex.More(); Ex.Next())
		{
			aShape = Ex.Current();
		}

	return aShape;
	}

TopoDS_Shape HSF::getfacefromshape(TopoDS_Shape shape1)
	{
	TopoDS_Shape myshape=shape1 ;
	TopExp_Explorer Ex;
	for (Ex.Init(myshape,TopAbs_FACE); Ex.More(); Ex.Next())
		{
			myshape = Ex.Current();
		}
	 return myshape;
	}

TopoDS_Shape HSF::getedgefromshape(TopoDS_Shape shape1)
	{
	TopoDS_Shape myshape=shape1 ;
	TopExp_Explorer Ex;
	for (Ex.Init(myshape,TopAbs_EDGE); Ex.More(); Ex.Next())
		{
			myshape = Ex.Current();
		}
	 return myshape;
	}

TopoDS_Shape HSF::AddNewFillSurface(QList<TopoDS_Shape> shapelist)
	{

	TopoDS_Shape Result;
	QList<TopoDS_Shape>::iterator i;
	 BRepFill_Filling* myfill = new BRepFill_Filling();
	 int count=0;
	for (i = shapelist.begin(); i != shapelist.end(); ++i)
		{
			TopoDS_Shape edge = *i;
			if ( edge.ShapeType() == TopAbs_ShapeEnum::TopAbs_EDGE)
			{
			 count ++;
			 TopoDS_Edge aEdge = TopoDS::Edge(*i);
			 myfill->Add(aEdge,GeomAbs_Shape::GeomAbs_C0);
			}
	
		}

	if (count > 0)
		{
			myfill->Build();
			Result = myfill->Face();
		}
	return Result;



	}

TopoDS_Shape HSF::AddNewFillSurface(TopoDS_Shape edge)
	{

	TopoDS_Shape Result;

	if ( edge.ShapeType() == TopAbs_ShapeEnum::TopAbs_EDGE)
		{
		TopoDS_Edge aEdge = TopoDS::Edge(edge);

	BRepFill_Filling* myfill = new BRepFill_Filling();
	myfill->Add(aEdge,GeomAbs_Shape::GeomAbs_C0);
	myfill->Build();
    Result = myfill->Face();

	return Result;
		} else if ( edge.ShapeType() == TopAbs_ShapeEnum::TopAbs_WIRE)
		{
		BRepFill_Filling* myfill = new BRepFill_Filling();
		
		TopExp_Explorer Ex;
		for (Ex.Init(edge,TopAbs_EDGE); Ex.More(); Ex.Next())
		{
			TopoDS_Shape myshape = Ex.Current();
			TopoDS_Edge aEdge = TopoDS::Edge(myshape);
			myfill->Add(aEdge,GeomAbs_Shape::GeomAbs_C0);
		}
		
		myfill->Build();
		Result = myfill->Face();

		return Result;

		}
	return Result;


	}
// adds a new circle passing through 3 points
//@parm p1
//@parm p2
//@parm p3

TopoDS_Edge HSF::AddNewCircle(gp_Pnt p1,gp_Pnt p2,gp_Pnt p3)
	{

	Handle_Geom_Circle mycircle = GC_MakeCircle(p1,p2,p3);
	
	TopoDS_Edge Result;
	if (mycircle.IsNull()) {return Result;}
	try
		{
		Result= BRepBuilderAPI_MakeEdge(mycircle);
		}
	catch(...)
		{
	
	return Result;
		}
	return Result;

	}

gp_Pnt HSF::GetCenterCircle(TopoDS_Shape circle)
	{

	
	
	gp_Pnt Result;
	if (circle.IsNull()) {return Result;}
	try
		{
		double fp,lp;
		Handle_Geom_Curve C = BRep_Tool::Curve(TopoDS::Edge(circle),fp,lp);

	Handle_Geom_Circle mycircle = Handle_Geom_Circle::DownCast(C);
	gp_Pnt Center = mycircle->Circ().Location();
	Result = Center;
		}
	catch(...)
		{
	
	return Result;
		}
	return Result;

	}

TopoDS_Shape HSF::AddNewSweep(TopoDS_Shape path, TopoDS_Shape crossection)
	{

	TopoDS_Edge Ec = TopoDS::Edge(crossection);
	TopoDS_Wire Wc = BRepBuilderAPI_MakeWire(TopoDS::Edge(path));
	//TopoDS_Face F = BRepBuilderAPI_MakeFaceAPI_MakePipe(W,F);
	BRepFill_Pipe mypipe =  BRepFill_Pipe(Wc,Ec);
	return mypipe.Shape();


	}

TopoDS_Shape HSF::AddNewArcofCircle(gp_Pnt p1,gp_Pnt p2,gp_Pnt p3)
	{

	Handle_Geom_TrimmedCurve mycircle = GC_MakeArcOfCircle(p1,p2,p3);

	TopoDS_Edge Result;
	if (mycircle.IsNull()) {return Result;}
	try
		{
		Result= BRepBuilderAPI_MakeEdge(mycircle);
		}
	catch(...)
		{
	
	return Result;
		}
	return Result;


	}

gp_Pln HSF::AddNewPlane(gp_Pnt p1,gp_Pnt p2,gp_Pnt p3)
	{
	Handle_Geom_Plane myplane =  GC_MakePlane (p1, p2, p3);
	return myplane->Pln();

	}


//Make a Plane from Geom <theplane> parallel to another 
//Pln <pln> and passing through a Pnt <point>. 
gp_Pln HSF::AddNewPlane(gp_Pln Pln, gp_Pnt Point)
	{
	Handle_Geom_Plane myplane =  GC_MakePlane (Pln, Point);
	return myplane->Pln();
	}

//Make a Plane from Geom <theplane> parallel to another 
//Pln <pln> at the distance <dist> which can be greater 
//or lower than zero. 
gp_Pln HSF::AddNewPlane(gp_Pln Pln, Standard_Real Dist)
	{
	Handle_Geom_Plane myplane =  GC_MakePlane (Pln, Dist ); 
	return myplane->Pln();
	}

TopoDS_Shape HSF::AddNewCorner(gp_Pnt p1, gp_Pnt p2 , gp_Pnt p3, double radius, gp_Pnt target)
	{
TopoDS_Shape result;
try
	{
gp_Pnt centerp = target;
gp_Pln avrgplane = HSF::AddNewPlane(p1,p2,p3);

gp_Pnt2d p12d = HSF::Get2dPntonSurfacefromPoint(avrgplane,p1);
gp_Pnt2d p22d = HSF::Get2dPntonSurfacefromPoint(avrgplane,p2);
gp_Pnt2d p32d = HSF::Get2dPntonSurfacefromPoint(avrgplane,p3);


gp_Lin2d l1 = GccAna_Lin2d2Tan(p12d,p22d,Precision::Confusion()).ThisSolution(1);
gp_Lin2d l2 = GccAna_Lin2d2Tan(p22d,p32d,Precision::Confusion()).ThisSolution(1);

GccEnt_QualifiedLin QL = GccEnt::Unqualified(l1);	
GccEnt_QualifiedLin QR = GccEnt::Unqualified(l2);
//Standard_Real radius2 = p1.Distance(p3)/5;                                                     
GccAna_Circ2d2TanRad TR (QL,QR,radius,Precision::Confusion()); 

int nbsols = TR.NbSolutions();
//qDebug() << "current panel has" << nbsols << "solutions";

double mindis = 10000000000000000;
TopoDS_Edge bestsol ;

for(int j = 1; j < nbsols+1; j++)
{
gp_Circ2d circ = TR.ThisSolution(j);

Standard_Real parsol,pararg;   
gp_Pnt2d tp1,tp2;  

TR.Tangency1(j,parsol,pararg,tp1);                        
TR.Tangency2(j,parsol,pararg,tp2); 

Handle(Geom2d_Circle) circ2d = new Geom2d_Circle(circ.Axis(),circ.Radius());    
Handle(Geom_Plane) plane3d = new Geom_Plane(avrgplane);
gp_Pnt tp13d,tp23d;
plane3d->D0(tp1.X(),tp1.Y(),tp13d);
plane3d->D0(tp2.X(),tp2.Y(),tp23d);

Handle(Geom_Curve) C3D = GeomAPI::To3d(circ2d,avrgplane);    
TopoDS_Edge corner =  BRepBuilderAPI_MakeEdge(C3D,tp13d,tp23d);     
gp_Pnt circog = HSF::GetCenterCircle(corner);

if (circog.Distance(centerp) < mindis)
	{
		mindis = circog.Distance(centerp);
		bestsol =corner;
	}
	}
result = bestsol;
	}
catch(...) { return result;}

return result;


	}
TopoDS_Edge HSF::AddNewCircle(const gp_Pnt& Center,const gp_Dir& Norm,const double Radius)
	{

	Handle_Geom_Circle mycircle = GC_MakeCircle(Center,Norm,Radius);
	
	
	TopoDS_Edge Result;
	if (mycircle.IsNull()) {return Result;}
	try
		{
		Result= BRepBuilderAPI_MakeEdge(mycircle);
		}
	catch(...)
		{
	
	return Result;
		}
	return Result;

	}

TopoDS_Edge HSF::AddNewHyberbolicCrv(gp_Pnt origin, gp_Vec dir,double maxradius,double minradius)
	{


  
  gp_Ax2 A2(origin,dir);
  Geom_Circle circle = Geom_Circle(A2, (maxradius + minradius)/2);
  gp_Pnt pointaway;
  circle.D0(0.5,pointaway);
	
  gp_Ax2 rotA2(pointaway,dir);
  Handle(Geom_Curve) hb = new Geom_Hyperbola(rotA2, maxradius, minradius);
  Standard_Real aFP, aLP;
  aFP = hb->FirstParameter();
  aLP = hb->LastParameter();

  //Handle(Geom_Curve) hbtrim = new Geom_TrimmedCurve (hb, 1000, 1000);

  
			
  TopoDS_Edge Result = BRepBuilderAPI_MakeEdge(hb,aFP,aLP);

  return Result;



	}

TopoDS_Shape HSF::AddNewConstrainedSurface(QList<TopoDS_Shape> shapelist)
	{

	GeomPlate_BuildPlateSurface BPSurf(3,15,2);

	QListIterator<TopoDS_Shape> glistIT(shapelist);
	while (glistIT.hasNext())
	{
	
	TopoDS_Shape currentshape = glistIT.next();
	TopoDS_Shape aShapeSection = currentshape;

					TopAbs_ShapeEnum aTypeSect = aShapeSection.ShapeType();
					if(aTypeSect == TopAbs_WIRE)
						{

						}

					else if(aTypeSect == TopAbs_EDGE) {
					  TopoDS_Edge E = TopoDS::Edge(aShapeSection);
						Handle(BRepAdaptor_HCurve) C = new BRepAdaptor_HCurve();
						C->ChangeCurve().Initialize(TopoDS::Edge(E));
						Handle(BRepFill_CurveConstraint) Cont= new
						BRepFill_CurveConstraint(C,0);
						BPSurf.Add(Cont);
					}
					else if(aTypeSect == TopAbs_VERTEX) {
					  TopoDS_Vertex aVert = TopoDS::Vertex(aShapeSection);
					  gp_Pnt vertp = BRep_Tool::Pnt(aVert);
					  // Point constraint
						Handle(GeomPlate_PointConstraint) PCont= new
						GeomPlate_PointConstraint(vertp,0);
						BPSurf.Add(PCont);
					}
	}






// Compute the Plate surface
BPSurf.Perform();
// Approximation of the Plate surface
Standard_Integer MaxSeg=9;
Standard_Integer MaxDegree=8;
Standard_Integer CritOrder=0;

Standard_Real dmax,Tol;
Handle(GeomPlate_Surface) PSurf = BPSurf.Surface();
dmax = std::max(0.0001,10*BPSurf.G0Error());
Tol=0.0001;
GeomPlate_MakeApprox Mapp(PSurf,Tol,MaxSeg,MaxDegree,dmax,CritOrder);
Handle (Geom_Surface) Surf (Mapp.Surface());
// create a face corresponding to the approximated Plate

Standard_Real Umin, Umax, Vmin, Vmax;
PSurf->Bounds( Umin, Umax, Vmin, Vmax);
TopoDS_Face Result = BRepBuilderAPI_MakeFace(Surf,Umin, Umax, Vmin, Vmax);


return Result;



	}


TopoDS_Shape HSF::AddNewSymmetry(TopoDS_Shape shape, gp_Pln pln1)
	{

		
		gp_Trsf symmetry; 
		symmetry.SetMirror(gp_Ax2(pln1.Location(),pln1.Axis().Direction(),pln1.XAxis().Direction()));

		BRepBuilderAPI_Transform aBRepTrsf(shape , symmetry,true);
		

		return aBRepTrsf.Shape() ;

	}


TopoDS_Shape HSF::AddNewSphereSurface(gp_Pnt center, double radius)
	{

	                      
  Handle(Geom_SphericalSurface) SP = new Geom_SphericalSurface(gp_Ax3(center,gp::DZ()),radius);
	
  Standard_Real u1, u2, v1, v2;
  SP->Bounds(u1,u2,v1,v2);
  fixParam(u1);
  fixParam(u2);
  fixParam(v1);
  fixParam(v2);

  //TopoDS_Shape Result = BRepBuilderAPI_MakeFace(SP, u1, u2, v1, v2);

  TopoDS_Shape Result1 = BRepBuilderAPI_MakeFace(SP, u1, u2/2, v1, v2);
  TopoDS_Shape Result2 = BRepBuilderAPI_MakeFace(SP, u2/2, u2, v1, v2);

  TopoDS_Compound folder;
  BRep_Builder B;
  B.MakeCompound(folder);
  B.Add(folder,Result1);
  B.Add(folder,Result2);

  return folder;

  //return Result;

	}

TopoDS_Shape HSF::AddNewToroidalSurface(gp_Pnt center, gp_Vec upvector, double circumference, double radius)
	{

	gp_Ax3 axis(center,gp_Dir(upvector));
	Handle_Geom_ToroidalSurface mytorus = new Geom_ToroidalSurface(axis,circumference,radius);
	

  Standard_Real u1, u2, v1, v2;
  mytorus->Bounds(u1,u2,v1,v2);
  fixParam(u1);
  fixParam(u2);
  fixParam(v1);
  fixParam(v2);

  TopoDS_Face Result = BRepBuilderAPI_MakeFace(mytorus, u1, u2, v1, v2);
  return Result;

	

	}


TopoDS_Shape HSF::AddNewRevolveSurface(TopoDS_Shape crv1, gp_Pnt origin, gp_Vec dir)
	{
	gp_Ax1 A1(origin,dir);
	gp_Ax2 A2(origin,dir);
	gp_Vec Xv(A2.XDirection().XYZ());
	gp_Vec Yv(A2.YDirection().XYZ());

	gp_Ax1 xA1(origin,dir);
	gp_Ax1 yA1(origin,Yv);


	Standard_Real af, al;
	Handle(Geom_Curve) spincrv = BRep_Tool::Curve(TopoDS::Edge(crv1),af,al);
	Handle(Geom_SurfaceOfRevolution) revolved  = new Geom_SurfaceOfRevolution(spincrv,A1);
	const Handle_Geom_Surface& theSurface = Handle_Geom_Surface::DownCast(revolved);

  Standard_Real u1, u2, v1, v2;
  theSurface->Bounds(u1,u2,v1,v2);
  fixParam(u1);
  fixParam(u2);
  fixParam(v1);
  fixParam(v2);

  TopoDS_Face Result = BRepBuilderAPI_MakeFace(theSurface, u1, u2, v1, v2);
  return Result;

	}


TopoDS_Shape HSF::AddNewPoint(gp_Pnt point)
	{
	 BRepBuilderAPI_MakeVertex mkVertex (point);
	 return mkVertex.Shape();

	}

TopoDS_Shape HSF::AddNewMake2d(TopoDS_Shape obj, gp_Pnt vieworigin, gp_Pnt viewtarget, gp_Vec viewnormal)
	{

	// Build The algorithm object
V3d_Coordinate DX,DY,DZ,XAt,YAt,ZAt, Vx,Vy,Vz ; 
DX = vieworigin.X();
DY = vieworigin.Y();
DZ = vieworigin.Z();
XAt = viewtarget.X();
YAt = viewtarget.Y();
ZAt = viewtarget.Z();
Vx = viewnormal.X();
Vy = viewnormal.Y();
Vz = viewnormal.Z();



Prs3d_Projector aPrs3dProjector(false,1,DX,DY,DZ,XAt,YAt,ZAt,Vx,Vy,Vz);
HLRAlgo_Projector aProjector = aPrs3dProjector.Projector();

Handle_HLRBRep_Algo myAlgo = new HLRBRep_Algo();
// Build the extraction object :
HLRBRep_HLRToShape aHLRToShape(myAlgo);

myAlgo->Projector(aProjector);

// Add Shapes into the algorithm

myAlgo->Add(obj,0);
// Set The Projector (myProjector is a



// Build HLR
myAlgo->Update();

// Set The Edge Status
myAlgo->Hide();



// extract the results :
TopoDS_Shape result;
try
	{
		TopoDS_Compound folder;
	BRep_Builder B;
	B.MakeCompound(folder);
	int viscount=0;

TopoDS_Shape VCompound = aHLRToShape.VCompound();
TopoDS_Shape Rg1LineVCompound = aHLRToShape.Rg1LineVCompound();
TopoDS_Shape RgNLineVCompound = aHLRToShape.RgNLineVCompound();
TopoDS_Shape OutLineVCompound = aHLRToShape.OutLineVCompound();
TopoDS_Shape IsoLineVCompound = aHLRToShape.IsoLineVCompound();
TopoDS_Shape HCompound = aHLRToShape.HCompound();
TopoDS_Shape Rg1LineHCompound = aHLRToShape.Rg1LineHCompound();
TopoDS_Shape RgNLineHCompound = aHLRToShape.RgNLineHCompound();
TopoDS_Shape OutLineHCompound = aHLRToShape.OutLineHCompound();
TopoDS_Shape IsoLineHCompound = aHLRToShape.IsoLineHCompound();

if(!VCompound.IsNull()) B.Add(folder,VCompound);
if(!Rg1LineVCompound.IsNull()) B.Add(folder,Rg1LineVCompound);
if(!RgNLineVCompound.IsNull()) B.Add(folder,RgNLineVCompound);
if(!OutLineVCompound.IsNull()) B.Add(folder,OutLineVCompound);
if(!IsoLineVCompound.IsNull()) B.Add(folder,IsoLineVCompound);
if(!HCompound.IsNull()) B.Add(folder,HCompound);
if(!Rg1LineHCompound.IsNull()) B.Add(folder,Rg1LineHCompound);
if(!RgNLineHCompound.IsNull()) B.Add(folder,RgNLineHCompound);
if(!OutLineHCompound.IsNull()) B.Add(folder,OutLineHCompound);
if(!IsoLineHCompound.IsNull()) B.Add(folder,IsoLineHCompound);


result = folder;
	}
catch(...){}

return result;

	}

TopoDS_Shape HSF::AddNewEllipse(gp_Pnt Origin,gp_Vec Normal, gp_Vec MajorVector, double MajorRadius,double MinorRadius)
	{
	 	gp_Ax2 ellipseAxis  = gp_Ax2 (Origin,Normal,MajorVector);

		//gp_Elips ellipse(ellipseAxis,MajorRadius,MinorRadius);
		Handle_Geom_Ellipse ellipse = new Geom_Ellipse(ellipseAxis, MajorRadius,MinorRadius);
		BRepBuilderAPI_MakeEdge edgebuilder(ellipse);
				
		return edgebuilder.Shape();

	}


static Standard_Boolean fixParam(Standard_Real& theParam)
{
  Standard_Boolean aResult = Standard_False;
  double MAX_PARAM = 1000;
  if (Precision::IsNegativeInfinite(theParam))
  {
    theParam = -MAX_PARAM;
    aResult = Standard_True;
  }
  if (Precision::IsPositiveInfinite(theParam))
  {
    theParam = MAX_PARAM;
    aResult = Standard_True;
  }
  return aResult;
}

//TopoDS_Shape HSF::AddNewSplit(TopoDS_Shape Stock, TopoDS_Shape Tool)
//	{
//
//	// do a section
//
//	TopoDS_Shape ResultShape;
//	
//     BRepAlgoAPI_Section asect(Stock,Tool, Standard_False);
//     asect.ComputePCurveOn1(Standard_True);
//     asect.Approximation(Standard_True);
//     asect.Build();
//
//     if (asect.IsDone()) {
//          // do a split
//          TopoDS_Shape R=asect.Shape();
//          QList<TopoDS_Edge> edges;
//          BRepFeat_SplitShape asplit(Stock);
//          for (TopExp_Explorer Ex(R,TopAbs_EDGE); Ex.More(); Ex.Next()) {
//               TopoDS_Shape anEdge=Ex.Current();
//               TopoDS_Shape aFace;
//               if (asect.HasAncestorFaceOn1(anEdge,aFace)) {
//                    TopoDS_Face F=TopoDS::Face(aFace);
//                    TopoDS_Edge E=TopoDS::Edge(anEdge);
//                    edges.append(E);
//                    asplit.Add(E,F);
//               }
//          }
//          asplit.Build();
//
//          // no edge, shape stay the same, means no intersection between plane and body
//          if (edges.count()==0)
//               return ResultShape;
//          
//          // split is done
//          if (asplit.IsDone()) {
//			
//				ResultShape = asplit.Shape();
//               // go through all faces and keep those you want
//               // in my case, I keept all those that are one the positive side of my 
//               // section plane
//               for (TopExp_Explorer iter(asplit.Shape(),TopAbs_FACE);iter.More();iter.Next()) {
//                    if (iter.Current().ShapeType()==TopAbs_FACE) {
//                         
//			        }
//		        } // end for loop
//			} // end of asplit done
//
//	 } // end of is asect done
//
//	
//	 return ResultShape;
//
//
//	}

TopoDS_Shape HSF::AddNewSplitW(TopoDS_Shape Stock, TopoDS_Shape Wire)
	{
		TopoDS_Shape nullshape;
				
		
		
		TopoDS_Shape R = Wire;

		if (R.IsNull()) return nullshape;


		BRepFeat_SplitShape asplit(Stock);

		for (TopExp_Explorer Ex(R,TopAbs_EDGE); Ex.More(); Ex.Next()) {
			 TopoDS_Shape anEdge = Ex.Current();
			 TopoDS_Shape aFace;
			 
				  TopoDS_Face F = TopoDS::Face(Stock);
				  TopoDS_Edge E = TopoDS::Edge(anEdge);
				  asplit.Add(E,F);
			 
		}

		try
			{
		asplit.Build();
			}

		catch(...)
			{
			 return nullshape;
			}

		if(!asplit.Shape().IsNull())
			{
			return asplit.Shape();
			} else {
			
			return nullshape;
				
			}


	}
TopoDS_Shape HSF::AddNewSplit(TopoDS_Shape Stock, TopoDS_Shape Tool)
	{
		TopoDS_Shape nullshape;
				

		BRepAlgoAPI_Section asect(Stock, Tool,Standard_False);
		asect.ComputePCurveOn1(Standard_True);
		asect.Approximation(Standard_True);
		asect.Build();
		TopoDS_Shape R = asect.Shape();

		if (!asect.ErrorStatus()== 0) return nullshape;


		BRepFeat_SplitShape asplit(Stock);

		for (TopExp_Explorer Ex(R,TopAbs_EDGE); Ex.More(); Ex.Next()) {
			 TopoDS_Shape anEdge = Ex.Current();
			 TopoDS_Shape aFace;
			 if (asect.HasAncestorFaceOn1(anEdge,aFace)) {
				  TopoDS_Face F = TopoDS::Face(aFace);
				  TopoDS_Edge E = TopoDS::Edge(anEdge);
				  asplit.Add(E,F);
			 }
		}

		try
			{
		asplit.Build();
			}

		catch(...)
			{
			 return nullshape;
			}

		if(!asplit.Shape().IsNull())
			{
			return asplit.Shape();
			} else {
			
			return nullshape;
				
			}


	}
TopoDS_Shape HSF::AddNewSplit(TopoDS_Shape Stock, gp_Pln plane1)
	{
		TopoDS_Shape nullshape;
				

		BRepAlgoAPI_Section asect(Stock,plane1,Standard_False);
		asect.ComputePCurveOn1(Standard_True);
		asect.Approximation(Standard_True);
		asect.Build();
		TopoDS_Shape R = asect.Shape();

		if (!asect.ErrorStatus()== 0) return nullshape;


		BRepFeat_SplitShape asplit(Stock);

		for (TopExp_Explorer Ex(R,TopAbs_EDGE); Ex.More(); Ex.Next()) {
			 TopoDS_Shape anEdge = Ex.Current();
			 TopoDS_Shape aFace;
			 if (asect.HasAncestorFaceOn1(anEdge,aFace)) {
				  TopoDS_Face F = TopoDS::Face(aFace);
				  TopoDS_Edge E = TopoDS::Edge(anEdge);
				  asplit.Add(E,F);
			 }
		}

		try
			{
		asplit.Build();
			}

		catch(...)
			{
			 return nullshape;
			}

		if(!asplit.Shape().IsNull())
			{
			return asplit.Shape();
			} else {
			
			return nullshape;
				
			}


	}
TopoDS_Shape HSF::AddNewSplitGeom(TopoDS_Shape Stock, TopoDS_Shape Tool)
	{
	TopoDS_Shape theResult;
	/*GEOMAlgo_Splitter PS;

	PS.AddShape(Stock);
	PS.AddTool(Tool);
	PS.Perform();

	TopoDS_Shape theResult = PS.Shape();*/

	return theResult;

	
	}

QList<QStringList> HSF::ReadDelimitedtoStringList(QString Filename,QString Delimeter)
	{
		QList<QStringList> Lines;
		if (Filename.isEmpty()) return Lines;
		
		

		QFile file(Filename);
		if (!file.open(QFile::ReadOnly)) return Lines;
		CSV csv(&file);
		QString Line("");
		Line= csv.readLine();
		
		qDebug() << "reading Line:" << Line;
		while(Line.size()>0)
		{
		 QStringList valuelist = Line.split(Delimeter);
		 Lines << valuelist;
		
		/*	if (valuelist.size()>1)
			{
				QStringList edges;
				for(int i=0;i<valuelist.size();i++)
					{
						edges << valuelist.at(i);
					}
				vertexedgelist.insert(index,edges);
				qDebug() << index << "," << edges;
			}*/
		Line= csv.readLine(); // read  next line
		}
		
		return Lines;

	}



TopoDS_Shape HSF::GetBiggestShell(TopoDS_Shape& shell)
{
     TopExp_Explorer EX;
     TopoDS_Shape     ashell;
	 TopoDS_Shape     maxshell;
     double maxArea = 0;
     double area;
     for (EX.Init(shell,TopAbs_SHELL); EX.More(); EX.Next())
     {
          ashell = EX.Current();
          area = GetArea(ashell);
          if (maxArea < area)
          {
               maxArea = area;
               maxshell = ashell;
          }          
     }
     return maxshell;
}

TopoDS_Shape HSF::GetBiggestFace(TopoDS_Shape& face)
{
     TopExp_Explorer EX;
     TopoDS_Shape     aface;
	 TopoDS_Shape     maxface;
     double maxArea = 0;
     double area;
     for (EX.Init(face,TopAbs_FACE); EX.More(); EX.Next())
     {
          aface = EX.Current();
          area = GetArea(aface);
          if (maxArea < area)
          {
               maxArea = area;
               maxface = aface;
          }          
     }
     return maxface;
}

TopoDS_Shape HSF::GetLowestVertex(TopoDS_Shape& Shape)
{
     TopExp_Explorer EX;
     TopoDS_Shape     minpoint;
	 double minz = 100000000000000000;
     
     for (EX.Init(Shape,TopAbs_VERTEX); EX.More(); EX.Next())
     {
	 TopoDS_Vertex v1 = TopoDS::Vertex(EX.Current());
	 gp_Pnt p1 = BRep_Tool::Pnt(v1);
	 if (p1.Z() < minz)
		 {
			minz = p1.Z();
			minpoint = v1;
		 }
     }
     return minpoint;
}



TopoDS_Shape HSF::GetSmallestFace(TopoDS_Shape& face)
{
     TopExp_Explorer EX;
     TopoDS_Shape     aface;
	 TopoDS_Shape     minface;
     double minArea = 9999999999999;
     double area;
     for (EX.Init(face,TopAbs_FACE); EX.More(); EX.Next())
     {
          aface = EX.Current();
          area = GetArea(aface);
          if ( area < minArea )
          {
               minArea = area;
               minface = aface;
          }          
     }
     return minface;
}

int HSF::getshapecount(TopoDS_Shape aShape,TopAbs_ShapeEnum shapetype)
{

	TopExp_Explorer solidExp;
    int subcount=0;
     for (solidExp.Init(aShape,shapetype); solidExp.More(); solidExp.Next())
     {
       subcount++;         
     }
     return subcount;

	}

QList<TopoDS_Shape> HSF::getshapelist(TopoDS_Shape aShape,TopAbs_ShapeEnum shapetype)
{

	QList<TopoDS_Shape> list;
	TopExp_Explorer solidExp;
    int subcount=0;
     for (solidExp.Init(aShape,shapetype); solidExp.More(); solidExp.Next())
     {
       subcount++;   
	   list << solidExp.Current();
     }
     return list;

	}

TopoDS_Shape HSF::applyuvcurve(TopoDS_Shape& shape2d, TopoDS_Shape& surface)
	{
	
	TopoDS_Compound multiobject;
	BRep_Builder B;
	B.MakeCompound(multiobject);
	
	if (surface.IsNull()) qDebug() << "surface is null";


	TopoDS_Shape surfshape = HSF::getfacefromshape(surface);
	
	Handle(Geom_Surface) mysrf = BRep_Tool::Surface(TopoDS::Face(surfshape));
	

	 TopExp_Explorer solidExp;
     for (solidExp.Init(shape2d,TopAbs_EDGE); solidExp.More(); solidExp.Next())
     {
          TopoDS_Shape aSolid = solidExp.Current();
		  TopoDS_Edge edge = TopoDS::Edge(aSolid);
		  TopoDS_Vertex V1, V2;
		  TopExp::Vertices(edge, V1, V2, Standard_True);

		  gp_Pnt p1 = BRep_Tool::Pnt(V1);  
		  gp_Pnt p2 = BRep_Tool::Pnt(V2);

		  Standard_Real p1xd = p1.X();
		  Standard_Real p1yd = p1.Y();
		  Standard_Real p2xd = p2.X();
		  Standard_Real p2yd = p2.Y();
			
		  gp_Pnt2d anEllipsePnt1;
		  gp_Pnt2d anEllipsePnt2;

			
			p1xd = p1xd ;
			p1yd = p1yd ;
			p2xd = p2xd ;
			p2yd = p2yd ;
			
			anEllipsePnt1 = gp_Pnt2d(p1xd,p1yd); 
			anEllipsePnt2 = gp_Pnt2d(p2xd,p2yd); 
		
			Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(anEllipsePnt1 , anEllipsePnt2);

			TopoDS_Edge aEdge1OnSurf1 = BRepBuilderAPI_MakeEdge(aSegment , mysrf);
			TopoDS_Wire threadingWire1 = BRepBuilderAPI_MakeWire(aEdge1OnSurf1);
			BRepLib::BuildCurves3d(aEdge1OnSurf1);

			TopoDS_Edge &edge3d = TopoDS::Edge(aEdge1OnSurf1);
			TopoDS_Shape theShape = edge3d;

			B.Add(multiobject,theShape);

		  
                
     }



	return multiobject;

					
	}


TopoDS_Shape HSF::GetBiggestSolid(TopoDS_Shape& solid)
{
     TopExp_Explorer solidExp;
     TopoDS_Shape     aSolid;
	 TopoDS_Shape     maxSolid;
     double maxVolume = 0;
     double volume;
     for (solidExp.Init(solid,TopAbs_SOLID); solidExp.More(); solidExp.Next())
     {
          aSolid = solidExp.Current();
          volume = GetVolume(aSolid);
          if (maxVolume < volume)
          {
               maxVolume = volume;
               solid = maxSolid;
          }          
     }
     return maxSolid;
}

double HSF::GetVolume(TopoDS_Shape &shape)
{
     GProp_GProps System;                     
	 BRepGProp::VolumeProperties(shape, System);
     return System.Mass();     
}


double HSF::GetMaxDis(TopoDS_Shape s1, TopoDS_Shape s2)
{

double max = 0;
BRepExtrema_DistShapeShape dst (s1, s2,0.1);
if (dst.IsDone()){

gp_Pnt P1, P2;
for (int i = 1; i <= dst.NbSolution(); i++) 
{
	P1 = dst.PointOnShape1(i);
	P2 = dst.PointOnShape2(i);
	Standard_Real Dist = P1.Distance(P2);
	if ( Dist >  max )
	{
		max = Dist;
	}
}
}

return max;
}


double HSF::GetArea(TopoDS_Shape &shape)
{
     GProp_GProps System;        
	 
	 BRepGProp::SurfaceProperties(shape, System);
     return System.Mass();     
}

gp_Pnt HSF::GetAveragePoint(QList<gp_Pnt> plist)
	{

	QList<gp_Pnt>::iterator i;
	
	int count=0;
	double x=0;
	double y=0;
	double z =0;
	for (i = plist.begin(); i != plist.end(); ++i)
		{
			count++;
			gp_Pnt curp = *i;
			x += curp.X();
			y += curp.Y();
			z += curp.Z();

		}

		x = x/count;
		y = y/count;
		z = z/count;
		
		gp_Pnt result(x,y,z);
		return result;


	}

	

gp_Pnt HSF::GetCOG(TopoDS_Shape &shape)
{
	if (shape.IsNull()) return gp_Pnt(0,0,0);
     GProp_GProps System;                     
	 BRepGProp::SurfaceProperties (shape, System);
	 
	 
	 
     return System.CentreOfMass();     
}

double HSF::GetLength(TopoDS_Shape &shape)
{
     GProp_GProps System;                     
	 BRepGProp::LinearProperties (shape, System);
     return System.Mass();
}


Handle(Geom_Curve) HSF::intersectsrf(const Handle(Geom_Surface)& S1,const Handle(Geom_Surface)& S2) 
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

TopoDS_Shape HSF::AddNewIntersectFast(TopoDS_Shape srf1,TopoDS_Shape srf2) 
{

TopoDS_Shape result;
BRepBuilderAPI_MakeWire thewire = BRepBuilderAPI_MakeWire();

TopExp_Explorer shapeX;
for (shapeX.Init(srf1,TopAbs_FACE); shapeX.More(); shapeX.Next())
{
TopoDS_Face aF1 = TopoDS::Face(shapeX.Current());


TopExp_Explorer shapeY;
for (shapeY.Init(srf2,TopAbs_FACE); shapeY.More(); shapeY.Next())
{
TopoDS_Face aF2 = TopoDS::Face(shapeY.Current());


double anApproxTol=1.e-7;

IntTools_FaceFace aFF;
aFF.SetParameters (true, 
true, 
true,
anApproxTol);

//if (!aListOfPnts.IsEmpty())
//aFF.SetList(aListOfPnts);

aFF.Perform(aF1, aF2);


if (aFF.IsDone()) {
// Add Interference to the Pool
double aTolR3D=aFF.TolReached3d();
double aTolR2D=aFF.TolReached2d();
if (aTolR3D < 1.e-7){
double aTolR3D=1.e-7;
} 
aFF.PrepareLines3D();
//
Standard_Integer aNbCurves, aNbPoints;
const IntTools_SequenceOfCurves& aCvs=aFF.Lines();
aNbCurves=aCvs.Length();


  for (int i=1; i<=aNbCurves; i++) {
    const IntTools_Curve& aIC=aCvs(i);
    Handle(Geom_Curve) mycrv = aIC.Curve();
	if (aIC.HasBounds())
		{
			double fp,lp;
			gp_Pnt fpoint, lpoint;
			aIC.Bounds(fp,lp,fpoint,lpoint);
			TopoDS_Edge myedge = BRepBuilderAPI_MakeEdge(mycrv);
			thewire.Add(myedge);
		}
  }
}
}
}

result =thewire.Shape();
return result ;
	


}
gp_Pnt2d HSF::AddNewIntersectCrv2d(TopoDS_Shape crv1,TopoDS_Shape crv2, int &nbpoints)
	{
	nbpoints = 0;
	if (crv1.IsNull() || crv2.IsNull()) 
		{
		return gp_Pnt2d();
		}

	crv1 = hsf::getedgefromshape(crv1);
	crv2 = hsf::getedgefromshape(crv2);
	Standard_Real af, al;
	Handle(Geom_Curve) cc1 = BRep_Tool::Curve(TopoDS::Edge(crv1),af,al);
	Handle(Geom_Curve) cc2 = BRep_Tool::Curve(TopoDS::Edge(crv2),af,al);


	Handle_Geom2d_Curve c1 = GeomAPI::To2d(cc1,gp_Pln(gp::Origin(),gp::DZ()));
	Handle_Geom2d_Curve c2 = GeomAPI::To2d(cc2,gp_Pln(gp::Origin(),gp::DZ()));

	Geom2dAPI_InterCurveCurve Intersector(c1,c2,Precision::Intersection());
	nbpoints = Intersector.NbPoints();
	if (nbpoints > 0 )
		{
			gp_Pnt2d point2d = Intersector.Point(1);
			return point2d;
		}

	return gp_Pnt2d();

	}

TopoDS_Shape HSF::AddNewJoinEdges(QList<TopoDS_Shape> shapelist)
	{

	BRepBuilderAPI_MakeWire MW;
	for (int i = 0 ; i < shapelist.count() ; i++)
	{
	TopoDS_Shape curedge = shapelist.at(i);
	MW.Add(TopoDS::Edge(curedge));                   
	}       
	
	return MW.Shape();

	}
TopoDS_Shape HSF::AddNewIntersectSrfW(TopoDS_Shape srf1,TopoDS_Shape srf2) 
{

//Handle(Geom_Surface) S1 = BRep_Tool::Surface(TopoDS::Face(srf1));
//Handle(Geom_Surface) S2 = BRep_Tool::Surface(TopoDS::Face(srf2));

////This class is instantiated as follows:
//	GeomAPI_IntSS Intersector(S1, S2, Precision::Intersection());
//
////Once the GeomAPI_IntSS object has been created, it can be interpreted.
////Calling the number of intersection curves
//Standard_Integer nb = Intersector. NbLines();
//BRepBuilderAPI_MakeWire MW;
//for(int i=1 ; i<= nb; i++)
//	{
////Calling the intersection curves
//Handle(Geom_Curve) C = Intersector.Line(i);
//
////where Index is an integer between 1 and Nb.
//BRepBuilderAPI_MakeEdge edgebuilder(C,C->FirstParameter(),C->LastParameter());
//if (edgebuilder.IsDone())
//	{
//		MW.Add(TopoDS::Edge(edgebuilder.Shape()));
//	}
//	}
//
//return MW.Shape();
BRepAlgoAPI_Section asect(srf1,srf2,Standard_False);
//asect.ComputePCurveOn1(Standard_True);
asect.Approximation(Standard_True);
asect.Build();
TopoDS_Shape R = asect.Shape();
qDebug()<< R.ShapeType();
return R;
}

gp_Pnt HSF::IntersectCrvtoCrv(TopoDS_Shape crv1, TopoDS_Shape crv2)
	{
		gp_Pnt result(0,0,0);

		return result;


	}



bool HSF::isintersecting(TopoDS_Shape srf1,TopoDS_Shape srf2)
{

qDebug() << "precision" << Precision::Intersection();
srf1 = HSF::getfacefromshape(srf1);
srf2 = HSF::getfacefromshape(srf2);
Handle(Geom_Surface) S1 = BRep_Tool::Surface(TopoDS::Face(srf1));
Handle(Geom_Surface) S2 = BRep_Tool::Surface(TopoDS::Face(srf2));
//double precision = Precision::Confusion();
double precision = 0.1;
qDebug() << "before intersect";
GeomAPI_IntSS Intersector(S1, S2, precision);
qDebug() << "after intersect";
if (Intersector. NbLines() > 0) { return true; 	} else { return false;}


}

TopoDS_Shape HSF::AddNewIntersectSrf(gp_Pln plane1,TopoDS_Shape srf) 
{
TopoDS_Shape nullshape;
				

		BRepAlgoAPI_Section asect(srf,plane1,Standard_False);
		asect.ComputePCurveOn1(Standard_True);
		asect.Approximation(Standard_True);
		asect.Build();
		TopoDS_Shape R = asect.Shape();
		if(R.IsNull() ) return nullshape;

		return R;

	}

TopoDS_Shape HSF::AddNewIntersectSrf(TopoDS_Shape srf1,TopoDS_Shape srf2) 
{


if (srf1.ShapeType() != TopAbs_ShapeEnum::TopAbs_FACE)
	{
	srf1 = HSF::getfacefromshape(srf1);
	}
if (srf2.ShapeType() != TopAbs_ShapeEnum::TopAbs_FACE)
	{
	srf2 = HSF::getfacefromshape(srf2);
	}

Handle(Geom_Surface) S1 = BRep_Tool::Surface(TopoDS::Face(srf1));
Handle(Geom_Surface) S2 = BRep_Tool::Surface(TopoDS::Face(srf2));

//This class is instantiated as follows:
GeomAPI_IntSS* Intersector = new GeomAPI_IntSS(S1, S2, Precision::Intersection());

//Once the GeomAPI_IntSS object has been created, it can be interpreted.
//Calling the number of intersection curves
Standard_Integer nb = Intersector-> NbLines();

//Calling the intersection curves
if (nb > 0) {
	Handle(Geom_Curve) C = Intersector->Line(1);

//where Index is an integer between 1 and Nb.
BRepBuilderAPI_MakeEdge edgebuilder(C,C->FirstParameter(),C->LastParameter());
				
return edgebuilder.Shape();
}
else
{
TopoDS_Shape nullshape;
return nullshape;
}
}



TopoDS_Shape HSF::AddNewIntersectSrf(gp_Pln pln1,gp_Pln pln2) 
{

Handle_Geom_Plane S1 =  GC_MakePlane (pln1);
Handle_Geom_Plane S2 =  GC_MakePlane (pln2);

//This class is instantiated as follows:
GeomAPI_IntSS Intersector(S1, S2, Precision::Intersection());

//Once the GeomAPI_IntSS object has been created, it can be interpreted.
//Calling the number of intersection curves
Standard_Integer nb = Intersector. NbLines();

//Calling the intersection curves
if (nb > 0) {
	Handle(Geom_Curve) C = Intersector.Line(1);

//where Index is an integer between 1 and Nb.
Handle(Geom_TrimmedCurve) spinaxis = new Geom_TrimmedCurve (C, -200, 200);
TopoDS_Shape Result = BRepBuilderAPI_MakeEdge(spinaxis);
				
return Result;
}
else
{
TopoDS_Shape nullshape;
return nullshape;
}
}


gp_Pnt HSF::AddNewIntersectCrvPln(TopoDS_Shape crv1,gp_Pln pln1) 
{
gp_Pnt intp;
Standard_Real af, al;
Handle(Geom_Curve) crv = BRep_Tool::Curve(TopoDS::Edge(crv1),af,al);

Handle_Geom_Plane aSurf = new Geom_Plane(pln1);

//This class is instantiated as follows:
GeomAPI_IntCS Intersector(crv, aSurf);

//Once the GeomAPI_IntSS object has been created, it can be interpreted.
//Calling the number of intersection curves
Standard_Integer nb = Intersector.NbPoints();

//Calling the intersection curves
if (nb > 0) {
intp = Intersector.Point(1);
}

return intp;

}



gp_Pnt HSF::AddNewIntersectCrvSrf(TopoDS_Shape crv1,TopoDS_Shape srf1) 
{

if (srf1.ShapeType() != TopAbs_ShapeEnum::TopAbs_FACE)
	{
	srf1 = HSF::getfacefromshape(srf1);
	}

Handle(Geom_Surface) S1 = BRep_Tool::Surface(TopoDS::Face(srf1));

gp_Pnt intp(0,0,0);
Standard_Real af, al;
Handle(Geom_Curve) crv = BRep_Tool::Curve(TopoDS::Edge(crv1),af,al);

//This class is instantiated as follows:
GeomAPI_IntCS Intersector(crv, S1);


//Once the GeomAPI_IntSS object has been created, it can be interpreted.
//Calling the number of intersection curves
Standard_Integer nb = Intersector.NbPoints();

//Calling the intersection curves
gp_Pnt low(0,0,0);
double minz = 1000000000000000;
if (nb > 0) {
	for (int i = 1 ; i<=nb;i++)
		{
			intp = Intersector.Point(i);
			if(intp.Z() < minz)
				{
					minz = intp.Z();
					low = intp;
				}
		}

}

return low;



}



TopoDS_Shape HSF::AddNewIntersectCrvSrfMulti(TopoDS_Shape crv1,TopoDS_Shape srf1,double &nbcount) 
{
  
TopoDS_Compound multiobject;
  BRep_Builder B;
  B.MakeCompound(multiobject);
  QList<TopoDS_Shape> pointlist;

 
			BRepExtrema_DistShapeShape dst (crv1, srf1,0.1);
			if (dst.IsDone()){
			nbcount = dst.NbSolution();
			gp_Pnt P1, P2;
			for (int i = 1; i <= dst.NbSolution(); i++) {
			  P1 = dst.PointOnShape1(i);
			  P2 = dst.PointOnShape2(i);
			  Standard_Real Dist = P1.Distance(P2);
			  if ( Dist <= Precision::Confusion()+0.1 ){
				gp_Pnt aPnt = P1;
				TopoDS_Shape intp = AddNewPoint(P1);
				if(!intp.IsNull())
				{
				  if (!pointlist.contains(intp))  pointlist << intp;
			//qDebug() << "point" << count << ":" << P1.X() << "," << P1.Y() << "," << P1.Z();
				}
				}
				}
				}

	for (int i = 0; i < pointlist.count(); i++)
		{
			B.Add(multiobject,pointlist.at(i));
		}
	return  multiobject;



}



gp_Pnt HSF::AddNewIntersectionPoint(TopoDS_Shape crv1,TopoDS_Shape crv2,int nbpoints)
	{
//Calculate Lines Intersection Point
gp_Pnt aPnt(0,0,0);
nbpoints = 0;


BRepExtrema_DistShapeShape dst (crv1, crv2);
    if (dst.IsDone())
      {
	gp_Pnt P1, P2;
	nbpoints = dst.NbSolution();
	double mindis = 100000000000000000;

	for (int i = 1; i <= dst.NbSolution(); i++) {
	  P1 = dst.PointOnShape1(i);
	  P2 = dst.PointOnShape2(i);
	  Standard_Real Dist = P1.Distance(P2);
	  if ( Dist < mindis)
	    aPnt = P1;
		mindis = Dist;
		}
		}

return aPnt;
}

//TopoDS_Shape HSF::AddNewIntersectionPointMulti(TopoDS_Shape crv1,TopoDS_Shape crv2)
//	{
//
//  TopExp_Explorer first;
//  
//  TopoDS_Compound multiobject;
//  BRep_Builder B;
//  B.MakeCompound(multiobject);
//  QList<TopoDS_Shape> pointlist;
//
//  int count = 0;
//  for (first.Init(crv1,TopAbs_EDGE); first.More(); first.Next())
//     {
//        TopoDS_Shape edge1 = first.Current();
//		TopExp_Explorer second;
//		for (second.Init(crv2,TopAbs_EDGE); second.More(); second.Next())
//		{
//		count ++;
//		TopoDS_Shape edge2 = second.Current();
//			BRepExtrema_DistShapeShape dst (edge1, edge2,0.1);
//			if (dst.IsDone())
//			  {
//			gp_Pnt P1, P2;
//			for (int i = 1; i <= dst.NbSolution(); i++) {
//			  P1 = dst.PointOnShape1(i);
//			  P2 = dst.PointOnShape2(i);
//			  Standard_Real Dist = P1.Distance(P2);
//			  if ( Dist <= Precision::Confusion() )
//				gp_Pnt aPnt = P1;
//				TopoDS_Shape intp = AddNewPoint(P1);
//				if(!intp.IsNull())
//				{
//				  if (!pointlist.contains(intp))  pointlist << intp;
//			//qDebug() << "point" << count << ":" << P1.X() << "," << P1.Y() << "," << P1.Z();
//				}
//				}
//				}
//
//		
//		
//		}
//
//     }
//     
//	for (int i = 0; i < pointlist.count(); i++)
//		{
//			B.Add(multiobject,pointlist.at(i));
//		}
//	return  multiobject;
//
//
//	}
//
//
//
//
//
//
//
//
//

TopoDS_Shape HSF::AddNewIntersectionPointMulti(TopoDS_Shape crv1,TopoDS_Shape crv2)
	{

    
  TopoDS_Compound multiobject;
  BRep_Builder B;
  B.MakeCompound(multiobject);
  QList<TopoDS_Shape> pointlist;

 
			BRepExtrema_DistShapeShape dst (crv1, crv2,0.1);
			if (dst.IsDone()){
				
			gp_Pnt P1, P2;
			for (int i = 1; i <= dst.NbSolution(); i++) {
			  P1 = dst.PointOnShape1(i);
			  P2 = dst.PointOnShape2(i);
			  Standard_Real Dist = P1.Distance(P2);
			  if ( Dist <= Precision::Confusion()+0.1 ){
				gp_Pnt aPnt = P1;
				TopoDS_Shape intp = AddNewPoint(P1);
				if(!intp.IsNull())
				{
				  if (!pointlist.contains(intp))  pointlist << intp;
			//qDebug() << "point" << count << ":" << P1.X() << "," << P1.Y() << "," << P1.Z();
				}
				}
				}
				}

	for (int i = 0; i < pointlist.count(); i++)
		{
			B.Add(multiobject,pointlist.at(i));
		}
	return  multiobject;


	}










TopoDS_Shape HSF::AddNewExtrude(TopoDS_Shape crv1 , gp_Vec dir , double length)
{

TopoDS_Shape aShape = BRepPrimAPI_MakePrism(crv1, dir * length, true).Shape();
	
	/*double fp,lp;
	TopAbs_ShapeEnum thetype = crv1.ShapeType();

	const Handle(Geom_Curve) C = BRep_Tool::Curve(TopoDS::Edge(crv1),fp,lp);
	Handle_Standard_Type thectype = C->DynamicType();
	Handle_Geom_SurfaceOfLinearExtrusion myextrude = new Geom_SurfaceOfLinearExtrusion(C, gp_Dir(dir));*/

	  //Standard_Real u1, u2, v1, v2;
	  //myextrude->Bounds(u1,u2,v1,v2);
	  //fixParam(u1);
	  //fixParam(u2);
	  //fixParam(v1);
	  //fixParam(v2);

	  //TopoDS_Face Result = BRepBuilderAPI_MakeFace(myextrude, u1, u2, v1, v2);
	  return aShape;
	
	


	}

void HSF::updateUserAIS(TopoDS_Shape aShape, Handle_User_AIS &anAIS, Handle_AIS_InteractiveContext ic)
	{
	
	if(aShape.IsNull()) return;

	if (anAIS.IsNull()){
	anAIS=new User_AIS(aShape,ic);
	ic->SetMaterial(anAIS,Graphic3d_NOM_PEWTER);
	ic->SetColor(anAIS, Quantity_NOC_BLACK);
	//ic->SetDeviationCoefficient(anAIS,10.0);
	//ic->SetHLRDeviationCoefficient(anAIS,10);
	

	//ic->SetDeviationAngle(45);
	//ic->SetTransparency(anAIS);
	ic->SetDisplayMode(anAIS,2,Standard_False);
	
	}
	
	
	//theContext->SetTransparency(AISBottle,0.6);
	//vc->getContext()->Display(AISBottle, 1,0,false,false);
	if (!anAIS->HasPresentation()){
		ic->Display(anAIS, 2,0,false,false);
		ic->Activate(anAIS);
		}
	else
		{
		ic->Activate(anAIS);
		anAIS->Set(aShape);
		//ic->Deactivate(anAIS);
		ic->Redisplay(anAIS);
		
		}


//BRepTools::Clean(aShape);
	}
void HSF::updateAIS(TopoDS_Shape aShape, Handle_AIS_Shape &anAIS, Handle_AIS_InteractiveContext ic)
	{

	if(aShape.IsNull()) return;

	if (anAIS.IsNull()){
	anAIS=new AIS_Shape(aShape);
	ic->SetMaterial(anAIS,Graphic3d_NOM_NEON_GNC);
	ic->SetColor(anAIS, Quantity_NOC_BLACK);
	ic->SetDisplayMode(anAIS,1,Standard_False);
	}

	
	
	//theContext->SetTransparency(AISBottle,0.6);
	//vc->getContext()->Display(AISBottle, 1,0,false,false);
	if (!anAIS->HasPresentation()){
		ic->Display(anAIS, 1,0,false,false);
		}
	else
		{
		anAIS->Set(aShape);
		ic->Deactivate(anAIS);
		ic->Redisplay(anAIS,true,true);
		}


		ic->Update(anAIS,true);
		ic->CurrentViewer()->Redraw();

//BRepTools::Clean(aShape);

					 
				  
	}


void HSF::AddNewAnnotation(QString intext, gp_Pnt loc)
	{

TCollection_AsciiString mynewtext = TCollection_AsciiString(intext.toAscii().data());
Handle(AIS_Text) AIStext = new AIS_Text(mynewtext,loc);
AIStext->SetColor(Quantity_Color(0,0,0,Quantity_TypeOfColor::Quantity_TOC_RGB));
ui::getInstance()->getWindowController()->getContext()->Display(AIStext,0);


	}

double HSF::map(double value, double low, double high, double tlow, double thigh)
	{

double mapped = (((value - low) / (high - low)) * (thigh - tlow)) + tlow;
return mapped;


	}

double HSF::getGray(QImage theImg, double u, double v, bool percent)
	{

int uVal = theImg.width() * u;
int vVal = theImg.height() * v;

QRgb col = theImg.pixel(uVal,vVal);
int gray = qGray(col);

double result = (double) gray;

if(percent)
{
	result = result / 255.0;
}

return result;

	}

gp_Pnt HSF::AddNewUVPt(TopoDS_Shape srf,double u,double v)
{
	Handle_Geom_Surface theSurface;
	TopoDS_Shape aFace = HSF::getfacefromshape(srf);
	theSurface = BRep_Tool::Surface(TopoDS::Face(aFace));
	Standard_Real u1, u2, v1, v2;
	BRepTools::UVBounds(TopoDS::Face(aFace),u1,u2,v1,v2);
	
	gp_Pnt isopoint;
	u = u * (u2 - u1);
	v = v * (v2 - v1);

	theSurface->D0(u,v,isopoint);

	return (isopoint);
}
gp_Pnt HSF::getpointfromshape(TopoDS_Shape point)
{
	gp_Pnt p1;
	if (!point.IsNull())
	{
		p1 = BRep_Tool::Pnt(TopoDS::Vertex(point));
		return p1;
	}
	return p1;

}

gp_Vec HSF::AddNewUVVec(TopoDS_Shape srf,double u,double v)
{
	Handle_Geom_Surface theSurface;
	TopoDS_Shape aFace = HSF::getfacefromshape(srf);
	theSurface = BRep_Tool::Surface(TopoDS::Face(aFace));
	Standard_Real u1, u2, v1, v2;
	BRepTools::UVBounds(TopoDS::Face(aFace),u1,u2,v1,v2);
	
	gp_Pnt isopoint;
	u = u * (u2 - u1);
	v = v * (v2 - v1);

	theSurface->D0(u,v,isopoint);

	gp_Vec ut,vt;
	gp_Pnt pt;
	theSurface->D1(u,v,pt,ut,vt);
	gp_Vec V = ut.Crossed(vt);

	return (V);
}


TopoDS_Shape HSF::lineptarc(gp_Pnt p1,gp_Pnt p2,gp_Vec t1,double maxrad)
{

gp_Pnt sp = p1;
gp_Pnt ep = p2;

gp_Pnt t1p = hsf::MovePointByVector(sp,t1,1);
gp_Vec t2(p1,p2);

gp_Pln Spln(sp,t1);
gp_Pln Epln(ep,t2);

gp_Pln pln1 = hsf::AddNewPlane(sp,t1p,ep);
TopoDS_Shape axis = hsf::AddNewIntersectSrf(Spln,Epln);

gp_Pnt o1 = hsf::AddNewIntersectCrvPln(axis,pln1);
TopoDS_Shape circ1 = hsf::AddNewCircle(sp,ep,o1);
Handle(Geom_Curve) circ1crv = hsf::convertshapetocurve(circ1);
TopoDS_Shape arc1 = BRepBuilderAPI_MakeEdge(circ1crv,sp,ep);

return circ1;
}

QList<TopoDS_Shape> HSF::biarcjunctiondomain(gp_Pnt p1,gp_Pnt p2,gp_Vec t1,gp_Vec t2, double arcratio)
{

QList<TopoDS_Shape>  outputs;
gp_Pnt sp = p1;
gp_Pnt ep = p2;

gp_Pnt t1p = hsf::MovePointByVector(sp,t1,1);
//gp_Vec t2(p1,p2);



gp_Pln Spln(sp,t1);
gp_Pln Epln(ep,t2);



gp_Pln pln1 = hsf::AddNewPlane(sp,t1p,ep);
TopoDS_Shape axis = hsf::AddNewIntersectSrf(Spln,Epln);
gp_Vec axisvec = hsf::getVectorTangentToCurveAtPoint(axis,0.5);
gp_Pnt o1 = hsf::AddNewIntersectCrvPln(axis,pln1);

gp_Vec SVec(Spln.Axis().Direction());
gp_Vec EVec(Epln.Axis().Direction());

double angleoriented = (3.14159265 -SVec.AngleWithRef(EVec,axisvec))/2;

gp_Pln rotpln1 = Spln.Rotated(gp_Ax1(o1,axisvec),angleoriented);
//gp_Pln rotpln2 = rotpln1.Rotated(gp_Ax1(o1,axisvec),90); //rotate 90 degs in radians

gp_Pln rotpln2 = rotpln1.Rotated(gp_Ax1(o1,axisvec),1.57079633); //rotate 90 degs in radians


gp_Pnt p2mirror1 = ep.Mirrored(gp_Ax2(rotpln1.Location(),rotpln1.Axis().Direction(),rotpln1.XAxis().Direction()));
gp_Pnt p2mirror2 = ep.Mirrored(gp_Ax2(rotpln2.Location(),rotpln2.Axis().Direction(),rotpln2.XAxis().Direction()));
gp_Pnt farpoint;



double dis1 = p1.Distance(p2mirror1);
double dis2 = p1.Distance(p2mirror2);

if(dis1 > dis2){farpoint = p2mirror1;} else {farpoint = p2mirror2;}


TopoDS_Shape circ1 = hsf::AddNewCircle(ep,sp,farpoint);
Handle(Geom_Curve) circ1crv = hsf::convertshapetocurve(circ1);
TopoDS_Shape arc1 = BRepBuilderAPI_MakeEdge(circ1crv,ep,sp);

TopoDS_Shape point2m1 = hsf::AddNewPoint(p2mirror1);
TopoDS_Shape point2m2 = hsf::AddNewPoint(p2mirror2);
outputs << arc1 << point2m1 << point2m2 <<axis;



return outputs;

	
	
}

//get a smaller biarc between the range ratio1-ratio2
QList<TopoDS_Shape> HSF::biarconcurve(TopoDS_Shape crv, double ratio1,double ratio2,double arcratio,double infinite)
{

gp_Pnt p1 = hsf::AddNewPointonCurve(crv,ratio1);
gp_Pnt p2 = hsf::AddNewPointonCurve(crv,ratio2);

gp_Vec t1 = hsf::getVectorTangentToCurveAtPoint(crv,ratio1);
gp_Vec t2 = hsf::getVectorTangentToCurveAtPoint(crv,ratio2);

QList<TopoDS_Shape> biarc = biarcptdir(p1,p2,t1,t2,arcratio,infinite);
return biarc;


}


// use the domain and build two biarc on it the biarc will not be on top of the actual curve so that they are tangent to one another.

QList<TopoDS_Shape> HSF::biarcptdir(gp_Pnt p1,gp_Pnt p2,gp_Vec t1,gp_Vec t2, double arcratio,double infinite)
	{


QList<TopoDS_Shape> domain = biarcjunctiondomain(p1,p2,t1,t2,arcratio);
gp_Pnt j = hsf::AddNewPointonCurve(domain.at(0),arcratio);

TopoDS_Shape biarc1 = lineptarc(p1,j,t1,infinite);
TopoDS_Shape biarc2 = lineptarc(p2,j,t2,infinite);
QList<TopoDS_Shape> returndata;
returndata << biarc1 << biarc2 << domain.at(0) << domain.at(1) << domain.at(2);
return returndata;


	}



//build 10 biarcs and get closest one to the crv.
TopoDS_Shape HSF::biarcoptimized(TopoDS_Shape crv, double ratio1,double ratio2,double percentinf,double percentsup,double infinite)
	{

	int viscount=0;
	TopoDS_Compound folder;
	BRep_Builder B;
	B.MakeCompound(folder);

double r1 = percentinf;
double r2 = percentinf + (percentsup-percentinf)/9;
double r3 = percentinf + (percentsup-percentinf)* 2/9;
double r4 = percentinf + (percentsup-percentinf)* 3/9;
double r5 = percentinf + (percentsup-percentinf)* 4/9;
double r6 = percentinf + (percentsup-percentinf)* 5/9;
double r7 = percentinf + (percentsup-percentinf)* 6/9;
double r8 = percentinf + (percentsup-percentinf)* 7/9;
double r9 = percentinf + (percentsup-percentinf)* 8/9;
double r10 = percentsup;

QList<TopoDS_Shape> biarc1 = biarconcurve(crv, ratio1, ratio2,r1, infinite);
QList<TopoDS_Shape> biarc2 = biarconcurve(crv, ratio1, ratio2,r2, infinite);
QList<TopoDS_Shape> biarc3 = biarconcurve(crv, ratio1, ratio2,r3, infinite);
QList<TopoDS_Shape> biarc4 = biarconcurve(crv, ratio1, ratio2,r4, infinite);
QList<TopoDS_Shape> biarc5 = biarconcurve(crv, ratio1, ratio2,r5, infinite);
QList<TopoDS_Shape> biarc6 = biarconcurve(crv, ratio1, ratio2,r6, infinite);
QList<TopoDS_Shape> biarc7 = biarconcurve(crv, ratio1, ratio2,r7, infinite);
QList<TopoDS_Shape> biarc8 = biarconcurve(crv, ratio1, ratio2,r8, infinite);
QList<TopoDS_Shape> biarc9 = biarconcurve(crv, ratio1, ratio2,r9, infinite);
QList<TopoDS_Shape> biarc10 = biarconcurve(crv, ratio1, ratio2,r10, infinite);

int count = biarc1.length();

double dis1a = hsf::GetMaxDis(biarc1.at(0),crv);
double dis2a = hsf::GetMaxDis(biarc2.at(0),crv);
double dis3a = hsf::GetMaxDis(biarc3.at(0),crv);
double dis4a = hsf::GetMaxDis(biarc4.at(0),crv);
double dis5a = hsf::GetMaxDis(biarc5.at(0),crv);
double dis6a = hsf::GetMaxDis(biarc6.at(0),crv);
double dis7a = hsf::GetMaxDis(biarc7.at(0),crv);
double dis8a = hsf::GetMaxDis(biarc8.at(0),crv);
double dis9a = hsf::GetMaxDis(biarc9.at(0),crv);
double dis10a = hsf::GetMaxDis(biarc10.at(0),crv);

double dis1b = hsf::GetMaxDis(biarc1.at(1),crv);
double dis2b = hsf::GetMaxDis(biarc2.at(1),crv);
double dis3b = hsf::GetMaxDis(biarc3.at(1),crv);
double dis4b = hsf::GetMaxDis(biarc4.at(1),crv);
double dis5b = hsf::GetMaxDis(biarc5.at(1),crv);
double dis6b = hsf::GetMaxDis(biarc6.at(1),crv);
double dis7b = hsf::GetMaxDis(biarc7.at(1),crv);
double dis8b = hsf::GetMaxDis(biarc8.at(1),crv);
double dis9b = hsf::GetMaxDis(biarc9.at(1),crv);
double dis10b = hsf::GetMaxDis(biarc10.at(1),crv);

double dis1 = std::max(dis1a,dis1b);
double dis2 = std::max(dis2a,dis2b);
double dis3 = std::max(dis3a,dis3b);
double dis4 = std::max(dis4a,dis4b);
double dis5 = std::max(dis5a,dis5b);
double dis6 = std::max(dis6a,dis6b);
double dis7 = std::max(dis7a,dis7b);
double dis8 = std::max(dis8a,dis8b);
double dis9 = std::max(dis9a,dis9b);
double dis10 = std::max(dis10a,dis10b);

QMap<double,QList<TopoDS_Shape>> map;
map.insert(dis1,biarc1);
map.insert(dis2,biarc2);
map.insert(dis3,biarc3);
map.insert(dis4,biarc4);
map.insert(dis5,biarc5);
map.insert(dis6,biarc6);
map.insert(dis7,biarc7);
map.insert(dis8,biarc8);
map.insert(dis9,biarc9);
map.insert(dis10,biarc10);

QList<double> distances = map.keys();
qSort(distances);

double minval = distances.at(0);
TopoDS_Shape arc1 = map.value(minval).at(0);
TopoDS_Shape arc2 = map.value(minval).at(1);


if (!arc1.IsNull()){
	B.Add(folder,arc1);
		viscount++;}

if (!arc2.IsNull()){
	B.Add(folder,arc2);
		viscount++;}


return folder;

	}