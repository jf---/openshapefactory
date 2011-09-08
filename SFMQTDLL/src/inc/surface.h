#ifndef SURFACE_H
#define SURFACE_H

#include <commonlibraries.h>

class surface
{
	public:
		surface(void);
		~surface(void);
		surface(TopoDS_Shape theShape);

		//sets
		void move(gp_Vec moveVector);
		void draw();
		void setSurface(TopoDS_Shape theShape);

		void setShape(TopoDS_Shape theShape) { myShape = theShape;};


		//gets
	protected:
		//the topoDS shape

	private:
		TopoDS_Shape myShape;
		Handle(User_AIS) aisp1;
		Handle(AIS_Shape) aisp2;
};

#endif