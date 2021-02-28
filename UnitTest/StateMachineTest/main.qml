import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import SMD 1.0
import "qml/ConfigManager"

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    ColumnLayout{
        Label{
            text: sm.stateStringDescription
            color: sm.stateColorDescription
        }
        RowLayout{
            Button{
                text: "Init"
                onClicked: {
                    sm.postEvent(SMD.Init)
                }
                enabled: sm.initEnabled
            }
            Button{
                text: "Start"
                onClicked: {
                    sm.postEvent(SMD.Start)
                }
                enabled: sm.startEnabled
            }
            Button{
                text: "Stop"
                onClicked: {
                    sm.postEvent(SMD.Stop)
                }
                enabled: sm.stopEnabled
            }
            Button{
                text: "Pause"
                onClicked: {
                    sm.postEvent(SMD.Pause)
                }
                enabled: sm.pauseEnabled
            }
            Button{
                text: "Purge"
                onClicked: {
                    sm.postEvent(SMD.Purge)
                }
                enabled: sm.purgeEnabled
            }
        }

        ObjectEditor{
            sectionName: "mashineStateIndicatorConfig"
            configModel: mashineStateIndicatorConfig
            listMaxWidth: 800
            Component.onCompleted: {
                init()
            }
        }
    }
}
