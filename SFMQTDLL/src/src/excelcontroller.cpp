#include "excelcontroller.h"

excelcontroller::excelcontroller(QObject *parent)
	: QObject(parent)
{
// Create a Excel.Application
    excel = new QAxObject( "Excel.Application", 0 );
// set Visible On
    excel->setProperty("Visible", true);

	this->AddSheet();
	
}

excelcontroller::~excelcontroller()
{

}

void excelcontroller::Open(QString FileName)
{
// Retrieve excel workbooks
    QAxObject * workbooks = excel->querySubObject( "Workbooks" );
	// Add 3 worksheets to a new workbook
    QAxObject * activeWorkbook=  workbooks->querySubObject("Open(QVariant)",FileName );
// Add 3 worksheets to a new workbook
   activesheet = excel->querySubObject( "ActiveSheet" );
   ActiveSheetName = activesheet->property("Name").toString();
}



void excelcontroller::AddSheet()
{

// Retrieve excel workbooks
    QAxObject * workbooks = excel->querySubObject( "Workbooks" );
// Add 3 worksheets to a new workbook
    QAxObject * activeWorkbook= workbooks->querySubObject("Add()");
	QString sheetname = activeWorkbook->property("Name").toString();
	activesheet = activeWorkbook->querySubObject( "ActiveSheet" );
// get worksheets collection
    QAxObject * sheets =  activeWorkbook->querySubObject("Sheets");
// count worksheets -> 3
    int sheetsCount = sheets->property("Count").toInt();
// get the last worksheet in the workbook
QAxObject * lastSheet =  sheets->querySubObject("Item(QVariant)",sheetsCount );



}

void excelcontroller::setcell(int row, int column, QString txtval)
{
//activesheet = excel->querySubObject( "ActiveSheet" );
//QString sheetname = activeWorkbook->property("Name").toString();
QAxObject * mycell =  activesheet->querySubObject("Cells(int,int)",row,column );
mycell->setProperty("Value",QVariant(txtval));
	
}


QString excelcontroller::getcell(int row, int column)
{
	
	QAxObject * mycell =  activesheet->querySubObject("Cells(QVariant,QVariant)",row,column );
	//QAxObject * mycell =  activesheet->property("Value").toString();
	QString value = mycell->property("Value2").toString();
	return value;
}
