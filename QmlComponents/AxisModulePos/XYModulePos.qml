import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../ConfigManager"
import ConfigElementType 1.0

GroupBox{
    property string moduleName: ""
    property string posName: ""
    property bool initSelfOnCompleted: true
    property bool isYZ: false

    topPadding: 20
    bottomPadding: 2

    ColumnLayout{
        RowLayout{
            ConfigRow{
                id: xPosViewer
                configName: "xPos"
                txtWidth: 90
                Component.onCompleted: {
                    if(isYZ){
                        lblName.text = "Y:"
                    }
                }
            }
            ConfigRow{
                id: yPosViewer
                configName: "yPos"
                txtWidth: 90
                Component.onCompleted: {
                    if(isYZ){
                        lblName.text = "Z:"
                    }
                }
            }
        }
        ModulePosEvent{
            id: btns
            spacing: 150
        }
    }
    Component.onCompleted: {
        if(!initSelfOnCompleted){
            return
        }

        if(title == ""){
            title = moduleName + "_" + posName
        }

        var modulePosModel = motionConfigManager.xyModulePos(moduleName, posName)
        if(modulePosModel === null){
            xPosViewer.visible = false
            yPosViewer.visible = false
            btns.visible = false
            console.error("Undefined module pos! module: " + moduleName + " pos: " + posName)
        }else{
            xPosViewer.configModel = modulePosModel
            yPosViewer.configModel = modulePosModel
            btns.configObj = modulePosModel
            xPosViewer.init(ConfigElementType.Double)
            yPosViewer.init(ConfigElementType.Double)
        }
    }
}

