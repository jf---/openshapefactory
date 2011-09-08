#ifndef SCRIPTWIDGET_H
#define SCRIPTWIDGET_H

#include <QWidget>
#include "ui_scriptwidget.h"
#include <QScriptEngine>
#include <qscriptedit_p.h>
#include <QScriptEngineDebugger.h>

#include "HsfScriptingInterface.h"
//#include <Qsci/qsciscintilla.h>


//using namespace TextEditor;
class QsciScintilla;

class scriptwidget : public QWidget
{
	Q_OBJECT

public:
	scriptwidget(QWidget *parent = 0);
	~scriptwidget();
	//static QScriptValue visshape(QScriptContext *context, QScriptEngine *engine);
	
	
	
private:
	Ui::scriptwidget ui;
	QScriptEngine myengine;
	QScriptEdit *myeditor;
	QsciScintilla *textEdit;
	//QScriptEngineDebugger *debugger;
	
	
    

	

public slots:

	void evaluatetext();
	void savecode();
	void readcodefile();
	void seteditor();
	

};

#endif // SCRIPTWIDGET_H
