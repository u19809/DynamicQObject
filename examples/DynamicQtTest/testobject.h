#ifndef TESTOBJECT_H
#define TESTOBJECT_H

#include <QObject>

class TestObject : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY( QString Txt READ txt WRITE setTxt NOTIFY textChanged )
    Q_ENUMS( TestFlags )
    
public:
    
    explicit TestObject(QObject *parent = 0);
    
    enum TestFlags {
        AAAA,
        BBBB,
        CCCC
    } ;
    
    QString txt() {
        return Txt;
    }
    
    Q_INVOKABLE void someMethod( const QString & T ) {
        setTxt( T );
    }
    
signals:
    
    void textChanged( const QString & Txt );
    
public slots:
    
    void setTxt( const QString & T ) {
        Txt = T;
        emit textChanged( Txt );
    }
    
private :
    
    QString Txt;
    
};

#endif // TESTOBJECT_H
