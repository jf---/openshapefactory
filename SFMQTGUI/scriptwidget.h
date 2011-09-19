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
class QAbstractItemModel;

class scriptwidget : public QWidget
{
	Q_OBJECT

public:
	scriptwidget(QWidget *parent = 0);
	~scriptwidget();
	//static QScriptValue visshape(QScriptContext *context, QScriptEngine *engine);
	
	
	Ui::scriptwidget ui;
	QsciScintilla *textEdit;
	QString gettext();
	QString gettextbyline(int linenumber);
	void settext(QString textval);
	QAbstractItemModel* modelFromFile(const QString& fileName,QCompleter* completer);


private:
	
	QScriptEngine myengine;
	QScriptEdit *myeditor;

	HsfScriptingInterface* hsfapi;

	
	//QScriptEngineDebugger *debugger;
	
	
    

	

public slots:

	void evaluatetext();
	void savecode();
	void readcodefile();
	void seteditor();
	void on3dSelectionChanged();

	void moveEvent( occviewport* widget, QMouseEvent* e  );
	void clickEvent( occviewport* widget, QMouseEvent* e );
	

	

	

};

#endif // SCRIPTWIDGET_H
