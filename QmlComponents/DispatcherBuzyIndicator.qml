import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Popup{
    id: popup
    z: 1000
    closePolicy: Popup.NoAutoClose
    property date startTime: new Date()
    property double elapsedTime: 0

    anchors.centerIn: Overlay.overlay
    background: Item {}

    contentItem: BusyIndicator{
        implicitWidth: 300
        implicitHeight: implicitWidth
        running: popup.opened

        Rectangle{
            width: parent.implicitWidth - 50
            height: width
            radius: width / 2
            color: "black"
            opacity: 0.8
            anchors.centerIn: parent

            ColumnLayout{
                anchors.centerIn: parent
                anchors.verticalCenterOffset: -15
                spacing: 25
                Text {
                    Layout.alignment: Qt.AlignHCenter
                    font.pixelSize: 18
                    color: "pink"
                    text: qsTr("Elapsed: ") + elapsedTime + " s"
                }
                Text {
                    id: txt
                    Layout.alignment: Qt.AlignHCenter
                    font.pixelSize: 25
                    color: "pink"
                    text: "..."
                }
            }
        }
    }

    Timer{
        id: timer
        interval: 100
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            elapsedTime = (new Date() - startTime) / 1000
        }
    }

    Connections{
        target: stateMachine
        onReqShowBuzyStatus:{
            txt.text = buzyContent
            startTime = new Date()
            timer.start()
            open()
        }
        onReqHideBuzyStatus:{
            timer.stop()
            close()
        }
    }
}
