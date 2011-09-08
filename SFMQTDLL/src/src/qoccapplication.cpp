
#include <QtCore/QDataStream>
#include <QtCore/QEvent>
#include <QtGui/QCursor>
#include <QtCore/QWaitCondition>
#include <QtCore/QMutex>
#include <QtCore/QThread>
#include <QString>
#include <QMessageBox>

#include "qoccdocument.h"
#include "qoccinternal.h"
#include "qoccapplication.h"
#include <QSplashScreen>

QoccApplication::QoccApplication(int &argc, char **argv, int _internal ) :
	QApplication (argc, argv, _internal),
	mySplash (NULL)
	{
// No longer needed for final Qt 4.4. version, but left in just in case!
#if (QT_VERSION >= QT_VERSION_CHECK(4, 4, 0))
	// setAttribute(Qt::AA_NativeWindows);
#endif

	UnitsAPI::SetLocalSystem(UnitsAPI_SystemUnits::UnitsAPI_SI);

}

QoccApplication::~QoccApplication()
{
	if ( mySplash )	{ delete mySplash; }
	
}




void QoccApplication::splashScreen ( const QPixmap &pixmap )
{
	QPixmap* p = (QPixmap*) &pixmap;
	if ( p->isNull() )
	{
		p = new QPixmap( ":/images/images/QoccSplash.png" );
	}
	mySplash = new QSplashScreen;

	if ( mySplash )
	{
		mySplash->setPixmap(QPixmap(":/images/images/QoccSplash.png"));
		mySplash->show();
		
	}
}



void QoccApplication::splashFinish ( QWidget* widget, long millisecs )
{
	if ( mySplash )
	{
		msleep ( millisecs );
		mySplash->finish( widget );
		delete mySplash;
		mySplash = NULL;
	}
}

void QoccApplication::msleep(unsigned long millisecs)
{
	QMutex mutex;
	QWaitCondition waitCondition;
	mutex.lock();
	waitCondition.wait( &mutex, millisecs );
	mutex.unlock();
}



