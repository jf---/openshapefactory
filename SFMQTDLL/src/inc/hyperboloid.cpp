#include "hyperboloid.h"

#include "shapefactory.h"


hyperboloid::hyperboloid(double innerRadius, double height, double heightUnder, double angle)
{
	surface::setSurface(hyperboloid::create(innerRadius, height, heightUnder, angle));
}

hyperboloid::~hyperboloid()
{
}

/*
Creates the hyperboloid using a formula. 
TODO: Update this so the hyperboloid actually represents a proper parametric model. 
*/
TopoDS_Shape hyperboloid::create(double innerRadius, double height, double heightUnder, double angle)
{
		int detail = 40;
		gp_Pnt Origin(0,0,0);
		gp_Vec Dir(0,0,1);

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