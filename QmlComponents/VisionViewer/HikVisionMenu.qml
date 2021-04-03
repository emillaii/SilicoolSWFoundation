import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "qrc:/qmlComponents/ConfigManager"

Menu{
    title: qsTr("HikVision")

    MenuItem {
        text: qsTr("配置")
        onClicked: {
            popupHikVisionConfig.open()
        }
    }
    MenuItem {
        text: qsTr("加载解决方案")

        Popup{
            id: popupHikVisionConfig
            x: 100
            y: 100
            contentItem: ObjectEditor{
                titleVisible: false
                configModel: hikVisionConfig
                Component.onCompleted: {
                    init()
                }
            }
        }

        onClicked: {
            tem.runSingleCmd(hikVision, "loadSolution")
        }
    }
    MenuItem {
        text: qsTr("打开VisionMaster")
        onClicked: {
            tem.runSingleCmd(hikVision, "startVisionMaster", [true])
        }
    }
    MenuItem {
        text: qsTr("关闭VisionMaster")
        onClicked: {
            tem.runSingleCmd(hikVision, "startVisionMaster", [false])
        }
    }
    MenuItem {
        text: qsTr("显示VisionMaster")
        onClicked: {
            tem.runSingleCmd(hikVision, "showVisionMaster", [true])
        }
    }
    MenuItem {
        text: qsTr("隐藏VisionMaster")
        onClicked: {
            tem.runSingleCmd(hikVision, "showVisionMaster", [false])
        }
    }
    MenuItem {
        text: qsTr("打印所有流程模块信息")
        onClicked: {
            tem.runSingleCmd(hikVision, "printProcessModuleIds")
        }
    }
}
