import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "qrc:/qmlComponents"

Window {
    visible: true
    width: 600
    height: 600
    title: qsTr("Hello World")

    SilicolMsgBox {
        id: silicolMsgBox

        uiOperationImplementation: uiOperationImpl
        Component.onCompleted: {
            uiOperation.setGrabMouseItem(silicolMsgBox.contentItem)
        }
    }

    Connections {
        target: msgBoxModel
        onMsgBoxCountChanged: {
            if (count > 0) {
                silicolMsgBox.msgBoxCount = count
                silicolMsgBox.open()
            } else {
                silicolMsgBox.close()
            }
        }
    }

    ColumnLayout{
        Button{
            text: "Init"
            onClicked: {
                serialPortTest.init()
            }
        }

        RowLayout{
            Label{
                text: "DataToWrite"
            }
            TextField{
                id: txtDataToWrite
            }
        }
        Button{
            text: "Write"
            onClicked: {
                serialPortTest.writeData(txtDataToWrite.text)
            }
        }
        Button{
            text: "ClearReadBuffer"
            onClicked: {
                serialPortTest.clearReadBuffer()
            }
        }
        RowLayout{
            Label{
                text: "Timeout"
            }
            TextField{
                id: txtTimeout
                text: "10000"
            }
        }

        RowLayout{
            Label{
                text: "Endmark"
            }
            TextField{
                id: txtEndMark
                text: "!"
            }
        }
        Button{
            text: "ReadEndMark"
            onClicked: {
                serialPortTest.readEndMark(txtEndMark.text, txtTimeout.text)
            }
        }

        RowLayout{
            Label{
                text: "Length"
            }
            TextField{
                id: txtLength
                text: "5"
            }
        }
        Button{
            text: "ReadLength"
            onClicked: {
                serialPortTest.readLength(txtLength.text, txtTimeout.text)
            }
        }

    }
}
