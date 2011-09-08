#ifndef GENERICFRAME_H
#define GENERICFRAME_H

#include <QtGui/QToolBar>
#include <QtGui/QMenu>
#include <QtGui/QFrame>

#include <qocccontroller.h>
#include <QoccMainWindow.h>

class GenericFrame : public QFrame
{
	Q_OBJECT

public:
	GenericFrame(QoccController* aContext = NULL, 
				QWidget *mainwindow = NULL, 
				QWidget *centerwidget = NULL,
				Qt::WindowFlags wflags = 0);
	~GenericFrame();

private:
	QoccMainWindow* myMainwindow;
	QoccController*  myController;
	QVBoxLayout*	myLayout;
	
	};

#endif // GENERICFRAME_H
