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
//#include "biarc.h"
//#include "slicer.h"
//#include "kilianroof.h"
#include "scriptwidget.h"

//#include "PythonQt.h"
//#include "PythonQtScriptingConsole.h"
//#include "PythonQtGui.h"

// //PythonQt.lib //PythonQtGui.lib

#include "qoccframe.h"

#include "HsfScriptingInterface.h"

#include <QFile>

#include <QSplitter>
#include <QCleanlooksStyle>

#include "tbb/tbbmalloc_proxy.h"

int main(int argc, char **argv) //Boot Loader Function
{
	
qRegisterMetaType<TopoDS_Shape>("TopoDS_Shape");
qRegisterMetaType<QMap<QString,QVariant>>("QMap<QString,QVariant>");


	QoccApplication app( argc, argv );
	QApplication::setStyle(new QCleanlooksStyle);

	appui::init(); // initializes the 2d gui canvas
	
	
	scriptwidget* scriptw = new scriptwidget();
//	QsciScintilla* textEdit = new QsciScintilla();
//	scriptw->seteditor(textEdit);



	QStringList arguments = app.arguments();
	
	if (arguments.count() > 1 )
	{
	
	QString scriptfilename = arguments.at(1);

	if(scriptfilename.contains("igs") || scriptfilename.contains("IGS"))
		{
       
		scriptfilename.replace("\\","\\\\");
		
		QString code = QString("shape = importigs(\"") + scriptfilename + QString(" \" ) \n vis(shape) \n fitall()");
	    
		scriptw->settext(code);
        scriptw->evaluatetext();

		goto runapp;
		
	}

	QFile data(scriptfilename);

	if (data.open(QFile::ReadOnly)) {
     QTextStream out(&data);
	 QString code = out.readAll();
	 data.close();

	 scriptw->evaluatetextonly(code);
	 qDebug() << scriptw->resultstream;
	}
	



	app.quit();
	return app.exit();


	}


	runapp:

	appui::getInstance()->ShowMaximized(); // shows the app window maximized

	//qGeomApp->symboltree->addnewwidget(scriptw);
	qGeomApp->splitter->insertWidget(0,scriptw);
	
	
//	qGeomApp->myview->getView()->SetAnimationModeOn();
	//qGeomApp->myview->getView()->EnableGLLight();
	qGeomApp->myview->getView()->SetAntialiasingOff();
	qGeomApp->myview->getView()->SetLightOff();

	


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


	

	//
	//PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut);
 //   PythonQtGui::init();



	//
	//PythonQtObjectPtr pycontext = PythonQt::self()->getMainModule();

	//

	// // register the type with QMetaType
 // qRegisterMetaType<QoccFrame>("qoccframe");
 // // add a wrapper object for the new variant type
 // //PythonQt::self()->addVariantWrapper("qoccframe", new QoccFrame());


	//HsfScriptingInterface* hsf = scriptw->hsfapi;

	//pycontext.addObject("hsf", scriptw); 
	//PythonQtScriptingConsole console(NULL, pycontext);
	//console.show();

	

	//MainWindow * mywin = new MainWindow();

	return app.exec();
}
