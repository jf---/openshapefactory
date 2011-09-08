/************************************************************************************

Butchered Code By:
Alexander Pena de Leon

Based Completely off of:
-Qtocc Harness by Peter Dolby
-Salome Geom Library

underlining frameworks:
open cascade
qt framework 4.0


***********************************************************************************/


#include <iostream>
#include <fstream>
using namespace std;
#include "qtdebug.hxx"
//#include "fdm_outdlg.h"
#include <QGraphicsScene>
//#include "qocctreewidget.h"
#include <QtCore/QString>
#include <QtCore/QObject>



#include <QtGui/QtGui>
#include <QtGui/QPlastiqueStyle>

#include "qoccapplication.h"
#include "qoccmainwindow.h"

#if defined(Q_WS_WIN)
#include "qtdotnetstyle.h"
#include "QCDEStyle"
#endif

#include <QSplashScreen>

//#include "mantis_blendsurfacenode.h"






 void myMessageOutput(QtMsgType type, const char *msg)
 {
     switch (type) {
     case QtDebugMsg:
         fprintf(stderr, "Debug: %s\n", msg);
         break;
     case QtWarningMsg:
         fprintf(stderr, "Warning: %s\n", msg);
         break;
     case QtCriticalMsg:
         fprintf(stderr, "Critical: %s\n", msg);
         break;
     case QtFatalMsg:
         fprintf(stderr, "Fatal: %s\n", msg);
         abort();
     }
 }

int main(int argc, char *argv[])
{
	
	QoccApplication app( argc, argv );
	

	QSplashScreen *mySplash = new QSplashScreen;
	mySplash->setPixmap(QPixmap(QString::fromUtf8(":/images/style/FMD_SPLASH.png")).scaledToWidth(600));
	mySplash->show();
	Qt::Alignment topright = Qt::AlignRight | Qt::AlignTop;
	mySplash->showMessage(QString::fromUtf8("Loading More CowBell ..."),topright,Qt::white);

#if defined(Q_WS_WIN)
	//QtDotNetStyle* style = new QtDotNetStyle();
	//QCDEStyle* style = new QCDEStyle();
	//app.setStyle(QStyleFactory::create("motif"));
	//cleanlooks,"motif", "cde", "plastique" and "cleanlooks

#endif
	
	qGeomApp->msleep ( 100 );
	QoccMainWindow *window = new QoccMainWindow();
    window->showMaximized();
	mySplash->finish(window);
	delete mySplash;
	
	int retval = app.exec();
	return retval;
}
