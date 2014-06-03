#include <QDebug>

#include "dynamictestobject.h"

DynamicTestObject::DynamicTestObject(QObject *parent) : DynamicQObject( parent )
{
    Txt = "This is the Txt property for the dynamic object";
}

QString DynamicTestObject::txt() {
    return Txt;
}

void DynamicTestObject::someMethod( const QString & T ) {
    setTxt( T );
}

void DynamicTestObject::setTxt( const QString & T ) {
    Txt = T;
    qDebug() << "Changed for txt property" << T;
    textChanged( Txt );
}

void DynamicTestObject::textChanged( const QString & Txt ) {
    void *_a[] = { 0, 
                   TOARG( Txt )
                 };
    
    emitDynamicSignal( 0, _a);
}
 
//////////////// DYNAMIC OBJECT /////////////////////

void DynamicTestObject::doInitializePrivateMetaObject( DynamicMetaObject& MO ) const {
    
    MO.push( DynamicSignal( "textChanged").setArg( "Txt", DynamicType( QMetaType::QString ) ) );
    MO.push( DynamicMethod( "someMethod").setArg( "Txt", DynamicType( QMetaType::QString ) ) );
    MO.push( DynamicSlot( "setTxt").setArg( "Txt", DynamicType( QMetaType::QString ) ) );
    MO.push( DynamicProperty( "Txt" ).setType( DynamicType( QMetaType::QString ) ).setStdCppSetter().setNotify( "textChanged" ) );

}

void * DynamicTestObject::doQtCast() {
    return static_cast<void*>(const_cast<DynamicTestObject*>(this));
}

bool DynamicTestObject::doQtPropertyCall( QMetaObject::Call _c, int _id, void ** _a ) {
    bool Result = true;
    
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
            case 0: *reinterpret_cast<QString*>( _v ) = txt(); break;
//        case 1: *reinterpret_cast< int*>(_v) = xx(); break;
//        case 2: *reinterpret_cast< int*>(_v) = xxx(); break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
            case 0: setTxt(*reinterpret_cast<QString*>(_v)); break;
//        case 1: setXX(*reinterpret_cast< int*>(_v)); break;
//        case 2: setXXX(*reinterpret_cast< int*>(_v)); break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
    } else if (_c == QMetaObject::QueryPropertyStored) {
    } else if (_c == QMetaObject::QueryPropertyEditable) {
    } else if (_c == QMetaObject::QueryPropertyUser) {
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 1) // 3= propertycount
            *reinterpret_cast<int*>(_a[0]) = -1;
    } else {
        Result = false;
    }
    
    return Result;
}

void DynamicTestObject::doQtMethodCall( QMetaObject::Call _c, int _id, void ** _a ) {
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
            case 0: textChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
            case 1: someMethod((*reinterpret_cast< QString(*)>(_a[1]))); break;
            case 2: setTxt((*reinterpret_cast< QString(*)>(_a[1]))); break;
//        case 1: xxChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< char*(*)>(_a[2]))); break;
//        case 2: { int _r = testMth((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< char*(*)>(_a[2])));
//            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
//        case 3: { int* _r = testMth2((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< char*(*)>(_a[2])));
//            if (_a[0]) *reinterpret_cast< int**>(_a[0]) = _r; }  break;
//        case 4: { TestClass* _r = testCls((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< char*(*)>(_a[2])));
//            if (_a[0]) *reinterpret_cast< TestClass**>(_a[0]) = _r; }  break;
//        case 5: { time_t* _r = heTime((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< char*(*)>(_a[2])));
//            if (_a[0]) *reinterpret_cast< time_t**>(_a[0]) = _r; }  break;
            default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DynamicTestObject::*_t)( const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DynamicTestObject::textChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DynamicTestObject::*_t)( const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DynamicTestObject::someMethod)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DynamicTestObject::*_t)( const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DynamicTestObject::setTxt)) {
                *result = 0;
                return;
            }
        }
//        {
//            typedef void (DynamicTestObject::*_t)(int , char * );
//            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TestClass::xxChanged)) {
//                *result = 1;
//            }
//        }
    }
}
