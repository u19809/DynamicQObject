#-------------------------------------------------
#
# Project created by QtCreator 2013-09-10T10:14:51
#
#-------------------------------------------------

unix:SRCROOT=~u19809/Projects/DynamicQtObject_GIT
unix:RTROOT=~u19809/Runtimes

win32:SRCROOT=C:\Data\Development\Projects\DynamicQtObject_GIT
win32:RTROOT=C:\Data\Development\Runtimes\

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
unix:LIBS += -L$$RTROOT/build-DynamicQtObject-Desktop_Qt_5_1_1_MinGW_32bit-Debug/debug -lDynamicQtObject
win32:LIBS += -L$$RTROOT\build-DynamicQtObject-Desktop_Qt_5_1_1_MinGW_32bit-Debug\debug -lDynamicQtObject
unix:INCLUDEPATH += $$SRCROOT
win32:INCLUDEPATH += $$SRCROOT
