#-------------------------------------------------
#
# Project created by QtCreator 2013-09-03T14:27:06
#
#-------------------------------------------------

QT       -= gui

TARGET = DynamicQtObject
TEMPLATE = lib

DEFINES += DYNAMICQTOBJECT_LIBRARY

SOURCES += \
    dynamicqobject.cpp

HEADERS += \
    dynamicqobject.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
