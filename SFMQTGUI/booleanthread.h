#pragma once


#include <QThread>
#include <TopoDS_Shape.hxx>



class booleanthread : public QThread
	{
	Q_OBJECT
public:
		booleanthread(QObject *parent = 0,TopoDS_Shape& sh1 = TopoDS_Shape(),TopoDS_Shape& sh2 = TopoDS_Shape())
     : QThread(parent),theshape1(sh1),theshape2(sh2)
	{
		int nullcount = 0;
		if(sh1.IsNull()) nullcount++;
		if(sh2.IsNull()) nullcount++;
		
		theshape1 = sh1;
		theshape2 = sh2;
		
		if(theshape1.IsNull()) nullcount++;
		if(theshape2.IsNull()) nullcount++;
		

		inputs << sh1 << sh2;


	};
		~booleanthread(void);
	
	
	

 signals:
    void finishedshape(const TopoDS_Shape& newval);

	protected:
		void run();

	private:
		
	TopoDS_Shape&  theshape1;
	TopoDS_Shape&  theshape2;
	QList<TopoDS_Shape> inputs;

	};


