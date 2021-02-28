import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../ConfigManager"
import ConfigElementType 1.0

GroupBox{
    property string moduleName: ""
    property string posName: ""
    property bool isUVW: false

    ColumnLayout{
        spacing: 0

        RowLayout{
            ConfigRow{
                id: xPosViewer
                configName: "xPos"
                txtWidth: 90
                Component.onCompleted: {
                    if(isUVW){
                        lblName.text = "U:"
                    }
                }
            }
            ConfigRow{
                id: yPosViewer
                configName: "yPos"
                txtWidth: 90
                Component.onCompleted: {
                    if(isUVW){
                        lblName.text = "V:"
                    }
                }
            }
            ConfigRow{
                id: zPosViewer
                configName: "zPos"
                txtWidth: 90
                Component.onCompleted: {
                    if(isUVW){
                        lblName.text = "W:"
                    }
                }
            }
        }
        ModulePosEvent{
            id: btns
            spacing: 300
        }
    }
    Component.onCompleted: {
        if(title == ""){
            title = moduleName + "_" + posName
        }

        var modulePosModel = motionConfigManager.xyzModulePos(moduleName, posName)
        if(modulePosModel === null){
            xPosViewer.visible = false
            yPosViewer.visible = false
            zPosViewer.visible = false
            btns.visible = false
            console.error("Undefined module pos! module: " + moduleName + " pos: " + posName)
        }else{
            xPosViewer.configModel = modulePosModel
            yPosViewer.configModel = modulePosModel
            zPosViewer.configModel = modulePosModel
            btns.configObj = modulePosModel
            xPosViewer.init(ConfigElementType.Double)
            yPosViewer.init(ConfigElementType.Double)
            zPosViewer.init(ConfigElementType.Double)
        }
    }
}

