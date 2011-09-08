#ifndef BEAMTEST_H
#define BEAMTEST_H

#include <QWidget>
#include "ui_beamtest.h"
#include <commonlibraries.h>

class beamtest : public QWidget
{
	Q_OBJECT

public:
	beamtest(QWidget *parent = 0);
	~beamtest();

private:
	Ui::beamtest ui;
	Handle_User_AIS aisp;


public slots:
	void setup();
	void update();
	void readfile();
	void readfile1();
	void writebinary();
	void readbinary();
	void writebinarytochunks();

	void readbinarywithcolor();
	void writebinarytochunkswithcolor();


};

#endif // BEAMTEST_H
