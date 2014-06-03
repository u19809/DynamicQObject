#include <QDebug>
#include <QMetaProperty>
#include <QMetaObject>
#include "dynamicqobject.h"

#define SMOC_REVISION            7
#define SMOC_HEADER              4
#define SMOC_FS                  14
#define SMOC_MTHSIGDATA          5
#define SMOC_PROPDATA            4
#define SMOC_ENUMDATA            4
#define SMOC_ENUMDATAVALUES      2

int DynamicType::type( DynamicStringSpace & SS ) const {
    return (TypeByName == 0 ) ? 
                Type : 0x8000000 | SS.add( TypeByName );
}

//
//
//

DynamicIndexSpace::DynamicIndexSpace() {
    Data = 0;
    Len = 0;
}

DynamicIndexSpace::~DynamicIndexSpace() {
    // Data should not be freed
//    if( Data ) {
//        free( Data );
//    }
}

void DynamicIndexSpace::add( uint Count, ... ) {
    va_list argp;
    uint * Run;
    va_start(argp, Count);
    
    Data = (uint*)realloc( Data, sizeof(uint)*(Len + Count) );
    
    Run = Data+Len;
    
    Len += Count;
    
    for( ; Count ; Count --, Run ++ ) {
        *Run = va_arg( argp, uint );
    }
   
}

void DynamicIndexSpace::dumpRaw() {
    QString Buf;
    int i = 0;
    int si = 0;
    
    qDebug() << "Count"<< Len;
    
    while( i < Len ) {
        int j;
        Buf = "";
        
        for( j = 0; j < 16 && i < Len; i ++, j ++ ) {
            Buf += QString().setNum( *(Data+i) ) + " ";
        }
        for( ; j < 16 ; j++ ) {
            Buf += QString().setNum( 0 ) + " ";
        }
        qDebug() << si << Buf;
        si += 16;
    }
}

//
//
//

DynamicStringSpace::DynamicStringSpace() {
    Data = 0;
    Len = 0;
    Count = 0;
    StrLengths = 0;
    StrOffsets = 0;
}

DynamicStringSpace::~DynamicStringSpace() {
    if( Data ) {
        free( Data );
        free( StrLengths );
        free( StrOffsets );
    }
}

int DynamicStringSpace::add( const char * StringToAdd ) {
    int StringCounter = 0;
    
    for( int idx = 0; idx < Len; idx += strlen( strAt(idx) )+1, StringCounter ++ ) { 
        
        // string present ?
        if( strcmp( strAt(idx), StringToAdd ) == 0 ) {
            // recycle string
            return StringCounter;
        }
    }
    
    // need to add string
    Count ++;
    
    int sl = strlen( StringToAdd ) + 1;
    Data = (char *)realloc( Data, sizeof(char)*(Len + sl) );
    
    // transfer string
    memcpy( Data+Len, StringToAdd, sl );
    
    StrLengths = (int *)realloc( StrLengths, sizeof(int)*Count );
    StrOffsets = (int *)realloc( StrOffsets, sizeof(int)*Count );
    
    *(StrLengths+Count-1) = sl;
    *(StrOffsets+Count-1) = Len;

    Len += sl;
    
    return Count-1;
}

void DynamicStringSpace::dumpRaw() {
    QByteArray Buf;
    QString SBuf;
    QChar C;
    int i = 0;
    int si = 0;
    
    qDebug() << "Count"<< Count << "TotalLength"<< Len;
    
    qDebug() << "Lengths/Offsets";
    while( i < Count ) {
        qDebug() << *(StrLengths+i) << *(StrOffsets+i);
        i ++;
    }
    
    i = 0;
    while( i < Len ) {
        int j;
        Buf.clear();
        SBuf.clear();
        
        for( j = 0; j < 16 && i < Len; i ++, j ++ ) {
            C = *(Data+i);
            Buf.append( C );
            SBuf.append( ( C.isPrint() ) ? QString(C) : Buf.right(1).toHex() );
        }
        for( ; j < 16 ; j++ ) {
            Buf.append( (char)0 );
            SBuf.append( " " );
        }
        qDebug() << si << Buf.toHex() << SBuf;
        si += 16;
    }
}

const QByteArrayData * DynamicStringSpace::allocateStringSpace() {
    
    /*
     * struct StringSpace_t {
     *    QByteArrayData data[ NoOfStringsInStringSpace ];
     *    char stringData[ AllStrings ];
     * };
     *
     */
    
    int SSOffset = sizeof( QByteArrayData ) * Count;
    char * FinalData = (char *)malloc( SSOffset + (sizeof( char ) * Len ) );
    
    // now put the data in it
    QByteArrayData * BADPtr;
    
    // alloc combined stringspace
    BADPtr = (QByteArrayData *)FinalData;
    
    // Initialize ByteArray
    for( int i = 0; i < Count ; i ++ ) {
        { 
            int l = *(StrLengths+i);
            int off = (SSOffset+(*(StrOffsets+i))-(i*sizeof(QByteArrayData)));
            qDebug() << "CNV" << i << SSOffset << ( SSOffset + (sizeof( char ) * Len ) ) << l-1 << off;
            QByteArrayData BA = Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET( l-1, off );
            memcpy( (BADPtr+i), &BA, sizeof( BA ) );
        }
    }
    
    // transfer strings
    memcpy( (void *)((intptr_t)FinalData+SSOffset), Data, Len );
    
    return (const QByteArrayData *)FinalData;
    
}

//
//
//

DynamicMetaObjectFactory * DynamicMetaObjectFactory::Factory = 0;

DynamicMetaObjectFactory::DynamicMetaObjectFactory() {
}

QMetaObject * DynamicMetaObjectFactory::find( const char * ClassName ) {
    DynamicMetaObjectFactory * F = factory();
    return F->CachedMetaObjects.value( ClassName, 0 );
}

QMetaObject * DynamicMetaObjectFactory::create( const char * ClassName ) {
    DynamicMetaObjectFactory * F = factory();
    QMetaObject * MetaObject = new QMetaObject;
    MetaObject->d.data = 0; // to indicate empty MO -> isInitialized() == false
    return F->CachedMetaObjects[ ClassName ] = MetaObject;
}
    
void DynamicMetaObjectFactory::shutdown() {
    DynamicMetaObjectFactory * F = factory();
    foreach( QMetaObject * MO, F->CachedMetaObjects ) {
        free( (void *)MO->d.stringdata );
        free( (void *)MO->d.data );
        delete MO;
    }
    F->CachedMetaObjects.clear();
}

//
//
//


DynamicMetaObject::DynamicMetaObject( const char *ClassName ) {
    this->ClassName = ClassName;
    
    // if it exists in the cache -> init it
    MetaObject = DynamicMetaObjectFactory::find(ClassName);
    
    if( ! MetaObject ) {
        MetaObject = DynamicMetaObjectFactory::create( ClassName );
    }
    //MetaObject = 0;
}

DynamicMetaObject::~DynamicMetaObject() {
    // MetaObject is cached
}

void DynamicMetaObject::transfer( QMetaObject & To ) {
    
    To.d.superdata = MetaObject->d.superdata;
    To.d.stringdata = MetaObject->d.stringdata;
    To.d.data = MetaObject->d.data;
    To.d.static_metacall = MetaObject->d.static_metacall;
    To.d.relatedMetaObjects = MetaObject->d.relatedMetaObjects;
    To.d.extradata = MetaObject->d.extradata;
    
    MetaObject = 0;
}

void DynamicMetaObject::finalize( const QMetaObject * MOOfSuper, 
                                  DynamicMetaObject::StaticMetacallFunction SMFt 
                                  ) {
    DynamicStringSpace Strings;
    DynamicIndexSpace Indices;
    
    int idx = Strings.add( ClassName );
    
    Indices.add( 4, 
                SMOC_REVISION,       // revision
                idx,                  // classname
                0,    0            // classinfo
                );
    
    // push methods and signals
    Indices.add( 2, 
                 Methods.count() + Slots.count() + Signals.count(),         // count of items
                 SMOC_FS                                    // starts after fixed header 
                 );
    int MthSpaceNeeded = SMOC_MTHSIGDATA * (Methods.count() + Slots.count() + Signals.count());
    int MthDataSpaceNeeded = 0;
    
    foreach( const DynamicMethod & Mth, Methods ) {
        MthDataSpaceNeeded += Mth.argCount()*2 + 1;           // + return value
    }
    foreach( const DynamicSignal & Sig, Signals ) {
        MthDataSpaceNeeded += Sig.argCount()*2 + 1;           // + return value
    }
    foreach( const DynamicSlot & Slot, Slots ) {
        MthDataSpaceNeeded += Slot.argCount()*2 + 1;           // + return value
    }
                 
    // push properties
    Indices.add( 2,
                 Properties.count(),
                 SMOC_FS + MthSpaceNeeded + MthDataSpaceNeeded
                 );                 
    
    // push enums and sets
    Indices.add( 2,
                 Enums.count(),
                 SMOC_FS + MthSpaceNeeded + MthDataSpaceNeeded + (Properties.count()*SMOC_PROPDATA)
                 );                 
    
    // push constructors
    Indices.add( 2, 0, 0 );         
    
    // push flags
    Indices.add( 1, 0 );         
    
    // push signalcount
    Indices.add( 1, Signals.count() );         
    
    // push signal data
    int ParameterOffset = SMOC_FS + MthSpaceNeeded; // where does the parameter section start
    
    foreach( const DynamicSignal & Sig, Signals ) {
        Indices.add( SMOC_MTHSIGDATA,
                     Strings.add( Sig.name() ),
                     Sig.arguments().count(),
                     ParameterOffset,
                     Sig.tag(),
                     Sig.flags()
                    );
        
        ParameterOffset += Sig.arguments().count()*2 + 1; // type + name + return value
    }
    
    // push method data
    foreach( const DynamicSlot & Slot, Slots ) {
        Indices.add( SMOC_MTHSIGDATA,
                     Strings.add( Slot.name() ),
                     Slot.arguments().count(),
                     ParameterOffset,
                     Slot.tag(),
                     Slot.flags()
                    );
        ParameterOffset += Slot.arguments().count()*2 + 1; // type + name + return value
    }
    
    // push method data
    foreach( const DynamicMethod & Mth, Methods ) {
        Indices.add( SMOC_MTHSIGDATA,
                     Strings.add( Mth.name() ),
                     Mth.arguments().count(),
                     ParameterOffset,
                     Mth.tag(),
                     Mth.flags()
                    );
        ParameterOffset += Mth.arguments().count()*2 + 1; // type + name + return value
    }
    
    
    // push signal parameters
    foreach( const DynamicSignal & Sig, Signals ) {
        Indices.add( 1, QMetaType::Void ); // sigs have no ret value
        foreach( const DynamicArgument & Arg, Sig.arguments() ) {
            Indices.add( 1, Arg.type().type( Strings ) );
        }
        foreach( const DynamicArgument & Arg, Sig.arguments() ) {
            Indices.add( 1, Strings.add( Arg.name() ) );
        }
    }
        
    // push slot parameters
    foreach( const DynamicSlot & Slot, Slots ) {
        Indices.add( 1, Slot.returnType().type( Strings ) ); // sigs have no ret value
        foreach( const DynamicArgument & Arg, Slot.arguments() ) {
            Indices.add( 1, Arg.type().type( Strings ) );
        }
        foreach( const DynamicArgument & Arg, Slot.arguments() ) {
            Indices.add( 1, Strings.add( Arg.name() ) );
        }
    }
    
    // push method parameters
    foreach( const DynamicMethod & Mth, Methods ) {
        Indices.add( 1, Mth.returnType().type( Strings ) ); // sigs have no ret value
        foreach( const DynamicArgument & Arg, Mth.arguments() ) {
            Indices.add( 1, Arg.type().type( Strings ) );
        }
        foreach( const DynamicArgument & Arg, Mth.arguments() ) {
            Indices.add( 1, Strings.add( Arg.name() ) );
        }
    }
    
    Indices.dumpRaw();
    // push property data
    foreach( const DynamicProperty & Prop, Properties ) {
        Indices.add( SMOC_PROPDATA-1,               // first push property data
                     Strings.add( Prop.name() ),
                     Prop.type().type( Strings ),
                     Prop.flags()
                     );
    }
    
    Indices.dumpRaw();
    // push property notify signals
    foreach( const DynamicProperty & Prop, Properties ) {
        if( Prop.notifySignalName() ) {
            Indices.add( 1,                 // first push property data
                         findSignalIndex( Prop.notifySignalName() )
                         );
        } else {
            Indices.add( 1, 0 );            // dummy value
        }
    }
    
    Indices.dumpRaw();
    // push enum data
    int EnumValuesIndex = Indices.len();
    foreach( const DynamicEnum & E, Enums ) {
        Indices.add( SMOC_ENUMDATA,
                     Strings.add( E.name() ),
                     E.flags(),
                     E.values().count(),
                     EnumValuesIndex
                     );
        
        EnumValuesIndex += E.values().count()*2;
    }
    
    Indices.dumpRaw();
    // push enum values
    foreach( const DynamicEnum & E, Enums ) {
        foreach( const DynamicEnum::EnumValue & P, E.values() ) {
            Indices.add( 2,
                         Strings.add( P.first ),
                         P.second
                         );
        }
    }
    
    // EOD    
    Indices.add( 1, 0 );
    
    Indices.dumpRaw();
    Strings.dumpRaw();
    
    MetaObject->d.superdata = MOOfSuper;
    MetaObject->d.stringdata = Strings.allocateStringSpace();
    MetaObject->d.data = Indices.data();
    MetaObject->d.static_metacall = SMFt;
    MetaObject->d.relatedMetaObjects = 0;
    MetaObject->d.extradata = 0;
    dump( false );
    dump( true );
}    

int DynamicMetaObject::findSignalIndex(const char *SignalName) {
    int Index = 0;
    foreach( const DynamicSignal & Sig, Signals ) {
        if( strcmp( Sig.name(), SignalName) == 0 ) {
            return Index;
        }
        Index ++;
    }
    
    Q_ASSERT( 0 );
    // should not occur
    
    return 0;    
}
    
const char * DynamicMetaObject::typeName( int Offset ) {
    if( at( Offset ) > 0x80000000 ) {
        int typestring = at( Offset ) - 0x80000000;
        return string( typestring );
    }
    
    return QMetaType::typeName( at( Offset ) );
}

const char * DynamicMetaObject::string( int Idx ) {
    return (const char *)((QByteArrayData *)MetaObject->d.stringdata+Idx)->data();
}

uint DynamicMetaObject::at( int Idx ) {
    return *((uint *)MetaObject->d.data+Idx);
}

void DynamicMetaObject::dumpRaw( ) {
    qDebug() << "Raw dump for class" << string( at( 1 ) );

    uint MthSigCount = at( 4 );
    uint MthSigOffset = at( 5 );
    uint SigCount = at( 13 );
    uint i;
    
    qDebug() << "Signals :" << SigCount << "Offset" << MthSigOffset;
    qDebug() << "Slots and Methods :" << (MthSigCount-SigCount);
    qDebug() << "Offset" << MthSigOffset;
    for( i = 0; i < MthSigCount; i ++ ) {    
        int argoff = at( MthSigOffset+2 );
        
        qDebug() << "Name :" << string( at( MthSigOffset ) ) 
                 << "Type :" << DynamicAbstractMethod::methodType( at( MthSigOffset+4 ) )
                 << "Tag :"<< at( MthSigOffset+3 ) 
                 << "Flags :" << at( MthSigOffset+4 ) 
                 << "Returns :" << typeName( argoff );
        for( uint i = 0; i < at( MthSigOffset+1 ); i ++ ) {
            qDebug() << "  " << (i+1) << string( at( argoff+1+i+at( MthSigOffset+1 ) ) ) << ":" << typeName( argoff+1+i );
        }
        
        MthSigOffset += 5;
    }
        
    // properties
    uint PropOffset = at( 7 );
    uint PropCount = at( 6 );
    
    qDebug() << "Properties :" << PropCount << "Offset" << PropOffset;
    
    for( i = 0; i < PropCount; i ++ ) {
        qDebug( ) << "Name :" << string( at( PropOffset+i*3 ) ) 
                  << "Type :" << typeName( PropOffset+i*3+1) 
                  << "Flags :" << at( PropOffset+i*3+2 ) 
                  << "NotifySigIdx :"
                  <<
                     ( ( at( PropOffset+i*3+2 ) & DynamicProperty::Notify ) ? 
                         (int) at( PropOffset+PropCount*3+i ) : -1 );
    }
    
    // enum/sets
    uint EnumOffset = at( 9 );
    uint EnumCount = at( 8 );
    
    qDebug() << "Enums :" << EnumCount;
    
    for( i = 0; i < EnumCount; i ++ ) {
        qDebug() << string( at( EnumOffset+i*4) ) << at( EnumOffset+i*4+1);
        uint keycount = at( EnumOffset+i*4+2);
        uint keyoff = at( EnumOffset+i*4+3);
        for( uint j = 0; j < keycount; j ++ ) {
            qDebug() << "  " << string( at( keyoff+j*2 ) ) << at( keyoff+j*2+1 );
        }
    }
}

void DynamicMetaObject::dump( bool QObjectBasedDump ) {
    
    if( QObjectBasedDump ) {
        dump( MetaObject );    
    } else {
        dumpRaw( );
    }
}

void DynamicMetaObject::dump(const QMetaObject *MO ) {

    qDebug() << "Qt dump for class" << MO->className();
    qDebug() << "Parent : " << MO->superClass()->className();
    qDebug() << "Propertycount : " << MO->propertyCount();
    for( int i = 0; i < MO->propertyCount(); i ++ ) {
        QMetaProperty PR = MO->property(i);
        qDebug() << "Property" << PR.name() << PR.type();
    }

    qDebug() << "MethodCount : " << MO->methodCount();
    for( int i = 0; i < MO->methodCount(); i ++ ) {
        const char * MthType;
        QMetaMethod MT = MO->method(i);
        switch( MT.methodType() ) {
            case QMetaMethod::Method : MthType = "Method"; break;
            case QMetaMethod::Signal : MthType = "Signal"; break;
            case QMetaMethod::Slot : MthType = "Slot"; break;
            case QMetaMethod::Constructor : MthType = "Constructor"; break;
        }
        qDebug() << MthType
                 << MT.methodSignature() 
                 << MT.parameterNames() 
                 << MT.parameterTypes() 
                 << MT.typeName();
    }

}

void DynamicMetaObject::dump( const char * ClsName ) {
    QMetaObject * MetaObject = DynamicMetaObjectFactory::find( ClsName );
    if( MetaObject ) {
        dump( MetaObject );
    }
}

//
//
//


DynamicQObject::DynamicQObject(QObject *parent) :
    QObject(parent)
{
   privateDynamicMetaObject.d.stringdata = 0;
}

DynamicQObject::~DynamicQObject()
{
    // no need to cleanup privateMetaObject as it is cached
}

void DynamicQObject::initializePrivateMetaObject() const {

    DynamicMetaObject AMO( className() );

    if( ! AMO.isInitialized() ) {
        // delegate initialization to allow insertion of dynamic interface
        doInitializePrivateMetaObject( AMO );

        // update header
        AMO.finalize( &QObject::staticMetaObject, 
                      (DynamicMetaObject::StaticMetacallFunction)DynamicQObject::qt_static_metacall );
    } 
    
    // take private copy of QMetaObject
    AMO.transfer( privateDynamicMetaObject );

    // PrivateMetaObject is now initialized tell the world
    metaObjectInitialized();

    DynamicMetaObject::dump( &privateDynamicMetaObject );
}

void DynamicQObject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    const QMetaObject * MO = _o->metaObject();
    Q_ASSERT(MO->cast(_o));

    // delegate call 
    static_cast<DynamicQObject *>(_o)->doQtMethodCall( _c, _id, _a );
}

const QMetaObject *DynamicQObject::metaObject() const
{
    if( privateDynamicMetaObject.d.stringdata == 0 ) {
        initializePrivateMetaObject();
    }

    // return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &privateMetaObject;
    return &privateDynamicMetaObject;
}

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DynamicQObject::getStaticMetaObject() {
    if( privateDynamicMetaObject.d.stringdata == 0 ) {
        initializePrivateMetaObject();
    }
    return privateDynamicMetaObject;
}
#endif //Q_NO_DATA_RELOCATION

void *DynamicQObject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;

    if (!strcmp(_clname, (const char *)privateDynamicMetaObject.d.stringdata->data() ) ) {
        // delegate to child class
        return doQtCast();
    }

    return QObject::qt_metacast(_clname);
}

int DynamicQObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    
    if (_id < 0) {
        return _id;
    }
    
    const QMetaObject * MO = metaObject();
    
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < MO->methodCount() ) {
            DynamicQObject::qt_static_metacall(this, _c, _id, _a);
        }
            
        _id -= MO->methodCount();
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < MO->methodCount()) {
            *reinterpret_cast<int*>(_a[0]) = -1;
        }
        _id -= MO->methodCount();
        
#ifndef QT_NO_PROPERTIES
    } else {
        // delegate to child class
        if( doQtPropertyCall( _c, _id, _a ) ) {
            _id -= MO->propertyCount();
        } // else return _id as is
#endif
        
    }
    
    return _id;
}
