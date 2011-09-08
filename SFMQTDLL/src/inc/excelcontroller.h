#ifndef EXCELCONTROLLER_H
#define EXCELCONTROLLER_H

#include <QObject>
#include <QAxObject>

class excelcontroller : public QObject
{
	Q_OBJECT

public:
	excelcontroller(QObject *parent = 0);
	void AddSheet();
	void Open(QString FileName);
	void setcell(int row, int column, QString txtval);
	QString getcell(int row, int column);
	QString ActiveSheetName;

	~excelcontroller();
	QAxObject * excel;
	QAxObject * activesheet;

private:
	
};

#endif // EXCELCONTROLLER_H
