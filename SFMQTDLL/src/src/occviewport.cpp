
#include <cmath>
#include <iostream>


#include <V3d_Plane.hxx>
#include <V3d_TypeOfZclipping.hxx>
#include <StdSelect_ViewerSelector3d.hxx>

#include <QtGui/QApplication>
#include <QtGui/QBitmap>
#include <QtGui/QPainter>
#include <QtGui/QInputEvent>
#include <QtGui/QColorDialog>
#include <QtGui/QPlastiqueStyle>
#include <QtCore/QTextStream>
#include <Wingdi.h>

#include "occviewport.h"
#include "commonlibraries.h"
#include "Visual3d_Layer.hxx"
#include <QDebug>
#include "shapefactory.h"
#include "cowbellprogress.h"
#include "QGLwidget"

//#include <windows.h>		// Header File For Windows
//#include <stdio.h>			// Header File For Standard Input/Output
//#include <gl\gl.h>			// Header File For The OpenGL32 Library
//#include <gl\glu.h>			// Header File For The GLu32 Library
//#include <GL\glut.h>



QString	occviewport::myStatusMessage = QObject::tr("");

/*!
\brief	Constructor for occviewport.
		This constructor creates an uninitialised window for rendering a
		Handle_V3d_View based OpenCASCADE 3D viewer.
\param	aContext The widget's Qocc context
\param	parent The widget's parent widget
\param	wflags The widget's window configuration flags
\see	initialize
*/
occviewport::occviewport( const QoccController* aController,
							QWidget *parent, Qt::WindowFlags f )
: QWidget( parent, f | Qt::MSWindowsOwnDC )
{
	// Intialise mamber variables,
	// gcc should stop complaining!
	myView            = NULL;
	myRubberBand      = NULL;
	myMode            = CurAction3d_Undefined;
	myGridSnap        = Standard_False;
	myViewResized     = Standard_False;
	myViewInitialized = Standard_False;
	myPrecision       = INITIAL_PRECISION;
	myViewPrecision   = 0.0;
	myDetection       = AIS_SOD_Nothing;
	myKeyboardFlags   = Qt::NoModifier;
	myMouseFlags      = Qt::NoButton;

	myController = (QoccController *) aController;

	//myView    = new NIS_View(myController->getViewer());
	myView    = myController->getViewer()->CreateView();
	myView->SetScale( 2 );

	int myint = 0;
	// Needed to generate mouse events
	setMouseTracking( true );

	// Avoid Qt background clears to improve resizing speed,
	// along with a couple of other attributes
	//setAutoFillBackground( false );
	//setAttribute( Qt::WA_NoSystemBackground );
#if (QT_VERSION >= QT_VERSION_CHECK(4, 4, 0))
	setAttribute( Qt::WA_NativeWindow );
#endif

	// This next attribute seems to be the secret of allowing OCC on Win32
	// to "own" the window, even though its only supposed to work on X11.
	setAttribute( Qt::WA_PaintOnScreen );

	// Here's a modified pick point cursor from AutoQ3D
	QBitmap curb1( 48, 48 );
	QBitmap curb2( 48, 48 );
	curb1.fill( QColor( 255, 255, 255 ) );
	curb2.fill( QColor( 255, 255, 255 ) );
	QPainter p;

	p.begin( &curb1 );
		p.drawLine( 24,  0, 24, 47 );
		p.drawLine(  0, 24, 47, 24 );
		p.setBrush( Qt::NoBrush );
		p.drawRect( 18, 18, 12, 12 );
	p.end();
	myCrossCursor = QCursor( curb2, curb1, 24, 24 );

	// Create a rubber band box for later mouse activity
	myRubberBand = new QRubberBand( QRubberBand::Rectangle, this );
	if (myRubberBand)
	{
		// If you don't set a style, QRubberBand doesn't work properly
		// take this line out if you don't believe me.
		myRubberBand->setStyle( (QStyle*) new QPlastiqueStyle() );
		#if (QT_VERSION >= QT_VERSION_CHECK(4, 4, 0))
			myRubberBand->setAttribute( Qt::WA_NativeWindow );
		#endif
	}

	
	

	getContext()->SetToHilightSelected( true );
	//getContext()->SetDeviationCoefficient( 0.001);

	getContext()->SetDeviationCoefficient( 0.0006);
	//getContext()->SetDeviationAngle(4);
	//myView->SetBackgroundColor(Quantity_NameOfColor::Quantity_NOC_GRAY);
	myView->SetBackgroundColor(Quantity_NameOfColor::Quantity_NOC_LIGHTGRAY);
	
	myView->EnableGLLight(true);
	myView->SetAntialiasingOff();
	myView->SetUp(V3d_TypeOfOrientation::V3d_Zpos);
	//TheTime = new QTime();
	//TheTime->start();


	//if (myView.IsNull()) return;
	//const Handle(NIS_InteractiveContext) Contextest = new NIS_InteractiveContext;
	//if (Contextest.IsNull()) return;
	
	//myController->setNISContext(Contextest);
	//myController->getNISContext()->AttachView(myView);
	


	/// initialize the gizmo stuff

	gizmoMove = CreateMoveGizmo();
    gizmoRotate = CreateRotateGizmo();
    gizmoScale = CreateScaleGizmo();

    //gizmo = gizmoMove;
	gizmo = gizmoRotate;
	gizmo->SetLocation( IGizmo::LOCATE_LOCAL );

	objectMatrix[0] =-0.3210;
	objectMatrix[1] = 0.0000;
	objectMatrix[2] = 0.9471;
	objectMatrix[3] = 0.0000;
	objectMatrix[4] = 0.0000;
	objectMatrix[5] = 1.0000;
	objectMatrix[6] = 0.0000;
	objectMatrix[7] = 0.0000;
	objectMatrix[8] = -0.9471;
	objectMatrix[9] = 0.0000;
	objectMatrix[10] = -0.3210;
	objectMatrix[11] = 0.0000;
	objectMatrix[12] = -137.1790;
	objectMatrix[13] = 16.4949;
	objectMatrix[14] = 375.4003;
	objectMatrix[15] = 1.0000;
		

    gizmo->SetEditMatrix( objectMatrix );

    gizmo->SetScreenDimension( this->width(), this->height() );
    gizmoMove->SetDisplayScale( 1000.f );
    gizmoRotate->SetDisplayScale( 1000.f );
    gizmoScale->SetDisplayScale( 1000.f );

	gizmo->SetLocation( IGizmo::LOCATE_WORLD );
	





	

}

/*!
\brief	Default destructor for occviewport.
		This should delete any memory and release any resources. No parameters
		required.
*/
occviewport::~occviewport()
{
	if ( myRubberBand )
	{
		delete myRubberBand;
	}
}

/*!
\brief	The initializeOCC() procedure.
		This function creates the widget's view using the interactive context
		provided. Currently it also creates a trihedron object in the lower left
		corner - this will eventually be managed by an external system setting.
\param	aContext Handle to the AIS Interactive Context managing the view
\return	nothing
*/
void occviewport::initializeOCC()
{
	Aspect_RenderingContext rc = 0;
	//myController = aContext;

	int windowHandle = (int) winId();
    short lo = (short)   windowHandle;
    short hi = (short) ( windowHandle >> 16 );

#ifdef WNT
	
	
    myWindow = new WNT_Window( Handle(Graphic3d_WNTGraphicDevice)
							   ::DownCast( myController->getViewer()->Device() ) ,
							   (int) hi, (int) lo );
	// Turn off background erasing in OCC's window
	myWindow->SetFlags( WDF_NOERASEBKGRND );
#else
	// rc = (Aspect_RenderingContext) glXGetCurrentContext(); // Untested!
    myWindow = new Xw_Window( Handle(Graphic3d_GraphicDevice)
									::DownCast( myController->getViewer()->Device() ),
							  (int) hi, (int) lo, Xw_WQ_SAMEQUALITY, Quantity_NOC_BLACK );
#endif // WNT

	if (!myView.IsNull())
	{
		myVisual = myView->View();

		//rc = (Aspect_RenderingContext) wglGetCurrentContext();
		// Set my window (Hwnd) into the OCC view
	    myView->SetWindow( myWindow, rc , paintCallBack, this  );
		//myView->SetWindow( myWindow);
		V3d_TypeOfVisualization aMode;
        		
#ifdef OCC_PATCHED
		aMode = V3d_ZBUFFER;
#else
		aMode = V3d_WIREFRAME;
		//aMode = V3d_ZBUFFER;
#endif

		// Set up axes (Trihedron).
		myView->TriedronDisplay( Aspect_TypeOfTriedronPosition::Aspect_TOTP_LEFT_LOWER, Quantity_NOC_BLACK, 0.1, aMode );

		// For testing OCC patches
		//myView->ColorScaleDisplay();

		// Map the window
		if (!myWindow->IsMapped())
		{
			myWindow->Map();
		}
		
		myView->EnableDepthTest(true);
		//myView->SetShadingModel(V3d_TypeOfShadingModel::V3d_GOURAUD);

		myViewResized = Standard_True;		// Force a redraw to the new window on next paint event
		myViewInitialized = Standard_True;	// This is to signal any connected slots that the view is ready.
		setMode( CurAction3d_Nothing );		// Set default cursor as a cross
		emit initialized();
		
		glClearDepth(1.0f);									// Depth Buffer Setup
	    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
        glDepthMask(1);




	}
}

/*!
\brief	getPlane exposes the current plane normal to external event function
		handlers as a gp_Dir.
\return	the direction in a gp_Dir
*/
gp_Dir occviewport::getDirection()
{
	return myController->getViewer()->PrivilegedPlane().Direction();
}

/*!
\brief	getPlane exposes the current input plane to external event function
		handlers as a gp_Pln.
*/
gp_Pln occviewport::getPlane()
{
	return myController->getViewer()->PrivilegedPlane();
}

/*!
\brief	getPoint exposes the current input co-ordinates to external event function
		handlers as a gp_Pnt.
*/
gp_Pnt occviewport::getPoint()
{
	gp_Pnt thePoint( x(), y(), z() );
	return thePoint;
}

/*!
\brief	Returns a NULL QPaintEngine
		This should result in a minor performance benefit.
*/
QPaintEngine* occviewport::paintEngine() const
{
	return NULL;
}

/*!
\brief	Paint Event
		Called when the Widget needs to repaint itself
\param	e The event data.
*/
void occviewport::paintEvent ( QPaintEvent * /* e */)
{
	if ( !myViewInitialized )
	{
		if ( winId() )
		{
			initializeOCC( );
		}
	}
	if ( !myView.IsNull() )
	{
		redraw( true );
	}
}

/*!
\brief	Resize event.
		Called when the Widget needs to resize itself, but seeing as a paint
		event always follows a resize event, we'll move the work into the
		paint event.
\param	e The event data.
*/
void occviewport::resizeEvent ( QResizeEvent * /* e */ )
{
	myViewResized = Standard_True;
	definewindowsize();
}

/*!
\brief	Mouse press event
\param	e The event data.
*/
void occviewport::mousePressEvent( QMouseEvent* e )
{
	myMouseFlags = e->button();
	myMouseState = e->buttons();

	

	// Cache the keyboard flags for the whole gesture
	myKeyboardFlags = e->modifiers();

	// The button mappings can be used as a mask. This code prevents conflicts
	// when more than one button pressed simutaneously.
	if ( e->button() & Qt::LeftButton )
	{
		onLeftButtonDown  ( myKeyboardFlags, e->pos() );
	}
	else if ( e->button() & Qt::RightButton )
	{
		onRightButtonDown ( myKeyboardFlags, e->pos() );
	}
	else if ( e->button() & Qt::MidButton )
	{
		onMiddleButtonDown( myKeyboardFlags, e->pos() );
	}

	gizmo->OnMouseDown(width() - e->globalX(),e->globalY());
	//SetCapture( hWnd );
	redraw();


}

/*!
\brief	Mouse release event
\param	e The event data.
*/
void occviewport::mouseReleaseEvent(QMouseEvent* e)
{
	myMouseFlags = e->button();
	myMouseState = e->buttons();
	
	redraw();							// Clears up screen when menu selected but not used.
	hideRubberBand();
	if ( e->button() & Qt::LeftButton )
	{
		onLeftButtonUp  ( myKeyboardFlags, e->pos() );
	}
	else if ( e->button() & Qt::RightButton )
	{
		onRightButtonUp ( myKeyboardFlags, e->pos() );
	}
	else if ( e->button() & Qt::MidButton )
	{
		onMiddleButtonUp( myKeyboardFlags, e->pos() );
	}

	if ( !(myKeyboardFlags & CASCADESHORTCUTKEY) )
	{
		emit mouseClicked( this, e );
	}

	gizmo->OnMouseUp(width() - e->globalX(),e->globalY());
	redraw();


}

/*!
\brief	Mouse move event, driven from application message loop
\param	e The event data.
*/
void occviewport::mouseMoveEvent( QMouseEvent* e )
{
	Standard_Real X, Y, Z;

	


	myMouseFlags = e->button();
	myMouseState = e->buttons();

	myCurrentPoint = e->pos();
	myKeyboardFlags = e->modifiers();

	//Check if the grid is active and that we're snapping to it
	if( myController->getViewer()->Grid()->IsActive() && myGridSnap )
	{
		myView->ConvertToGrid( myCurrentPoint.x(), myCurrentPoint.y(), myCurrentX, myCurrentY, myCurrentZ );
		sendStatusLocation();
		emit mouseMoved( this, e );
	}
	else //	this is the standard case
	{
		if ( convertToPlane( myCurrentPoint.x(), myCurrentPoint.y(), X, Y, Z ) )
		{
			myCurrentX = precision( X );
			myCurrentY = precision( Y );
			myCurrentZ = precision( Z );
			sendStatusLocation();
			emit mouseMoved( this, e );
		}
		else
		{
			emit sendStatus ( tr("Indeterminate Point") );
		}
	}
	if ( !myController->isDrawing() )
	{
		onMouseMove( e->buttons(), myKeyboardFlags, e->pos() );
	}

	gizmo->OnMouseMove(width() - e->globalX(),e->globalY());
	redraw();


	//drawlayer();
}

/*!
\brief	A leave event is sent to the widget when the mouse cursor leaves
		the widget.
		This sub-classed event handler fixes redraws when gestures are interrupted
		by use of parent menus etc. (Candidate for change)
\param	e	The event data
*/
void occviewport::leaveEvent ( QEvent* )
{
	myMouseFlags = Qt::NoButton;
}

/*!
\brief	The QWheelEvent class contains parameters that describe a wheel event.
*/
void occviewport::wheelEvent ( QWheelEvent* e )
{
	if ( !myView.IsNull() )
	{
		Standard_Real currentScale = myView->Scale();
		if ( e->delta() > 0 )
		{
			currentScale *= 1.10; // +10%
		}
		else
		{
			currentScale /= 1.10; // -10%
		}
		myView->SetScale( currentScale );
		viewPrecision( true );
	}
	else
	{
		e->ignore();
	}
}

/*!
\brief	Go idle
		This called from various locations, and also exposed as a slot.
*/
void occviewport::idle( )
{
	setMode( CurAction3d_Nothing );
}

/*!
\brief	The main redraw function
		This called from various locations.
*/
void occviewport::redraw( bool isPainting )
{
	if ( !myView.IsNull() )					// Defensive test.
	{
		if ( myViewResized )
		{
			myView->MustBeResized();
			viewPrecision( true );
		}
		else
		{
			// Don't repaint if we are already redrawing
			// elsewhere due to a keypress or mouse gesture
			if ( !isPainting || ( isPainting && myMouseFlags == Qt::NoButton ) )
			{
				myView->Redraw();
			}
		}
	}
	myViewResized = Standard_False;
}

/*!
\brief	Just fits the current window
		This function just fits the current objects to the window, without
		either reducing or increasing the Z extents. This can cause clipping
		of the objects after rotation.
\return	Nothing
*/
void occviewport::fitExtents( void )
{
	if (!myView.IsNull())
	{
		myView->FitAll();
		viewPrecision( true );
	}
}

/*!
\brief	Fits the model to view extents
		This function fits the current objects to the window,
		reducing or increasing the Z extents as needed.
\return	Nothing
*/
void occviewport::fitAll( void )
{
	if (!myView.IsNull())
	{
		myView->ZFitAll();
		myView->FitAll();
		viewPrecision( true );
	}
}

//-----------------------------------------------------------------------------
/*!
\brief	Sets up the view for a rubberband window zoom
\return	Nothing
*/
void occviewport::fitArea( void )
{
    setMode( CurAction3d_WindowZooming );
}

/*!
\brief	Sets up the view for a dynamic zoom
\return	Nothing
*/
void occviewport::zoom( void )
{
    setMode( CurAction3d_DynamicZooming );
}

/*!
\brief	Sets up the view for panning
\return	Nothing
*/
void occviewport::pan( void )
{
    setMode( CurAction3d_DynamicPanning );
}

/*!
\brief	Sets up the view for dynamic rotation
\return	Nothing
*/
void occviewport::rotation( void )
{
    setMode( CurAction3d_DynamicRotation );
}

/*!
\brief	Sets up the view for global panning, whatever that is!
\return	Nothing
*/
void occviewport::globalPan()
{
	if ( !myView.IsNull() )
	{
		// save the current zoom value
		myCurZoom = myView->Scale();
		// Do a Global Zoom
		myView->FitAll();
		viewPrecision( true );
		// Set the mode
		setMode( CurAction3d_GlobalPanning );
	}
}

/*!
\brief	This aligns the view to the current privilege plane.
		Not so interesting at the moment, but wait until custom
		grid planes are implemented!
\return	Nothing
*/
void occviewport::viewGrid()
{
	if( !myView.IsNull() )
	{
		myView->SetFront();
	}
}

/*!
\brief	View from canonical "front".
\return	Nothing
*/
void occviewport::viewFront()
{
	setProjection( V3d_Yneg );
}

/*!
\brief	View from canonical "back".
\return	Nothing
*/
void occviewport::viewBack()
{
	setProjection( V3d_Ypos );
}

/*!
\brief	View from canonical "top".
		This is traditional XOY axis.
\return	Nothing
*/
void occviewport::viewTop()
{
	setProjection( V3d_Zpos );
}

/*!
\brief	View from canonical "bottom".
\return	Nothing
*/
void occviewport::viewBottom()
{
	setProjection( V3d_Zneg );
}

/*!
\brief	View from canonical "left".
\return	Nothing
*/
void occviewport::viewLeft()
{
	setProjection( V3d_Xneg );
}

/*!
\brief	View from canonical "right".
\return	Nothing
*/
void occviewport::viewRight()
{
	setProjection( V3d_Xpos );
}

/*!
\brief	View using axonometric projection.
\return	Nothing
*/
void occviewport::viewAxo()
{
	setProjection( V3d_XnegYnegZpos );
}

void occviewport::viewTopFront()
{
	setProjection( V3d_YnegZpos );
}

void occviewport::viewReset()
{
	if( !myView.IsNull() )
	{
	    myView->Reset();
	}
}

void occviewport::hiddenLineOff()
{
	if( !myView.IsNull() )
	{
	    QApplication::setOverrideCursor( Qt::WaitCursor );
		myView->SetComputedMode( Standard_False );
		QApplication::restoreOverrideCursor();
	}
}

void occviewport::hiddenLineOn()
{
	if( !myView.IsNull() )
	{
	    QApplication::setOverrideCursor( Qt::WaitCursor );
		myView->SetComputedMode( Standard_True );
		QApplication::restoreOverrideCursor();
	}
}

void occviewport::background()
{
    QColor aColor ;
    Standard_Real R1;
    Standard_Real G1;
    Standard_Real B1;
    myView->BackgroundColor(Quantity_TOC_RGB,R1,G1,B1);
    aColor.setRgb(( int )R1*255, ( int )G1*255, ( int )B1*255);

    QColor aRetColor = QColorDialog::getColor(aColor);

    if( aRetColor.isValid() )
	{
        R1 = aRetColor.red()/255.;
        G1 = aRetColor.green()/255.;
        B1 = aRetColor.blue()/255.;
        myView->SetBackgroundColor(Quantity_TOC_RGB,R1,G1,B1);
    }
    redraw();
}

void occviewport::setReset ()
{
	if( !myView.IsNull() )
	{
		myView->SetViewOrientationDefault() ;
		viewPrecision( true );
	}
}

/*!
\brief	This function handles left button down events from the mouse.
*/
void occviewport::onLeftButtonDown(  Qt::KeyboardModifiers nFlags, const QPoint point )
{
    myStartPoint = point;
    if ( nFlags & CASCADESHORTCUTKEY )
    {
		setMode( CurAction3d_DynamicZooming );
    }
    else
    {
        switch ( myMode )
        {
            case CurAction3d_Nothing:
				// emit pointClicked( myCurrentX, myCurrentY, myCurrentZ );
				break;

			case CurAction3d_Picking:
				break;

            case CurAction3d_DynamicZooming:
				break;

            case CurAction3d_WindowZooming:
				break;

            case CurAction3d_DynamicPanning:
				break;

            case CurAction3d_GlobalPanning:
                break;

            case CurAction3d_DynamicRotation:
                myView->StartRotation( myStartPoint.x(), myStartPoint.y() );
                break;

            default:
                break;
        }
    }
}

/*!
\brief	This function handles middle button down events from the mouse.
*/
void occviewport::onMiddleButtonDown(  Qt::KeyboardModifiers nFlags, const QPoint point )
{
	myStartPoint = point;
    if ( nFlags & CASCADESHORTCUTKEY )
	{
		setMode( CurAction3d_DynamicPanning );
	}
}

/*!
\brief	This function handles right button down events from the mouse.
*/
void occviewport::onRightButtonDown(  Qt::KeyboardModifiers nFlags, const QPoint point )
{
	myStartPoint = point;
    if ( nFlags & CASCADESHORTCUTKEY )
    {
		setMode( CurAction3d_DynamicRotation );
		myView->StartRotation( point.x(), point.y(),0.4 );
    }
    else if ( nFlags & MULTISELECTIONKEY )
	{
		setMode( CurAction3d_Picking );
	}
	else
    {
        emit popupMenu ( this, point );
    }
}

/*!
\brief	This function handles left button up events from the mouse.
		This marks the end of the gesture.
*/
void occviewport::onLeftButtonUp(  Qt::KeyboardModifiers nFlags, const QPoint point )
{
	myCurrentPoint = point;
	if ( nFlags & CASCADESHORTCUTKEY )
    {
		// Deactivates dynamic zooming
		setMode( CurAction3d_Nothing );
	}
	else
	{
		switch( myMode )
		{

			case CurAction3d_Nothing:
				//break;

			case CurAction3d_Picking:
				// Shouldn't get here yet
				if ( myCurrentPoint == myStartPoint )
				{
   					inputEvent( nFlags & MULTISELECTIONKEY );
				}
				else
				{
					dragEvent( myStartPoint, myCurrentPoint, nFlags & MULTISELECTIONKEY );
				}
				break;

			case CurAction3d_DynamicZooming:
				viewPrecision( true );
				break;

			case CurAction3d_WindowZooming:
				if ( (abs( myCurrentPoint.x() - myStartPoint.x() ) > ValZWMin ) ||
					 (abs( myCurrentPoint.y() - myStartPoint.y() ) > ValZWMin ) )
				{
					myView->WindowFitAll( myStartPoint.x(),
										  myStartPoint.y(),
										  myCurrentPoint.x(),
										  myCurrentPoint.y() );
				}
				viewPrecision( true );
				break;

			case CurAction3d_DynamicPanning:
				break;

			case CurAction3d_GlobalPanning :
				myView->Place( myCurrentPoint.x(), myCurrentPoint.y(), myCurZoom );
				break;

			case CurAction3d_DynamicRotation:
				break;

			default:
				break;
		}
	}
    emit selectionChanged();
}

/*!
\brief	Middle button up event handler.
		This marks the end of the gesture.
*/
void occviewport::onMiddleButtonUp(  Qt::KeyboardModifiers nFlags, const QPoint point )
{
    myCurrentPoint = point;
	if ( nFlags & CASCADESHORTCUTKEY )
    {
		setMode( CurAction3d_Nothing );
	}
}

/*!
\brief	Right button up event handler.
		This marks the end of the gesture.
*/
void occviewport::onRightButtonUp(  Qt::KeyboardModifiers nFlags, const QPoint point )
{
	myCurrentPoint = point;
	if ( nFlags & CASCADESHORTCUTKEY )
    {
		setMode( CurAction3d_Nothing );
	}
	else if ( nFlags & MULTISELECTIONKEY )
	{
		getContext()->HilightNextDetected( getView() );
		setMode( CurAction3d_Picking );
	}
	else
	{
		if ( myMode == CurAction3d_Nothing )
		{
			emit popupMenu ( this, point );
		}
		else
		{
			setMode( CurAction3d_Nothing );
		}
	}
}

/*!
\brief	Mouse move event handler.
\param	buttons
\param	nFlags
\param	point
\return Nothing
*/
void occviewport::onMouseMove( Qt::MouseButtons buttons,
								Qt::KeyboardModifiers nFlags,
								const QPoint point )
{
	myCurrentPoint = point;
	Handle(V3d_View) aview = this->getView();
	
	AIS_StatusOfDetection status = myController->getContext()->MoveTo( point.x(), point.y(), aview );

	if ( buttons & Qt::LeftButton )
	{
		switch ( myMode )
		{
			case CurAction3d_Nothing:
				// break;

			case CurAction3d_Picking:
				// Shouldn't get here yet
				if ( status == AIS_StatusOfDetection::AIS_SOD_Nothing )				{
				drawRubberBand ( myStartPoint, myCurrentPoint );
				}
				dragEvent( myStartPoint, myCurrentPoint, nFlags & MULTISELECTIONKEY );
				

				break;

			case CurAction3d_DynamicZooming:
				myView->Zoom(	myStartPoint.x(),
								myStartPoint.y(),
								myCurrentPoint.x(),
								myCurrentPoint.y() );
				viewPrecision( true );
				myStartPoint = myCurrentPoint;
				break;

			case CurAction3d_WindowZooming:
				drawRubberBand ( myStartPoint, myCurrentPoint );
				break;

			case CurAction3d_DynamicPanning:
				myView->Pan( myCurrentPoint.x() - myStartPoint.x(),
						     myStartPoint.y() - myCurrentPoint.y() );
				myStartPoint = myCurrentPoint;
				break;

			case CurAction3d_GlobalPanning:
				break;

			case CurAction3d_DynamicRotation:
				myView->Rotation( myCurrentPoint.x(), myCurrentPoint.y() );
				break;

			default:
				break;
		}
	}
	else if( buttons & Qt::RightButton )
	{
		switch ( myMode )
		{
			case CurAction3d_DynamicRotation:
				if ( nFlags & CASCADESHORTCUTKEY )
				{
					myView->Rotation( myCurrentPoint.x(), myCurrentPoint.y() );
				}
				break;

			default:
				break;
		}
	}
	else if ( buttons & Qt::MidButton )
	{
		switch ( myMode )
		{
			case CurAction3d_Nothing:
				// break;
	
			case CurAction3d_DynamicPanning:
				if ( nFlags & CASCADESHORTCUTKEY )
				{
					myView->Pan( myCurrentPoint.x() - myStartPoint.x(),
								 myStartPoint.y() - myCurrentPoint.y() );
					myStartPoint = myCurrentPoint;
				}
				break;

			case CurAction3d_GlobalPanning:
				break;

			default:
				break;
		}
	}
	else
	{
				moveEvent( myCurrentPoint );
	
    }
}

/*!
\brief	Move event detection handler
This handles object highlighting during movement of the mouse across the view.
*/
AIS_StatusOfDetection occviewport::moveEvent( QPoint point )
{
	AIS_StatusOfDetection status;


	try
		{
	Handle(V3d_View) aview = this->getView();
	
	
	
	status = myController->getContext()->MoveTo( point.x(), point.y(), aview );
	qDebug() << "point" << point.x() << "," << point.y() << ":" << status;
		}
	catch(...)
		{

		}
	return status;
}

/*!
\brief	Drag event handler.
\param	startPoint	The gesture start point.
\param	endPoint	The gesture end point.
\param	multi		Allows selection of multiple objects.
\return The status of pick.
*/
AIS_StatusOfPick occviewport::dragEvent( const QPoint startPoint, const QPoint endPoint, const bool multi )
{
	AIS_StatusOfPick pick = AIS_SOP_NothingSelected;
	Handle(V3d_View) aview = this->getView();
	AIS_StatusOfDetection status = myController->getContext()->MoveTo( startPoint.x(), startPoint.y(), aview );
	if (status == AIS_StatusOfDetection::AIS_SOD_Nothing)
		{
	if (multi)
	{
		pick = myController->getContext()->ShiftSelect( std::min (startPoint.x(), endPoint.x()),
													    std::min (startPoint.y(), endPoint.y()),
													    std::max (startPoint.x(), endPoint.x()),
													    std::max (startPoint.y(), endPoint.y()),
													     myView );
	}
	else
	{
		pick = myController->getContext()->Select( std::min (startPoint.x(), endPoint.x()),
												   std::min (startPoint.y(), endPoint.y()),
												   std::max (startPoint.x(), endPoint.x()),
												   std::max (startPoint.y(), endPoint.y()),
												   myView );
	}
	emit selectionChanged();
	} else

		{
			
			
			AIS_StatusOfDetection status = myController->getContext()->MoveTo(startPoint.x(), startPoint.y(), myView);
			if (status != AIS_StatusOfDetection::AIS_SOD_Nothing )
				{

		    myController->getContext()->Select();
			Handle(AIS_InteractiveContext) ctx = myController->getContext();
			Handle(AIS_InteractiveObject) curais;

			double minx = std::min (startPoint.x(), endPoint.x());
			double maxx = std::max (startPoint.x(), endPoint.x());
			double sX,sY,sZ,eX,eY,eZ;
			convertToPlane(startPoint.x(),startPoint.y(),sX,sY,sZ);
			convertToPlane(endPoint.x(),endPoint.y(),eX,eY,eZ);

			gp_Pnt start3d(sX,sY,sZ);
			gp_Pnt end3d(eX,eY,eZ);


			double deltax = endPoint.x() - startPoint.x();
			
			if (ctx->NbSelected() > 0)
				{
				myController->getContext()->InitSelected();
				curais = myController->getContext()->SelectedInteractive();
			
				}
			else 
				{
				if (ctx->NbCurrents() > 0)
					{
					myController->getContext()->InitCurrent();
					curais = myController->getContext()->Current();
					}
				
				}
			if (!curais.IsNull())
				{
					//cowbellprogress *cob = new cowbellprogress(this,endPoint.x(),endPoint.y());
					//cob->show();
					Handle_User_AIS curuserais = Handle_User_AIS::DownCast(curais);
					TopoDS_Shape curshape = curuserais->myShape;
					gp_Pnt loc1 = hsf::GetCOG(curshape);
					gp_Pnt loc2 = loc1;
					loc2.SetX(loc1.X() + deltax);
					TopoDS_Shape moved = hsf::movepointopoint(curshape,start3d,end3d);
					hsf::updateUserAIS(moved,curuserais,ctx);
				}
				}

		}
	return pick;
}

/*!
\brief	This function handles object selection. 
		If there are more than one detected object under the mouse, 
		selection is changed/supplemented on subsequent clicks
\param	multi	Selects multiple objects if true (default false).
\return The status of the objects under the cursor
*/
AIS_StatusOfPick occviewport::inputEvent( bool multi )
{
	AIS_StatusOfPick pick = AIS_SOP_NothingSelected;

	if( multi )
	{
		pick = getContext()->ShiftSelect();
	}
	else
	{
		pick = getContext()->Select();
	}
	if ( pick != AIS_SOP_NothingSelected )
	{
		emit selectionChanged();
	}

	return pick;
}


bool occviewport::dump(Standard_CString theFile)
{
  redraw();
  return myView->Dump(theFile);
}

/*!
	\brief This function sets the current cursor for the given interraction mode.
	\param mode		The interraction mode
*/
void occviewport::setMode( const CurrentAction3d mode )
{
	if ( mode != myMode )
	{
		switch( mode )
		{
			case CurAction3d_DynamicPanning:
				setCursor( Qt::SizeAllCursor );
				break;
			case CurAction3d_DynamicZooming:
				setCursor( Qt::CrossCursor );
				break;
			case CurAction3d_DynamicRotation:
				setCursor( Qt::CrossCursor );
				break;
			case CurAction3d_GlobalPanning:
				setCursor( Qt::CrossCursor );
				break;
			case CurAction3d_WindowZooming:
				setCursor( Qt::PointingHandCursor );
				break;
			case CurAction3d_Nothing:
				//setCursor( myCrossCursor );
				setCursor( Qt::CrossCursor );
				break;
			default:
				setCursor( Qt::ArrowCursor );
				break;
		}
		myMode = mode;
	}
}

/*!
	\brief This is a Utility function for rounding the input value to a specific DP
*/
Standard_Real occviewport::precision( Standard_Real aReal )
{
	Standard_Real preciseReal;
	Standard_Real thePrecision = std::max (myPrecision, viewPrecision());

	if ( myPrecision != 0.0 )
	{
		preciseReal =  SIGN(aReal) * floor((std::abs(aReal) + thePrecision * 0.5) / thePrecision) * thePrecision;
	}
	else
	{
		preciseReal = aReal;
	}
	return preciseReal;
}

/*! ------------------------------------------------------------------------------------
\brief	ConvertToPlane convert 2d window position to 3d point on priviledged plane.
		This routine was provided by Matra Datavision during Foundation training.
		There still appears to be a pixel error in y co-ordinate transforms.
\param	Xs				The screen's x co-ordinate (in)
\param	Ys				The screen's y co-ordinate (in)
\param	X				The output x position on the privileged plane (out)
\param	Y				The output y position on the privileged plane (out)
\param	Z				The output z position on the privileged plane (out)
\return	Standard_Boolean indicating success or failure
*/
Standard_Boolean occviewport::convertToPlane(const Standard_Integer Xs,
											     const Standard_Integer Ys,
												 Standard_Real& X,
												 Standard_Real& Y,
												 Standard_Real& Z)
{
	Standard_Real Xv, Yv, Zv;
	Standard_Real Vx, Vy, Vz;
	gp_Pln aPlane(myView->Viewer()->PrivilegedPlane());

#ifdef OCC_PATCHED
	myView->Convert( Xs, Ys, Xv, Yv, Zv );
#else
	// The + 1 overcomes a fault in OCC, in "OpenGl_togl_unproject_raster.c",
	// which transforms the Y axis ordinate. The function uses the height of the
	// window, not the Y maximum which is (height - 1).
	myView->Convert( Xs, Ys + 1, Xv, Yv, Zv );
	
#endif

	myView->Proj( Vx, Vy, Vz );
	gp_Lin aLine(gp_Pnt(Xv, Yv, Zv), gp_Dir(Vx, Vy, Vz));
	IntAna_IntConicQuad theIntersection( aLine, aPlane, Precision::Angular() );
	if (theIntersection.IsDone())
	{
		if (!theIntersection.IsParallel())
		{
			if (theIntersection.NbPoints() > 0)
			{
				gp_Pnt theSolution(theIntersection.Point(1));
				X = theSolution.X();
				Y = theSolution.Y();
				Z = theSolution.Z();
				return Standard_True;
			}
		}
	}
	return Standard_False;
}

/*!
\brief	Draws the rubberband box
		This function is designed to reduce "flicker" as the box is redrawn,
		especially when the origin in the bottom corner of the window
\param	origin		A QPoint defining the screen origin
\param	position	A QPoint defining the current cursor screen location
*/
void occviewport::drawRubberBand( const QPoint origin, const QPoint position )
{
	if ( myRubberBand )
	{
		myRubberBand->hide();
		redraw();
		myRubberBand->setGeometry( QRect( origin, position ).normalized() );
		myRubberBand->show();
	}
}

/*!
\brief	Shows the rubberband box
*/
void occviewport::showRubberBand( void )
{
	if ( myRubberBand )
	{
		myRubberBand->show();
	}
}

/*!
\brief	Hides the rubberband box
*/
void occviewport::hideRubberBand( void )
{
	if ( myRubberBand )
	{
		myRubberBand->hide();
	}
}

/*!
\brief	Static OpenCascade callback proxy
*/
int occviewport::paintCallBack (Aspect_Drawable /* drawable */,
                                 void* aPointer,
                                 Aspect_GraphicCallbackStruct* /* data */)
{
	occviewport *aWidget = (occviewport *) aPointer;
	// Some defensive testing to establish valid pointers,
	// before testing background flag;
	if ( aWidget )
	{
		if ( aWidget->getController() )
		{
			if ( aWidget->getController()->isDrawingBackground() )
			{
				aWidget->paintOCC();
				
			}
		}
	}
	return 0;
}

/*!
\brief	The OpenGL paint routine in the callback.
*/
void occviewport::paintOCC( void )
{


	 if (gizmo)
    {
	    //glEnable( GL_BLEND );
	    //glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_COLOR );
		
		//glPushMatrix();

		float modelview[16];
		float modelproj[16];
		glGetFloatv( GL_MODELVIEW_MATRIX, modelview );
		glGetFloatv( GL_PROJECTION_MATRIX, modelproj);

		gizmo->SetCameraMatrix( modelview, modelproj );
        gizmo->Draw();

		//glDisable(GL_BLEND);
	    //glPopMatrix();
    }


	GLboolean isPlaneActive[GL_MAX_CLIP_PLANES];
	int i;

	glDisable( GL_LIGHTING );
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
    glMatrixMode( GL_PROJECTION );
	glPushMatrix();
    glLoadIdentity();

	GLfloat left   = -1.0f;
	GLfloat right  =  1.0f;
	GLfloat bottom = -1.0f;
	GLfloat top    =  1.0f;
	GLfloat depth  =  1.0f;

	for (i = 0; i < GL_MAX_CLIP_PLANES; i++)
	{
		isPlaneActive[i] = glIsEnabled(GL_CLIP_PLANE0 + i);
		glDisable(GL_CLIP_PLANE0 + i);
	}

    glOrtho( left, right, bottom, top, 1.0, -1.0 );
	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_COLOR );
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		

	GLfloat redval = 24.0/255.0;
	GLfloat greenval = 62.0/255.0;
	GLfloat blueval = 101/255.0;

	/*GLfloat redval = 1.0;
	GLfloat greenval = 1.0;
	GLfloat blueval = 1.0;*/

    glBegin( GL_QUADS);
    {
		glColor4f  (  0.4f, 0.4f, 0.4f, 0.4f );
	  //glColor4f  (  redval, greenval, blueval, 1.0f );
      glVertex3d (  left, bottom, depth );
      glVertex3d ( right, bottom, depth );
      glColor4f  (  redval, greenval, blueval, 1.0f );
      glVertex3d ( right,    top, depth );
      glVertex3d (  left,    top, depth );
    }
    glEnd();

	

	//lets make our circles look a little nicer!
	//ofSetCircleResolution(40);
//	int elapsedtime = TheTime->restart();
	
	/*if (elapsedtime > 200)
	{
	 drawlayer();
	 TheTime->start();
	}*/

	glDisable(GL_BLEND);
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();

	for (i = 0; i < GL_MAX_CLIP_PLANES; i++)
	{
		if (isPlaneActive[i])
		{ 
			glEnable(GL_CLIP_PLANE0 + i);
		}
	}
}


void occviewport::definewindowsize()
{
	Handle(Aspect_Window) hWin = myView->Window();
	w=width();
	h=height();

	//gizmo stuff
	if (gizmo)
        gizmo->SetScreenDimension( w, h );
	
	
	//hWin->Size(w,h);
}


void occviewport::drawlayer()
{
	/* Handle_V3d_View myView = myView;*/
     
   
	//glBlendFunc( GL_ZERO, GL_ONE );
	 
     Visual3d_Layer* myLayer=new Visual3d_Layer(myView->View()->ViewManager(), Aspect_TOL_OVERLAY, Standard_False);
     myLayer->SetViewport(w,h);
     myLayer->SetOrtho(0,w,h,0,(Aspect_TypeOfConstraint) -1);
     
     // then you could draw on the given layer
     myLayer->Begin();
     
     // draw some text
     Standard_CString aString = nameofdetected.ToCString();
     int xPos = myCurrentPoint.x(); 
     int yPos = myCurrentPoint.y(); 
     int textSize = 15;
	 myLayer->SetColor(Quantity_NOC_ORANGE);
	 myLayer->SetTextAttributes( Graphic3d_NameOfFont::Graphic3d_NOF_ASCII_ITALIC_COMPLEX, Aspect_TODT_NORMAL, Quantity_NOC_ORANGE );
     myLayer->DrawText(aString, myCurrentPoint.x(), myCurrentPoint.y(), textSize);// !!!!! CRASH ERROR !!!!!!!!
     
   /*   draw a polyline
     Standard_Real wn = 2.0, hn = (2.0/Standard_Real(w))*Standard_Real(h);
     Standard_Real x1 = -0.9, x2= x1+wn-.2, y1 = -0.9, y2=y1+hn-.2;
     myLayer->BeginPolyline();
     myLayer->AddVertex(x1, y1, Standard_False);
     myLayer->AddVertex(x2, y2);
     myLayer->ClosePrimitive();*/
     
     // draw a filled box
	 /*Quantity_Color(i,i*i,i*i,Quantity_TOC_RGB)*/
     myLayer->SetColor(Quantity_NOC_BLUE1);
	 for (int i = 1;i<3 ;i++)
	 {
		 
     myLayer->DrawRectangle(xPos + (i*13),yPos,12,12);
	 }
     //myLayer->SetTransparency(50);
	 //myLayer->SetColor(Quantity_NOC_YELLOW);
  //   myLayer->DrawRectangle(0,h-50,w,50);
	 //myLayer->SetColor(Quantity_NOC_BLACK);
  //   myLayer->DrawRectangle(0,h-40,w,10);
	 myLayer->SetColor(Quantity_NOC_GREEN);
     //myLayer->DrawRectangle(71,200,50,150);
	 myLayer->SetColor(Quantity_NOC_YELLOW);
     myLayer->SetTextAttributes( Graphic3d_NameOfFont::Graphic3d_NOF_ASCII_ITALIC_COMPLEX, Aspect_TODT_NORMAL, Quantity_NOC_YELLOW );
     //myLayer->DrawText(nameofdetected.ToCString(), 20, 200, textSize);// !!!!! CRASH ERROR !!!!!!!!
     
          // and close the layer and redraw
     myLayer->End();
     myView->Redraw();

}

/*!
\brief	This routine calculates the minimum sensible precision for the point
        selection routines, by setting an minumum resolution to a decade one
		higher than the equivalent pixel width.
\param	resized		Indicates that recaculation os required due to state
                    changes in the view.
*/
Standard_Real occviewport::viewPrecision( bool resized )
{

	Standard_Real X1, Y1, Z1;
	Standard_Real X2, Y2, Z2;
	if (!myWindow.IsNull())
	{
		if (resized || myViewPrecision == 0.0)
		{
			myView->Convert( 0, 0, X1, Y1, Z1 );
			myView->Convert( 1, 0, X2, Y2, Z2 );
			Standard_Real pixWidth = std::sqrt(  std::pow( X2 - X1, 2 )
										  + std::pow( Y2 - Y1, 2 )
										  + std::pow( Z2 - Z1, 2 ) ) ;
			if ( pixWidth != 0.0 )
			{
				// Return the precision as the next highest decade above the pixel width
				myViewPrecision = std::pow( 10.0, std::floor( std::log10( pixWidth ) + 1.0 ) );
			}
			else
			{
				// Return the user precision if window not defined
				myViewPrecision = myPrecision;
			}
		}
	}
	return myViewPrecision;
}

void occviewport::newShape()
{
	// Model changed
	fitAll();
}

void occviewport::refresh()
{
	// Model changed
	redraw( false );
}

//void occviewport::setStatusMessage (const QString& aString)
//{
//	myStatusMessage = aString;
//	emit sendStatus(aString);
//}

void occviewport::sendStatusLocation ()
{
	QString aString;
	QTextStream ts(&aString);
	ts << myStatusMessage << "(" << x() << "," << y() << "," << z() << ")";
	emit sendStatus(aString);

	
}

void occviewport::setProjection( V3d_TypeOfOrientation orientation )
{
	// This doubling of projections fixes rendering problems when
	// clipping planes are active - a test should be developed
	// for only doing this when necessary (to do list!)
	if( !myView.IsNull() )
	{
		for (int i = 0; i < 2; i++)
		{
			myView->SetProj( orientation );
		}
	}	
}


//Handle_GEOM_Object occviewport::GetGEOMFromAIS(Handle_AIS_InteractiveObject ais)
//{
//	Handle_GEOM_Object obj;
//	if( ais.IsNull() )
//		return obj;
//
//	Handle_TPrsStd_AISPresentation pres = Handle_TPrsStd_AISPresentation::DownCast( ais->GetOwner() );
//	if( pres.IsNull() )
//		return obj;
//	TDF_Label lab = pres->Label();
//
//	bool bFound = false;
//	bool bRoot = false;
// 	while( !bFound && !bRoot )
//	{
//		obj = GEOM_Object::GetObject( lab );
//		if( !obj.IsNull() )
//			bFound = true;
//		if( lab.IsRoot() )
//			bRoot = true;
//		lab = lab.Father();
//	}
//	return obj;
//}
//
//
//
//
//Handle(AIS_InteractiveObject) occviewport::GetAISFromGEOM( Handle_GEOM_Object obj )
//{
//	Handle(AIS_InteractiveObject) ais;
//	Handle(TPrsStd_AISPresentation) prs_ref = GetPresentationFromGEOM( obj );
//	if( prs_ref.IsNull() )
//		return ais;
//	else
//		return prs_ref->GetAIS();
//}
//Handle(TPrsStd_AISPresentation) occviewport::GetPresentationFromGEOM( Handle_GEOM_Object obj )
//{
//	Handle(TPrsStd_AISPresentation) pres;
//	if(obj != NULL)
//	{
//		if( !obj.IsNull() )
//		{
//			TDF_Label aResultLabel = obj->GetLastFunction()->GetEntry().FindChild(RESULT_LABEL);
//			Handle(TPrsStd_AISPresentation) prs_ref;
//			if( aResultLabel.FindAttribute(TPrsStd_AISPresentation::GetID(), prs_ref) )
//				return prs_ref;
//		}
//	}
//	return pres;
//}
//
//
