#include "pythonwidget.h"
#include "shapefactory.h"
#include "parametricsfordummies.h"
#include "bash.h"
#include "PythonQt.h"
#include "PythonQtScriptingConsole.h"

pythonwidget::pythonwidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setup();
}

pythonwidget::~pythonwidget()
{

}
void pythonwidget::setup()
{
//connect(ui.execute,SIGNAL(pressed()),this,SLOT(update()));

}

void pythonwidget::update()
{

INITPART

  
  //ui.consolegrid->addWidget(console,1);
  
  
 

  
  // shows how to call the method with a text that will be append to the browser

ENDPART


}
