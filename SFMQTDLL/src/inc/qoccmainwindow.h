/************************************************************************************
**
** This file is part of the QtOPENCASCADE Toolkit.
**
** Copyright (C) 2006, 2007, 2008 QtOCC Team Members
**               Peter Dolbey, Marc Britten, Stephane Routelous
**               Stefan Boeykens, Pawel Dobrolowski, Walter Steffe
**               Álvaro Castro Castilla, Dirk Burski, Fotis Sioutis
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

#include "qocccontroller.h"
#include "qoccframe.h"
#include "occviewport.h"
#include "qoccinputoutput.h"
#include "occviewport.h"



class QAction;
class QLabel;
class QMenu;

class Q_DECL_EXPORT QoccMainWindow : public QMainWindow
{
    Q_OBJECT

public:

    QoccMainWindow();
	~QoccMainWindow() {};
	QoccController* myController;

	QoccFrame*      mySketchFrame;
	QoccFrame*      myGeometryFrame;
	QoccFrame*      myAssemblyFrame;
	QoccFrame*      myMeshFrame;

protected:
	virtual void closeEvent( QCloseEvent* event );
	
signals:
	void fileOpening();
	void fileOpened();

private slots:
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void fileImport();
    void fileExport();
    void filePrint();
	void fileClose();

    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void about();
	/*void bottle();*/
    void aboutQt();
	//void xyzPosition ( occviewport* widget );
	//void addPoint (V3d_Coordinate X, V3d_Coordinate Y, V3d_Coordinate Z);

	void statusMessage (const QString aMessage);

	void selectNeutralPoint( void );
	void selectVertices( void );
	void selectEdges( void );
	void selectWires( void );
	void selectFaces( void );
	void selectShells( void );
	void selectSolids( void );
	void selectCompSolids( void );
	void selectCompounds( void );
	void selectShapes( void );
	


	int checkSave();

private:
	QAction* createAction(	const QString& caption, 
							const QString& shortcut, 
							const QString& tip,
							const char* slot,
							const QObject* target = NULL);

    void createActions( void );
    void createMenus( void );
	void createToolBars( void );

    QMenu *fileMenu;
    QMenu *editMenu;
	QMenu *excelmenu;
	QMenu *drawMenu;
	QMenu *viewMenu;
	QMenu *viewActionsMenu;
	QMenu *viewDisplayMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;
	QMenu *gridMenu;
	QMenu *selectMenu;

    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *importAction;
    QAction *exportAction;
    QAction *printAction;
    QAction *exitAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *aboutAction;

	QAction *fitAction;
	QAction *fitAllAction;
	QAction *zoomAction;
	QAction *panAction;
	QAction *rotAction;

	QAction *gridXYAction;
	QAction *gridXZAction;
	QAction *gridYZAction;
	QAction *gridOnAction;
	QAction *gridOffAction;
	QAction *gridRectAction;
	QAction *gridCircAction;

	QAction *viewFrontAction;
	QAction *viewBackAction;
	QAction *viewTopAction;
	QAction *viewBottomAction;
	QAction *viewLeftAction;
	QAction *viewRightAction;
	QAction *viewAxoAction;
	QAction *viewResetAction;
	QAction *viewGridAction;		// Grid view action 

	QAction *selectNeutralAction;
	QAction *selectCompoundAction;	// TopAbs_COMPOUND
	QAction *selectCompSolidAction;	// TopAbs_COMPSOLID
	QAction *selectSolidAction;		// TopAbs_SOLID
	QAction *selectShellAction;		// TopAbs_SHELL
	QAction *selectFaceAction;		// TopAbs_FACE
	QAction *selectWireAction;		// TopAbs_WIRE
	QAction *selectEdgeAction;		// TopAbs_EDGE
	QAction *selectVertexAction;	// TopAbs_VERTEX
	QAction *selectShapeAction;		// TopAbs_SHAPE

	QAction *refreshGraphAction;

	QAction *pointAction;
	QAction *PointParameterAction;	//build point on curve by parameter
	QAction *PointsParameterAction; // loop of points
	QAction *BezierSplineAction;     // Bezier Spline
	QAction *circle2PtAction;		// Circle drawing action
	QAction *ellipseCenter2ptAction;		// Circle drawing action
	QAction *line2PtAction;			// Line drawing action
	QAction *intersectaction;			// intersection drawing action
	QAction *uvcrvonsrfaction;			// Add new UV crv on parameter space of surface drawing action
	QAction *ellispoidfromexcelaction;
	
    QAction *EditObjectAction;
	QAction *AddXlSheetAction;
	
	QAction *cancelDrawAction;

	QAction *backgroundAction;

    QAction *aboutQtAction;

	//Dialog Actions
	QAction *dlgCircArcAction;

	//Sample Action
	QAction *bottleAction;
	

	QTabBar*		myTabBar;
    QTabWidget*		myTabWidget;
    QWidget*		myTab;


	

	QToolBar*		drawToolBar;
	
	QString myLastFolder;
	/*QoccTreeWidget* myTree;
	QDockWidget *myDockTree;*/

};

#endif

