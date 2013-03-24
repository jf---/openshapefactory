#pragma once
#include <QoccMainWindow.h>

class appui;
class QoccMainWindow;



class appui {
  public:
	  

    static appui* getInstance();

    ~appui();

	QoccMainWindow *getWindow();
	void showFullScreen();
	QoccController* getWindowController();
	Handle_AIS_InteractiveContext getWindowContext();
	void Statusbar(QString message);
	void ShowMaximized();
	static void init();

  protected:
	appui(); // constructor
	

  private:
	static bool HasInstantiated;
	static appui* self;
	QoccMainWindow *window;
};
