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
#include <QTextBlock>
#include <QAbstractItemModel>
#include <QStringListModel>

#include <QSettings>
#include <QShortcut>

//#include "gradients.h"
 


scriptwidget::scriptwidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	

	/*QVBoxLayout* myLayout = new QVBoxLayout( this );
	myLayout->setMargin(0);
	myLayout->setSpacing(0);
	myLayout->addWidget(ui.tabWidget);*/

	ui.tab_2->setLayout(ui.thetextlayout);
	ui.tab->setLayout(ui.slider_tab_layout);
	this->setLayout(ui.verticalLayout);

	connect(ui.evalbutton, SIGNAL(pressed()), this, SLOT(evaluatetext()));

	// connect widget events to the context wrapper
	/*connect( ui::getInstance()->getWindowController(),       SIGNAL(mouseMoved(occviewport*, QMouseEvent*)), 
		     this,   SLOT  (moveEvent(occviewport*,  QMouseEvent*)) );*/
	// connect widget events to the context wrapper
	connect( appui::getInstance()->getWindowController(),       SIGNAL(clickEvent(occviewport*, QMouseEvent*)), 
		     this,   SLOT  (clickEvent(occviewport*,   QMouseEvent*)) );
	// connect widget events to the context wrapper
	connect( appui::getInstance()->getWindowController(),       SIGNAL(selectionChanged()) , 
		     this,   SLOT  (onSelectionChanged()) );




//connect(ui.text, SIGNAL(pressed()), this, SLOT(seteditor()));
	
	myeditor = new QScriptEdit(0); // addtexteditor found it in QT debugger
	////myeditor->show();
	////myeditor->setFont(QFont("Arial",14));

	//connect( this->ui.interactivecheck,   SIGNAL(stateChanged ( int ) ) , 
	//	     this,   SLOT  (makeinteractive_text( bool )) );



    //ui.thetextlayout->addWidget(myeditor);
	this->seteditor();

	 QString folder = QCoreApplication::applicationDirPath();
	/* QString codefilename = folder + QString("/openshapefactory_syntax.osf");

	

	 QCompleter* completer = new QCompleter(myeditor);
     completer->setModel(modelFromFile(codefilename,completer));
	 completer->setModelSorting(QCompleter::UnsortedModel);
	 completer->setCaseSensitivity(Qt::CaseInsensitive);
     completer->setWrapAround(false);
	 myeditor->setCompleter(completer);
	 */



	

	//seteditor();

	readcodefile();

	hsfapi = new HsfScriptingInterface() ;
	hsfapi->setparentwidget(this);
	//myengine.globalObject().setProperty("hsf",myengine.newQObject(hsfapi));
	
	myengine.importExtension("qt.core");
	myengine.importExtension("qt.gui");
	QScriptValue myglobal =  myengine.newQObject(hsfapi);
	myglobal.setPrototype(myengine.globalObject());
	myengine.setGlobalObject(myglobal);


	//myengine.globalObject().setProperty("hsf", myhsfval); // this another way of adding functions using the function signature

	QScriptValue frontend = myengine.newQObject(ui.tab);
	myengine.globalObject().setProperty("gui", frontend);
	
	
	
	int childcount = ui.sliderset->count();
	for(int i=0;i<childcount;i++){
		QSlider* widgetitem = qobject_cast<QSlider*>(ui.sliderset->itemAt(i)->widget());
		//QLabel* widgetitem2 = qobject_cast<QLabel*>(ui.labelset->itemAt(i)->widget());
		
		if(widgetitem){
			QString objname = widgetitem->objectName();
			QString obj2name = objname + QString("txt");

			connect(widgetitem,SIGNAL(sliderMoved(int)),this,SLOT(evaluatetext()));
			frontend.setProperty(objname,myengine.newQObject(widgetitem));
			//frontend.setProperty(obj2name,myengine.newQObject(widgetitem2));
			
		}
	}

	//ShadeWidget* myshade = new ShadeWidget(ShadeWidget::RedShade, this);

	
	myuploader = new uploader(appui::getInstance()->getWindow(),this);
	myuploader->setscriptwidget(this);
	

	

}




void scriptwidget::toggleinteractivity()
{


}

void scriptwidget::makeinteractive_text(bool value)
{
if (value)
{
	connect(myeditor, SIGNAL(textChanged()), this, SLOT(evaluatetext())); //make code interactive
} else {

	disconnect(myeditor, SIGNAL(textChanged()), this, SLOT(evaluatetext()));

}


}

void scriptwidget::makeinteractive_mouse(bool value)
{

}


void scriptwidget::on3dSelectionChanged()
{
evaluatetext();
}

void scriptwidget::moveEvent( occviewport* widget, QMouseEvent* e  )
{
	hsfapi->setmousepos(widget->getPoint());
	evaluatetext();
}



void scriptwidget::clickEvent( occviewport* widget, QMouseEvent* e )
{
	hsfapi->setmousepos(widget->getPoint());
	evaluatetext();
	QString posmsg("widgetpos:" + e->pos().x() + tr(",") + e->pos().y());
	hsfapi->print(QScriptValue(posmsg));

}


QAbstractItemModel* scriptwidget::modelFromFile(const QString& fileName,QCompleter* completer)
 {
     QFile file(fileName);
     if (!file.open(QFile::ReadOnly))
         return new QStringListModel(completer);


     QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

     QStringList words;

     while (!file.atEnd()) {
         QByteArray line = file.readLine();
         if (!line.isEmpty())
             words << line.trimmed();
     }


     QApplication::restoreOverrideCursor();
 
     return new QStringListModel(words, completer);
 }

void scriptwidget::seteditor()
{
	textEdit = new QsciScintilla();

	
	

	QShortcut* shortcut_ctrl_space = new QShortcut(QKeySequence("Ctrl+Space"),textEdit);
    connect(shortcut_ctrl_space, SIGNAL(activated()), textEdit,SLOT(autoCompleteFromAll()));


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
	
	
	jscript->setColor(QColor("#BDAF9D"),QsciLexerCPP::Default);
	jscript->setPaper(QColor("#2A211C"),QsciLexerCPP::Default);

	jscript->setDefaultColor(QColor("#BDAF9D"));
	jscript->setDefaultPaper(QColor("#2A211C"));

	jscript->setColor(QColor("#FF3A83"),QsciLexerCPP::Number);
	jscript->setPaper(QColor("#2A211C"),QsciLexerCPP::Number);

	jscript->setColor(QColor("#37A3ED"),QsciLexerCPP::Keyword);
	jscript->setPaper(QColor("#2A211C"),QsciLexerCPP::Keyword);

	jscript->setColor(QColor("#BDAE9D"),QsciLexerCPP::Identifier);
	jscript->setPaper(QColor("#2A211C"),QsciLexerCPP::Identifier);

	jscript->setColor(QColor("#00FF40"),QsciLexerCPP::DoubleQuotedString);
	jscript->setPaper(QColor("#2A211C"),QsciLexerCPP::DoubleQuotedString);

	jscript->setColor(QColor("#80FF00"),QsciLexerCPP::SingleQuotedString);
	jscript->setPaper(QColor("#2A211C"),QsciLexerCPP::SingleQuotedString);

	jscript->setColor(QColor("#666666"),QsciLexerCPP::Comment);
	jscript->setPaper(QColor("#2A211C"),QsciLexerCPP::Comment);

	jscript->setColor(QColor("#666666"),QsciLexerCPP::CommentLine);
	jscript->setPaper(QColor("#2A211C"),QsciLexerCPP::CommentLine);

	jscript->setColor(QColor("#FFFF80"),QsciLexerCPP::CommentDoc);
	jscript->setPaper(QColor("#2A211C"),QsciLexerCPP::CommentDoc);

	jscript->setColor(QColor("#E5C138"),QsciLexerCPP::Operator);
	jscript->setPaper(QColor("#2A211C"),QsciLexerCPP::Operator);

	textEdit->setMarginsForegroundColor(QColor("#E5C138"));
	textEdit->setMarginsBackgroundColor(QColor("#2A211C"));

	textEdit->setAutoFillBackground(true);
	textEdit->setCaretLineVisible(true);
	textEdit->setCaretWidth(5);
	textEdit->setCaretForegroundColor(QColor("#E5C138"));
	textEdit->setCaretLineBackgroundColor(QColor("#2A211C"));

	textEdit->setFoldMarginColors(QColor("#E5C138"),QColor("#2A211C"));


	


	
	
	//jscript->setfon

	//jscript->setColor(QColor("FFAA00"));
	//jscript->setPaper(QColor("0B161D"));
	
	textEdit->setMarginWidth(0, fm.width( "0000" ));
    textEdit->setMarginLineNumbers(0, true);

	



	textEdit->setEdgeMode(QsciScintilla::EdgeLine);
    textEdit->setEdgeColumn(0);
	textEdit->setEdgeColor(QColor("green"));
	
	

	//QSettings mysets("H:\\mysettings","openshapefactory");
	//jscript->writeSettings(mysets);
	
	
	

	//QString folder = QCoreApplication::applicationDirPath();
	//QString themefile = folder + QString("//themes//Bespin.xml");

	
	
	textEdit->setLexer(jscript);
	textEdit->setFolding(QsciScintilla::FoldStyle::BoxedTreeFoldStyle,2);
	textEdit->setIndentationGuides(true);

	textEdit->setAutoCompletionSource(QsciScintilla::AutoCompletionSource::AcsAll);
	textEdit->autoCompleteFromDocument();
	
	textEdit->setAutoIndent(true);
	textEdit->setBraceMatching(QsciScintilla::BraceMatch::SloppyBraceMatch);
	
	textEdit->setCallTipsStyle(QsciScintilla::CallTipsStyle::CallTipsContext);
	
	textEdit->show();
	textEdit->setAutoCompletionShowSingle(false);

	textEdit->autoCompleteFromAll();
	textEdit->autoCompletionFillupsEnabled();
	textEdit->autoCompletionReplaceWord();
	textEdit->annotationDisplay();
	

	textEdit->zoomIn(2);
	ui.thetextlayout->addWidget(textEdit);

	
    
	textEdit->setCallTipsStyle(QsciScintilla::CallTipsContext);
    //connect(textEdit, SIGNAL(cursorPositionChanged(int,int)), textEdit, SLOT(autoCompleteFromDocument())); //make code interactive



	


	//connect(textEdit, SIGNAL(textChanged()), this, SLOT(evaluatetext())); //make code interactive

	
	
}
scriptwidget::~scriptwidget()
{

}


QString scriptwidget::gettextbyline (int linenumber)
{
	if (myeditor->isVisible())
	{
	QString lineat = myeditor->document()->findBlockByLineNumber(linenumber).text();
	return lineat;
	} else if (textEdit->isVisible())
	{
	QString lineat  = textEdit->text(linenumber-1);
	//qDebug() << "gettextline:" << lineat;
	return lineat;
	}

}
QString scriptwidget::gettext()
{
if (myeditor->isVisible())
{
	return myeditor->toPlainText();
} else if (textEdit)
{
	return textEdit->text();
}

}

void scriptwidget::evaluatetext()
{

QTime time;
time.start();

savecode();

QString linenumbers;
QString text = gettext();

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
	 myengine.clearExceptions();
	 myengine.collectGarbage();
	 
 } else
 {
	 ui.listener->addItem(result.toString());
	 
	 double milliseconds = time.elapsed();
     double seconds = (milliseconds/1000);
     double minutes = seconds/60;
     double hours = minutes/60;

	 QString timetxt;
     QTextStream linestream(&timetxt);
     linestream << "h" << hours << ":m " << minutes << ":s " << seconds << ".ms" << milliseconds; 
		
	 appui::getInstance()->getWindow()->statusBar()->showMessage(timetxt);

	 //ui.listener->addItem(result.toString() + timetxt);

	 

 }


 ui.listener->scrollToBottom();


}





void scriptwidget::evaluatetextonly(QString text)
{


savecode();

QString linenumbers;


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
	 resultstream = QString("Oops!");
 }


 if (myengine.hasUncaughtException()) {
     int line = myengine.uncaughtExceptionLineNumber();
	 resultstream = QString("Oops!:")+ QString::number(line) + QString(" ") + result.toString();

	 ui.listener->addItem(resultstream);
	 myengine.clearExceptions();
	 myengine.collectGarbage();

	 qDebug() << resultstream << "\n";
	 
 } else
 {
	resultstream = result.toString();
	 ui.listener->addItem(resultstream);
	 qDebug() << resultstream << "\n";
 }


 ui.listener->scrollToBottom();


}




void scriptwidget::savecode()
{

	QString folder = QCoreApplication::applicationDirPath();
	
	QString codefilename = folder + QString("/code.osf");

	QFile data(codefilename);
	if (data.open(QFile::ReadWrite)) {
     QTextStream out(&data);
     out << gettext();
	}
	data.close();
	
}

void scriptwidget::settext(QString textval)
{
	if (myeditor->isVisible())
{
	myeditor->setPlainText(textval);
} else if (textEdit)
{
	return textEdit->setText(textval);
}
}

void scriptwidget::readcodefile()
{

	QString folder = QCoreApplication::applicationDirPath();
	QString codefilename = folder + QString("/code.osf");

	QFile data(codefilename);
	if (data.open(QFile::ReadOnly)) {
     QTextStream out(&data);
	 settext(out.readAll());
	}
	data.close();
	
}