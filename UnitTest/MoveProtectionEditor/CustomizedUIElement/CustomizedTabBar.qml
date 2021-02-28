import QtQuick 2.12
import QtQuick.Controls 2.12

TabButton {
    id: control

    text: qsTr("TabButton")

    contentItem: Text {
        id: txt
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.down ? "#17a81a" : "#21be2b"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: txt.contentWidth > 50 ? 30 : txt.contentWidth + 5
        implicitHeight: 15
        opacity: enabled ? 1 : 0.3
        border.color: control.down ? "#17a81a" : "#21be2b"
        border.width: 1
        radius: 2
    }
}
