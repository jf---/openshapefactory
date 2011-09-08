#ifndef MYSLIDERFORM_H
#define MYSLIDERFORM_H

#include <QWidget>
//#include "ui_mysliderform.h"

class mysliderform : public QWidget
{
	Q_OBJECT

public:
	mysliderform(QWidget *parent = 0);
	~mysliderform();

private:
	Ui::mysliderform ui;
};

#endif // MYSLIDERFORM_H
