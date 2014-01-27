

#include <QtGui/QApplication>
#include <qDebug>

#include <StlMesh_Mesh.hxx>
#include <MeshVS_TextPrsBuilder.hxx> 


#include <RWStl.hxx>
#include <OSD_Path.hxx>
#include <MeshVS_Mesh.hxx>
#include <MeshVS_Drawer.hxx>
#include <MeshVS_DrawerAttribute.hxx>
#include <MeshVS_MeshPrsBuilder.hxx>
#include <XSDRAWSTLVRML_DataSource.hxx>
#include <Graphic3d_MaterialAspect.hxx>
#include <MeshVS_ElementalColorPrsBuilder.hxx>

#include "qoccinputoutput.h"
#include <User_AIS.hxx>
#include <QProgressDialog>

#include <QTime>

QoccInputOutput::QoccInputOutput(void)
{
}

QoccInputOutput::~QoccInputOutput(void)
{
}

bool QoccInputOutput::importMesh( const QString& fileName,
								  const FileFormat format,
								  const Handle(AIS_InteractiveContext)& ic )
{
	 //* STL Loader!
	Q_UNUSED(format);
	OSD_Path thePath(fileName.toAscii().data());
	Handle( StlMesh_Mesh ) aSTLMesh = RWStl::ReadFile(thePath);
	
	
	Handle( MeshVS_Mesh  ) aMesh = new MeshVS_Mesh();
	
	Handle( XSDRAWSTLVRML_DataSource ) aDS = new XSDRAWSTLVRML_DataSource( aSTLMesh );

	
	aMesh->SetDataSource(aDS);
	aMesh->AddBuilder( new MeshVS_MeshPrsBuilder( aMesh), Standard_True );//False -> No selection 
	//MeshVS_ElementalColorPrsBuilder* meshbuilder = new MeshVS_ElementalColorPrsBuilder( aMesh ,MeshVS_DMF_ElementalColorDataPrs | MeshVS_DMF_OCCMask );

	//meshbuilder->SetDataSource(aDS);

	

	
	/*TColStd_MapIteratorOfPackedMapOfInteger it (aDS->GetAllElements());
	Handle( MeshVS_HArray1OfSequenceOfInteger ) aTopo;
	Standard_Integer NbNodes;*/

 // for( ; it.More(); it.Next() )
 // {
 //   Standard_Integer aKey = it.Key();

 //   aDS->Get3DGeom( aKey, NbNodes, aTopo ) ;
	//
	//	MeshVS_EntityType elemtype;
	//	aDS->GetGeomType(aKey,true,elemtype);

	//	if (elemtype == MeshVS_ET_Face)
	//	{
	//		double r = double(rand()) / double(RAND_MAX);
	//		double g = double(rand()) / double(RAND_MAX);
	//		double b = double(rand()) / double(RAND_MAX);
	//		meshbuilder->SetColor1(aKey,Quantity_Color(r,g,b,Quantity_TypeOfColor::Quantity_TOC_RGB));
	//		//meshbuilder->SetColor2(aKey,Quantity_Color(r,g,b,Quantity_TypeOfColor::Quantity_TOC_RGB));
	//	}

	//}
  



	//aMesh->AddBuilder( meshbuilder, Standard_True );//False -> No selection 

	

//aMesh->AddBuilder( aBuilder, Standard_True );

aMesh->GetDrawer()->SetBoolean(MeshVS_DA_FrontMaterial,Graphic3d_NOM_SHINY_PLASTIC);// Graphic3d_NOM_DEFAULT );
aMesh->GetDrawer()->SetBoolean(MeshVS_DA_DisplayNodes,Standard_False);
aMesh->GetDrawer()->SetBoolean(MeshVS_DA_ColorReflection,Standard_True);

aMesh->SetDisplayMode (2);
QTime time;
time.start();

ic->Display( aMesh,Standard_False );

double milliseconds = time.elapsed();
double seconds = (milliseconds/1000);
double minutes = seconds/60;
double hours = minutes/60;


QString timetxt;
QTextStream linestream(&timetxt);
linestream << "h" << hours << ":m " << minutes << ":s " << seconds << ".ms" << milliseconds; 

qDebug() << timetxt;

	



	/*Handle(MeshVS_TextPrsBuilder) textpres = new MeshVS_TextPrsBuilder(aMesh,20,Quantity_Color(255,0,0,Quantity_TypeOfColor::Quantity_TOC_RGB),MeshVS_DMF_TextDataPrs );

	TColStd_PackedMapOfInteger elements;
	elements =  aDS->GetAllNodes();
	int countofelem = elements.Extent();

	for(int i = 0 ; i < countofelem; i++)
		{
		
		int id = i;
		TCollection_AsciiString thetext = "U";
		textpres->SetText(false,id,thetext);
		}
		
		aMesh->AddBuilder(textpres,true);
	*/
	//aMesh->GetDrawer()->SetBoolean(MeshVS_DA_DisplayNodes,Standard_False); //MeshVS_DrawerAttribute
	//aMesh->GetDrawer()->SetBoolean(MeshVS_DA_ShowEdges,Standard_True);
	//aMesh->GetDrawer()->SetMaterial(MeshVS_DA_FrontMaterial,Graphic3d_NameOfMaterial::Graphic3d_NOM_ALUMINIUM);
	
	
	
	
	
	//aMesh->SetDisplayMode( MeshVS_DMF_ElementalColorDataPrs ); // Mode as defaut
	//aMesh->SetHilightMode( MeshVS_DMF_Shading ); // Wireframe as default hilight mode
	
	//ic->Display(aMesh);

	return true;
}

bool QoccInputOutput::importModel(  const QString& fileName,
								    const FileFormat format,
                                    const Handle(AIS_InteractiveContext)& ic )
{
	bool result;
	QApplication::setOverrideCursor( Qt::WaitCursor );
	if (format == FormatSTL)
	{
		result = importMesh( fileName, format, ic );
	}
	else
	{
		Handle(TopTools_HSequenceOfShape) shapes = importModel( format, fileName );
		if ( shapes.IsNull() || !shapes->Length() )
		{
			result = false;
		}
		else
		{
			result = true;
			for ( int i = 1; i <= shapes->Length(); i++ )
			{

				Handle(User_AIS) anAISShape = new User_AIS( shapes->Value( i ),ic );
				ic->SetMaterial( anAISShape, Graphic3d_NameOfMaterial::Graphic3d_NOM_NEON_PHC);
		        ic->SetColor( anAISShape, Quantity_NOC_BLACK);
		        ic->SetDisplayMode( anAISShape, 2, Standard_False );
		        ic->Display(anAISShape, Standard_False);
			}
		}
	}
	QApplication::restoreOverrideCursor();
    // ic->UpdateCurrentViewer();

		/*	QoccController* vc = qGeomApp->activeWindow()->getController();
			QoccDocument* doc = vc->getDocument();
			GEOM_Solver* thesolver = qGeomApp->getSolver();
			
			TopAbs_ShapeEnum lc;
			int doc_id = doc->id();*/


  return result;
}

Handle(TopTools_HSequenceOfShape) QoccInputOutput::importModel( const FileFormat format, 
																															  const QString& file )
{
  Handle(TopTools_HSequenceOfShape) shapes;
  try {
      switch ( format )
      {
        case FormatBREP:
          shapes = importBREP( file );
          break;
        case FormatIGES:
          shapes = importIGES( file );
          break;
        case FormatSTEP:
          shapes = importSTEP( file );
          break;
        case FormatCSFDB:
          shapes = importCSFDB( file );
          break;
		case FormatCATIA:
          shapes = importCATIA( file );
          break;
	      default:
		      // To Do - Error message here?
		      break;
      }
  } 
  catch ( Standard_Failure ) 
  {
     shapes.Nullify();
  }
  return shapes;
}


/******************************************************************
 * EXPORT FUNCTIONALITY
 ******************************************************************/

bool QoccInputOutput::exportModel( const QString& fileName,
								   const FileFormat format,
								   const Handle(AIS_InteractiveContext)& ic )
{

	Handle(TopTools_HSequenceOfShape) shapes = getvShapes( ic );
    if ( shapes.IsNull() || !shapes->Length() )
	{
		return false;
	}

    QApplication::setOverrideCursor( Qt::WaitCursor );
    bool stat = exportModel( format, fileName, shapes );
    QApplication::restoreOverrideCursor();

    return stat;
}

bool QoccInputOutput::exportModel( const FileFormat format, 
								   const QString& file, 
								   const Handle(TopTools_HSequenceOfShape)& shapes )
{
  bool status;
  try {
      switch ( format )
      {
        case FormatBREP:
          status = exportBREP( file, shapes );
          break;
        case FormatIGES:
          status = exportIGES( file, shapes );
          break;
        case FormatSTEP:
          status = exportSTEP( file, shapes );
          break;
        case FormatCSFDB:
          status = exportCSFDB( file, shapes );
          break;
        case FormatSTL:
          status = exportSTL( file, shapes );
          break;
        case FormatVRML:
          status = exportVRML( file, shapes );
          break;
    }
  } 
  catch ( Standard_Failure ) 
  {
    status = false;
  }
  return status;
}

Handle(TopTools_HSequenceOfShape) QoccInputOutput::getShapes( const Handle(AIS_InteractiveContext)& ic )
{
  Handle(TopTools_HSequenceOfShape) aSequence;
  Handle(AIS_InteractiveObject) picked;
  for ( ic->InitCurrent(); ic->MoreCurrent(); ic->NextCurrent() )
  {
    Handle(AIS_InteractiveObject) obj = ic->Current();
    if ( obj->IsKind( STANDARD_TYPE( AIS_Shape ) ) )
    {
      TopoDS_Shape shape = Handle_AIS_Shape::DownCast(obj)->Shape();
      if ( aSequence.IsNull() )
			{
        aSequence = new TopTools_HSequenceOfShape();
			}
      aSequence->Append( shape );
    }
  }
  return aSequence;
}



Handle(TopTools_HSequenceOfShape) QoccInputOutput::getvShapes( const Handle(AIS_InteractiveContext)& ic )
{
	Handle(TopTools_HSequenceOfShape) aSequence ;
	AIS_ListOfInteractive aList;
	ic->DisplayedObjects( aList );
	AIS_ListIteratorOfListOfInteractive aListIterator;
	for ( aListIterator.Initialize( aList ); aListIterator.More(); aListIterator.Next() )
	{

		Handle(AIS_InteractiveObject) obj = aListIterator.Value();
		
		TopoDS_Shape shape;
		if ( obj->IsKind( STANDARD_TYPE( User_AIS ) ) ) // check if its a user  ais shape
		{
		 shape = Handle_User_AIS::DownCast(obj)->myShape;
		}

		if ( obj->IsKind( STANDARD_TYPE( AIS_Shape ) ) ) // check if its the standar ais shape
		{
		 shape = Handle_AIS_Shape::DownCast(obj)->Shape();
		}

		if(!shape.IsNull()) // if not null append
			{
		  if ( aSequence.IsNull() )
				{
			aSequence = new TopTools_HSequenceOfShape();
				}

		  aSequence->Append( shape );
			}
		
	}

    return aSequence;
}
Handle(TopTools_HSequenceOfShape) QoccInputOutput::importBREP( const QString& file )
{
	Handle(TopTools_HSequenceOfShape) aSequence;
    TopoDS_Shape aShape;
	BRep_Builder aBuilder;

	Standard_Boolean result = BRepTools::Read(  aShape, file.toAscii().data(), aBuilder );
	if ( result )
  {
	  aSequence = new TopTools_HSequenceOfShape();
		aSequence->Append( aShape );
  }

  return aSequence;
}

Handle(TopTools_HSequenceOfShape) QoccInputOutput::importIGES( const QString& file )
{
  Handle(TopTools_HSequenceOfShape) aSequence;
  IGESControl_Reader Reader;
  int status = Reader.ReadFile( file.toAscii().data() );

  if ( status == IFSelect_RetDone )
  {
    aSequence = new TopTools_HSequenceOfShape();
    Reader.TransferRoots();
	
	
    TopoDS_Shape aShape = Reader.OneShape();
    aSequence->Append( aShape );
  }
	return aSequence;
}

Handle(TopTools_HSequenceOfShape) QoccInputOutput::importIGESprogress( const QString& file )
{
  Handle(TopTools_HSequenceOfShape) aSequence;
  IGESControl_Reader Reader;
  int status = Reader.ReadFile( file.toAscii().data() );

  QProgressDialog progress("Exporting IGES File", "Abort Copy", 1,100);
  if ( status == IFSelect_RetDone )
  {
	  // Interface_TraceFile::SetDefault();
	  bool failsonly = false;
	  Reader.PrintCheckLoad( failsonly, IFSelect_ItemsByEntity );

	  int nbr = Reader.NbRootsForTransfer();
	  
	  Reader.PrintCheckTransfer( failsonly, IFSelect_ItemsByEntity );
	  for ( Standard_Integer n = 1; n <= nbr; n++ )
	  {
	    bool ok = Reader.TransferOneRoot( n );
			if (ok)
			{
				int nbs = Reader.NbShapes();
				if ( nbs > 0 )
				{
					aSequence = new TopTools_HSequenceOfShape();
					for ( int i = 1; i <= nbs; i++ )
					{
						TopoDS_Shape shape = Reader.Shape( i );
						aSequence->Append( shape );
						double percent = (n/nbr) * (i/nbs) * 100 ;
						progress.setValue(percent);
					}
				}
			}
    }
  }


	return aSequence;
}

Handle(TopTools_HSequenceOfShape) QoccInputOutput::importSTEP( const QString& file )
{
	Handle(TopTools_HSequenceOfShape) aSequence;

	STEPControl_Reader aReader;
	
	IFSelect_ReturnStatus status = aReader.ReadFile( file.toAscii().data() );
	if ( status == IFSelect_RetDone )
  {
	  // Interface_TraceFile::SetDefault();
	  bool failsonly = false;
	  aReader.PrintCheckLoad( failsonly, IFSelect_ItemsByEntity );

	  int nbr = aReader.NbRootsForTransfer();
	  
	  aReader.PrintCheckTransfer( failsonly, IFSelect_ItemsByEntity );
	  for ( Standard_Integer n = 1; n <= nbr; n++ )
	  {
	    bool ok = aReader.TransferRoot( n );
			if (ok)
			{
				int nbs = aReader.NbShapes();
				if ( nbs > 0 )
				{
					aSequence = new TopTools_HSequenceOfShape();
					for ( int i = 1; i <= nbs; i++ )
					{
						TopoDS_Shape shape = aReader.Shape( i );
						aSequence->Append( shape );
					}
				}
			}
    }
  }
	return aSequence;
}

Handle(TopTools_HSequenceOfShape) QoccInputOutput::importCSFDB( const QString& file )
{
	Handle(TopTools_HSequenceOfShape) aSequence;

  // Check file type
  if ( FSD_File::IsGoodFileType( file.toAscii().data() ) != Storage_VSOk )
    return aSequence;

  static FSD_File fileDriver;
  TCollection_AsciiString aName( file.toAscii().data() );
  if ( fileDriver.Open( aName, Storage_VSRead ) != Storage_VSOk )
      return aSequence;

  Handle(ShapeSchema) schema = new ShapeSchema();
  Handle(Storage_Data) data  = schema->Read( fileDriver );
  if ( data->ErrorStatus() != Storage_VSOk )
	{
    return aSequence;
	}

  fileDriver.Close();

  aSequence = new TopTools_HSequenceOfShape();
  Handle(Storage_HSeqOfRoot) roots = data->Roots();
  for ( int i = 1; i <= roots->Length() ; i++ )
  {
    Handle(Storage_Root) r = roots->Value( i );
    Handle(Standard_Persistent) p = r->Object();
    Handle(PTopoDS_HShape) aPShape = Handle(PTopoDS_HShape)::DownCast(p);
    if ( !aPShape.IsNull() )
    {
      PTColStd_PersistentTransientMap aMap;
      TopoDS_Shape aTShape;
      MgtBRep::Translate( aPShape, aMap, aTShape, MgtBRep_WithTriangle );
      aSequence->Append( aTShape );
    }
  }

  return aSequence;
}

bool QoccInputOutput::exportBREP( const QString& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
  if ( shapes.IsNull() || shapes->IsEmpty() )
      return false;

  TopoDS_Shape shape = shapes->Value( 1 );
  return BRepTools::Write( shape, file.toAscii().data() );
}

bool QoccInputOutput::exportIGES( const QString& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
  if ( shapes.IsNull() || shapes->IsEmpty() )
      return false;

  IGESControl_Controller::Init();
  IGESControl_Writer writer( Interface_Static::CVal( "XSTEP.iges.unit" ),
                               Interface_Static::IVal( "XSTEP.iges.writebrep.mode" ) );
	QProgressDialog progress("Exporting IGES File", "Abort Copy", 1,100);
	for ( int i = 1; i <= shapes->Length(); i++ )
		{
		try
			{
			/*qDebug() << "writing shape" << i/shapes->Length() << ":" << */
			writer.AddShape ( shapes->Value( i ) );
			progress.setValue((i*100/shapes->Length()));
			}
		catch(...)
			{
				
			}
		}
	writer.ComputeModel();
	return writer.Write( file.toAscii().data() );
}

bool QoccInputOutput::exportSTEP( const QString& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
  if ( shapes.IsNull() || shapes->IsEmpty() )
      return false;

  IFSelect_ReturnStatus status;

  STEPControl_Writer writer;
	for ( int i = 1; i <= shapes->Length(); i++ )
  {
		status = writer.Transfer( shapes->Value( i ), STEPControl_AsIs );
    if ( status != IFSelect_RetDone )
		{
      return false;
		}
  }

  status = writer.Write( file.toAscii().data() );

  switch ( status )
  {
    case IFSelect_RetError:
      myInfo = tr( "INF_DATA_ERROR" );
      break;
    case IFSelect_RetFail:
      myInfo = tr( "INF_WRITING_ERROR" );
      break;
    case IFSelect_RetVoid:
      myInfo = tr( "INF_NOTHING_ERROR" );
      break;
    default:
	    break;
  }
  return status == IFSelect_RetDone;
}

bool QoccInputOutput::exportCSFDB( const QString& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{

  if ( shapes.IsNull() || shapes->IsEmpty() )
	{
      return false;
	}

  static FSD_File fileDriver;

  Handle(ShapeSchema) schema = new ShapeSchema();
  Handle(Storage_Data) data  = new Storage_Data();
  data->ClearErrorStatus();

  data->SetApplicationName( TCollection_ExtendedString( "Sample Import / Export" ) );
  data->SetApplicationVersion( "1" );
  data->SetDataType( TCollection_ExtendedString( "Shapes" ) );
  data->AddToUserInfo( "Storing a persistent set of shapes in a flat file" );
  data->AddToComments( TCollection_ExtendedString( "Application is based on CasCade 5.0 Professional" ) );

  if ( fileDriver.Open( file.toAscii().data(), Storage_VSWrite ) != Storage_VSOk )
  {
      myInfo = tr( "INF_TRANSLATE_ERROR_CANTSAVEFILE" ).arg( file );
      return false;
  }

  PTColStd_TransientPersistentMap aMap;
	for ( int i = 1; i <= shapes->Length(); i++ )
	{
		TopoDS_Shape shape = shapes->Value( i );
		if ( shape.IsNull() )
		{
			myInfo = tr( "INF_TRANSLATE_ERROR_INVALIDSHAPE" );
			return false;
    }

		
    Handle(PTopoDS_HShape) pshape = MgtBRep::Translate( shape, aMap, MgtBRep_WithTriangle );
		TCollection_AsciiString objName = TCollection_AsciiString( "Object_" ) + TCollection_AsciiString( i );
		data->AddRoot( objName, pshape );
	}

  schema->Write( fileDriver, data );
  fileDriver.Close();

  if ( data->ErrorStatus() != Storage_VSOk )
  {
      myInfo = tr( "INF_TRANSLATE_ERROR_CANTSAVEDATA" );
      return false;
  }
  return true;
}

bool QoccInputOutput::exportSTL( const QString& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
  if ( shapes.IsNull() || shapes->IsEmpty() )
	{  
		return false;
	}

	TopoDS_Compound res;
	BRep_Builder builder;
	builder.MakeCompound( res );

	for ( int i = 1; i <= shapes->Length(); i++ )
	{
		TopoDS_Shape shape = shapes->Value( i );
		if ( shape.IsNull() )
		{
			myInfo = tr( "INF_TRANSLATE_ERROR_INVALIDSHAPE" );
			return false;
    }
		builder.Add( res, shape );
	}

	StlAPI_Writer writer;
	writer.Write( res, file.toAscii().data() );

	return true;
}

bool QoccInputOutput::exportVRML( const QString& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
  if ( shapes.IsNull() || shapes->IsEmpty() )
	{
    return false;
	}

	TopoDS_Compound res;
	BRep_Builder builder;
	builder.MakeCompound( res );

	for ( int i = 1; i <= shapes->Length(); i++ )
	{
		TopoDS_Shape shape = shapes->Value( i );
		if ( shape.IsNull() )
		{
			myInfo = tr( "INF_TRANSLATE_ERROR_INVALIDSHAPE" );
			return false;
    }
		builder.Add( res, shape );
	}

	VrmlAPI_Writer writer;
	writer.Write( res, file.toAscii().data() );

  return true;
}



bool QoccInputOutput::checkFacetedBrep( const Handle(TopTools_HSequenceOfShape)& shapes )
{
	bool err = false;
	for ( int i = 1; i <= shapes->Length(); i++ )
	{
	  TopoDS_Shape shape = shapes->Value( i );

    for ( TopExp_Explorer fexp( shape, TopAbs_FACE ); fexp.More() && !err; fexp.Next() )
		{
		  Handle(Geom_Surface) surface = BRep_Tool::Surface( TopoDS::Face( fexp.Current() ) );
		  if ( !surface->IsKind( STANDARD_TYPE( Geom_Plane ) ) )
			{
		    err = true;
			}
		}

    for ( TopExp_Explorer eexp( shape, TopAbs_EDGE ); eexp.More() && !err; eexp.Next() )
		{
		  Standard_Real fd, ld;
		  Handle(Geom_Curve) curve = BRep_Tool::Curve( TopoDS::Edge( eexp.Current() ), fd, ld );
	    if ( !curve->IsKind( STANDARD_TYPE( Geom_Line ) ) )
			{
	      err = true;
			}
		}
	}
	return !err;
}


Handle(TopTools_HSequenceOfShape) QoccInputOutput::importCATIA( const QString& file )
{

	Handle(TopTools_HSequenceOfShape) aSequence;
	//aSequence = new TopTools_HSequenceOfShape();

 //   TopoDS_Shape aShape;

 //  char* File_in  = NULL;
 //  TCollection_AsciiString filein( file.toAscii().data() );

 //  File_in  = (char*) filein.ToCString();

 //  //  ===========================  Execution
 //  char* FileType = OCCTopoDSRead_FileType (File_in);
 //  qDebug() <<" Input File Type : " << FileType;
 //  int stat = OCCTopoDSRead_Access(FileType);
 //  if (stat == 0) qDebug()<<" - Not supported";
 //  if (stat <  0) qDebug()<<" - No licence";
 //  qDebug() << "\n";

 //  //DKOC_GetShapeInfo data;
 //  try {
 //  
	//  OCCTopoDS_Read (File_in,aShape);

 //  }
 //  catch (...) {
 //    stat = -10;
 //  }


  
//  aSequence->Append( aShape );

  
  

  return aSequence;
}
