#include "passionfacade.h"
#include "ui.h"
#include "shapefactory.h"
#include "cowbellprogress.h"
#include "parametricsfordummies.h"
#include "bash.h"
#include <QTime>

passionfacade::passionfacade(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setup();
}

passionfacade::~passionfacade()
{

}

void passionfacade::purgememory()
	{
		
	int purgestatus = Standard::Purge();
	if (purgestatus > 0) 
		{
		ui::getInstance()->Statusbar(QString("Memory Purge Worked"));
		} else {
		ui::getInstance()->Statusbar(QString("Memory Purge Did Not Work"));
		}



}



void passionfacade::setup()
	{
	//SLIDERRELEASED(update) //macro to initialize 
		SLIDERMOVED(update)
	}
void passionfacade::update()
	{
	QTime profiler;
	profiler.start();
	
	INITPART // initialize the part
	
	double upperx = get(upperx);
	double lowerx = get(lowerx);
	double upperz = get(upperz);
	double angle = get(angle);
	double hbangle = get(hbangle);

	gp_Pnt p1(lowerx,0,0);
	gp_Pnt p2(lowerx,500,-50);
	gp_Pnt p3(0,1000,-100);

	gp_Pnt p4(-upperx/2,0,upperz/4);
	gp_Pnt p5(-upperx,500,upperz/2);
	gp_Pnt p6(0,1000,upperz);


	QList<gp_Pnt> plist1;
	plist1 << p1 << p2 << p3;

	QList<gp_Pnt> plist2;
	plist2 << p4 << p5 << p6;

	visgeo(spline1,SplineInterpSShape(plist1))
	visgeo(spline2,SplineInterpSShape(plist2))

	int count =30;
	FOR(i,1,count)
		pgeo(p1,PointonCurve(spline1,i * 1.0/count))
		pgeo(p2,PointonCurve(spline2,i * 1.0/count))
		visgeo(l1,Lineptpt(p1,p2))
		pgeo(midp,PointonCurve(l1,0.5))
		gp_Vec v1 = HSF::getVectorTangentToCurveAtPoint(l1,0.5);
		visgeo(hb,HyberbolicShell(midp,v1,hbangle,3,p1.Distance(p2)))
	END

	HSF::axistoaxis
	qDebug() << "took:" << profiler.elapsed() << "before draw";
	ENDPART // finalize
	qDebug() << "took:" << profiler.elapsed() << "after draw";
	
		
	}


