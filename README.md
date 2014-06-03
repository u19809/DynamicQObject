DynamicQObject
==============

Create Moc-less QObject child classes at runtime

This project implements an extension to Qt that allows for the creation of QObject children without the use of Q_OBJECT and moc for Qt > 5.x.

The extension does not require any changes to the Qt library.

It allows for the creation of QObject instances at runtime and thus on-the-fly which is useful if you want to use Qt together with objects provided by external dynamic object providers like soap or .Net or scripting languages and that offer inspection API on those objects. The Extension offers a framework through which the inspection API of those external object providers can be used to feed the QMetaObject and to generate glue code between the QObject system and the foreign object system.

I tried to make the framework as generic as possible to provide for future versions of Qt (in fact the API was first written using Qt 4.8 but since Qt 4 was almost obsoleted I decided no longer to implement Qt 4)

The main code is small 30 KB.  The entire zip with a Qt and QML examples just 60 K compressed.

this example shows what the use of DynamicQObject looks like for the Qt C++

  
    // regular MOC-ed QObject
    QObject * TO = new TestObject( this ); 
    connect( TO, SIGNAL( textChanged(const QString & ) ), 
             ui->PropertyValue_LBL, SLOT( setText( const QString & ) ) );
    connect( this, SIGNAL( updateProperty(const QString &)), 
             TO, SLOT( setTxt( const QString & ) ) );
             
    // this class does not use MOC at all
    QObject * DTO = new DynamicTestObject( this ); 
    connect( DTO, SIGNAL( textChanged(const QString & ) ), 
             ui->PropertyValue2_LBL, SLOT( setText( const QString & ) ) );
    connect( this, SIGNAL( updateProperty2(const QString &)), 
             DTO, SLOT( setTxt( const QString & ) ) );

this is a QML code snippet

    QtQuick2ApplicationViewer viewer;
    TestObject * TO = new TestObject();
    DynamicTestObject * DTO = new DynamicTestObject();
    // register some predefined objects
    QQmlContext *ctxt = viewer.rootContext();
    ctxt->setContextProperty("staticTestObject", TO );
    ctxt->setContextProperty("dynamicTestObject", DTO );
    viewer.setMainQmlFile(QStringLiteral("qml/DynamicQMLTest/main.qml"));  

and using it from qml

        Text {
            text : "setProperty"
            MouseArea {
                anchors.fill : parent
                // activeObject is either a real MOC object or a dynamic object ... completely transparent to QML
                onClicked : activeObject.Txt = "New property value"
            }
        }
        Text {
            text : "Use Dynamic Object"
            MouseArea {
                anchors.fill : parent
                onClicked : {
                    if ( parent.text == "Use Dynamic Object" ) {
                        parent.text = "Use Static Object"
                        activeObject = dynamicTestObject;
                    } else {
                        parent.text = "Use Dynamic Object"
                        activeObject = staticTestObject;
                    }
                }
            }
        }
