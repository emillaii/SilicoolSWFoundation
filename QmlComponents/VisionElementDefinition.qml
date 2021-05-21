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

    Popup{
        id: popupRenameVision

        x: 900
        y: 30
        closePolicy: Popup.CloseOnEscape

        ColumnLayout{
            Label{
                text: qsTr("按'ESC'退出！")
            }
            Label{
                text: qsTr("请确认本次开启软件前，renameManagerConfig.json中，将enable设为true！")
            }
            Label{
                text: qsTr("重命名完成后，请改回false，以免影响运行性能!")
            }
            Label{
                text: qsTr("重命名完成后，请不要再修改任何配置，并立即重启软件！")
            }
            RowLayout{
                Label{
                    text: qsTr("元素类型:")
                }
                ComboBox{
                    id: cmbElementType
                    model: ["Camera", "VisionLocation"]
                }
            }
            RowLayout{
                Label{
                    text: qsTr("原名称:")
                }
                TextField{
                    id: txtOldName
                    implicitWidth: 300
                    selectByMouse: true
                }
            }
            RowLayout{
                Label{
                    text: qsTr("新名称:")
                }
                TextField{
                    id: txtNewName
                    implicitWidth: 300
                    selectByMouse: true
                }
            }
            Button{
                text: qsTr("重命名")
                onClicked: {
                    if(cmbElementType.currentText == ""){
                        return
                    }
                    if(cmbElementType.currentText == "Camera"){
                        visionManager.renameCamera(txtOldName.text, txtNewName.text)
                    }
                    else if(cmbElementType.currentText == "VisionLocation"){
                        visionManager.renamePR(txtOldName.text, txtNewName.text)
                    }
                    txtOldName.clear()
                    txtNewName.clear()
                }
            }
        }
    }

    contentItem: ColumnLayout{
        RowLayout{
            Label{
                text: qsTr("按ESC退出")
            }
            Button{
                text: qsTr("重命名")
                onClicked: {
                    popupRenameVision.open()
                }
            }
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
