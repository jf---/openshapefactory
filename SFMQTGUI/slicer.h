#ifndef SLICER_H
#define SLICER_H

#include <QWidget>
#include "ui_slicer.h"
#include <commonlibraries.h>
#include <shapefactory.h>

class slicer : public QWidget
{
	Q_OBJECT

public:
	slicer(QWidget *parent = 0);
	~slicer();

private:
	Ui::slicer ui;
	Handle_User_AIS aisp;
	TopoDS_Shape EOS;
	TopoDS_Shape EOS2;
	QList<TopoDS_Shape> resultlist;
	QList<TopoDS_Shape> shapelist;
	int shapecount;

	public slots:
	void setup();
	void update();
	void appendtolist(const TopoDS_Shape& shape1);
	void loadfaces1();
	void loadfaces2();

	//void displayshapes();
	//TopoDS_Shape fuse(const TopoDS_Shape &shape1);
	/*void addToCompound(TopoDS_Shape &compound, const TopoDS_Shape &shape);
	void fuse(TopoDS_Shape &shape);*/
};











#endif // SLICER_H


