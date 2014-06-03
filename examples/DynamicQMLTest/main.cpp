#include <QtGui/QGuiApplication>
#include <QQmlContext>
#include "qtquick2applicationviewer.h"
#include "dynamictestobject.h"
#include "testobject.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;
    TestObject * TO = new TestObject();
    DynamicTestObject * DTO = new DynamicTestObject();
    
    // register some predefined objects
    QQmlContext *ctxt = viewer.rootContext();
    ctxt->setContextProperty("staticTestObject", TO );
    ctxt->setContextProperty("dynamicTestObject", DTO );
    
    viewer.setMainQmlFile(QStringLiteral("qml/DynamicQMLTest/main.qml"));    
    viewer.showExpanded();

    return app.exec();
}
