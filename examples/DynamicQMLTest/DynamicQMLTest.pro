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
unix:LIBS += -LC:\Data\Development\Runtimes\build-DynamicQtObject-Desktop_Qt_5_1_1_MinGW_32bit-Debug\debug -lDynamicQtObject
win32:LIBS += -LC:\Data\Development\Runtimes\build-DynamicQtObject-Desktop_Qt_5_1_1_MinGW_32bit-Debug\debug -lDynamicQtObject
unix:INCLUDEPATH += C:\Data\Development\Projects\DynamicQtObject C:\Data\Development\Projects\DynamicQtObject\DynamicQtTest
win32:INCLUDEPATH += C:\Data\Development\Projects\DynamicQtObject C:\Data\Development\Projects\DynamicQtObject\DynamicQtTest

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()
