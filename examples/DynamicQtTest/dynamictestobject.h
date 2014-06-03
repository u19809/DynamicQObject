#ifndef DYNAMICTESTOBJECT_H
#define DYNAMICTESTOBJECT_H

#include <dynamicqobject.h>

class DynamicTestObject : public DynamicQObject {
public:
    DynamicTestObject( QObject * parent = 0 );
    
    QString txt();
    void someMethod( const QString & T );
    void setTxt( const QString & T );
    void textChanged( const QString & Txt );
        
private :
        
    QString Txt;
    
protected :

    const char * className() const {
        return "DynamicTestObject";
    }
    void * doQtCast();
    bool doQtPropertyCall(QMetaObject::Call _c, int _id, void ** _a ) ;
    void doQtMethodCall(QMetaObject::Call _c, int _id, void ** _a ) ;
    void doInitializePrivateMetaObject( DynamicMetaObject& MO ) const ;
    
};

#endif // DYNAMICTESTOBJECT_H
