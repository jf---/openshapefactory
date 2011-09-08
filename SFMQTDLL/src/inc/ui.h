#pragma once
#include <QoccMainWindow.h>

class ui;

class ui {
  public:
    
    static ui* getInstance();
    ~ui();
	QoccMainWindow *getWindow();
	void showFullScreen();
	QoccController* getWindowController();
	Handle_AIS_InteractiveContext getWindowContext();
	void Statusbar(QString message);
	void ShowMaximized();
	static void init();

  protected:
	ui(); // constructor
	

  private:
	static bool HasInstantiated;
	static ui* self;
	QoccMainWindow *window;
};
