import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "CustomizedUIElement"
import "ConfigManager"
import ConfigElementType 1.0

Popup {
    contentWidth: 700
    contentHeight: 700
    closePolicy: Popup.CloseOnEscape
    contentItem: ColumnLayout{
        Label{
            text: qsTr("按ESC退出")
        }

        MyTabView{
            id: visionDefinition

            deleteUnvisibleTab: false
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Component{
            id: cameraDef

            ArrayEditor{
                configModel: $cameraDef
                titleVisible: false
                Component.onCompleted: {
                    init()
                }
            }
        }
        Component{
            id: lscDef

            ArrayEditor{
                configModel: $lscDef
                titleVisible: false
                Component.onCompleted: {
                    init()
                }
            }
        }
        Component{
            id: locationDef

            ArrayEditor{
                titleVisible: false
                configModel: $visionLocationDef
                Component.onCompleted: {
                    init()
                }
            }
        }
        Component.onCompleted: {
            visionDefinition.addTab(qsTr("相机定义"), cameraDef)
            visionDefinition.addTab(qsTr("光源控制器定义"), lscDef)
            visionDefinition.addTab(qsTr("PR定义"), locationDef)
        }
    }
}
