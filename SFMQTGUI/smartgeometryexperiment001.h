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


#ifndef SMARTGEOMETRYEXPERIMENT001_H
#define SMARTGEOMETRYEXPERIMENT001_H

#include <QWidget>
#include "ui_smartgeometryexperiment001.h"
#include <QWidget>
#include <QMap>
#include <commonlibraries.h>
#include <QSet>

class SmartGeometryExperiment001 : public QWidget
{
	Q_OBJECT

public:
	SmartGeometryExperiment001(QWidget *parent = 0);
	~SmartGeometryExperiment001();

private:
	Ui::SmartGeometryExperiment001Class ui;
	Handle_User_AIS aisp;
public slots:

	void setup();
	void update();
};

#endif // SMARTGEOMETRYEXPERIMENT001_H
