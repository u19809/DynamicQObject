#ifndef DYNAMICQTOBJECT_H
#define DYNAMICQTOBJECT_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QPair>

#if QT_VERSION < 0x050000
#error "Dynamic objects requires at least version 5 of Qt"
#endif

class DynamicStringSpace;

class DynamicType {
public :
    
    DynamicType( QMetaType::Type T = QMetaType::Void ) {
        Type = T;
        TypeByName = 0;
    }

    DynamicType( const char * TBN ) {
        Type = QMetaType::UnknownType;
        TypeByName = TBN;
    }
    
    DynamicType & operator =( const DynamicType & DT ) {
        Type = DT.Type;
        TypeByName = DT.TypeByName;
        return *this;
    }
    
    int type( DynamicStringSpace & SS ) const;
    
    inline const char * typeName() {
        if( TypeByName == 0 ) {
            // builtin type
            return QMetaType::typeName( Type );
        }
        return TypeByName;
    }
    
private :
    
    QMetaType::Type             Type;
    const char                  * TypeByName;

};

class DynamicProperty {
public :
    
    // flags for properties
    typedef enum {
        Invalid             = 0x00000000,
        Readable            = 0x00000001,
        Writable            = 0x00000002,
        Resettable          = 0x00000004,
        EnumOrFlag          = 0x00000008,
        StdCppSet           = 0x00000100,
    //     Override = 0x00000200,
        Constant            = 0x00000400,
        Final               = 0x00000800,
        Designable          = 0x00001000,
        ResolveDesignable   = 0x00002000,
        Scriptable          = 0x00004000,
        ResolveScriptable   = 0x00008000,
        Stored              = 0x00010000,
        ResolveStored       = 0x00020000,
        Editable            = 0x00040000,
        ResolveEditable     = 0x00080000,
        User                = 0x00100000,
        ResolveUser         = 0x00200000,
        Notify              = 0x00400000,
        Revisioned          = 0x00800000
    } PropertyFlags;
    
    DynamicProperty( const char * N ) {
        Name = N;
        NotifySignalName = 0;
        // default property flags
        Flags = ResolveEditable | Stored | Scriptable | Designable | Writable | Readable; 
    }
    
    DynamicProperty & operator =( const DynamicProperty & DT ) {
        Name = DT.Name;
        Type = DT.Type;
        Flags = DT.Flags;
        NotifySignalName = DT.NotifySignalName;
        return *this;
    }

    inline DynamicProperty & setType( const DynamicType & DT ) {
        Type = DT;
        return *this;
    }
    
    inline DynamicProperty & setStdCppSetter() { 
        Flags |= StdCppSet;
        return *this;
    }
        
    inline DynamicProperty & setFlags( int fl ) {
        Flags = fl;
        return *this;
    }
    
    inline DynamicProperty & setNotify( const char * NSN ) {
        NotifySignalName = NSN;
        Flags |= Notify;
        return *this;
    }
    
    inline const char * name() const {
        return Name;
    }
    
    inline int flags() const {
        return Flags;
    }
    
    inline const DynamicType & type() const {
        return Type;
    }
    
    inline const char * notifySignalName() const {
        return NotifySignalName;
    }
    
private :
    
    const char                  * Name;
    int                         Flags;
    const char                  * NotifySignalName;
    DynamicType                 Type;
    
};

class DynamicArgument {
    
public :
    
    DynamicArgument( const char * N, const DynamicType & DT ) {
        Name = N;
        Type = DT;
    }
    
    DynamicArgument & operator =( const DynamicArgument & DT ) {
        Name = DT.Name;
        Type = DT.Type;
        return *this;
    }
    
    inline const DynamicType & type() const {
        return Type;
    }
    
    inline const char * name() const {
        return Name;
    }
    
private :
    
    const char                  * Name;
    DynamicType                 Type;
};

class DynamicAbstractMethod {
    
public :
    
    typedef enum {
        AccessPrivate       = 0x00,
        AccessProtected     = 0x01,
        AccessPublic        = 0x02,
        AccessMask          = 0x03, //mask
    
        MethodMethod        = 0x00,
        MethodSignal        = 0x04,
        MethodSlot          = 0x08,
        MethodConstructor   = 0x0c,
        MethodTypeMask      = 0x0c,
    
        MethodCompatibility = 0x10,
        MethodCloned        = 0x20,
        MethodScriptable    = 0x40,
        MethodRevisioned    = 0x80
    } MethodFlags;
    
    DynamicAbstractMethod( const char * N, int Fl ) {
        Name = N;
        Flags = Fl;
        Tag = 2;
    }
    
    static inline const char * methodType( int Fl ) {
        if( Fl & ~ MethodSlot ) {
            return "Slot";
        }
        if( Fl & ~ MethodMethod ) {
            return "Method";
        }
        if( Fl & ~ MethodSignal ) {
            return "Signal";
        }
        return "Other";
    }
    
    DynamicAbstractMethod & operator =( const DynamicAbstractMethod & DT ) {
        Name = DT.Name;
        ReturnType = DT.ReturnType;
        Arguments = DT.Arguments;
        return *this;
    }
    
    inline DynamicAbstractMethod & setReturnType( const DynamicType & DT ) {
        ReturnType = DT;
        return *this;
    }
    
    inline DynamicAbstractMethod & setFlags( int F ) {
        Flags = F;
        return *this;
    }
    
    inline DynamicAbstractMethod & setTag( int T ) {
        Tag = T;
        return *this;
    }
    
    inline DynamicAbstractMethod & setArg( const char * N, const DynamicType & DT ) {
        Arguments.append( DynamicArgument( N, DT ) );
        return *this;
    }

    inline int flags( void ) const {
        return Flags;
    }
    
    inline int tag( void ) const {
        return Tag;
    }
    
    inline int argCount( void ) const {
        return Arguments.count();
    }
    
    inline const DynamicType & returnType( void ) const {
        return ReturnType;
    }
    
    inline const QList<DynamicArgument> & arguments(void ) const {
        return Arguments;
    }
    
    inline const char * name( void ) const {
        return Name;
    }
    
private :
    
    const char                  * Name;
    int                         Flags;
    int                         Tag;
    DynamicType                 ReturnType;
    QList<DynamicArgument>      Arguments;

};



class DynamicMethod : public DynamicAbstractMethod {
    
public :
    
    DynamicMethod( const char * N ) : DynamicAbstractMethod( N, AccessPublic | MethodMethod ) {
    }    
    
    inline DynamicMethod & operator =( const DynamicMethod & DT ) {
        DynamicAbstractMethod::operator =( DT );
        return *this;
    }
    
    inline DynamicMethod & setReturnType( const DynamicType & DT ) {
        DynamicAbstractMethod::setReturnType( DT );
        return *this;
    }
    
    inline DynamicMethod & setFlags( int F ) {
        DynamicAbstractMethod::setFlags( F );
        return *this;
    }
    
    inline DynamicMethod & setTag( int T ) {
        DynamicAbstractMethod::setTag( T );
        return *this;
    }
    
    inline DynamicMethod & setArg( const char * N, const DynamicType & DT ) {
        DynamicAbstractMethod::setArg( N, DT );
        return *this;
    }
};

class DynamicSignal : public DynamicAbstractMethod {
    
public :

    DynamicSignal( const char * N ) : DynamicAbstractMethod( N, AccessPublic | MethodSignal ) {
        setReturnType( DynamicType( QMetaType::Void ) );
    }    
    
    inline DynamicSignal & operator =( const DynamicSignal & DT ) {
        DynamicAbstractMethod::operator =( DT );
        return *this;
    }
    
    inline DynamicSignal & setReturnType( const DynamicType & /*DT*/ ) {
        // no return types for signals
        return *this;
    }
    
    inline DynamicSignal & setFlags( int F ) {
        DynamicAbstractMethod::setFlags( F );
        return *this;
    }
    
    inline DynamicSignal & setTag( int T ) {
        DynamicAbstractMethod::setTag( T );
        return *this;
    }
    
    inline DynamicSignal & setArg( const char * N, const DynamicType & DT ) {
        DynamicAbstractMethod::setArg( N, DT );
        return *this;
    }
};

class DynamicSlot : public DynamicAbstractMethod {
    
public :

    DynamicSlot( const char * N ) : DynamicAbstractMethod( N, AccessPublic | MethodSlot ) {
        setReturnType( DynamicType( QMetaType::Void ) );
    }    
    
    inline DynamicSlot & operator =( const DynamicSlot & DT ) {
        DynamicAbstractMethod::operator =( DT );
        return *this;
    }
    
    inline DynamicSlot & setReturnType( const DynamicType & /*DT*/ ) {
        // no return types for slots
        return *this;
    }
    
    inline DynamicSlot & setFlags( int F ) {
        DynamicAbstractMethod::setFlags( F );
        return *this;
    }
    
    inline DynamicSlot & setTag( int T ) {
        DynamicAbstractMethod::setTag( T );
        return *this;
    }
    
    inline DynamicSlot & setArg( const char * N, const DynamicType & DT ) {
        DynamicAbstractMethod::setArg( N, DT );
        return *this;
    }
};

class DynamicEnum {
public :
    
    typedef QPair< const char *, uint > EnumValue;
    
    DynamicEnum( const char * N ) {
        Name = N;
        Flags = 0;
    }
    
    DynamicEnum & operator =( const DynamicEnum & DT ) {
        Name = DT.Name;
        Flags = DT.Flags;
        Values = DT.Values;
        return *this;
    }
    
    inline DynamicEnum & setFlag( int F ) {
        Flags = F;
        return *this;
    }
    
    inline DynamicEnum & setValue( const char * Name, uint value ) {
        Values.append( QPair<const char *, uint>( Name, value ) );
        return *this;
    }
    
    inline const char * name() const {
        return Name;
    }
    
    inline int flags() const {
        return Flags;
    }
    
    inline const QList< EnumValue > & values() const {
        return Values;
    }
    
private :
    
    const char              * Name;
    int                     Flags;
    QList< EnumValue >     Values;
    
};

class DynamicIndexSpace {
    
public :
    
    DynamicIndexSpace();
    ~DynamicIndexSpace();
    
    inline int len() const {
        return Len;
    }
    
    inline uint * data() const {
        return Data;
    }
    
    void add( uint Count, ... );
    inline uint idxAt( int Idx ) {
        return *((uint *)( (intptr_t)Data + Idx ));
    }

    void dumpRaw();

private :
    
    uint            * Data;
    int             Len;
};    

class DynamicStringSpace {
    
public :
    
    DynamicStringSpace();
    ~DynamicStringSpace();
    
    const QByteArrayData *allocateStringSpace();
    
    inline int len() const {
        return Len;
    }
    
    inline int count() const {
        return Count;
    }
    
    inline const char * data() const {
        return Data;
    }
    
    int add( const char * String );
    inline const char * strAt( int Idx ) {
        return (const char *)( (intptr_t)Data + Idx );
    }
    
    void dumpRaw();

private :
    
#if QT_VERSION >= 0x050000
    int             * StrLengths;
    int             * StrOffsets;
#endif
    char            * Data;
    int             Len;
    int             Count;
};    

class DynamicMetaObject {

public :

    DynamicMetaObject( const char * ClassName );
    ~DynamicMetaObject();
    
    inline bool isInitialized() {
        // MO really contains data
        return MetaObject && MetaObject->d.data != 0;
    }

    void transfer( QMetaObject & TransferTo );
    
    // dump Qt Metaobject to qDebug using QObject API
    static void dump( const QMetaObject * MO );
    static void dump( const char * ClassName );
    
    // dump myself
    // QObjectBasedDump = true : uses QObject API, false dumps the tables
    void dump( bool QObjectBasedDump = true );

    void push( const DynamicProperty & Prop ) {
        Properties.append( Prop );
    }
    
    void push( const DynamicEnum & Enum ) {
        Enums.append( Enum );
    }
    void push( const DynamicSignal & Sig ) {
        Signals.append( Sig );
    }
    void push( const DynamicSlot & Slot ) {
        Slots.append( Slot );
    }
    void push( const DynamicMethod & Mth ) {
        Methods.append( Mth );
    }

    typedef void (* StaticMetacallFunction)(QObject *_o, QMetaObject::Call _c, int _id, void **_a);
    
    void finalize( const QMetaObject * MOOfSuper, 
                   DynamicMetaObject::StaticMetacallFunction SMFt 
                   );
    
private :

    int findSignalIndex( const char * SignalName );
    const char * typeName( int Offset );
    void dumpMthOrSigAt( int Offset );
    const char *  string( int Idx );
    uint at( int Idx );
    void dumpRaw( void );

    const char                                          * ClassName;
    QMetaObject                                         * MetaObject;
    
    QList<DynamicMethod>                                Methods;
    QList<DynamicSlot>                                  Slots;
    QList<DynamicProperty>                              Properties;
    QList<DynamicSignal>                                Signals;
    QList<DynamicEnum>                                  Enums;
    
};

class DynamicMetaObjectFactory {
    
public :
    
    DynamicMetaObjectFactory();
  
    static void shutdown();
    static QMetaObject * find( const char * ClassName );
    static QMetaObject * create( const char * ClassName );

    QMap<const char *,QMetaObject *>             CachedMetaObjects;
    
private :
    
    static DynamicMetaObjectFactory * factory() {
        if( Factory == 0 ) {
            Factory = new DynamicMetaObjectFactory;
        }
        return Factory;
    }
    
    static DynamicMetaObjectFactory * Factory;
    
};

// convert argument of signal to value
#define TOARG(x) const_cast<void*>(reinterpret_cast<const void*>(&x))

class DynamicQObject : public QObject
{
    
// special version of Q_OBJECT -> copied from Q_OBJECT def in qobjectdefs.h

// BEGIN OF Q_OBJECT
public: 
    Q_OBJECT_CHECK // same
    mutable QMetaObject privateDynamicMetaObject;  // this is different !!
    virtual const QMetaObject *metaObject() const;  // same
    virtual void *qt_metacast(const char *); // same
    QT_TR_FUNCTIONS // same
    virtual int qt_metacall(QMetaObject::Call, int, void **); // same
private: // same
    Q_DECL_HIDDEN_STATIC_METACALL static void qt_static_metacall(QObject *, QMetaObject::Call, int, void **); // same
    struct QPrivateSignal {};// same
    
// END OF Q_OBJECT

public:

    explicit DynamicQObject(QObject *parent = 0);
    ~DynamicQObject();

protected :
    
    inline void emitDynamicSignal( int SignalID, void ** Args ) {
        QMetaObject::activate(this, metaObject(), SignalID, Args );
    }
    
    // return classname of child class
    virtual const char * className() const = 0;
    
    // convert QObject to proper child class
    virtual void * doQtCast( void ) = 0;
    
    // call property methods ( return TRUE if _c refers to correct request for property IO )
    virtual bool doQtPropertyCall( QMetaObject::Call _c, int _id, void ** a ) = 0;

    // call methods
    virtual void doQtMethodCall(QMetaObject::Call _c, int _id, void ** a ) = 0;

    // initialize metaobject
    virtual void doInitializePrivateMetaObject( DynamicMetaObject & AMO ) const = 0;
    
    // to tell child class that the meta object is now initialized for this QObject (if needed)
    // usage sample : virtual void setBehaviorList( const QList<QByteArray> * BA ) const = 0;
    virtual void metaObjectInitialized() const {
    }
    
    void initializePrivateMetaObject() const;

};

#endif // DYNAMICQTOBJECT_H
