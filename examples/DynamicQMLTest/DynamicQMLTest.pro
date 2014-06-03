unix:SRCROOT=~u19809/Projects/DynamicQtObject_GIT
unix:RTROOT=~u19809/Runtimes

win32:SRCROOT=C:\Data\Development\Projects\DynamicQtObject_GIT
win32:RTROOT=C:\Data\Development\Runtimes\

# Add more folders to ship with the application, here
folder_01.source = qml/DynamicQMLTest
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

HEADERS += ..\DynamicQtTest\testobject.h

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp ..\DynamicQtTest\testobject.cpp ..\DynamicQtTest\dynamictestobject.cpp

# add DynamicQtObject libs
unix:LIBS += -L$$RTROOT/build-DynamicQtObject-Desktop_Qt_5_1_1_MinGW_32bit-Debug/debug -lDynamicQtObject
win32:LIBS += -L$$RTROOT\build-DynamicQtObject-Desktop_Qt_5_1_1_MinGW_32bit-Debug\debug -lDynamicQtObject
unix:INCLUDEPATH += $$SRCROOT $$SRCROOT/DynamicQtTest
win32:INCLUDEPATH += $$SRCROOT $$SRCROOT\DynamicQtTest

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()
