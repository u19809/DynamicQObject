#-------------------------------------------------
#
# Project created by QtCreator 2013-09-10T10:14:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DynamicQtTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    testobject.cpp \
    dynamictestobject.cpp

HEADERS  += mainwindow.h \
    testobject.h \
    dynamictestobject.h

FORMS    += mainwindow.ui

# add DynamicQtObject libs
unix:LIBS += -LC:\Data\Development\Runtimes\build-DynamicQtObject-Desktop_Qt_5_1_1_MinGW_32bit-Debug\debug -lDynamicQtObject
win32:LIBS += -LC:\Data\Development\Runtimes\build-DynamicQtObject-Desktop_Qt_5_1_1_MinGW_32bit-Debug\debug -lDynamicQtObject
unix:INCLUDEPATH += C:\Data\Development\Projects\DynamicQtObject
win32:INCLUDEPATH += C:\Data\Development\Projects\DynamicQtObject
