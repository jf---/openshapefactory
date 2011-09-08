#pragma once
#include "surface.h"

class hyperboloid;

class hyperboloid:
public surface
{
	public:
		hyperboloid(double innerRadius, double height, double heightUnder, double angle); // constructor
		~hyperboloid();

	private:
		TopoDS_Shape create(double innerRadius, double height, double heightUnder, double angle);
};
