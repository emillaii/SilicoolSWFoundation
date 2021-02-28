import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Rectangle{
    property double widthHeight: 30
    property string iconSource: ""
    property alias btn: btn

    width: widthHeight
    height: widthHeight
    radius: widthHeight / 2
    Image {
        anchors.fill: parent
        source: iconSource
        fillMode: Image.PreserveAspectFit
    }
    MouseArea{
        id: btn
        anchors.fill: parent
        cursorShape: Qt.OpenHandCursor
        onPressed: {
            cursorShape = Qt.ClosedHandCursor
        }
        onReleased: {
            cursorShape = Qt.OpenHandCursor
        }
    }
}
