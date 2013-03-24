// original modified from the QTOCC project by Peter Dolby



#ifndef OCCVIEWPORTWIDGET_H
#define OCCVIEWPORTWIDGET_H

#include <QtGui/QRubberBand>
#include <QtGui/QToolBar>
//#include <QGLWidget>


#include "commonlibraries.h"
#include "Visual3d_Layer.hxx"
#include <NIS_View.hxx> 
#include <QTime>
#include <qocccontroller.h>
#include <QString>

#include "iGizmo.h" // gizmo include




/** the key for multi selection */
#define MULTISELECTIONKEY  Qt::ShiftModifier   

/** The key for shortcut ( use to activate dynamic rotation, panning ) */
#define CASCADESHORTCUTKEY Qt::ControlModifier 

#define ValZWMin 1 /** For elastic bean selection */

class Handle(Visual3d_View);

class Q_DECL_EXPORT occviewport : public QWidget 
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

	occviewport(  const QoccController* aController = NULL, 
				   QWidget *parent = NULL, 
				   Qt::WindowFlags wflags = 0 );

    ~occviewport();

	inline QoccController*					getController( void ) { return myController; }
	inline Handle(AIS_InteractiveContext)&	getContext( void )    { return myController->getContext(); }
	inline const Handle(NIS_InteractiveContext)	getNISContext( void )    { return myController->getNISContext(); }
	inline Handle(V3d_Viewer)&				getViewer( void )     { return myController->getViewer(); }
	
	inline Handle(V3d_View)&				getView( void )       { return myView; }
	//inline Handle(NIS_View)&				getView( void )       { return myView; }

	inline V3d_Coordinate x( void ) { return myCurrentX; }
	inline V3d_Coordinate y( void ) { return myCurrentY; }
	inline V3d_Coordinate z( void ) { return myCurrentZ; }

	inline Qt::MouseButtons      buttons()  const { return myMouseState; }
	inline Qt::MouseButtons      button()   const { return myMouseFlags; }
	inline Qt::KeyboardModifiers keyboard() const { return	myKeyboardFlags; }



	

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

	
signals:

	void initialized();
	void selectionChanged();
	void mouseMoved   ( occviewport* widget, QMouseEvent* e );
	void mouseClicked ( occviewport* widget, QMouseEvent* e );
	void sendStatus   ( const QString aMessage );
	void vertexStatus   ( const QString aMessage );

	//! Just a placeholder for now
	void popupMenu ( const occviewport* aView, const QPoint aPoint ); 
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

	Handle(V3d_View)      myView;
	//Handle(NIS_View)      myView;
	
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
	
	//the gizmos stuff
	float objectMatrix[16];
	IGizmo *gizmo ;
	IGizmo *gizmoMove;
	IGizmo *gizmoRotate;
	IGizmo *gizmoScale;


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
