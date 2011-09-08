#ifndef UI_H
#define UI_H

#include <QWidget>
#include "ui_ui.h"

class ui : public QWidget
{
	Q_OBJECT

public:
	ui(QWidget *parent = 0);
	~ui();

private:
	Ui::uiClass ui;
};

#endif // UI_H
