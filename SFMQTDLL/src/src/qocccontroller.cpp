

#include <QtGui/QMouseEvent>

#include <qnamespace.h>
#include "qoccdocument.h"
#include "qoccapplication.h"
#include "qocccontroller.h"
#include "occviewport.h"
#include "qoccinternal.h"


#include "AIS_MapOfInteractive.hxx"	//put this in the right place!!!!!!


QoccController::QoccController()
{
	// Create the OCC Viewers
	TCollection_ExtendedString a3DName( "Visual3D" );
	myViewer = createViewer( "DISPLAY", a3DName.ToExtString(), "", 1000.0 );
	myViewer->Init();
	myViewer->SetZBufferManagment( Standard_False );
	myViewer->SetDefaultViewProj( V3d_Zpos );	// Top view
 	myContext = new AIS_InteractiveContext( myViewer );
	myNISContext = new NIS_InteractiveContext;

	myGridType       = Aspect_GT_Rectangular;
	myGridMode       = Aspect_GDM_Lines;
	myGridColor      = Quantity_NOC_RED4;
	myGridTenthColor = Quantity_NOC_GRAY90;
	myCommand		 = NULL;
	myDocument		 = NULL;
	myIsDrawing		 = false;
	myDrawBackground = true;
	myLcType		 = TopAbs_COMPOUND;
	myeditdlg =NULL;

	myContext->HiddenLineAspect();

	setGridOffset ( 0.0 );
	qGeomApp->Controller = this;

}

QoccController::QoccController( QoccDocument* qDoc )
{
	// Create the OCC Viewers
	TCollection_ExtendedString a3DName( "Visual3D" );
	myViewer = createViewer( "DISPLAY", a3DName.ToExtString(), "", 1000.0 );
	myViewer->Init();
	myViewer->SetZBufferManagment( Standard_False );
	myViewer->SetDefaultViewProj( V3d_Zpos );	// Top view
	

	myContext = new AIS_InteractiveContext( myViewer );
	myContext->SetDisplayMode( AIS_Shaded );
	myContext->HiddenLineAspect();

	myGridType       = Aspect_GT_Rectangular;
	myGridMode       = Aspect_GDM_Lines;
	myGridColor      = Quantity_NOC_RED4;
	myGridTenthColor = Quantity_NOC_GRAY90;
	myCommand		 = NULL;
	myDocument		 = qDoc;
	myIsDrawing		 = false;
	myDrawBackground = true;
	myLcType		 = TopAbs_COMPOUND;

	setGridOffset ( 0.0 );
	myeditdlg =NULL;

}

QoccController::~QoccController()
{
	if ( myCommand )
	{
		delete myCommand;
	}
}

#ifndef WNT
Handle(V3d_Viewer) QoccController::createViewer(	const Standard_CString aDisplay,
													const Standard_ExtString aName,
													const Standard_CString aDomain,
													const Standard_Real ViewSize )
{
    static Handle(Graphic3d_GraphicDevice) defaultdevice;

    if( defaultdevice.IsNull() )
	{
		defaultdevice = new Graphic3d_GraphicDevice( getenv(aDisplay) );
	}

    return new V3d_Viewer(	defaultdevice,
							aName,
							aDomain,
							ViewSize,
							V3d_XposYnegZpos,
							Quantity_NOC_BLACK,
                    		V3d_ZBUFFER,
							V3d_GOURAUD,
							V3d_WAIT );
}
#else
Handle(V3d_Viewer) QoccController::createViewer(	const Standard_CString /* aDisplay */,
													const Standard_ExtString aName,
													const Standard_CString aDomain,
													const Standard_Real ViewSize )
{
    static Handle(Graphic3d_WNTGraphicDevice) defaultdevice;
    if( defaultdevice.IsNull() )
	{
        defaultdevice = new Graphic3d_WNTGraphicDevice();
	}

    return new V3d_Viewer(	defaultdevice,
							aName,
							aDomain,
							ViewSize,
							V3d_XposYnegZpos,
							Quantity_NOC_BLACK,
                    		V3d_ZBUFFER,
							V3d_GOURAUD,
							V3d_WAIT );
}
#endif  // WNT

bool QoccController::isDrawing()
{
 return false;
}

/*!
\brief	Deletes all objects.

		This function deletes all dispayed objects from the AIS context.
		No parameters.
*/
void QoccController::deleteAllObjects()
{
	AIS_ListOfInteractive aList;
	myContext->DisplayedObjects( aList );
	AIS_ListIteratorOfListOfInteractive aListIterator;
	for ( aListIterator.Initialize( aList ); aListIterator.More(); aListIterator.Next() )
	{
		myContext->Remove( aListIterator.Value(), Standard_False );
	}
}
/*!
\brief	Sets the privileged plane to the XY Axis.
*/
void QoccController::gridXY( void )
{
	myGridColor = Quantity_NOC_RED4;
	myViewer->Grid()->SetColors( myGridColor, myGridTenthColor );
	gp_Ax3 aPlane(gp_Pnt( 0.0, 0.0, 0.0 ), gp_Dir( 0.0, 0.0, 1.0 ));
	myViewer->SetPrivilegedPlane( aPlane );
}
/*!
\brief	Sets the privileged plane to the XZ Axis.

		Note the negative direction of the Y axis.
		This is corrrect for a right-handed co-ordinate set.
*/
void QoccController::gridXZ( void )
{
	myGridColor = Quantity_NOC_BLUE4;
	myViewer->Grid()->SetColors( myGridColor, myGridTenthColor );
	gp_Ax3 aPlane( gp_Pnt(0., 0., 0.),gp_Dir(0., -1., 0.) );
	myViewer->SetPrivilegedPlane( aPlane );
}
/*!
\brief	Sets the privileged plane to the XY Axis.
 */
void QoccController::gridYZ ( void )
{
	myGridColor = Quantity_NOC_GREEN4;
	myViewer->Grid()->SetColors( myGridColor, myGridTenthColor );
	gp_Ax3 aPlane( gp_Pnt( 0., 0., 0.), gp_Dir( 1., 0., 0. ) );
	myViewer->SetPrivilegedPlane( aPlane );
}

/*!
\brief	Turn the grid on.
 */
void QoccController::gridOn( void )
{
	myViewer->ActivateGrid( myGridType , myGridMode );
	myViewer->SetGridEcho ( Standard_True );
}

/*!
\brief	Turn the grid off.
*/
void QoccController::gridOff( void )
{
	myViewer->DeactivateGrid();
	myViewer->SetGridEcho( Standard_False );
}

/*!
\brief	Select a rectangular grid
*/
void QoccController::gridRect( void )
{
	myGridType = Aspect_GT_Rectangular;
	myViewer->ActivateGrid( myGridType , myGridMode );
	myViewer->Grid()->SetColors( myGridColor, myGridTenthColor );
}

/*!
\brief	Select a circular grid.
*/
void QoccController::gridCirc( void )
{
	myGridType = Aspect_GT_Circular;
	myViewer->ActivateGrid( myGridType , myGridMode );
	myViewer->Grid()->SetColors( myGridColor, myGridTenthColor );
}

/*!
\brief	Sets the current interface command
*/
void QoccController::setCommand( QoccCommand* command )
{
	clearCommand();
	myCommand = command;
}

/*!
\brief	Remove or cancel the current command
*/
void QoccController::clearCommand( )
{
	if (myCommand)
	{
		delete myCommand;
		myCommand = NULL;
	}
}

void QoccController::setGridOffset( Quantity_Length offset )
{
	Quantity_Length radius;
	Quantity_Length xSize, ySize;
	Quantity_Length oldOffset;

	myViewer->CircularGridGraphicValues( radius, oldOffset );
	myViewer->SetCircularGridGraphicValues( radius, offset);

	myViewer->RectangularGridGraphicValues( xSize, ySize, oldOffset );
	myViewer->SetRectangularGridGraphicValues( xSize, ySize, offset );
}

void QoccController::setLocalContext( TopAbs_ShapeEnum lcType )
{
	clearLocalContext();
	myContext->OpenLocalContext();
	myContext->ActivateStandardMode( lcType );
	myLcType = lcType;
}

void QoccController::clearLocalContext( void )
{
	if (hasLocalContext())
	{
		myContext->ClearLocalContext();
		myContext->CloseLocalContext();
	}
}

void QoccController::clearAllLocalContexts( void )
{
	if (hasLocalContext())
	{
		myContext->CloseAllContexts();
	}
}

bool QoccController::hasLocalContext( void )
{
	 return (bool) myContext->HasOpenedContext();
}

void QoccController::moveEvent( occviewport* widget, QMouseEvent* e  )
{
	
	myViewer->Update();
	
}

void QoccController::clickEvent( occviewport* widget, QMouseEvent* e )
{

		if ( e->button() & Qt::RightButton )
		{
			clearCommand();
			myViewer->Update();
		}
		else
		{
						
		}
	

	
}

void QoccController::SetEditDlg(QDialog* thedlg)
{ 
	myeditdlg = thedlg;
}





void QoccController::undo( void )
{
	//myDocument->undo();
	update();
}

void QoccController::redo( void )
{
	//myDocument->redo();
	update();
}

void QoccController::update( void )
{
	emit completed();
	myViewer->Update();
	myViewer->Redraw();

}

void QoccController::sendCompleted( void )
{
	emit completed();
}

void QoccController::onSelectionChanged()
{
	emit selectionChanged();
}

void QoccController::onViewSelectionChanged( AIS_ListOfInteractive& newSelected )
{
	AIS_MapOfInteractive mapSelected( newSelected.Extent() );
	AIS_ListIteratorOfListOfInteractive it;

	// select all the objects in newSelected 
	// and put the selected objects in a map (for quicker lookups?)
	for( it.Initialize( newSelected ); it.More(); it.Next() )
	{
		if( myContext->HasOpenedContext() )
		{
			if( !myContext->IsSelected( it.Value() ) )
			{
				myContext->AddOrRemoveSelected( it.Value() );
			}
		}
		else
		{
			if( !myContext->IsCurrent( it.Value() ) )
			{
				myContext->AddOrRemoveCurrentObject( it.Value() );
			}
		}
		mapSelected.Add( it.Value() );
	}

	AIS_ListOfInteractive	clearList;
	//find the objects to be deselected
	if( myContext->HasOpenedContext() )
	{
		for( myContext->InitSelected(); myContext->MoreSelected(); myContext->NextSelected() )
		{
			if( !mapSelected.Contains( myContext->SelectedInteractive() ) )
			{
				clearList.Append( myContext->SelectedInteractive() );
			}
		}
	}
	else
	{
		for( myContext->InitCurrent(); myContext->MoreCurrent(); myContext->NextCurrent() )
		{
			if( !mapSelected.Contains( myContext->Current() ) ) 
			{
				clearList.Append( myContext->Current() );
			}
		}
	}
	// deselect them
	if( myContext->HasOpenedContext() )
	{
		for( it.Initialize( clearList ); it.More(); it.Next() )
		{
			myContext->AddOrRemoveSelected( it.Value() );
		}
	}
	else
	{
		for( it.Initialize( clearList ); it.More(); it.Next() )
		{
			myContext->AddOrRemoveCurrentObject( it.Value() );
		}
	}
}

void QoccController::setDrawBackground ( bool drawBackground ) 
{
	myDrawBackground = drawBackground;
}

void QoccController::ChangeBackgroundColor()
{
	
}