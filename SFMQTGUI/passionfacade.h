#ifndef PASSIONFACADE_H
#define PASSIONFACADE_H

#include <QWidget>
#include "ui_passionfacade.h"
#include <commonlibraries.h>

class passionfacade : public QWidget
{
	Q_OBJECT

public:
	passionfacade(QWidget *parent = 0);
	~passionfacade();

private:
	Ui::passionfacade ui;
	Handle_User_AIS aisp;
public slots:

	void setup();
	void update();
	void purgememory();
};

#endif // PASSIONFACADE_H
