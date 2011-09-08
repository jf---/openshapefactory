/************************************************************************************
**
** This file is part of the QtOPENCASCADE Toolkit.
**
** Copyright (C) 2006, 2007, 2008 QtOCC Team Members
**               Peter Dolbey, Marc Britten, Stephane Routelous
**               Stefan Boeykens, Pawel Dobrowolski, Walter Steffe
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
$Revision: 109 $
$Date: 2008-10-23 09:15:49 -1100 (Thu, 23 Oct 2008) $
$Author: pdolbey $
*/

#ifndef QOCC3DWIDGET_H
#define QOCC3DWIDGET_H

#include <QtGui/QRubberBand>
#include <QtGui/QToolBar>
//#include <QGLWidget>


#include "qocc.h"
#include "qoccdocument.h"
#include "qocccontroller.h"
#include "Visual3d_Layer.hxx"
#include <NIS_View.hxx> 
#include <QTime>



/** the key for multi selection */
#define MULTISELECTIONKEY  Qt::ShiftModifier   

/** The key for shortcut ( use to activate dynamic rotation, panning ) */
#define CASCADESHORTCUTKEY Qt::ControlModifier 

#define ValZWMin 1 /** For elastic bean selection */

class Handle(Visual3d_View);

class QOCC_DECLSPEC Qocc3dWidget : public QWidget 
{
	Q_OBJECT

public:

	enum CurrentAction3d {	CurAction3d_Undefined,
							CurAction3d_Nothing, 
							CurAction3d_Picking,
							CurAction3d_DynamicZooming,
							CurAction3d_WindowZooming, 
							CurAction3d_DynamicPanning,
							CurAction3d_GlobalPanning, 
							CurAction3d_DynamicRotation };
/*
	enum ViewAction {	ViewFitAllId, 
						ViewFitAreaId, 
						ViewZoomId, 
						ViewPanId, 
						ViewGlobalPanId,
						ViewFrontId, 
						ViewBackId, 
						ViewTopId, 
						ViewBottomId, 
						ViewLeftId, 
						ViewRightId,
						ViewAxoId, 
						ViewRotationId, 
						ViewResetId, 
						ViewHlrOffId, 
						ViewHlrOnId };
*/
public:

	Qocc3dWidget(  const QoccController* aController = NULL, 
				   QWidget *parent = NULL, 
				   Qt::WindowFlags wflags = 0 );

    ~Qocc3dWidget();

	inline QoccController*					getController( void ) { return myController; }
	inline Handle(AIS_InteractiveContext)&	getContext( void )    { return myController->getContext(); }
	inline const Handle(NIS_InteractiveContext)	getNISContext( void )    { return myController->getNISContext(); }
	inline Handle(V3d_Viewer)&				getViewer( void )     { return myController->getViewer(); }
	
	//inline Handle(V3d_View)&				getView( void )       { return myView; }
	inline Handle(NIS_View)&				getView( void )       { return myView; }

	inline V3d_Coordinate x( void ) { return myCurrentX; }
	inline V3d_Coordinate y( void ) { return myCurrentY; }
	inline V3d_Coordinate z( void ) { return myCurrentZ; }

	inline Qt::MouseButtons      buttons()  const { return myMouseState; }
	inline Qt::MouseButtons      button()   const { return myMouseFlags; }
	inline Qt::KeyboardModifiers keyboard() const { return	myKeyboardFlags; }



	void setStatusMessage ( const QString& aString );

	// Overrides
	QPaintEngine* paintEngine() const;

	// Accessors
	gp_Dir getDirection();
	gp_Pln getPlane();
	gp_Pnt getPoint();
	TCollection_AsciiString nameofdetected;
	
	void redraw( bool isPainting = false );
	void setProjection( V3d_TypeOfOrientation orientation );
	Visual3d_Layer* myLayer;
	Standard_Integer w, h;
	void definewindowsize();
	//Handle_GEOM_Object GetGEOMFromAIS(Handle_AIS_InteractiveObject ais);
 //   Handle(AIS_InteractiveObject) GetAISFromGEOM( Handle_GEOM_Object obj );
	//Handle(TPrsStd_AISPresentation) GetPresentationFromGEOM( Handle_GEOM_Object obj );
	//
	
signals:

	void initialized();
	void selectionChanged();
	void mouseMoved   ( Qocc3dWidget* widget, QMouseEvent* e );
	void mouseClicked ( Qocc3dWidget* widget, QMouseEvent* e );
	void sendStatus   ( const QString aMessage );
	//! Just a placeholder for now
	void popupMenu ( const Qocc3dWidget* aView, const QPoint aPoint ); 
	void error ( int errorCode, QString& errorDescription );

public slots:

	void idle();
	void fitExtents();
	void fitAll();
    void fitArea();
    void zoom();
    void pan();
    void globalPan();
    void rotation();
    void hiddenLineOn();
    void hiddenLineOff();
	void background();
	void viewFront();
    void viewBack();
    void viewTop();
    void viewBottom();
    void viewLeft();
    void viewRight();
    void viewAxo();
	void viewTopFront();
	void viewGrid();
	void viewReset();
	void setReset();

	void newShape();
	void refresh();

protected: // methods

	void initializeOCC();

    virtual void paintEvent        ( QPaintEvent* e );
    virtual void resizeEvent       ( QResizeEvent* e );
    virtual void mousePressEvent   ( QMouseEvent* e );
    virtual void mouseReleaseEvent ( QMouseEvent* e );
    virtual void mouseMoveEvent    ( QMouseEvent* e );
	virtual void wheelEvent        ( QWheelEvent* e );

	virtual void leaveEvent		   ( QEvent * );

private: // members

#ifdef WNT
    Handle_WNT_Window	  myWindow;
#else
    Handle_Xw_Window	  myWindow;
#endif // WNT

	//Handle(V3d_View)      myView;
	Handle(NIS_View)      myView;
	
	Handle(Visual3d_View) myVisual;
	QoccController*		  myController;
					
	Standard_Boolean	  myViewResized;
	Standard_Boolean	  myViewInitialized;
    Quantity_Factor       myCurZoom;
    Standard_Boolean	  myGridSnap;
	AIS_StatusOfDetection myDetection;

	V3d_Coordinate        myCurrentX;
	V3d_Coordinate        myCurrentY;
	V3d_Coordinate        myCurrentZ;
		
	QRubberBand*          myRubberBand;
	QPoint		          myStartPoint;
	QPoint		          myCurrentPoint;
	
	Standard_Real	      myPrecision;
	Standard_Real	      myViewPrecision;
	Standard_Boolean      myMapIsValid;

	Qt::KeyboardModifiers myKeyboardFlags;
	Qt::MouseButton		  myMouseFlags;
	Qt::MouseButtons	  myMouseState;
	QCursor				  myCrossCursor;
	QToolBar*			  myToolBar;
	void drawlayer();
	QTime* TheTime;
	
	// Shared message
	static QString		  myStatusMessage;

protected: // members
    CurrentAction3d       myMode;


protected: // methods

	virtual void onLeftButtonDown  ( Qt::KeyboardModifiers nFlags, const QPoint point );
    virtual void onMiddleButtonDown( Qt::KeyboardModifiers nFlags, const QPoint point );
    virtual void onRightButtonDown ( Qt::KeyboardModifiers nFlags, const QPoint point );
    virtual void onLeftButtonUp    ( Qt::KeyboardModifiers nFlags, const QPoint point );
    virtual void onMiddleButtonUp  ( Qt::KeyboardModifiers nFlags, const QPoint point );
    virtual void onRightButtonUp   ( Qt::KeyboardModifiers nFlags, const QPoint point );

	virtual void onMouseMove  ( Qt::MouseButtons buttons, 
						Qt::KeyboardModifiers nFlags, const QPoint point );

	virtual AIS_StatusOfPick		dragEvent ( const QPoint startPoint, const QPoint endPoint, const bool multi = false );
    virtual AIS_StatusOfPick		inputEvent( const bool multi = false );
    virtual AIS_StatusOfDetection	moveEvent ( const QPoint point );

    virtual void setMode( const CurrentAction3d mode );
    virtual CurrentAction3d getMode(){ return myMode; };

	virtual Standard_Real precision( Standard_Real aReal );
	virtual Standard_Real viewPrecision( bool resized = false );

	virtual void drawRubberBand( const QPoint origin, const QPoint position );
	virtual void showRubberBand( void );
	virtual void hideRubberBand( void );

	void sendStatusLocation ( void );

	virtual Standard_Boolean convertToPlane(const Standard_Integer Xs, 
											const Standard_Integer Ys, 
											Standard_Real& X,
											Standard_Real& Y,
											Standard_Real& Z);

										  
	virtual void paintOCC();
	static int paintCallBack (Aspect_Drawable, 
							  void*, 
							  Aspect_GraphicCallbackStruct*);
	inline QPoint getStartPoint(){ return myStartPoint; };
	inline void setStartPoint( const QPoint point ){ myStartPoint = point; };

	inline QPoint getCurrentPoint(){ return myCurrentPoint; };
	inline void setCurrentPoint( const QPoint point ){ myCurrentPoint = point; };

	inline Quantity_Factor getCurZoom(){ return myCurZoom; };
	inline void setCurZoom( const Quantity_Factor zoom ){ myCurZoom = zoom; };

	inline Qt::KeyboardModifiers getKeyboardFlags(){ return myKeyboardFlags; };
	inline void setKeyboardFlags( const Qt::KeyboardModifiers flags ){ myKeyboardFlags = flags; };

public:

	bool dump(Standard_CString theFile);

};

////#define qGeomApp (static_cast<QoccApplication *>(QCoreApplication::instance()))
//#define qGeomEngine qGeomApp->getEngine()
//#define qGeomSolver qGeomApp->getSolver()

#endif // Qocc3dWidget_H
