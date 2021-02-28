import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../CustomizedUIElement"
import "../ConfigManager"

Item {
    property var maxListHeight: 700

    Component{
        id: cameraConfigger
        ArrayEditor{
            titleVisible: false
            configModel: cameraConfigs
            listMaxHeight: maxListHeight
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: calibrationConfigger
        ArrayEditor{
            titleVisible: false
            configModel: calibrationConfigs
            listMaxHeight: maxListHeight
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: visionLocationConfigger
        ArrayEditor{
            titleVisible: false
            configModel: visionLocationConfigs
            listMaxHeight: maxListHeight
            Component.onCompleted: {
                init()
            }
        }
    }

    MyTabView{
        id: viewer
        anchors.fill: parent
        trContext: "VisionConfigger"
    }

    Component.onCompleted: {
        viewer.addTab(qsTr("Camera"), cameraConfigger)
        viewer.addTab(qsTr("Calibration"), calibrationConfigger)
        viewer.addTab(qsTr("VisionLocation"), visionLocationConfigger)
    }
}
