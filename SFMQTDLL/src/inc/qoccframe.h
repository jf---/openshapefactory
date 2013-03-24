/************************************************************************************
**
** This file is part of the QtOPENCASCADE Toolkit.
**
** Copyright (C) 2006, 2007, 2008, 2009 QtOCC Team Members
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

#ifndef QOCCVIEWFRAME_H
#define QOCCVIEWFRAME_H

#include <QtGui/QToolBar>
#include <QtGui/QMenu>
#include <QtGui/QFrame>
#include <qoccapplication.h>

#include "qocc.h"

class QoccMainWindow;
class occviewport;
class QoccController;
class QVBoxLayout;
class QHBoxLayout;
class Handle_AIS_InteractiveContext;

class Q_DECL_EXPORT QoccFrame : public QFrame
{
	Q_OBJECT

public:

	QoccFrame ( QoccController* aContext = NULL, 
				QWidget *parent = NULL, 
				Qt::WindowFlags wflags = 0 );

    ~QoccFrame();

	occviewport*   getWidget()  { return myWidget; };
	QToolBar*       getToolBar() { return myToolBar; };

	QLabel* curlabel;
	

	


signals:

public slots:
	void fitArea      ();
	void fitAll       ();
	void fitExtents   ();
	void viewAxo      ();
	void viewTop      ();
	void viewLeft     ();
	void viewFront    ();
	void viewTopFront ();
	void handlelabelcontextmenu( const QPoint & pos );

protected: // methods

private: // members
	QoccController*  myController;
	occviewport*    myWidget;
	
	QToolBar*		myToolBar;
	QVBoxLayout*	myLayout;
	QHBoxLayout*    mySubLayout;
	QoccMainWindow* myParent;

	QAction* actionIdle;

    QAction *actionRotation;
	QAction *actionPan;
	
	QAction* actionZoomGroup;
	QMenu*   popupZoomGroup;
	QAction* actionFitExtents;
	QAction* actionFitArea;
	QAction* actionFitAll;

	QAction* actionViewGroup;
	QMenu*   popupViewGroup;
	QAction* actionViewTop;
	QAction* actionViewFront;
	QAction* actionViewTopFront;
	QAction* actionViewLeft;
	QAction* actionViewAxo;

private: // methods
	void setToolbarActions( void );

	void createActions( void );

	void connectActions( void );
	void disconnectActions( void );

	void connectParent( void );
	void connectParent(occviewport* thewidget);
	void disconnectParent( void );

	void createZoomGroup( void );
	void createViewGroup( void );

	void invokeAction( QAction* group, QAction* action, const char* signal );

public:

};

//#define qGeomApp (static_cast<QoccApplication *>(QCoreApplication::instance()))

#endif // QOCCVIEWFRAME_H
