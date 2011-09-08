#ifndef SGEXP001_H
#define SGEXP001_H

#include <QWidget>
#include "ui_sgexp001.h"
#include <QWidget>
#include <QMap>
#include <commonlibraries.h>
#include <QSet>


class sgexp001 : public QWidget
{
	Q_OBJECT

public:
	sgexp001(QWidget *parent = 0);
	~sgexp001();

private:
	Ui::sgexp001 ui;
	Handle_User_AIS aisp;
public slots:

	void setup();
	void update();
	void purgememory();
};

#endif // SGEXP001_H
