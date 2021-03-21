import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

RowLayout{
    property var configObj: null
    property alias readBtn: readBtn
    property alias moveToBtn: moveToBtn

    Button{
        id: readBtn
        enabled: userManagement.currentAuthority >= 2
        text: qsTr("Read")
        onClicked: {
            configObj.handleEvent("UpdatePos")
        }
    }
    Button{
        id: moveToBtn
        text: qsTr("MoveTo")
        onClicked: {
            configObj.handleEvent("MoveTo")
        }
    }
}
