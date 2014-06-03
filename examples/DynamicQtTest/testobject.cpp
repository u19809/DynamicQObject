#include "testobject.h"

TestObject::TestObject(QObject *parent) :
    QObject(parent)
{
       Txt = "This is the txt property value";
}
