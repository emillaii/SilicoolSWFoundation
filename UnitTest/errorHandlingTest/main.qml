import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    SilicolMsgBox{
        id: silicolMsgBox

        Component.onCompleted: {
            uiOperation.setGrabMouseItem(silicolMsgBox.contentItem)
        }
    }

    Connections{
        target: msgBoxModel
        onMsgBoxCountChanged:{
            if(count > 0)
            {
                silicolMsgBox.msgBoxCount = count
                silicolMsgBox.open()
            }
            else{
                silicolMsgBox.close()
            }
        }
    }

    ColumnLayout{
        Button{
            text: "pick new lens"
            onClicked: {
                tem.runSingleCmd(lensPicker, "pickNewLens")
            }
        }
        Button{
            text: "place lens to lut"
            onClicked: {
                tem.runSingleCmd(lensPicker, "placeLensToLut")
            }
        }
    }
}
