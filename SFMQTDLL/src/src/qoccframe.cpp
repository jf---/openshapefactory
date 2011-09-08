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

/*
$Revision: 65 $
$Date: 2008-05-08 08:06:43 -1100 (Thu, 08 May 2008) $
$Author: pdolbey $
*/

/*!
\class	QoccFrame
\brief	This class provides a QFrame derived object containing a toolbar and an
		OpenCascade viewer widget (\see occviewport). The toolbar wraps several 
		utility functions that manipulate the view.
\author	Peter C. Dolbey
*/
#include <QtGui/QVBoxLayout>
#include <QtGui/QAction>

#include "qoccframe.h"
#include "occviewport.h"
#include "qoccmainwindow.h"

#include "graphwidget.h"
#include <QTabWidget>
#include "genericframe.h"
#include <QTreeWidget>
#include <QSplitter>

//#include "scriptwidget.h"



/*!
\brief	Constructor for QoccFrame.
		This constructor creates an uninitialised window for rendering a
		Handle_V3d_View based OpenCASCADE 3D viewer.
\param	aContext The widget's AIS context
\param	parent The widget's parent widget
\param	wflags The widget's window configuration flags
\see	initialize
*/



QoccFrame::QoccFrame(	QoccController* aController, 
						QWidget *parent, 
						Qt::WindowFlags wflags ) : 
	QFrame( parent, wflags )
{
	myController = aController;
	myParent = static_cast<QoccMainWindow*> (parent);

	QTabWidget* myTabWidget = new QTabWidget( this );
	myTabWidget->setObjectName( QString::fromUtf8( "leftsidetoolbox" ) );
	myTabWidget->setTabShape( QTabWidget::Rounded );

	

	myLayout = new QVBoxLayout( this );
	myLayout->setObjectName(QString::fromUtf8("VboxLayout"));
	myLayout->setMargin(0);
	myLayout->setSpacing(0);

	mySubLayout = new QHBoxLayout( this );
	mySubLayout->setObjectName(QString::fromUtf8("HboxLayout"));
	mySubLayout->setMargin(0);
	mySubLayout->setSpacing(0);

	myToolBar = new QToolBar( this );
	myToolBar->setAutoFillBackground(true);   
	
	myWidget = new occviewport( myController, parent, wflags );
	//myWidget->setMinimumWidth(1000);
	myWidget->getView()->EnableDepthTest();
	myWidget->getView()->EnableGLLight();

	GraphWidget* symboltree = new GraphWidget(parent);


	createActions();
	setToolbarActions();
	connectActions();
	connectParent();

//	scriptwidget* scriptw = new scriptwidget(myTabWidget);
	//qGeomApp->symboltree->addnewwidget(scriptw);

	//myTabWidget->addTab(new GenericFrame(aController,myParent,scriptw), tr( "symbolic tree" ) );
	myTabWidget->addTab(new GenericFrame(aController,myParent,symboltree), tr( "symbolic tree" ) );
	//myTabWidget->setMinimumWidth(300);

	QSplitter *splitter = new QSplitter(parent);
    splitter->addWidget(myTabWidget);
    splitter->addWidget(myWidget);


	mySubLayout->addWidget( splitter);
	//mySubLayout->addWidget( myWidget );


	myLayout->insertLayout(0, mySubLayout);

	myLayout->addWidget( myToolBar );
	qGeomApp->myview = myWidget;
	qGeomApp->symboltree = symboltree;








}
/*! 
\brief	The class destructor
*/
QoccFrame::~QoccFrame()
{
	disconnectActions();
	disconnectParent();

	delete actionFitExtents;
	delete actionFitArea;
	delete actionFitAll;
	delete actionZoomGroup;

    delete myToolBar;
	delete myLayout;
	delete myWidget;
}

/*!
\brief	createActions sets up all the singleton toolbar actions, and calls 
		the grouped action functions.
*/
void QoccFrame::createActions( void)
{
	actionIdle = new QAction( tr("Idle"), this );
	actionIdle->setStatusTip(tr("Selection Mode"));
	actionIdle->setIcon(QIcon(QString::fromUtf8(":/icons/actions/22x22/arrow.png")));

	createZoomGroup();
	createViewGroup();
	
    actionRotation = new QAction( tr("Rotate"), this );
	// actionRotation->setShortcut(tr("Ctrl+R"));
    actionRotation->setStatusTip(tr( "Window rotation") );
	actionRotation->setIcon(QIcon(QString::fromUtf8(":/icons/icons/viewrotate.png")));

	actionPan = new QAction( tr("Pan"), this );
    actionPan->setStatusTip(tr( "Window panning") );
	actionPan->setIcon(QIcon(QString::fromUtf8(":/icons/icons/viewpan.png")));
}

/*!
\brief	This function builds the menu options for the zoom tool group
		and sets the initial default action.
		p.s. there must be a better way of doing this type of menu!
*/
void QoccFrame::createZoomGroup( void )
{
	actionZoomGroup = new QAction( this );
	{
		actionFitExtents = new QAction(tr("&Fit to Extents"), this);
		actionFitExtents->setStatusTip(tr("Show entire drawing"));
		actionFitExtents->setIcon(QIcon(QString::fromUtf8(":/icons/actions/22x22/viewmagall.png")));

		actionFitArea = new QAction(tr("&Zoom Selected Area"), this);
		actionFitArea->setStatusTip(tr("Zoom to selected region"));
		actionFitArea->setIcon(QIcon(QString::fromUtf8(":/icons/actions/22x22/viewmagfit.png")));

		actionFitAll = new QAction(tr("Zoom &All"), this);
		actionFitAll->setStatusTip(tr("Zoom entire object"));
		actionFitAll->setIcon(QIcon(QString::fromUtf8(":/icons/actions/22x22/viewmag.png")));

		popupZoomGroup = new QMenu ( this );
		popupZoomGroup->addAction (actionFitExtents);
		popupZoomGroup->addAction (actionFitArea);
		popupZoomGroup->addAction (actionFitAll);
	}
	actionZoomGroup->setText(actionFitExtents->text());
	actionZoomGroup->setStatusTip(actionFitExtents->statusTip());
	actionZoomGroup->setIcon(actionFitExtents->icon());
	actionZoomGroup->setMenu(popupZoomGroup);
}

/*!
\brief	This function builds the menu options for the view tool group
		and sets the initial default action.
*/
void QoccFrame::createViewGroup( void )
{
	actionViewGroup = new QAction ( this );
	{
		actionViewTop = new QAction(tr("View from &Top"), this);
		actionViewTop->setStatusTip(tr("Top View"));
		actionViewTop->setIcon(QIcon(QString::fromUtf8(":/icons/actions/22x22/planview.png")));

		actionViewFront = new QAction(tr("View from &Front"), this);
		actionViewFront->setStatusTip(tr("Front View"));
		actionViewFront->setIcon(QIcon(QString::fromUtf8(":/icons/actions/22x22/frontview.png")));

		actionViewLeft = new QAction(tr("View from &Left"), this);
		actionViewLeft->setStatusTip(tr("Left View"));
		actionViewLeft->setIcon(QIcon(QString::fromUtf8(":/icons/actions/22x22/leftview.png")));

		actionViewTopFront = new QAction(tr("View from Top Front"), this);
		actionViewTopFront->setStatusTip(tr("Top Front View"));
		actionViewTopFront->setIcon(QIcon(QString::fromUtf8(":/icons/actions/22x22/topfrontview.png")));

		actionViewAxo = new QAction(tr("&Axonometric View"), this);
		actionViewAxo->setStatusTip(tr("Axo View"));
		actionViewAxo->setIcon(QIcon(QString::fromUtf8(":/icons/actions/22x22/axoview.png")));

		popupViewGroup = new QMenu ( this );
		popupViewGroup->addAction (actionViewTop);
		popupViewGroup->addAction (actionViewFront);
		popupViewGroup->addAction (actionViewLeft);
		popupViewGroup->addSeparator ();
		popupViewGroup->addAction (actionViewTopFront);
		popupViewGroup->addAction (actionViewAxo);
	}
	actionViewGroup->setText(actionViewAxo->text());
	actionViewGroup->setStatusTip(actionViewAxo->statusTip());
	actionViewGroup->setIcon(actionViewAxo->icon());
	actionViewGroup->setMenu(popupViewGroup);
}

/*!
\brief	This adds all the actions to the widgets toolbar
*/
void QoccFrame::setToolbarActions()
{
	myToolBar->addAction( actionIdle );
	myToolBar->addSeparator();
	myToolBar->addAction( actionZoomGroup );
	myToolBar->addSeparator();
	myToolBar->addAction( actionViewGroup );

	myToolBar->addAction( actionRotation );
	myToolBar->addAction( actionPan );
}

/*!
\brief	This function makes all the connections between the actions and the various slots
*/
void QoccFrame::connectActions()
{
	connect( actionIdle, SIGNAL(triggered()), myWidget, SLOT(idle()));

	connect( actionZoomGroup,  SIGNAL(triggered()), myWidget, SLOT(fitExtents()) );
	{
		connect( actionFitExtents, SIGNAL(triggered()), this, SLOT(fitExtents()) );
		connect( actionFitArea,    SIGNAL(triggered()), this, SLOT(fitArea()) );
		connect( actionFitAll,     SIGNAL(triggered()), this, SLOT(fitAll()) );
	}
	connect( actionViewGroup,  SIGNAL(triggered()), myWidget, SLOT(viewAxo()) );
	{
		connect( actionViewTop,      SIGNAL(triggered()), this, SLOT(viewTop()) );
		connect( actionViewLeft,     SIGNAL(triggered()), this, SLOT(viewLeft()) );
		connect( actionViewFront,    SIGNAL(triggered()), this, SLOT(viewFront()) );
		connect( actionViewTopFront, SIGNAL(triggered()), this, SLOT(viewTopFront()) );
		connect( actionViewAxo,      SIGNAL(triggered()), this, SLOT(viewAxo()) );
	}

	connect(actionRotation, SIGNAL(triggered()), myWidget, SLOT(rotation()));
	connect(actionPan     , SIGNAL(triggered()), myWidget, SLOT(pan()));
}

/*!
\brief	This function breaks all the connections on the actions.
*/
void QoccFrame::disconnectActions()
{
	actionIdle->disconnect();
	
	actionZoomGroup->disconnect();
	actionFitExtents->disconnect();
	actionFitArea->disconnect();
	actionFitAll->disconnect();

	actionViewGroup->disconnect();
	actionViewTop->disconnect();
	actionViewLeft->disconnect();
	actionViewFront->disconnect();
	actionViewTopFront->disconnect();
	actionViewAxo->disconnect();

	actionRotation->disconnect();
	actionPan->disconnect();
}

/*!
\brief	This function provides the "glue" to connect the events from the OCC child widget
		back up to the parent window / widget, or the UI controller.
*/
void QoccFrame::connectParent()
{
	// Connect the messages from this widget to the status bar
	// display on the main window.
	connect( myWidget, SIGNAL(sendStatus(const QString)), 
			 myParent, SLOT  (statusMessage(const QString)) );

	// Connect application event signals to the widget
	connect( myParent, SIGNAL(fileOpening()), 
		     myWidget, SLOT  (refresh()) );
	connect( myParent, SIGNAL(fileOpened()),  
		     myWidget, SLOT  (newShape()) );

	// connect widget events to the context wrapper
	connect( myWidget,       SIGNAL(mouseMoved(occviewport*, QMouseEvent*)), 
		     myController,   SLOT  (moveEvent(occviewport*,  QMouseEvent*)) );
	// connect widget events to the context wrapper
	connect( myWidget,       SIGNAL(mouseClicked(occviewport*, QMouseEvent*)), 
		     myController,   SLOT  (clickEvent(occviewport*,   QMouseEvent*)) );
	// connect widget events to the context wrapper
	connect( myWidget,       SIGNAL(selectionChanged()) , 
		     myController,   SLOT  (onSelectionChanged()) );

}

void QoccFrame::connectParent(occviewport* thewidget)
{
	// Connect the messages from this widget to the status bar
	// display on the main window.
	connect( thewidget, SIGNAL(sendStatus(const QString)), 
			 myParent, SLOT  (statusMessage(const QString)) );

	// Connect application event signals to the widget
	connect( myParent, SIGNAL(fileOpening()), 
		     thewidget, SLOT  (refresh()) );
	connect( myParent, SIGNAL(fileOpened()),  
		     thewidget, SLOT  (newShape()) );

	// connect widget events to the context wrapper
	connect( thewidget,       SIGNAL(mouseMoved(occviewport*, QMouseEvent*)), 
		     myController,   SLOT  (moveEvent(occviewport*,  QMouseEvent*)) );
	// connect widget events to the context wrapper
	connect( thewidget,       SIGNAL(mouseClicked(occviewport*, QMouseEvent*)), 
		     myController,   SLOT  (clickEvent(occviewport*,   QMouseEvent*)) );
	// connect widget events to the context wrapper
	connect( thewidget,       SIGNAL(selectionChanged()) , 
		     myController,   SLOT  (onSelectionChanged()) );

}

void QoccFrame::disconnectParent()
{
	myWidget->disconnect();
}

void QoccFrame::invokeAction( QAction* group, QAction* action, const char* signal )
{
	group->setText     ( action->text() );
	group->setStatusTip( action->statusTip() );
	group->setIcon     ( action->icon() );
	if ( myWidget )
	{
		group->disconnect( );
		connect ( group, SIGNAL(triggered()), myWidget, signal );
		group->trigger();
	}
}

void QoccFrame::fitExtents( void )
{
	invokeAction ( actionZoomGroup,  actionFitExtents, SLOT(fitExtents()) );
}

void QoccFrame::fitArea( void )
{
	invokeAction ( actionZoomGroup,  actionFitArea, SLOT( fitArea()) );
}

void QoccFrame::fitAll( void )
{
	invokeAction ( actionZoomGroup,  actionFitAll, SLOT(fitAll()) );
}

void QoccFrame::viewAxo( void )
{
	invokeAction ( actionViewGroup, actionViewAxo, SLOT(viewAxo()) );
}

void QoccFrame::viewTop( void )
{
	invokeAction ( actionViewGroup, actionViewTop, SLOT(viewTop()) );
}

void QoccFrame::viewFront( void )
{
	invokeAction ( actionViewGroup,  actionViewFront, SLOT(viewFront()) );
}

void QoccFrame::viewTopFront( void )
{
	invokeAction ( actionViewGroup,  actionViewTopFront, SLOT(viewTopFront()) );
}

void QoccFrame::viewLeft( void )
{
	invokeAction ( actionViewGroup, actionViewLeft, SLOT(viewLeft()) );
}



