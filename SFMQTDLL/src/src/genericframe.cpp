#include "genericframe.h"
#include <QtGui/QVBoxLayout>

GenericFrame::GenericFrame(QoccController* aController, 
						QWidget *mainwindow,
						QWidget *centerwidget,
						Qt::WindowFlags wflags ) : 
QFrame( mainwindow , wflags )
{
myController = aController;
myMainwindow = static_cast<QoccMainWindow*> (mainwindow);

myLayout = new QVBoxLayout( this );
myLayout->setObjectName(QString::fromUtf8("VboxLayout"));
myLayout->setMargin(0);
myLayout->setSpacing(0);
myLayout->addWidget( centerwidget);

}

GenericFrame::~GenericFrame()
{
delete myLayout;
}

