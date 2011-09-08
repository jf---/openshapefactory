/************************************************************************************
**
** This file is part of the Flexible Digital Modeller
**
** Copyright (C) 2010,2011 Alexander Peña de Leon
**
**This work is licensed under the Creative Commons 
**Attribution-NonCommercial-ShareAlike 3.0 Australia License. 
**To view a copy of this license, 
**visit http://creativecommons.org/licenses/by-nc-sa/3.0/au/ 
**or send a letter to Creative Commons, 171 Second Street, 
**Suite 300, San Francisco, California, 94105, USA.
**
***********************************************************************************/



#ifndef PARAMETRICSFORDUMMIES_H
#define PARAMETRICSFORDUMMIES_H

#define addtolist(name,obj) name << obj;
#define shapedata TopoDS_Shape
/* convert to a specific unit*/
#define tomm(value)\
	UnitsAPI::AnyToLS((double)value * 0.0393700787 ,"in.");\
/* poitn list*/
#define shapelist(name)\
	QList<TopoDS_Shape> name;\
/* poitn list*/
	#define pointlist(name)\
	QList<gp_Pnt> name;\
/* replace gp point with pointdata*/
#define pointdata(name,x,y,z)\
	gp_Pnt name(x,y,z);\
	/* replace gp vec with vector data */
#define vectordata(name,x,y,z)\
	gp_Vec name(x,y,z);\
/* get value from ui */
#define getu(name)\
	tomm(ui.##name->value())\
	/* get value from ui */
#define get(name)\
	ui.##name->value();\
		/* get value from ui */
#define getdval(name) (double)ui.##name->value()\
/* initialize part with context and folder */
#define INITPART\
	UnitsAPI::SetLocalSystem(UnitsAPI_MDTV);\
	Handle_AIS_InteractiveContext ic = ui::getInstance()->getWindowContext();\
	int viscount=0;\
	TopoDS_Compound folder;\
	BRep_Builder B;\
	B.MakeCompound(folder);\
/* create shape and visulize */
#define visgeo(name,func)\
	shapedata name = HSF::AddNew##func;\
	if (!name.IsNull()){\
	B.Add(folder,name);\
	viscount++;}\
	

/* create shape and visulize */
#define vis(name)\
	if (!name.IsNull()){\
	B.Add(folder,name);\
		viscount++;}\

/* create shape only */
#define geo(name,func)\
	shapedata name = HSF::AddNew##func;\
/* create pointdata from hsf */
#define pgeo(name,func)\
	gp_Pnt name = HSF::AddNew##func;\
/* panelize surface and initialize standard variable names */
#define PANELIZE(surface1,x,y)\
		QMap<QString,gp_Ax1> plist = HSF::BuildPointGridonSrf(surface1,x,y);\
		QMapIterator<QString,gp_Ax1> i(plist);\
		while (i.hasNext()){\
		i.next();\
			QString currentname = i.key();\
			QString epn = HSF::GetNextUvName(currentname,1,0);\
			QString sepn = HSF::GetNextUvName(currentname,1,1);\
			QString swpn = HSF::GetNextUvName(currentname,0,1);\
			if(plist.contains(epn) && plist.contains(sepn) && plist.contains(swpn)){\
				gp_Pnt p1 = i.value().Location();\
				gp_Pnt p2 = plist.value(epn).Location();\
				gp_Pnt p3 = plist.value(sepn).Location();\
				gp_Pnt p4 = plist.value(swpn).Location();\
				gp_Vec v1 = i.value().Direction();\
				gp_Vec v2 = plist.value(epn).Direction();\
				gp_Vec v3 = plist.value(sepn).Direction();\
				gp_Vec v4 = plist.value(swpn).Direction();\
/* panelize surface and initialize standard variable names */
#define PANELIZEGLOBAL(surface1,x,y)\
		QMap<QString,gp_Ax1> plist = HSF::BuildPointGridonSrf(surface1,x,y);\
		QMapIterator<QString,gp_Ax1> i(plist);\
		        gp_Pnt p1 ;\
				gp_Pnt p2 ;\
				gp_Pnt p3 ;\
				gp_Pnt p4 ;\
				gp_Vec v1 ;\
				gp_Vec v2 ;\
				gp_Vec v3 ;\
				gp_Vec v4 ;\
				gp_Pnt op1;\
				gp_Pnt op2;\
				gp_Pnt op3;\
				gp_Pnt op4;\
		while (i.hasNext()){\
		i.next();\
			QString currentname = i.key();\
			QString epn = HSF::GetNextUvName(currentname,1,0);\
			QString sepn = HSF::GetNextUvName(currentname,1,1);\
			QString swpn = HSF::GetNextUvName(currentname,0,1);\
			if(plist.contains(epn) && plist.contains(sepn) && plist.contains(swpn)){\
				 p1 = i.value().Location();\
				 p2 = plist.value(epn).Location();\
				 p3 = plist.value(sepn).Location();\
				 p4 = plist.value(swpn).Location();\
				 v1 = i.value().Direction();\
				 v2 = plist.value(epn).Direction();\
				 v3 = plist.value(sepn).Direction();\
				 v4 = plist.value(swpn).Direction();\

/* create offset points from panel */
#define PANELOFFSET(offset)\
	gp_Pnt op1 = HSF::MovePointByVector(p1,v1,offset);\
	gp_Pnt op2 = HSF::MovePointByVector(p2,v2,offset);\
	gp_Pnt op3 = HSF::MovePointByVector(p3,v3,offset);\
	gp_Pnt op4 = HSF::MovePointByVector(p4,v4,offset);\
	/* create offset points from panel */
#define PANELOFFSETGLOBAL(offset)\
	op1 = HSF::MovePointByVector(p1,v1,offset);\
	op2 = HSF::MovePointByVector(p2,v2,offset);\
	op3 = HSF::MovePointByVector(p3,v3,offset);\
	op4 = HSF::MovePointByVector(p4,v4,offset);\
/* finalize the panelize scope */
#define ENDPANELIZE\
				}\
				}\
				//ui::getInstance()->Statusbar(i.key());
/* finish part */
#define ENDPART\
	if (viscount>0){\
	HSF::updateUserAIS(folder,aisp,ic);}\
	//purgememory();\

/* initializes sliders on release */

#define SLIDERRELEASED(updatefunc)\
int childcount2 = ui.sliderset->count();\
for(int i=0;i<childcount2;i++){\
	QSlider* widgetitem = qobject_cast<QSlider*>(ui.sliderset->itemAt(i)->widget());\
	if(widgetitem){\
	QString objname = widgetitem->objectName();\
	connect(widgetitem,SIGNAL(sliderReleased()),this,SLOT(##updatefunc()));\
	}\
	}\
/* initializes sliders on move */
#define SLIDERMOVED(updatefunc)\
int childcount = ui.sliderset->count();\
for(int i=0;i<childcount;i++){\
	QSlider* widgetitem = qobject_cast<QSlider*>(ui.sliderset->itemAt(i)->widget());\
	if(widgetitem){\
	QString objname = widgetitem->objectName();\
	connect(widgetitem,SIGNAL(sliderMoved(int)),this,SLOT(##updatefunc()));\
	}\
	}\
/* end of macros */
#define DECLAREUIWIDGETHEADER(uiclassstring)\
class #uiclassstring : public QWidget\
	{\
	Q_OBJECT\
public:\
#uiclassstring(QWidget *parent = 0);\
	~#uiclassstring();\
private:\
	Ui::#uiclassstring ui;\
	Handle_User_AIS aisp;\
public slots:\
	void setup();\
	void update();\
	void purgememory();\
	};\
/* end of macros */
#define FINISHMACRO #endif\



#endif // end of parametrics for dummies