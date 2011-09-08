//#include "parametricsfordummies.h"
//#include "ui_zha_jcm.h"
//DECLAREUIWIDGETHEADER(classname)



#ifndef ZHA_JCM_H
#define ZHA_JCM_H

#include <QWidget>
#include <QMap>
#include "ui_zha_jcm.h"
#include <commonlibraries.h>
#include <QSet>


class ZHA_JCM : public QWidget
{
	Q_OBJECT

public:
	ZHA_JCM(QWidget *parent = 0);
	~ZHA_JCM();

private:
	Ui::ZHA_JCM ui;
	Handle_User_AIS aisp;
public slots:

	void setup();
	void update();
	void update2();
	void purgememory();

};


#endif // ZHA_JCM_H
