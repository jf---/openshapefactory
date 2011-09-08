#ifndef MVCPATTERN
#define MVCPATTERN


#include <qocccontroller.h>
#include <commonlibraries.h>
#include <serial.h>
#include <QTimer>

class mvcPattern 
{


public:
	mvcPattern (QoccController* controller);
	~mvcPattern();

protected:
	QoccController* mycontroller;
	Handle_User_AIS aisp;
	

	

public slots:

	//virtual void  setup();
	//virtual void update();
	//virtual void draw(TopoDS_Shape theshape);
	


};

#endif // MVC_H
