import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import "qrc:/qmlComponents/LogViewer"

Window {
    visible: true
    width: 1500
    height: 800
    title: qsTr("Hello World")

    RowLayout{
        ColumnLayout{
            RowLayout{
                Label{
                    text: "Count"
                }
                TextField{
                    id: txtCount
                    text: "2"
                }
            }
            RowLayout{
                Button{
                    text: "Create"
                    onClicked: {
                        $lwm.create(txtCount.text)
                    }
                }
                Button{
                    text: "Destroy"
                    onClicked: {
                        $lwm.destroy()
                    }
                }
            }
            RowLayout{
                Label{
                    text: "Interval"
                }
                TextField{
                    id: txtInterval
                    text: "100"
                }
            }
            RowLayout{
                Button{
                    text: "Start"
                    onClicked: {
                        $lwm.start(txtInterval.text)
                    }
                }
                Button{
                    text: "Stop"
                    onClicked: {
                        $lwm.stop()
                    }
                }
            }
            Button{
                text: "ChangeVisible"
                onClicked: {
                    logViewer.visible = !logViewer.visible
                }
            }
        }

        LogViewContainer{
            id: logViewer
            width: 1200
            height: 700
            Component.onCompleted: {
                logToViewerHandler = logHandler
                init()
            }
        }
    }
}
