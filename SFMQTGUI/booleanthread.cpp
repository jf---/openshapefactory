#include "booleanthread.h"
#include <BRepAlgoAPI_Common.hxx>
#include <shapefactory.h>

booleanthread::~booleanthread(void)
	{
	}

void booleanthread::run()
 {
 //BRepAlgoAPI_BooleanOperation* myop = new BRepAlgoAPI_BooleanOperation(obj1, obj2, BOP_COMMON);
 //myop->Build();
 //TopoDS_Shape result = myop->Shape();
//int counter = 0;
// if(theshape1.IsNull()) return;
// if(theshape2.IsNull()) return;

	 TopoDS_Shape f1 = hsf::getfacefromshape(theshape1);
	 TopoDS_Shape f2 = hsf::getfacefromshape(theshape2);

	 TopoDS_Shape result = hsf::AddNewIntersectSrf(f1,f2);
 //TopoDS_Shape intcrv = hsf::move(theshape1,gp_Vec(0,0,10));

  emit finishedshape(result);



 /*try{ const TopoDS_Shape & result = bcom->Shape();
	 emit finishedshape(result);
	 } catch(...) {
	 int errorstat = bcom->ErrorStatus();
	 qDebug() << "error:" << errorstat;
	 }
*/

//while(!bcom->IsDone())
//	{
//		
//
//	}



		/*const TopoDS_Shape & result = bcom->Shape();*/
		

 //int nbp;
 //

 
 };
