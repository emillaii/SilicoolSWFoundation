import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "./MoveProtection"

Window {
    visible: true
    width: 1000
    height: 800
    title: qsTr("Hello World")

    RowLayout{
        Layout.fillWidth: true
        Layout.fillHeight: true

        MoveProtectionEditor{
            Layout.fillWidth: true
            Layout.fillHeight: true

            configModel: collisionGroupConfigs
            Component.onCompleted: {
                init()
            }
        }
//        MoveProtectionEditor{
//            configModel: collisionGroupConfigs
//            Component.onCompleted: {
//                init()
//            }
//        }
    }
}
