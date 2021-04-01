import QtQuick 2.0
import QtQuick.Controls 2.12

BusyIndicator{
    implicitWidth: 200
    implicitHeight: 200
    running: true
    Text {
        anchors.centerIn: parent
        font.pixelSize: 30
        text: "Loading"
    }
}
