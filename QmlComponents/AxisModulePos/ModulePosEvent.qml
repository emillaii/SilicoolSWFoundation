import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

RowLayout{
    property var configObj: null
    Button{
        enabled: userManagement.currentAuthority >= 2
        text: qsTr("Read")
        onClicked: {
            configObj.handleEvent("UpdatePos")
        }
    }
    Button{
        text: qsTr("MoveTo")
        onClicked: {
            configObj.handleEvent("MoveTo")
        }
    }
}
