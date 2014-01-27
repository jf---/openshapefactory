// AIS_Gauss.cpp: implementation of the AIS_Gauss class.
//
//////////////////////////////////////////////////////////////////////


//// following the code on this link:
//http://www.opencascade.org/org/forum/thread_1125/


#include "AIS_Gauss.hxx"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif
IMPLEMENT_STANDARD_HANDLE(AIS_Gauss,AIS_InteractiveObject)
IMPLEMENT_STANDARD_RTTI(AIS_Gauss,AIS_InteractiveObject)
//
// Foreach ancestors, we add a IMPLEMENT_STANDARD_SUPERTYPE and 
// a IMPLEMENT_STANDARD_SUPERTYPE_ARRAY_ENTRY macro.
// We must respect the order: from the direct ancestor class
// to the base class.
//
IMPLEMENT_STANDARD_TYPE(AIS_Gauss)
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
IMPLEMENT_STANDARD_TYPE_END(AIS_Gauss)


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
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>
#include <Handle_Poly_Triangulation.hxx>
#include <Poly_Triangulation.hxx>
#include <Poly_Connect.hxx>
//#include <Graphic3d_Array1OfVertexNC.hxx>
#include <Aspect_Array1OfEdge.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TColgp_Array1OfPnt2d.hxx>
#include <Poly_Array1OfTriangle.hxx>
#include <GeomLProp_SurfaceTool.hxx>
#include <GeomLProp_SLProps.hxx>
#include <BRepTools.hxx>
#include <Precision.hxx>
#include <Poly_Triangle.hxx>
#include <Graphic3d_ArrayOfTriangleFans.hxx>
#include <Graphic3d_AspectFillArea3d.hxx>
#include <TopoDS_Face.hxx>
#include <BRepMesh.hxx>

#include <Graphic3d_Group.hxx>
#include <Graphic3d_Vertex.hxx>
//#include <Graphic3d_VertexN.hxx>
//#include <Graphic3d_Array1OfVertexN.hxx>
#include <gp_Vec.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Prs3d_IsoAspect.hxx>
#include <Graphic3d_AspectFillArea3d.hxx>
#include <Precision.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <gp_Pnt.hxx>
#include <Geom_Surface.hxx>
#include <Adaptor3d_Surface.hxx>
#include <BRepAdaptor_Surface.hxx>


#include <Graphic3d_ArrayOfTriangleStrips.hxx>
#include <BRepMesh_FastDiscret.hxx>
#include <Bnd_Box.hxx>
#include <qDebug>
#include <Graphic3d_ArrayOfTriangles.hxx>
#include <Graphic3d_ArrayOfPolylines.hxx>

#include <QGradient>
#include <QLinearGradient>
#include <QGradientStops>
#include <Graphic3d_AspectLine3d.hxx>
#include <Graphic3d_Vertex.hxx>




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//AIS_Gauss::AIS_Gauss(const Graphic3d_Array1OfVertex &points)
//:cloud_data(points)
//{
//
//}

AIS_Gauss::AIS_Gauss(TopoDS_Shape theshape, bool showedges)
{

myShape = theshape;
_showedges = showedges;

}




AIS_Gauss::~AIS_Gauss()
{

}

void AIS_Gauss::Compute(const Handle(PrsMgr_PresentationManager3d)& aPresentationManager,
                             const Handle(Prs3d_Presentation)& aPresentation,
                             const Standard_Integer aMode)
{



int gradationsteps = 20;
int numberColors = gradationsteps;
int redsteps = gradationsteps/3;
int bluesteps = gradationsteps/3;
int greensteps = gradationsteps -(redsteps + bluesteps);



float *curvatureColors = new float[(gradationsteps-1)*3]; //blue to red or whatever

int redcounter = 0;
int greencounter =0;
int bluecounter = 0;

int colorcount = 0;
for(int cs=0 ; cs < gradationsteps ; cs++)
{

double myR ;
double myG ;
double myB ;

if (cs <= (redsteps -1) )
{
myR = 1 - ((redcounter-1) / (double) redsteps);
myG = 0;
myB = 0;
if (myR = 0.1) myG = 1.0;
redcounter ++;
} else if (cs > (redsteps -1) && cs < (redsteps + greensteps -1) )
{
myR = 0;
myG = 1-((greencounter-1) / (double) greensteps);
myB = 0;
if (myG = 0.1) myB = 1.0;
greencounter++;
} else if (cs > (redsteps + greensteps -1) )
{
myR = 0;
myG = 0;
myB = 1-((bluecounter-1) / (double)bluesteps);

bluecounter++;
}


int rindex = colorcount ;
int gindex = colorcount+1;
int bindex = colorcount+2 ;

curvatureColors[rindex]= myR;
curvatureColors[gindex]= myG;
curvatureColors[bindex]= myB;

colorcount+=3;

}









	
Handle(Graphic3d_Structure) theStructure = Handle(Graphic3d_Structure)::DownCast(aPresentation);
Handle(Graphic3d_Group) mygroup= new Graphic3d_Group(theStructure);

bool visualizationDone;
bool flatShading = false;



//Explore the shape and get all the triangles of the corresponding mesh
TopExp_Explorer Ex;
for (Ex.Init(myShape,TopAbs_FACE); Ex.More(); Ex.Next())
{
	TopoDS_Shape curshape = Ex.Current();
	Handle(Geom_Surface) mysurf = BRep_Tool::Surface(TopoDS::Face(curshape));

TopoDS_Face myFace = TopoDS::Face(curshape);




TopLoc_Location L;
Handle(Poly_Triangulation) myT = BRep_Tool::Triangulation(myFace, L);

int nbnodes = myT->NbNodes();
int nbtriangles = myT->NbTriangles();

//Poly_Connect pc(myT);     
const TColgp_Array1OfPnt& Nodes = myT->Nodes();
const TColgp_Array1OfPnt2d& UVNodes = myT->UVNodes();
const Poly_Array1OfTriangle& triangles = myT->Triangles();

Handle(Graphic3d_ArrayOfTriangles) trianglearr = new Graphic3d_ArrayOfTriangles(triangles.Length()* 3,0,false,true);
//Graphic3d_Array1OfVertexNC Points(1,triangles.Length()* 3);

Handle(Graphic3d_ArrayOfPolylines) polylines = new Graphic3d_ArrayOfPolylines ( triangles.Length()* 6,triangles.Length()* 6 );

Handle(Graphic3d_ArrayOfPolylines) vectorlines = new Graphic3d_ArrayOfPolylines ( nbnodes * 2,nbnodes * 2 );


Standard_Real fUMin, fUMax, fVMin, fVMax;
BRepTools::UVBounds(TopoDS::Face(myFace), fUMin, fUMax, fVMin, fVMax);

Standard_Real fU = (fUMax-fUMin)/2.0;
Standard_Real fV = (fVMax-fVMin)/2.0;

GeomLProp_SLProps prop(mysurf, fU, fV, 1, Precision::Confusion());



double maxVisualizedCurvature = 0;
double minVisualizedCurvature = 0;

for (int i = 1; i < UVNodes.Length()+1; i++)
{
gp_Pnt2d uvval = UVNodes.Value(i);
prop.SetParameters(uvval.X(),uvval.Y());
double g = prop.MeanCurvature();//GaussianCurvature();
if (g < minVisualizedCurvature)minVisualizedCurvature = g;
if (g > maxVisualizedCurvature)maxVisualizedCurvature = g;
}






//you could use something like this:

double span = 1;         
span = (maxVisualizedCurvature - minVisualizedCurvature) / numberColors;
int spanNumber = 0;
double curvatureStrength = 0;

//loop over all faces & triangles

double prevR;
double prevG;
double prevB;
double R;
double G;
double B;

//For each triangle get the three nodes
//Graphic3d_Array1OfVertexNC Points(1,3);
for (int i = 1; i < nbnodes; i++)
{

prop.SetParameters(UVNodes.Value(i).X(),UVNodes.Value(i).Y());
gp_Dir mindir;
gp_Dir maxdir;
prop.CurvatureDirections(maxdir,mindir);

gp_Pnt orpoint = Nodes.Value(i);

gp_Vec aV  = gp_Vec(mindir).Normalized();
aV = aV.Multiplied(200);
gp_Pnt newpoint;
newpoint.SetXYZ(orpoint.XYZ() + aV.XYZ());



// edge1
vectorlines->AddBound ( 2 );
vectorlines->AddVertex ( orpoint.X(), orpoint.Y(), orpoint.Z() );
vectorlines->AddVertex ( newpoint.X(), newpoint.Y(), newpoint.Z() );
         
}


if (myT->HasUVNodes())
{

for (int i = 1; i < triangles.Length()+1; i++)
{

	Poly_Triangle mytriangle = triangles.Value(i);
	
	int n1,n2,n3;
	mytriangle.Get(n1,n2,n3);

	gp_Pnt p1 = Nodes.Value(n1);
	gp_Pnt p2 = Nodes.Value(n2);
	gp_Pnt p3 = Nodes.Value(n3);



	if (_showedges)
	{
	// edge1
	polylines->AddBound ( 2 );
	polylines->AddVertex ( p1.X(), p1.Y(), p1.Z() );
	polylines->AddVertex ( p2.X(), p2.Y(), p2.Z() );

	// edge1
	polylines->AddBound ( 2 );
	polylines->AddVertex ( p2.X(), p2.Y(), p2.Z() );
	polylines->AddVertex ( p3.X(), p3.Y(), p3.Z() );

	// edge1
	polylines->AddBound ( 2 );
	polylines->AddVertex ( p3.X(), p3.Y(), p3.Z() );
	polylines->AddVertex ( p1.X(), p1.Y(), p1.Z() );

	}

	


	//trianglearr->AddBound(3);
	

    for(int k = 1; k <= 3 ; k++)
    {
		int vertindex = mytriangle.Value(k);
         if(k == 1)
              prop.SetParameters(UVNodes(n1).X(), UVNodes(n1).Y());                                        
         else if(k == 2)
              prop.SetParameters(UVNodes(n2).X(), UVNodes(n2).Y());
         else
              prop.SetParameters(UVNodes(n3).X(), UVNodes(n3).Y());

         if (prop.IsCurvatureDefined())
         {                                                       
            
			// switch curvature types later like the mean curvature or the max curvature.
              double curvature = prop.MeanCurvature();//GaussianCurvature();

			  

              visualizationDone = true;

              if(span != 0)
              {
                   spanNumber = 1 + (int) ( (curvature - minVisualizedCurvature) /span);
                   curvatureStrength = (curvature - minVisualizedCurvature - (spanNumber - 1) * span) / span;
              }                                                  
              else
              {
                   curvatureStrength = 1.0;
                   spanNumber = 0;
              }                              
              
              if( (spanNumber < 0) || (spanNumber > numberColors - 1) ||
                   (curvatureStrength > 1.0) || (curvatureStrength < 0) )
              {
                   curvatureStrength = 1.0;

                   if(spanNumber > numberColors - 1)
                        spanNumber = numberColors - 1;
                   else if(spanNumber < 0)
                        spanNumber = 0;
              }

              if(spanNumber > 0 )
              {
                   prevR = curvatureColors[(spanNumber - 1) + 0];
                   prevG = curvatureColors[(spanNumber - 1) + 1];
                   prevB = curvatureColors[(spanNumber - 1) + 2];
              }
              else
              {
                   prevR = curvatureColors[0];
                   prevG = curvatureColors[1];
                   prevB = curvatureColors[2];
              }

              R = prevR + (float)curvatureStrength * ( curvatureColors[spanNumber + 0] - prevR );
              G = prevG + (float)curvatureStrength * ( curvatureColors[spanNumber + 1] - prevG );
              B = prevB + (float)curvatureStrength * ( curvatureColors[spanNumber + 2] - prevB );                                                                 

              if(flatShading == true)
              {
                   R = ceil(R);
                   G = ceil(G);
                   B = ceil(B);

				   

              }
			  
			  gp_Pnt mynodept = Nodes.Value(vertindex);
			  if (R > 1) R = 1;
			  if (G > 1) G = 1;
			  if (B > 1) B = 1;
			  Quantity_Color thecolor(R,G,B,Quantity_TOC_RGB);

			  //qDebug() << "color and gauss:" << R << "," << G << "," << B << "-" << curvature;

			  trianglearr->AddVertex(mynodept,thecolor);
			  
              //points(k).SetColor(Quantity_Color(R,G,B,Quantity_TOC_RGB));

			  
			 // mygroup->Marker(Graphic3d_Vertex(mynodept.X(),mynodept.Y(),mynodept.Z()));

         }
    }                                                                                                              

} // end of lopping thorugh points

} // end of has uv nodes



// draw mesh triangles

Handle(Graphic3d_AspectFillArea3d) aspect = new Graphic3d_AspectFillArea3d();
aspect->SetInteriorStyle(Aspect_IS_SOLID);
//mygroup->Clear();

//*******changes for occ 6.6.0
//mygroup->BeginPrimitives();
//
//mygroup->SetPrimitivesAspect(aspect);
//mygroup->AddPrimitiveArray(trianglearr);
//mygroup->EndPrimitives();
//*******changes for occ 6.6.0

//*********changes for occ 6.6.0
//// draw mesh edges inside polylines array
//mygroup->SetPrimitivesAspect ( new Graphic3d_AspectLine3d( Quantity_NameOfColor::Quantity_NOC_BLACK , Aspect_TOL_SOLID, 0.1 ) );
//mygroup->BeginPrimitives ();
//mygroup->AddPrimitiveArray ( vectorlines);
//mygroup->EndPrimitives ();
//*********changes for occ 6.6.0


if(_showedges)
{
// draw mesh edges inside polylines array
////*********changes for occ 6.6.0
//mygroup->SetPrimitivesAspect ( new Graphic3d_AspectLine3d( Quantity_NameOfColor::Quantity_NOC_BLACK , Aspect_TOL_SOLID, 0.1 ) );
//mygroup->BeginPrimitives ();
//mygroup->AddPrimitiveArray ( polylines );
//mygroup->EndPrimitives ();
////*********changes for occ 6.6.0
}

//delete mygroup;
//delete curvatureColors;




} // end of iterator on face







}





void AIS_Gauss::setshape (TopoDS_Shape curshape)
{
if (!curshape.IsNull() || !curshape.IsEqual(myShape))
{
myShape = curshape;
}
//Redisplay();
}

void AIS_Gauss::setshowedges (bool showedges)
{
_showedges = showedges;
}


void AIS_Gauss::Compute(const Handle(Prs3d_Projector)& aProjector,
                             const Handle(Prs3d_Presentation)& aPresentation) 
 {
 }

//void AIS_Gauss::Compute(const Handle(PrsMgr_PresentationManager2d)& aPresentationManager, 
//                         const Handle(Graphic2d_GraphicObject)& aGrObj, 
//                         const Standard_Integer unMode)
//{
//}

void AIS_Gauss::ComputeSelection(const Handle(SelectMgr_Selection)& aSelection, 
				      const Standard_Integer unMode)
{

}

