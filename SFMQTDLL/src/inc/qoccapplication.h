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



#ifndef QOCCAPPLICATION_H
#define QOCCAPPLICATION_H

#include <QtCore/QObject>
#include <QtGui/QApplication>
#include <QtGui/QPixmap>
#include <QtGui/QSplashScreen>

#include "commonlibraries.h"
#include "qocccontroller.h"


class QoccDocument;
class GraphWidget;

class Q_DECL_EXPORT QoccApplication : public QApplication
{

	Q_OBJECT

public:

  
  QoccApplication( int &argc, char **argv, int = QT_VERSION );
  ~QoccApplication( );

	void splashScreen ( const QPixmap &pixmap = QPixmap() );
	void splashFinish ( QWidget* w, long millisecs );
	void msleep ( unsigned long millisecs );

	occviewport*  myview;
	QoccController* Controller;
	QSplashScreen* mySplash;
	GraphWidget* symboltree;
	

};

#define qGeomApp (dynamic_cast<QoccApplication *>(QCoreApplication::instance()))


#endif // QOCCAPPLICATION_H
