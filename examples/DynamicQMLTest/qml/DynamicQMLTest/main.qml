import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

ColumnLayout {
    width: 360
    height: 360
    property variant activeObject : staticTestObject
    Rectangle {
        id : rect
        anchors.top : parent.top
        anchors.bottom : buttonBar.top
        width : parent.width
        Text {
            text: activeObject.Txt
            anchors.centerIn: parent
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                Qt.quit();
            }
        }
    }
    RowLayout {
        id : buttonBar
        anchors.bottom : parent.bottom
        height : 40
        width : parent.width
        Text {
            text : "invokeMethod"
            MouseArea {
                anchors.fill : parent
                onClicked : activeObject.someMethod( "Method invoked")
            }
        }
        Text {
            text : "invokeSlot"
            MouseArea {
                anchors.fill : parent
                onClicked : activeObject.setTxt( "Slot invoked")
            }
        }
        Text {
            text : "setProperty"
            MouseArea {
                anchors.fill : parent
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
    }
}    
