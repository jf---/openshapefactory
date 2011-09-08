#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include <QWidget>
#include "ui_beziercurve.h"
#include <commonlibraries.h>

	struct splinepoint{
		 int index;
		 gp_Pnt pole;
		 double weight;
	     };


class beziercurve : public QWidget
{
	Q_OBJECT

public:
	beziercurve(QWidget *parent = 0);
	~beziercurve();

private:
	int updatecount;
	Ui::beziercurve ui;
	Handle_User_AIS aisp;
	
QList<Handle(AIS_Text)> textlist ;
QList<splinepoint> splinepointlist;


public slots:

	void setup();
	void update();
	void loadcsvpoints();

};

#endif // BEZIERCURVE_H
