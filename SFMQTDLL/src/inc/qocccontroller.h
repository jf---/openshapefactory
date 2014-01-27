
#ifndef QOCCCONTROLLER_H
#define QOCCCONTROLLER_H

#include <QtCore/QObject>
#include "commonlibraries.h"
#include "qoccdocument.h"
#include <QDialog>

#include <TopAbs_ShapeEnum.hxx>

class QoccCommand;
class occviewport;
class QMouseEvent;

class Q_DECL_EXPORT QoccController : public QObject
{

	Q_OBJECT

public:

    QoccController();
	QoccController( QoccDocument* qDoc );
    ~QoccController();
    void SetEditDlg(QDialog* thedlg);

	Handle(V3d_Viewer)&             getViewer( void )  { return myViewer; }
    Handle(AIS_InteractiveContext)& getContext( void ) { return myContext; }
	void setNISContext(Handle(NIS_InteractiveContext) incontrol){myNISContext = incontrol;}
	const Handle(NIS_InteractiveContext) getNISContext( void ) { return myNISContext; }
	QoccDocument*					getDocument( void ){ return myDocument; }
	int								id( void )         { return myDocument->id(); }

	Handle(V3d_Viewer) createViewer( const Standard_CString aDisplay,
				                     const Standard_ExtString aName,
				                     const Standard_CString aDomain,
				                     const Standard_Real ViewSize,
				                     const V3d_TypeOfOrientation ViewProj,
				                     const Standard_Boolean ComputedMode,
				                     const Standard_Boolean aDefaultComputedMode );

	TopAbs_ShapeEnum localContext() { return myLcType; }
	void setLocalContext( TopAbs_ShapeEnum lcType );
	void clearLocalContext( void );
	void clearAllLocalContexts( void );
	bool hasLocalContext( void );
//    Handle_GEOM_Object GetGEOMFromAIS(Handle_AIS_InteractiveObject ais);
	void deleteAllObjects( void );

	void setGridOffset ( Quantity_Length offset );

	bool isCommandActive( void )	{ return !(myCommand == NULL); }
	bool isDrawing( void );		

	QoccCommand* getCommand( void ) { return myCommand; }
	void setCommand ( QoccCommand* command );
	void clearCommand ( void );

	void undo( void );
	void redo( void );

	void update( void );

	void sendCompleted( void );

public slots:

	void gridXY   ( void );
	void gridXZ   ( void );
	void gridYZ   ( void );
	void gridOn   ( void );
	void gridOff  ( void );
	void gridRect ( void );
	void gridCirc ( void );

	void clickEvent(occviewport* widget, QMouseEvent* e);
	void moveEvent (occviewport* widget, QMouseEvent* e);

	void onSelectionChanged();
	void onViewSelectionChanged( AIS_ListOfInteractive& selected );
	

	void setDrawBackground ( bool drawBackground = true );
	void ChangeBackgroundColor();
	inline bool getDrawBackground () { return myDrawBackground; }
	inline bool isDrawingBackground() { return myDrawBackground; }

signals:

	void selectionChanged();
	void error (int errorCode, QString& errorDescription);
	void completed();
	void mouseMoved   ( occviewport* widget, QMouseEvent* e );
	void mouseClicked ( occviewport* widget, QMouseEvent* e );

private:

	Handle(V3d_Viewer)				myViewer;
	Handle(AIS_InteractiveContext)	myContext;
	Handle(NIS_InteractiveContext)	myNISContext;
	Aspect_GridType					myGridType;
	Aspect_GridDrawMode				myGridMode;
	Quantity_NameOfColor			myGridColor;
	Quantity_NameOfColor			myGridTenthColor;
	QoccCommand*					myCommand;
	QoccDocument*					myDocument;
	bool							myIsDrawing;
	TopAbs_ShapeEnum				myLcType;
	bool							myDrawBackground;
	QDialog*                        myeditdlg;
};

#endif // QOCCCONTROLLER_H
