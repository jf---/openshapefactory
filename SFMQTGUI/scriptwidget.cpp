#include "scriptwidget.h"

#include "shapefactory.h"
#include "ui.h"
#include "User_AIS.hxx"

#include "SGMGUI_COMMON.h"
# include <QHBoxLayout>
#include "AIS_Gauss.hxx" 

#include "parametricsfordummies.h"
#include <QScriptable>

#include <QFile>
#include <QTextStream>

#include <Qsci/qsciscintilla.h>
#include "Qsci/qscilexerjavascript.h"
#include <qsciapis.h>
#include <QFont>
#include <QFontMetrics>

 

scriptwidget::scriptwidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	
	connect(ui.evalbutton, SIGNAL(pressed()), this, SLOT(evaluatetext()));
//connect(ui.text, SIGNAL(pressed()), this, SLOT(seteditor()));
	
	//myeditor = new QScriptEdit(0); // addtexteditor found it in QT debugger
	//myeditor->show();
//	ui.vertlayout->addWidget(myeditor);
	

	

	seteditor();

	HsfScriptingInterface* hsfapi = new HsfScriptingInterface() ;
	//myengine.globalObject().setProperty("hsf",myengine.newQObject(hsfapi));

	myengine.setGlobalObject(myengine.newQObject(hsfapi));

	//myengine.globalObject().setProperty("hsf", myhsfval); // this another way of adding functions using the function signature

	QScriptValue frontend = myengine.newQObject(ui.tab);
	myengine.globalObject().setProperty("gui", frontend);
	
	
	
	int childcount = ui.sliderset->count();
	for(int i=0;i<childcount;i++){
		QSlider* widgetitem = qobject_cast<QSlider*>(ui.sliderset->itemAt(i)->widget());
		
		if(widgetitem){
			QString objname = widgetitem->objectName();
			connect(widgetitem,SIGNAL(sliderMoved(int)),this,SLOT(evaluatetext()));
			
			frontend.setProperty(objname,myengine.newQObject(widgetitem));
		}
	}


	

	

}

void scriptwidget::seteditor()
{
	textEdit = new QsciScintilla();
	

	//textEdit->autoCompleteFromAll();

	QsciLexerJavaScript*  jscript = new QsciLexerJavaScript(textEdit);

 /*   QsciAPIs* api = new QsciAPIs(jscript);
    api->add("hsf.makeline(p1,p2)");
    api->add("hsf.makepoint(x,y,z)");
    api->add("hsf.addshapelist(list,object)");
    api->prepare();*/
 
	//textEdit->setAutoCompletionThreshold(1);


	QFont font;
    font.setFamily("arial");
    font.setFixedPitch(true);
    font.setPointSize(12);
	//font.setStretch(20);
	//font.setBold(true);
	
	font.setWeight(300);
	font.setStyleStrategy(QFont::StyleStrategy::PreferQuality);

	
	QFontMetrics fm = QFontMetrics(font);

	jscript->setFont(font);
	
	textEdit->setMarginWidth(0, fm.width( "0000" ));
    textEdit->setMarginLineNumbers(0, true);

	textEdit->setEdgeMode(QsciScintilla::EdgeLine);
    textEdit->setEdgeColumn(0);
    textEdit->setEdgeColor(QColor("#FF0000"));

	
	
	
	textEdit->setLexer(jscript);
	textEdit->setFolding(QsciScintilla::FoldStyle::BoxedTreeFoldStyle);

	//textEdit->setAutoCompletionSource(QsciScintilla::AutoCompletionSource::AcsAll);
	textEdit->setAutoIndent(true);
	textEdit->setBraceMatching(QsciScintilla::BraceMatch::SloppyBraceMatch);
	
	textEdit->setCallTipsStyle(QsciScintilla::CallTipsStyle::CallTipsContext);
	
	textEdit->show();
	textEdit->zoomIn(2);
	ui.vertlayout->addWidget(textEdit);


	connect(textEdit, SIGNAL(textChanged()), this, SLOT(evaluatetext())); //make code interactive

	
	readcodefile();
}
scriptwidget::~scriptwidget()
{

}

void scriptwidget::evaluatetext()
{


savecode();

QString linenumbers;
QString text = textEdit->text();

QString newtext = QString("initpart()\n") + text + QString("\n endpart()");
text = newtext;

//debugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
QScriptValue result;
 try
 {
  result = myengine.evaluate(text);
 }
 catch(...)
 {
	 ui.listener->addItem(QString("Oops!"));
 }


 if (myengine.hasUncaughtException()) {
     int line = myengine.uncaughtExceptionLineNumber();
	 ui.listener->addItem( QString("Oops!:")+ QString::number(line) + QString(" ") + result.toString());
 } else
 {
	 ui.listener->addItem(result.toString());
 }


 ui.listener->scrollToBottom();


}




void scriptwidget::savecode()
{

	QString folder = QCoreApplication::applicationDirPath();
	
	QString codefilename = folder + QString("/code.3dsketchpad");

	QFile data(codefilename);
	if (data.open(QFile::ReadWrite)) {
     QTextStream out(&data);
     out << textEdit->text();
	}
	data.close();
	
}

void scriptwidget::readcodefile()
{

	QString folder = QCoreApplication::applicationDirPath();
	QString codefilename = folder + QString("/code.3dsketchpad");

	QFile data(codefilename);
	if (data.open(QFile::ReadOnly)) {
     QTextStream out(&data);
	 textEdit->setText(out.readAll());
	}
	data.close();
	
}