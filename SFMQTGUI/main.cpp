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

#include "SGMGUI_COMMON.h" // loads all the necessary evil's
//#include <tools\texteditor\mainwindow.h>



// add the includes to your widgets here:
// follow this general form: #include widgetname.h

//#include "mus.h"
////#include "apdl_mex_design_001.h"
//#include "beamtest.h"
//#include "algae.h"
#include "biarc.h"
#include "slicer.h"
#include "kilianroof.h"
#include "scriptwidget.h"


int main(int argc, char **argv) //Boot Loader Function
{
	
qRegisterMetaType<TopoDS_Shape>("TopoDS_Shape");

	//HMODULE dll = LoadLibrary("myserver.dll");
 //typedef HRESULT(__stdcall *DllRegisterServerProc)();
 //DllRegisterServerProc DllRegisterServer =
 //    (DllRegisterServerProc)GetProcAddress(dll, "DllRegisterServer");

 //HRESULT res = E_FAIL;
 //if (DllRegisterServer)
 //    res = DllRegisterServer();
 //if (res != S_OK)
     



	QoccApplication app( argc, argv );

	ui::init(); // initializes the 2d gui canvas
	ui::getInstance()->ShowMaximized(); // shows the app window maximized
	
	//Standard::SetReentrant(true);
	

	//mus* mymus = new mus();
	//qGeomApp->symboltree->addnewwidget(mymus);
		
	//apdl_mex_design_001* experiment1 = new apdl_mex_design_001();
	//qGeomApp->symboltree->addnewwidget(experiment1);

	//beamtest* bt = new beamtest();
	//qGeomApp->symboltree->addnewwidget(bt);

	////algae* ag = new algae();
	////qGeomApp->symboltree->addnewwidget(ag);
	//
	//biarc* ba = new biarc();
	//qGeomApp->symboltree->addnewwidget(ba);
	//
	//slicer* slice = new slicer();
	//qGeomApp->symboltree->addnewwidget(slice);

	//kilianroof* kr = new kilianroof();
	//qGeomApp->symboltree->addnewwidget(kr);

	scriptwidget* scriptw = new scriptwidget();
//	QsciScintilla* textEdit = new QsciScintilla();
//	scriptw->seteditor(textEdit);


	qGeomApp->symboltree->addnewwidget(scriptw);

	
	qGeomApp->myview->getView()->SetAnimationModeOn();
	//qGeomApp->myview->getView()->EnableGLLight();
	qGeomApp->myview->getView()->SetAntialiasingOff();
	qGeomApp->myview->getView()->SetLightOff();
	

	
	

	

	//MainWindow * mywin = new MainWindow();

	return app.exec();
}
