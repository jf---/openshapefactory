#include "cowbellprogress.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>

cowbellprogress::cowbellprogress(QWidget *parent,double x,double y)
	: QWidget(parent)
{

//this->show();
//this->setGeometry(x,y,200,200);
QHBoxLayout* myLayout = new QHBoxLayout( this );
myLayout->setObjectName(QString::fromUtf8("VboxLayout"));
myLayout->setMargin(0);
myLayout->setSpacing(0);
myLayout->setAlignment(Qt::AlignHCenter);

QVBoxLayout* mySubLayout = new QVBoxLayout( this );
mySubLayout->setObjectName(QString::fromUtf8("HboxLayout"));
mySubLayout->setMargin(0);
mySubLayout->setSpacing(0);

QLabel* label = new QLabel();
mySubLayout->addWidget( label);
label->show();

QLabel* label2 = new QLabel(QString("Loading More CowBell ..."));
mySubLayout->addWidget( label2);
label2->show();

myLayout->insertLayout(1,mySubLayout);

QMovie *movie = new QMovie("H:/DEV/FDM/FDM_GEOM/Debug/animations/morecowbell.gif");

//label->setMovie(movie);
//movie->start();


this->setStyleSheet(QString::fromUtf8("background-color: rgba(0, 0, 0,120);color: rgb(255, 255, 255);"));
this->setAttribute(Qt::WA_TranslucentBackground);
this->setWindowFlags(Qt::FramelessWindowHint);
//this->showFullScreen();

QTimer* mytimer = new QTimer();
connect(mytimer,SIGNAL(timeout()),this, SLOT(close()));

//this->showFullScreen();
mytimer->start(800);

//this->adjustSize();
//this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());
this->move(x,y);



}

cowbellprogress::~cowbellprogress()
{

}


