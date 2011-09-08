/*****************************************************************************\
Copyright (c) 2008, Nicolas Tessore
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the author nor the names of its contributors may be
      used to endorse or promote products derived from this software without
      specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\*****************************************************************************/

#ifndef METAPOINTER_H_
#define METAPOINTER_H_

/*
  Do not show any of the MetaPointer
  contents to moc
*/
#ifndef Q_MOC_RUN

#include <QObject>
#include <QPointer>
#include <QList>
#include <QMetaMethod>
#include <QMetaObject>

template<class T>
class MetaPointer;

/*
  QObject::connect "overloads" for MetaPointer
*/

template<class T>
bool metaConnect(const MetaPointer<T> &metaSender, const char* signal, const QObject* receiver, const char* method, Qt::ConnectionType type = Qt::AutoCompatConnection)
{
    return QObject::connect(&metaSender, signal, receiver, method, type);
}

template<class T>
bool metaConnect(const QObject* sender, const char* signal, const MetaPointer<T> &metaReceiver, const char* method, Qt::ConnectionType type = Qt::AutoCompatConnection)
{
    return QObject::connect(sender, signal, &metaReceiver, method, type);
}


/*
  MetaPointer class
*/

template<class T>
class MetaPointer : public QPointer<T>, public QObject
{
public:
    MetaPointer()
    {
        setupMetaObject();
    }
    
    MetaPointer(T* ptr)
        : QPointer(p)
    {
        setupMetaObject();
    }
    
    MetaPointer(const MetaPointer<T> &mp)
        : QPointer(mp.p)
    {
        setupMetaObject();
    }
    
    ~MetaPointer()
    {
        if(data())
            foreach(int id, signalIds)
                QMetaObject::connect(data(), id, this, id);
    }
    
    MetaPointer<T> & operator=(const MetaPointer<T>& mp)
    {
        return operator=(mp.data());;
    }
    
    MetaPointer<T> & operator=(T* p)
    {
        if(data())
            foreach(int id, signalIds)
                QMetaObject::disconnect(data(), id, this, id);
        
        QPointer::operator=(p);
        
        if(p)
            foreach(int id, signalIds)
                QMetaObject::connect(p, id, this, id);
        
        return *this;
    }
    
// begin Qt MetaObject forwarding
public:
    void setupMetaObject()
    {
        for(int i = 0; i < T::staticMetaObject.methodCount(); ++i)
        {
            QMetaMethod m = T::staticMetaObject.method(i);
            if(m.methodType() == QMetaMethod::Signal)
                signalIds.append(i);
        }
    }
    
    const QMetaObject* metaObject() const
    {
        return &T::staticMetaObject;
    }

    void* qt_metacast(const char *_clname)
    {
        if(!_clname) return 0;
        if(data()) return data()->qt_metacast(_clname);
        return QObject::qt_metacast(_clname);
    }

    int qt_metacall(QMetaObject::Call _c, int _id, void **_a)
    {
        if(sender() == data() && data())
        {
            QMetaObject::activate(this, _id, _a);
            return -1;
        }
        
        if(_id < T::staticMetaObject.methodCount())
        {
            if(data()) return data()->qt_metacall(_c, _id, _a);
            return _id;
        }
        
        _id -= T::staticMetaObject.methodCount();
        
        return _id;
    }
    
private:
    QList<int> signalIds;
// end Qt MetaObject System Forwarding
};

#endif /*Q_MOC_RUN*/

#endif /*METAPOINTER_H_*/
