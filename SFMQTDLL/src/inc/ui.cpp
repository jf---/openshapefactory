#include "ui.h"
class appui;

bool appui::HasInstantiated = false;
appui* appui::self = 0;


appui* appui::getInstance(){
  if(appui::HasInstantiated == false){
	  appui::self = new appui();
	  appui::HasInstantiated = true;
  }
  return appui::self;
}




appui::appui(){ // constructor
	window = new QoccMainWindow();
}

appui::~appui(){ // constructor
  
}

void appui::init()
	{
	appui::self = appui::getInstance();
	}
QoccMainWindow *appui::getWindow()
{
	//window = new QoccMainWindow();
	return window; //new QoccMainWindow();
}

QoccController* appui::getWindowController()
{
	return window->myController;
}

Handle_AIS_InteractiveContext appui::getWindowContext()
{
	return window->myController->getContext();
}


void appui::Statusbar(QString message)
{
	window->statusBar()->showMessage(message);
}

void appui::showFullScreen()
{
	window->showFullScreen();
	window->myController->update();
}


void appui::ShowMaximized()
{
	window->show();
	window->myController->update();
}

