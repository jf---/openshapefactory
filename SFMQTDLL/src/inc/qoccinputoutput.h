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
#ifndef QOCCINPUTOUTPUT_H
#define QOCCINPUTOUTPUT_H

#include <QtCore/QObject>
//#include "qocc.h"
#include "commonlibraries.h"

//#include <DKOC_GetShapeInfo.h>
//#include "OCCTopoDSRead.h"



//#include <User_AIS.hxx>

class Q_DECL_EXPORT QoccInputOutput : public QObject
{
//	Q_OBJECT

public:

    enum FileFormat{	FormatBREP, 
						FormatIGES, 
						FormatSTEP, 
						FormatCSFDB, 
						FormatVRML, 
						FormatSTL,
						FormatCATIA};

	QoccInputOutput(void);
	~QoccInputOutput(void);

    bool importModel( const QString& fileName, 
					  const FileFormat format, 
					  const Handle(AIS_InteractiveContext)& ic );

    bool exportModel( const QString& fileName, 
					  const FileFormat format, 
					  const Handle(AIS_InteractiveContext)& ic);

	bool importMesh( const QString& fileName,
					 const FileFormat format,
					 const Handle(AIS_InteractiveContext)& ic );

	Handle(TopTools_HSequenceOfShape) importBREP ( const QString& );

	Handle(TopTools_HSequenceOfShape) importIGES ( const QString& );
	Handle(TopTools_HSequenceOfShape) importIGESprogress ( const QString& );
	
	Handle(TopTools_HSequenceOfShape) importSTEP ( const QString& );
	Handle(TopTools_HSequenceOfShape) importCSFDB( const QString& );
	Handle(TopTools_HSequenceOfShape) importCATIA( const QString& );


    QString info() const;
	Handle(TopTools_HSequenceOfShape) getShapes( const Handle_AIS_InteractiveContext& oc);
	bool                               exportModel( const FileFormat format, 
												    const QString&,
                                                    const Handle_TopTools_HSequenceOfShape& );
	bool exportBREP ( const QString& fileName, const Handle(TopTools_HSequenceOfShape)& shapes );
	bool exportIGES ( const QString& fileName, const Handle(TopTools_HSequenceOfShape)& shapes );
	bool exportSTEP ( const QString& fileName, const Handle(TopTools_HSequenceOfShape)& shapes );
	bool exportCSFDB( const QString& fileName, const Handle(TopTools_HSequenceOfShape)& shapes );
	bool exportSTL  ( const QString& fileName, const Handle(TopTools_HSequenceOfShape)& shapes );
	bool exportVRML ( const QString& fileName, const Handle(TopTools_HSequenceOfShape)& shapes );


signals:

	void error (int errorCode, QString& errorDescription);

private:

    Handle(TopTools_HSequenceOfShape)  importModel( const FileFormat format, 
									    		    const QString& fileName );

    
	
    
	Handle(TopTools_HSequenceOfShape) getvShapes( const Handle_AIS_InteractiveContext& oc);

	

	

    bool checkFacetedBrep( const Handle(TopTools_HSequenceOfShape)& );

	// Attributes
	
	QString myInfo;

};

#endif

