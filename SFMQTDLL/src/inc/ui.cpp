#include "ui.h"
class ui;

bool ui::HasInstantiated = false;
ui* ui::self = 0;


ui* ui::getInstance(){
  if(ui::HasInstantiated == false){
	  ui::self = new ui();
	  ui::HasInstantiated = true;
  }
  return ui::self;
}




ui::ui(){ // constructor
	window = new QoccMainWindow();
}

ui::~ui(){ // constructor
  
}

void ui::init()
	{
	ui::self = ui::getInstance();
	}
QoccMainWindow *ui::getWindow()
{
	//window = new QoccMainWindow();
	return window; //new QoccMainWindow();
}

QoccController* ui::getWindowController()
{
	return window->myController;
}

Handle_AIS_InteractiveContext ui::getWindowContext()
{
	return window->myController->getContext();
}


void ui::Statusbar(QString message)
{
	window->statusBar()->showMessage(message);
}

void ui::showFullScreen()
{
	window->showFullScreen();
	window->myController->update();
}


void ui::ShowMaximized()
{
	window->showMaximized();
	window->myController->update();
}

