/************************************************************************************
**
** This file is part of the QtOPENCASCADE Toolkit.
**
** Copyright (C) 2006, 2007, 2008 QtOCC Team Members
**               Peter Dolbey, Marc Britten, Stephane Routelous
**               Stefan Boeykens, Pawel Dobrolowski, Walter Steffe
**               Alvaro Castro Castilla, Dirk Burski, Fotis Sioutis
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
** 
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
***********************************************************************************/

/*
$Revision: 82 $
$Date: 2008-07-16 21:34:09 -1100 (Wed, 16 Jul 2008) $
$Author: dirkski $
*/

#include <QtGui/QtGui>
#include <QtGui/QFileDialog>
#include <QtCore/QTextStream>
#include <QtCore/QFileInfo>
#include <QtCore/QString>

#include <Aspect_RectangularGrid.hxx>
#include <V3d_View.hxx>

#include "qoccapplication.h"
#include "qoccdocument.h"
#include "qoccmainwindow.h"
//#include "qocccommands.h"
//#include "dialogs\qoccdlgeditobject.h"
#include "trace.h"
#include "excelcontroller.h"
#include <QProgressDialog>
#include <QMap>
#include <QMapIterator>
#include <BRepLib.hxx>
//#include <GEOMImpl_ShapeDriver.hxx>
#include <qDebug.h>
//#include "graphwidget.h"




void LoadBottle ( Handle(AIS_InteractiveContext) theContext );
void ShowOrigin ( Handle(AIS_InteractiveContext) theContext );
void AddVertex  ( double x, double y, double z, Handle(AIS_InteractiveContext) theContext );

QoccMainWindow::QoccMainWindow()
: myLastFolder(tr(""))
{
	
	//this->setWindowFlags(Qt::FramelessWindowHint);
	//this->setWindowFlags(Qt::Tool);

	//QsciScintilla* textEdit = new QsciScintilla();


	Handle_TDocStd_Document aDoc;
	QoccDocument*           qDoc = NULL;
	
	qDoc = new QoccDocument(  1 );
	
	myController  = new QoccController( qDoc );
	

	myTabWidget = new QTabWidget( this );
	myTabWidget->setObjectName( QString::fromUtf8( "myTabWidget" ) );
	myTabWidget->setTabShape( QTabWidget::Rounded );

	mySketchFrame = new QoccFrame( myController, this );
	
	myTabWidget->addTab( mySketchFrame, tr( "View1" ) );
	

	setCentralWidget( myTabWidget );
	
		
	createActions();
	createMenus();
	//createToolBars();
	/*createDockTree();*/
	
    statusBar()->showMessage(tr("A context menu is available by right-clicking"));
	
	//pawel start 2008-03-07
	setWindowIcon(QIcon(QString::fromUtf8(":/icons/icons/qtocc_logo.png")));
	//pawel stop 2008-03-07
	setWindowTitle(tr("FLEX-IN/FLEX-OUT"));
	setMinimumSize(160, 160);

	

	
}

void QoccMainWindow::fileNew()
{
	
    statusBar()->showMessage(tr("Invoked File|New"));
	myController->clearCommand ();

	int ret = checkSave();
	if( ret == QMessageBox::Save )
		fileSave();
	else if( ret == QMessageBox::Cancel )
		return;

//	qGeomApp->closeDocument( myController->getDocument() );

	//qGeomApp->msleep ( 200 );
    
	Handle_TDocStd_Document aDoc;
	QoccDocument*           qDoc = NULL;
	
	qDoc = new QoccDocument(  1 );
	
	myController  = new QoccController( qDoc );

	myTabWidget = new QTabWidget( this );
	myTabWidget->setObjectName( QString::fromUtf8( "Geometry View Tab" ) );
	myTabWidget->setTabShape( QTabWidget::Rounded );

	mySketchFrame = new QoccFrame( myController, this );
	myTabWidget->addTab( mySketchFrame, tr( "Geometry View" ) );

	/*doc->updatePresentations( doc->getOcafDoc()->Main() );*/
	
	setDockNestingEnabled(true);
	setCentralWidget( myTabWidget );
	//createDockTree();		
	myController->sendCompleted();

	


}

void QoccMainWindow::fileOpen()
{
	
	
}

void QoccMainWindow::fileSave()
{
 	
}

void QoccMainWindow::fileSaveAs()
{
	
}

void QoccMainWindow::fileImport()
{
	QString		fileName;
	QString		fileType;
	QFileInfo	fileInfo;

	QoccInputOutput::FileFormat format;
	QoccInputOutput reader;

  statusBar()->showMessage(tr("Invoked File|Open"));
	myController->clearCommand ();

	fileName = QFileDialog::getOpenFileName ( this,
				  							  tr("Import File"),
											  myLastFolder,
											  tr( "All drawing types (*.brep *.rle *.igs *iges *.stp *.step *stl);;"
											      "BREP (*.brep *.rle);;"
												  "CATIA (*.CATPart *.CATProduct);;"
												  "STEP (*.step *.stp);;"
												  "IGES (*.iges *.igs);;"
												  "STL (*.stl)" ));
	if (!fileName.isEmpty())
	{
		fileInfo.setFile(fileName);
		fileType = fileInfo.suffix();
		if (fileType.toLower() == tr("brep") || fileType.toLower() == tr("rle"))
		{
			format = QoccInputOutput::FormatBREP;
		}
		if (fileType.toLower() == tr("step") || fileType.toLower() == tr("stp"))
		{
			format = QoccInputOutput::FormatSTEP;
		}
		if (fileType.toLower() == tr("iges") || fileType.toLower() == tr("igs"))
		{
			format = QoccInputOutput::FormatIGES;
		}
		if (fileType.toLower() == tr("catpart") || fileType.toLower() == tr("catproduct"))
		{
		format = QoccInputOutput::FormatCATIA;
		}
		if (fileType.toLower() == tr("stl"))
		{
			format = QoccInputOutput::FormatSTL;
		}
		myController->clearAllLocalContexts();
		myLastFolder = fileInfo.absolutePath();
		emit fileOpening();
		reader.importModel ( fileInfo.absoluteFilePath(), format, myController->getContext() );
		emit fileOpened();

		

	}
}

void QoccMainWindow::fileExport()
{

	QString		fileName;
	QString		fileType;
	QFileInfo	fileInfo;

	QoccInputOutput::FileFormat format;
	QoccInputOutput reader;

  statusBar()->showMessage(tr("Invoked File|Open"));
	myController->clearCommand ();

	fileName = QFileDialog::getSaveFileName ( this,
				  							  tr("Export File"),
											  myLastFolder,
											  tr( "All drawing types (*.brep *.rle *.igs *iges *.stp *.step *stl);;"
											      "BREP (*.brep *.rle);;"
												  "STEP (*.step *.stp);;"
												  "IGES (*.iges *.igs);;"
												  "STL (*.stl)" ));
	if (!fileName.isEmpty())
	{
		fileInfo.setFile(fileName);
		fileType = fileInfo.suffix();
		if (fileType.toLower() == tr("brep") || fileType.toLower() == tr("rle"))
		{
			format = QoccInputOutput::FormatBREP;
		}
		if (fileType.toLower() == tr("step") || fileType.toLower() == tr("stp"))
		{
			format = QoccInputOutput::FormatSTEP;
		}
		if (fileType.toLower() == tr("iges") || fileType.toLower() == tr("igs"))
		{
			format = QoccInputOutput::FormatIGES;
		}
		if (fileType.toLower() == tr("stl"))
		{
			format = QoccInputOutput::FormatSTL;
		}
	
		myController->clearAllLocalContexts();
		myLastFolder = fileInfo.absolutePath();
		
		reader.exportModel( fileInfo.absoluteFilePath(), format, myController->getContext() );
		

		

	}


}

void QoccMainWindow::filePrint()
{
  statusBar()->showMessage(tr("Invoked File|Print"));
	myController->clearCommand ();
}

void QoccMainWindow::fileClose()
{
    statusBar()->showMessage(tr("Invoked File|Close"));
	myController->clearCommand ();

	fileNew();
}

void QoccMainWindow::undo()
{
  statusBar()->showMessage(tr("Invoked Edit|Undo"));
	myController->undo();
	myController->clearCommand ();
}

void QoccMainWindow::redo()
{
  statusBar()->showMessage(tr("Invoked Edit|Redo"));
	myController->redo();
	myController->clearCommand ();
}

void QoccMainWindow::cut()
{
  statusBar()->showMessage(tr("Invoked Edit|Cut"));
	myController->clearCommand ();
}

void QoccMainWindow::copy()
{
  statusBar()->showMessage(tr("Invoked Edit|Copy"));
	myController->clearCommand ();
}

void QoccMainWindow::paste()
{
  statusBar()->showMessage(tr("Invoked Edit|Paste"));
	myController->clearCommand ();
}

void QoccMainWindow::about()
{
  statusBar()->showMessage(tr("Invoked Help|About"));
	myController->clearCommand ();
  QMessageBox::about(this, tr("About Menu"),
                     tr("This Software was developed by <b>Alexander Pena de Leon</b>"
	                      "for the submission of his PHD in Architecture"
						  "@ Sial in RMIT University Australia."));
}

void QoccMainWindow::aboutQt()
{
	statusBar()->showMessage(tr("Invoked Help|About Qt"));
	qApp->aboutQt();
}

void QoccMainWindow::selectNeutralPoint()
{
	statusBar()->showMessage(tr("Invoked Select|Clear Filters"));
	myController->clearAllLocalContexts();
}
void QoccMainWindow::selectFaces()
{
	statusBar()->showMessage(tr("Invoked Select|Faces"));
	myController->setLocalContext(TopAbs_FACE);
}

void QoccMainWindow::selectVertices()
{
	statusBar()->showMessage(tr("Invoked Select|Vertices"));
	myController->setLocalContext(TopAbs_VERTEX);
}

void QoccMainWindow::selectEdges()
{
	statusBar()->showMessage(tr("Invoked Select|Edges"));
	myController->setLocalContext(TopAbs_EDGE);
}
void QoccMainWindow::selectWires()
{
	statusBar()->showMessage(tr("Invoked Select|Wires"));
	myController->setLocalContext(TopAbs_WIRE);
}
void QoccMainWindow::selectShells()
{
	statusBar()->showMessage(tr("Invoked Select|Shells"));
	myController->setLocalContext(TopAbs_SHELL);
}
void QoccMainWindow::selectSolids()
{
	statusBar()->showMessage(tr("Invoked Select|Solids"));
	myController->setLocalContext(TopAbs_SOLID);
}
void QoccMainWindow::selectCompSolids()
{
	statusBar()->showMessage(tr("Invoked Select|Compound Solids"));
	myController->setLocalContext(TopAbs_COMPSOLID);
}
void QoccMainWindow::selectCompounds()
{
	statusBar()->showMessage(tr("Invoked Select|Compounds"));
	myController->setLocalContext(TopAbs_COMPOUND);
}
void QoccMainWindow::selectShapes()
{
	statusBar()->showMessage(tr("Invoked Select|Shapes"));
	myController->setLocalContext(TopAbs_SHAPE);
}





void QoccMainWindow::statusMessage (const QString aMessage)
{
	statusBar()->showMessage(aMessage);
}

/*!
\brief	This routine is a utility function for creating actions. Use tr("str")
		in the calling function to provide support for language translations.
\param	caption		the menu caption.
\param  shortcut	keyboard shot.
\param	tip			the tool tip text.
\param	slot		the slot function to be called, e.g. use SLOT(fooBar()) .
\param	target		the QObject target of the connection, if not this object.
*/
QAction* QoccMainWindow::createAction(const QString& caption,
									  const QString& shortcut,
									  const QString& tip,
									  const char* slot,
									  const QObject* target)
{
	QAction* anAction = new QAction( caption, this );
    anAction->setShortcut( shortcut );
    anAction->setStatusTip( tip );
	connect( anAction, SIGNAL(triggered()), target == NULL ? this : target, slot );
	return anAction;
}

void QoccMainWindow::createActions()
{
	newAction     = createAction( tr("&New"),     tr("Ctrl+N"), tr("Create a new file"),         SLOT(fileNew()) );
	openAction    = createAction( tr("&Open..."), tr("Ctrl+O"), tr("Open an existing file"),     SLOT(fileOpen()) );
	saveAction    = createAction( tr("&Save"),    tr("Ctrl+S"), tr("Save the document to disk"), SLOT(fileSave()) );
	saveAsAction  = createAction( tr("Save &As"), tr(""), tr("Save the document to disk"), SLOT(fileSaveAs()) );
	importAction  = createAction( tr("&Import"),  tr("Ctrl+I"), tr("Import a file"), SLOT(fileImport()) );
	exportAction  = createAction( tr("&Export"),  tr("Ctrl+E"), tr("Export the model"), SLOT(fileExport()) );
	printAction   = createAction( tr("&Print..."),tr("Ctrl+P"), tr("Print the document"),        SLOT(filePrint()) );

	exitAction  = createAction( tr("E&xit"),    tr("Ctrl+X"), tr("Exit the application"),      SLOT(close()) );

	undoAction  = createAction( tr("&Undo"),    tr("Ctrl+Z"), tr("Undo the last operation"),   SLOT(undo()) );
	redoAction  = createAction( tr("&Redo"),    tr("Ctrl+Y"), tr("Redo the last operation"),   SLOT(redo()) );
	cutAction   = createAction( tr("Cu&t"),     tr("Ctrl+X"), tr("Cut to the clipboard"),      SLOT(cut()) );
	copyAction  = createAction( tr("&Copy"),    tr("Ctrl+C"), tr("Copy to the clipboard"),     SLOT(copy()) );
	pasteAction = createAction( tr("&Paste"),   tr("Ctrl+V"), tr("Paste the clipboard"),       SLOT(paste()) );

	aboutAction   = createAction( tr("&About"),    tr(""), tr("Show the application's About box"),  SLOT(about()) );
	aboutQtAction = createAction( tr("About &Qt"), tr(""), tr("Show the Qt library's About box"),   SLOT(aboutQt()) );
	gridOnAction  = createAction( tr("&Grid On"),  tr("Ctrl+G"), tr("Turn the grid on"), SLOT(gridOn()),  myController );
	gridOffAction = createAction( tr("Gri&d Off"), tr("Ctrl+D"), tr("Turn the grid off"), SLOT(gridOff()), myController );

	gridXYAction = createAction( tr("XY Grid"), tr(""), tr("Grid on XY Plane"), SLOT(gridXY()), myController );
	gridXZAction = createAction( tr("XZ Grid"), tr(""), tr("Grid on XZ Plane"), SLOT(gridXZ()), myController );
	gridYZAction = createAction( tr("YZ Grid"), tr(""), tr("Grid on YZ Plane"), SLOT(gridYZ()), myController );

	gridRectAction = createAction( tr("Rectangular"), tr(""), tr("Retangular grid"), SLOT(gridRect()), myController );
	gridCircAction = createAction( tr("Circular"),    tr(""), tr("Circular grid"),   SLOT(gridCirc()), myController );

	pointAction = new QAction(tr("&Points"), this);
    pointAction->setStatusTip(tr("Points"));
	pointAction->setIcon(QIcon(QString::fromUtf8(":/icons/icons/point.png")));
	connect( pointAction, SIGNAL( triggered() ), this, SLOT( vertex()) );

	PointParameterAction = new QAction(tr("&Point Parameter"), this);
    PointParameterAction->setStatusTip(tr("Point Parameter "));
	PointParameterAction->setIcon(QIcon(QString::fromUtf8(":/icons/icons/point.png")));
	connect( PointParameterAction, SIGNAL( triggered() ), this, SLOT( vertexbyparameter()) );

	PointsParameterAction = new QAction(tr("&Loop:Point Parameter"), this);
	PointsParameterAction->setStatusTip(tr("Loop:Point Parameter "));
	PointsParameterAction->setIcon(QIcon(QString::fromUtf8(":/icons/icons/point.png")));
	connect( PointsParameterAction, SIGNAL( triggered() ), this, SLOT( vertexsbyparameter()) );
	
	BezierSplineAction = new QAction(tr("&Bezier Spline"), this);
	BezierSplineAction->setStatusTip(tr("Bezier Spline "));
	BezierSplineAction->setIcon(QIcon(QString::fromUtf8(":/icons/icons/spline.png")));
	connect( BezierSplineAction, SIGNAL( triggered() ), this, SLOT( bezierspline()) );

	EditObjectAction = new QAction(tr("&Edit Object"), this);
	EditObjectAction->setStatusTip(tr("Edit Object "));
	//EditObjectAction->setIcon(QIcon(QString::fromUtf8(":/icons/icons/spline.png")));
	connect( EditObjectAction, SIGNAL( triggered() ), this, SLOT( ShoweditObject()) );

	
	circle2PtAction = new QAction(tr("&Circle Centre, Radius"), this);
	circle2PtAction->setStatusTip(tr("Circle - centre, radius"));
	circle2PtAction->setIcon(QIcon(QString::fromUtf8(":/icons/icons/circlecentre.png")));
	connect( circle2PtAction, SIGNAL( triggered() ), this, SLOT( circleCentre()) );

	ellipseCenter2ptAction = new QAction(tr("&Ellipse, Center Minor Major"), this);
	ellipseCenter2ptAction->setStatusTip(tr("Ellipse, Center Minor Major"));
	ellipseCenter2ptAction->setIcon(QIcon(QString::fromUtf8(":/icons/icons/circlecentre.png")));
	connect( ellipseCenter2ptAction, SIGNAL( triggered() ), this, SLOT( ellipsecenterminormajor()) );

	line2PtAction = new QAction(tr("&Line"), this);
	line2PtAction->setStatusTip(tr("Line - 2 point"));
	line2PtAction->setIcon(QIcon(QString::fromUtf8(":/icons/icons/line.png")));
	connect( line2PtAction, SIGNAL( triggered() ), this, SLOT( line2Pt()) );

	intersectaction = new QAction(tr("&Intersect"), this);
	intersectaction->setStatusTip(tr("Intersect - 2 shapes"));
	intersectaction->setIcon(QIcon(QString::fromUtf8(":/icons/icons/line.png")));
	connect( intersectaction, SIGNAL( triggered() ), this, SLOT( intersectshapes()) );

	uvcrvonsrfaction = new QAction(tr("&UVcrv"), this);
	uvcrvonsrfaction->setStatusTip(tr("UVcrv on surface"));
	uvcrvonsrfaction->setIcon(QIcon(QString::fromUtf8(":/icons/icons/spline.png")));
	connect( uvcrvonsrfaction, SIGNAL( triggered() ), this, SLOT( uvcrvonsrf() ) );

	

	cancelDrawAction = new QAction(tr("Cancel"), this);
	cancelDrawAction->setStatusTip(tr("Cancel"));
	connect( cancelDrawAction, SIGNAL( triggered() ), this, SLOT( cancelDraw()) );
	
	AddXlSheetAction = new QAction(tr("AddExcelSheet"), this);
	AddXlSheetAction->setStatusTip(tr("AddExcelSheet"));
	connect( AddXlSheetAction, SIGNAL( triggered() ), this, SLOT( AddSheettoExcel()) );
	

/*	To be completed
	QAction *selectNeutralAction;
	QAction *selectCompoundAction;	
	QAction *selectCompSolidAction;	
	QAction *selectCompSolid;		
	QAction *selectShapeAction;		
*/
	selectVertexAction  = createAction(tr("&Vertices"), "", tr("Select Vertices"),     SLOT(selectVertices()));
	selectEdgeAction    = createAction(tr("&Edges"),    "", tr("Select Edges"),        SLOT(selectEdges()));
	selectWireAction    = createAction(tr("&Wires"),    "", tr("Select Wires"),        SLOT(selectWires()));
	selectFaceAction    = createAction(tr("&Faces"),    "", tr("Select Faces"),        SLOT(selectFaces()));
	selectShellAction   = createAction(tr("&Shells"),   "", tr("Select Shells"),       SLOT(selectShells()));
	selectNeutralAction = createAction(tr("&Normal"),   "", tr("No selection filter"), SLOT(selectNeutralPoint()));

	// And the bottle example

	bottleAction = new QAction(tr("Load &Bottle"), this);
	bottleAction->setShortcut(tr("Ctrl+B"));
	bottleAction->setStatusTip(tr("Bottle sample."));
	connect(bottleAction, SIGNAL(triggered()), this, SLOT(bottle()));

}

void QoccMainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu( tr("&File") );
	{
		fileMenu->addAction( newAction );
		fileMenu->addAction( openAction );
		fileMenu->addAction( saveAction );
		fileMenu->addAction( saveAsAction );
		fileMenu->addAction( importAction );
		fileMenu->addAction( exportAction );
		fileMenu->addAction( printAction );
	
/*
		Comment out the 2 lines below to hide the
		Load Bottle menu option - still left in for
		now as a demo feature.
*/
		fileMenu->addSeparator();
		fileMenu->addAction( bottleAction );

		fileMenu->addSeparator();
		fileMenu->addAction( exitAction );
	}

	editMenu = menuBar()->addMenu( tr("&Edit") );
	{
		editMenu->addAction( undoAction );
		editMenu->addAction( redoAction );
		editMenu->addSeparator();
		editMenu->addAction( cutAction );
		editMenu->addAction( copyAction );
		editMenu->addAction( pasteAction );
	}

		excelmenu = menuBar()->addMenu( tr("&Excel") );
	{
		excelmenu->addAction( AddXlSheetAction );
	}

	

	drawMenu = menuBar()->addMenu( tr("&Draw") );
	{
		drawMenu->addAction( pointAction );
		drawMenu->addAction( PointParameterAction );
		drawMenu->addAction( PointsParameterAction );
		drawMenu->addAction( line2PtAction );
		drawMenu->addAction( circle2PtAction );
		drawMenu->addAction(ellipseCenter2ptAction);
		drawMenu->addAction( BezierSplineAction );
		drawMenu->addAction( intersectaction );
		drawMenu->addAction( uvcrvonsrfaction );
		
		
		drawMenu->addSeparator();
		drawMenu->addAction( cancelDrawAction );
		drawMenu->addAction( EditObjectAction);
	}

	selectMenu = menuBar()->addMenu( tr("&Select") );
		selectMenu->addAction( selectVertexAction );
		selectMenu->addAction( selectEdgeAction );
		selectMenu->addAction( selectWireAction );
		selectMenu->addAction( selectFaceAction );
		selectMenu->addAction( selectShellAction );
		selectMenu->addSeparator();
		selectMenu->addAction( selectNeutralAction );

	viewMenu = menuBar()->addMenu( tr("&View") );
/*
		viewDisplayMenu = viewMenu->addMenu( tr("&Display") );
			viewDisplayMenu->addAction( viewFrontAction );
			viewDisplayMenu->addAction( viewBackAction );
			viewDisplayMenu->addAction( viewTopAction );
			viewDisplayMenu->addAction( viewBottomAction );
			viewDisplayMenu->addAction( viewLeftAction );
			viewDisplayMenu->addAction( viewRightAction );
			viewDisplayMenu->addSeparator();
			viewDisplayMenu->addAction( viewAxoAction );
			viewDisplayMenu->addAction( viewGridAction );
			viewDisplayMenu->addSeparator();
			viewDisplayMenu->addAction( viewResetAction );
			viewDisplayMenu->addSeparator();
			viewDisplayMenu->addAction( backgroundAction );
*/
/*
		viewActionsMenu = viewMenu->addMenu( tr("&Actions") );
			viewActionsMenu->addAction( fitAction );
			viewActionsMenu->addAction( fitAllAction );
			viewActionsMenu->addAction( zoomAction );
			viewActionsMenu->addAction( panAction );
			viewActionsMenu->addAction( rotAction );
*/

	gridMenu = viewMenu->addMenu( tr("&Grid") );
	{
		gridMenu->addAction( gridOnAction );
		gridMenu->addAction( gridOffAction );
		gridMenu->addSeparator();
		gridMenu->addAction( gridXYAction );
		gridMenu->addAction( gridXZAction );
		gridMenu->addAction( gridYZAction );
		gridMenu->addSeparator();
		gridMenu->addAction( gridRectAction );
		gridMenu->addAction( gridCircAction );
	}
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);

}

// Now for the old QtOCCViewWidget slots (still to be completed)
/*
	fitAction = new QAction(tr("&Fit Window"), this);
	fitAction->setShortcut(tr("Ctrl+F"));
    fitAction->setStatusTip(tr("Fit to window"));
    connect(fitAction, SIGNAL(triggered()), myOCC, SLOT(fitExtents()));

	fitAllAction = new QAction(tr("Fit &All"), this);
	fitAllAction->setShortcut(tr("Ctrl+A"));
    fitAllAction->setStatusTip(tr("Fit contents to viewport"));
    connect(fitAllAction, SIGNAL(triggered()), myOCC, SLOT(fitAll()));

	zoomAction = new QAction(tr("&Zoom"), this);
	zoomAction->setStatusTip(tr("Zoom in window"));
    connect(zoomAction, SIGNAL(triggered()), myOCC, SLOT(fitArea()));

	panAction = new QAction(tr("&Pan"), this);
    panAction->setStatusTip(tr("Window panning"));
    connect(panAction, SIGNAL(triggered()), myOCC, SLOT(pan()));

	rotAction = new QAction(tr("&Rotate"), this);
	rotAction->setShortcut(tr("Ctrl+R"));
    rotAction->setStatusTip(tr("Window rotation"));
    connect(rotAction, SIGNAL(triggered()), myOCC, SLOT(rotation()));
*/
	// Standard View
/*
	viewFrontAction = new QAction(tr("Front"), this);
	viewFrontAction->setStatusTip(tr("View From Front"));
    connect(viewFrontAction, SIGNAL(triggered()), myOCC, SLOT(viewFront()));

	viewBackAction = new QAction(tr("Back"), this);
	viewBackAction->setStatusTip(tr("View From Back"));
    connect(viewBackAction, SIGNAL(triggered()), myOCC, SLOT(viewBack()));

	viewTopAction = new QAction(tr("Top"), this);
	viewTopAction->setStatusTip(tr("View From Top"));
    connect(viewTopAction, SIGNAL(triggered()), myOCC, SLOT(viewTop()));

	viewBottomAction = new QAction(tr("Bottom"), this);
	viewBottomAction->setStatusTip(tr("View From Bottom"));
    connect(viewBottomAction, SIGNAL(triggered()), myOCC, SLOT(viewBottom()));

	viewLeftAction = new QAction(tr("Left"), this);
	viewLeftAction->setStatusTip(tr("View From Left"));
    connect(viewLeftAction, SIGNAL(triggered()), myOCC, SLOT(viewLeft()));

	viewRightAction = new QAction(tr("Right"), this);
	viewRightAction->setStatusTip(tr("View From Right"));
    connect(viewRightAction, SIGNAL(triggered()), myOCC, SLOT(viewRight()));

	viewAxoAction = new QAction(tr("Axonometric"), this);
	viewAxoAction->setStatusTip(tr("Axonometric view"));
    connect(viewAxoAction, SIGNAL(triggered()), myOCC, SLOT(viewAxo()));

	viewGridAction = new QAction(tr("Grid"), this);
	viewGridAction->setStatusTip(tr("View from grid"));
    connect(viewGridAction, SIGNAL(triggered()), myOCC, SLOT(viewGrid()));

	viewResetAction = new QAction(tr("Reset"), this);
	viewResetAction->setStatusTip(tr("Reset the view"));
    connect(viewResetAction, SIGNAL(triggered()), myOCC, SLOT(viewReset()));

	backgroundAction = new QAction( tr("&Background"), this );
	backgroundAction->setStatusTip(tr("Change the background colour"));
	connect(backgroundAction, SIGNAL(triggered()), myOCC, SLOT(background()));

	// The co-ordinates from the view
	connect( myOCC, SIGNAL(mouseMoved(V3d_Coordinate,V3d_Coordinate,V3d_Coordinate)),
		     this,   SLOT(xyzPosition(V3d_Coordinate,V3d_Coordinate,V3d_Coordinate)) );

	// Add a point from the view
	connect( myOCC, SIGNAL(pointClicked(V3d_Coordinate,V3d_Coordinate,V3d_Coordinate)),
		     this,   SLOT (addPoint    (V3d_Coordinate,V3d_Coordinate,V3d_Coordinate)) );

	connect( myOCC, SIGNAL(sendStatus(const QString)),
		     this,  SLOT  (statusMessage(const QString)) );
*/


void QoccMainWindow::createToolBars()
{
	drawToolBar = new QToolBar (tr("Drawing tools"), this);
	drawToolBar->setMinimumHeight(50);
	drawToolBar->setMinimumWidth(50);
	

	this->addToolBar(Qt::TopToolBarArea, drawToolBar);
	drawToolBar->setObjectName(QString::fromUtf8("drawToolBar"));
    drawToolBar->setCursor(QCursor(Qt::PointingHandCursor));
    
	drawToolBar->addAction( pointAction );
	drawToolBar->addAction( PointParameterAction);
	drawToolBar->addAction( PointsParameterAction);
	drawToolBar->addAction( line2PtAction );
	drawToolBar->addAction( circle2PtAction );
	drawToolBar->addAction( BezierSplineAction );
	drawToolBar->addAction( uvcrvonsrfaction );
	


}


//void QoccMainWindow::createDockTree()
//{
//	if( myTree )
//		delete myTree;
//	if( myDockTree )
//	{
//		viewMenu->removeAction(myDockTree->toggleViewAction());
//		removeDockWidget(myDockTree);
//		delete myDockTree;
//	}
//	myDockTree = new QDockWidget(tr("Specification tree"), this);
//	myTree = new QoccTreeWidget( myController, this );
//	myDockTree->setWidget(myTree);
//	addDockWidget(Qt::LeftDockWidgetArea , myDockTree);
//	viewMenu->addAction(myDockTree->toggleViewAction());
//	
//}

int QoccMainWindow::checkSave()
{
	//if( myController->getDocument()->getOcafDoc()->IsChanged() )
	//	return QMessageBox::question( this, "QtGeom.", "The document has been modified.\nDo you want to save your changes?", 
	//						   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save );
	//else
	return 0;
	
}

void QoccMainWindow::closeEvent( QCloseEvent* event )
{
	int ret = checkSave();
	if( ret == QMessageBox::Save )
		fileSave();
	else if( ret == QMessageBox::Cancel )
		event->setAccepted( false );
}
