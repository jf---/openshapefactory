
#ifndef QOCCDOCUMENT_H
#define QOCCDOCUMENT_H

#include <QtCore/QObject>



class Q_DECL_EXPORT QoccDocument : public QObject
{

	Q_OBJECT

public:

	QoccDocument( int anID = 0);
	~QoccDocument( );

	virtual int id() { return myID; }

	
private:

	int myID;

};

#endif // QOCCDOCUMENT_H
