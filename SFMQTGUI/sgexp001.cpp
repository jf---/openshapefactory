#include "sgexp001.h"
#include "ui.h"
#include "shapefactory.h"
#include "cowbellprogress.h"
#include "parametricsfordummies.h"
#include "bash.h"
#include "BRepOffsetAPI_Sewing.hxx"
#include "BRepBuilderAPI_MakeSolid.hxx"
#include "BRepFilletAPI_MakeFillet.hxx"
#include "GEOMAlgo_Splitter.hxx"

sgexp001::sgexp001( QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setup();
}

sgexp001::~sgexp001()
{
}

void sgexp001::purgememory()
	{
		
	int purgestatus = Standard::Purge();
	if (purgestatus > 0) 
		{
		ui::getInstance()->Statusbar(QString("Memory Purge Worked"));
		} else {
		ui::getInstance()->Statusbar(QString("Memory Purge Did Not Work"));
		}

	}
void sgexp001::setup()
{
	SLIDERRELEASED(update) //macro to initialize sliders on release
	//SLIDERMOVED(update) //macro to initialize sliders on move
}

void sgexp001::update()
	{
		INITPART // code starts here
		double middleoffset = get(x)
		double middlez = get(y)
		double topz = get(z)
		double radius1 = get(radius1)
		double radius2 = get(radius2)
		double radius3 = get(radius3)
		double ucount = get(ucount)
		double vcount = get(vcount)
		double hbangle = get(hbangle)
		int hbsel = get(hbsel)
		double hbheightoutside = get(hbheightoutside)
		double hbheightinside = get(hbheightinside)
		double hbinrad1 = get(hbinrad1)
		double hbinrad2 = get(hbinrad2)
		// here we are making points
		pointdata(origin1,0,0,0)
		pointdata(origin2,0,middleoffset,middlez)
		pointdata(origin3,0,0,topz)
		// and then vectors
		vectordata(up,0,0,1)
		visgeo(c1,Circle(origin1,up,radius1))
		visgeo(c2,Circle(origin2,up,radius2))
		visgeo(c3,Circle(origin3,up,radius3))
		/*shapelist(sections)
		addtolist(sections,c1)
		addtolist(sections,c2)
		addtolist(sections,c3)*/

		pointdata(slpp1,0,0,0)
		pointdata(slpp2,middlez,middleoffset,0)
		pointdata(slpp3,topz,0,50)

		pointlist(spline1list)
		addtolist(spline1list,slpp1)
		addtolist(spline1list,slpp2)
		addtolist(spline1list,slpp3)
		
		geo(spline1,SplineSShape(spline1list))
		TopoDS_Shape spline2 = HSF::move(spline1,gp_Vec(0,0,300));
		//visgeo(extrude1,Extrude(spline1,gp_Vec(0,0,1),300))

		shapelist(sections)
		addtolist(sections,spline1)
		addtolist(sections,spline2)
		//addtolist(sections,c3)


		geo(extrude1,LoftSurface(sections))

		Handle_Geom_Surface mysrf = HSF::convertfacetogeom(extrude1);
		TopoDS_Face aFace = HSF::convertgeomsurfaceface(mysrf);


		PANELIZE(extrude1,ucount,vcount) // this is a macro
		PANELOFFSET(-radius1/3) // this is a macro
		
		/*visgeo(edge1,Lineptdir(p1,v1,0,20))
		visgeo(edge2,Lineptdir(p2,v2,0,20))
		visgeo(edge3,Lineptdir(p3,v3,0,20))
		visgeo(edge4,Lineptdir(p4,v4,0,20))*/
		geo(edge1,Lineptpt(op1,op2))
		geo(edge2,Lineptpt(op2,op3))
		geo(edge3,Lineptpt(op3,op4))
		geo(edge4,Lineptpt(op4,op1))

		PANELOFFSETGLOBAL(radius1/3)

		geo(outeredge1,Lineptpt(op1,op2))
		geo(outeredge2,Lineptpt(op2,op3))
		geo(outeredge3,Lineptpt(op3,op4))
		geo(outeredge4,Lineptpt(op4,op1))

		shapelist(frontfacelist)
		addtolist(frontfacelist,outeredge1)
		addtolist(frontfacelist,outeredge2)
		addtolist(frontfacelist,outeredge3)
		addtolist(frontfacelist,outeredge4)
		geo(frontface,FillSurface(frontfacelist))

		shapelist(backfacelist)
		addtolist(backfacelist,edge1)
		addtolist(backfacelist,edge2)
		addtolist(backfacelist,edge3)
		addtolist(backfacelist,edge4)
		geo(backface,FillSurface(backfacelist))

		geo(topflap,BlendSurface(edge1,outeredge1))
		geo(rightflap,BlendSurface(edge2,outeredge2))
		geo(bottomflap,BlendSurface(edge3,outeredge3))
		geo(leftflap,BlendSurface(edge4,outeredge4))
		
		BRepOffsetAPI_Sewing(1.0e-06, Standard_True);
        BRepOffsetAPI_Sewing sew;
		sew.Add(backface);
		sew.Add(topflap);
		sew.Add(rightflap);
		sew.Add(bottomflap);
		sew.Add(leftflap);
		sew.Add(frontface);
		sew.Perform();
		TopoDS_Shape shell = sew.SewedShape();

		BRepBuilderAPI_MakeSolid brep_solid(TopoDS::Shell(shell));
		TopoDS_Solid top_solid = brep_solid.Solid();
		

		////Body : Apply Fillets
		//BRepFilletAPI_MakeFillet mkFillet(top_solid);
		//TopExp_Explorer aEdgeExplorer(top_solid , TopAbs_EDGE);
		//while(aEdgeExplorer.More()){
		//TopoDS_Edge aEdge = TopoDS::Edge(aEdgeExplorer.Current());
		////Add edge to fillet algorithm
		//mkFillet.Add(radius2/10.0 , aEdge);
		//aEdgeExplorer.Next();
		//}
	


		//vis(mkFillet.Shape())








	geo(diag1,Lineptpt(p1,p3))

		pgeo(midp0,PointonCurve(diag1,0.5))
		pgeo(midp1,PointonCurve(edge1,0.5))
		pgeo(midp2,PointonCurve(edge2,0.5))
		pgeo(midp3,PointonCurve(edge3,0.5))
		pgeo(midp4,PointonCurve(edge4,0.5))

		double l1 = HSF::GetLength(edge1);
		double l2 = HSF::GetLength(edge2);
		double l3 = HSF::GetLength(edge3);
		double l4 = HSF::GetLength(edge4);

		
		
		gp_Vec midv0 = HSF::getVectorNormaltoSurfaceatPoint(aFace,midp0);
		gp_Vec midv1 = HSF::getVectorNormaltoSurfaceatPoint(aFace,midp1);
		gp_Vec midv2 = HSF::getVectorNormaltoSurfaceatPoint(aFace,midp2);
		gp_Vec midv3 = HSF::getVectorNormaltoSurfaceatPoint(aFace,midp3);
		gp_Vec midv4 = HSF::getVectorNormaltoSurfaceatPoint(aFace,midp4);

			visgeo(vec0,Lineptdir(midp0, midv0, 0, 20))
			visgeo(vec1,Lineptdir(midp1, midv1, 0, 20))
			visgeo(vec2,Lineptdir(midp2, midv2, 0, 20))
			visgeo(vec3,Lineptdir(midp3, midv3, 0, 20))
			visgeo(vec4,Lineptdir(midp4, midv4, 0, 20))


	





		

		//geo(hb1,HyberbolicShell(midp0,midv0,hbangle,p1.Distance(p2)/4, p1.Distance(op1)*6))

		/*geo(hb2,HyberbolicShell(midp1,midv1,hbangle,l1/4, l1*2))
		geo(hb3,HyberbolicShell(midp2,midv2,hbangle,l2/4, l2*2))
		geo(hb4,HyberbolicShell(midp3,midv3,hbangle,l3/4, l3*2))
		geo(hb5,HyberbolicShell(midp4,midv4,hbangle,l4/4, l4*2))*/
		
		
	gp_Ax3 axisorigin(gp_Pnt(0,0,0), gp_Vec(0,0,1));

	geo(hb0,HyperboloidbyFormula(hbinrad1,hbheightinside, hbheightinside, hbangle/50.0,gp_Pnt(0,0,0), gp_Vec(0,0,1)))
	gp_Ax3 axis0(midp0, midv0);
	TopoDS_Shape hb0xx = HSF::axistoaxis(hb0, axisorigin,axis0);
	//vis(hb0xx)

	geo(hb1,HyperboloidbyFormula(hbinrad2,hbheightoutside, hbheightoutside, hbangle/50.0,gp_Pnt(0,0,0), gp_Vec(0,0,1)))
	gp_Ax3 axis1(midp1, midv1);
	TopoDS_Shape hb1xx = HSF::axistoaxis(hb1, axisorigin,axis1);
	//vis(hb1xx)
	
	geo(hb2,HyperboloidbyFormula(hbinrad2,hbheightoutside, hbheightoutside, hbangle/50.0,gp_Pnt(0,0,0), gp_Vec(0,0,1)))
	gp_Ax3 axis2(midp2, midv2);
	TopoDS_Shape hb2xx = HSF::axistoaxis(hb2, axisorigin,axis2);
	//vis(hb2xx)

	geo(hb3,HyperboloidbyFormula(hbinrad2,hbheightoutside, hbheightoutside, hbangle/50.0,gp_Pnt(0,0,0), gp_Vec(0,0,1)))
	gp_Ax3 axis3(midp3, midv3);
	TopoDS_Shape hb3xx = HSF::axistoaxis(hb3, axisorigin,axis3);
	//vis(hb3xx)

	geo(hb4,HyperboloidbyFormula(hbinrad2,hbheightoutside, hbheightoutside, hbangle/50.0,gp_Pnt(0,0,0), gp_Vec(0,0,1)))
	gp_Ax3 axis4(midp4, midv4);
	TopoDS_Shape hb4xx = HSF::axistoaxis(hb4, axisorigin,axis4);
	//vis(hb4xx)

	GEOMAlgo_Splitter PS;
	PS.AddShape(hb0xx);
    PS.AddTool(hb1xx);
	PS.AddTool(hb2xx);
	PS.AddTool(hb3xx);
	PS.AddTool(hb4xx);
    PS.Perform();
	TopoDS_Shape theResult = PS.Shape();
	vis(theResult)


	ENDPANELIZE // this is a macro

	//TopoDS_Shape split1;

	//switch(hbsel){
	//case 1:
	//	split1 = HSF::AddNewSplit(hb1, hb2);
	//	break;
	//case 2:
	//	split1 = HSF::AddNewSplit(hb1, hb3);
	//	break;
	//case 3:
	//	split1 = HSF::AddNewSplit(hb1, hb4);
	//	break;
	//case 4:
	//	split1 = HSF::AddNewSplit(hb1, hb5);
	//	break;
	//}
	//	vis(split1)

			
		
		ENDPART
	}
