import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Column{
        Button{
            text: "invoke"
            onClicked: {
                t.invokeTest()
            }
        }
        Button{
            text: "signal slot block queue"
            onClicked: {
                t.signalSlotQueueTest()
            }
        }
        Button{
            text: "signal slot direct"
            onClicked: {
                t.signalSlotDirectTest()
            }
        }
    }
}
