#include "surface.h"
#include "ui.h"
#include "shapefactory.h"

surface::surface(void)
{
}

surface::~surface(void)
{
	//when object is destructed, we also have to remove it from the Interactive context.
	//Handle_AIS_InteractiveContext ic = ui::getInstance()->getWindowContext();
	
	//use false here to stop the screen updating until the new object is created.
	//ic->Remove(aisp1, false);	
}

surface::surface(TopoDS_Shape theShape)
{
	surface::myShape = theShape;
}

void surface::setSurface(TopoDS_Shape theShape)
{
	surface::myShape = theShape;
}


/**
* TODO: This is not working....
*/
void surface::move(gp_Vec moveVector)
{

	gp_Trsf translation; 
	translation.SetTranslation(moveVector);
	surface::myShape.Move(TopLoc_Location(translation));
}

/**
This sends the object to AIS to be drawn, but it does not actually tell AIS to update, so all the objects can be drawn at once and then updated
*/
void surface::draw()
{
	Handle_AIS_InteractiveContext ic = ui::getInstance()->getWindowContext();
	HSF::updateAIS(myShape,aisp2,ic);
}