import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../ConfigManager"
import ConfigElementType 1.0
import MotionElement 1.0

GroupBox{
    property string zModuleName: ""
    property string zPosName: ""
    property string xyModuleName: ""
    property string xyPosName: ""
    property bool initSelfOnCompleted: true

    property bool moveZFirst: false
    property bool moveXyFirst: false

    property bool moveXyBtnVisible: true
    property bool moveZBtnVisible: true

    ColumnLayout{
        RowLayout{
            ConfigRow{
                id: xPosViewer
                configName: "xPos"
                txtWidth: 90
            }
            ConfigRow{
                id: yPosViewer
                configName: "yPos"
                txtWidth: 90
            }
            ConfigRow{
                id: zPosViewer
                configName: "pos"
                txtWidth: 90
            }
        }
        RowLayout{
            id: btns
            property var zModuleConfigObj: null
            property var xyModuleConfigObj: null
            Button{
                enabled: userManagement.currentAuthority >= 2
                text: qsTr("Read")
                onClicked: {
                    btns.zModuleConfigObj.handleEvent("UpdatePos")
                    btns.xyModuleConfigObj.handleEvent("UpdatePos")
                }
            }
            Button{
                text: qsTr("MoveTo")
                onClicked: {
                    if(moveZFirst && moveXyFirst){
                        console.error("Logic error")
                        return
                    }
                    if(!moveZFirst && !moveXyFirst){
                        btns.zModuleConfigObj.handleEvent("MoveTo")
                        btns.xyModuleConfigObj.handleEvent("MoveTo")
                        return
                    }

                    var moveZCmd = ["runInstruction", MotionElement.SingleAxis, zModuleName, "moveTo", [zPosName, true]]
                    var moveXyCmd = ["runInstruction", MotionElement.XYModule, xyModuleName, "moveTo", [xyPosName, true]]
                    if(moveZFirst){
                        tem.runMultiCmdsHasArg(motionManager, [moveZCmd, moveXyCmd])
                    }
                    if(moveXyFirst){
                        tem.runMultiCmdsHasArg(motionManager, [moveXyCmd, moveZCmd])
                    }

                }
            }
            Button{
                visible: moveXyBtnVisible
                text: qsTr("MoveXY")
                onClicked: {
                    btns.xyModuleConfigObj.handleEvent("MoveTo")
                }
            }
            Button{
                visible: moveZBtnVisible
                text: qsTr("MoveZ")
                onClicked: {
                    btns.zModuleConfigObj.handleEvent("MoveTo")
                }
            }
        }
    }
    Component.onCompleted: {
        if(!initSelfOnCompleted){
            return
        }

        if(title == ""){
            title = xyModuleName + "_" + xyPosName + "+" + zModuleName + "_" + zPosName
        }

        var xyModulePosModel = motionConfigManager.xyModulePos(xyModuleName, xyPosName)
        var zModulePosModel = motionConfigManager.sAxisModulePos(zModuleName, zPosName)
        if(xyModulePosModel === null || zModulePosModel === null){
            xPosViewer.visible = false
            yPosViewer.visible = false
            zPosViewer.visible = false
            btns.visible = false
            if(xyModulePosModel === null){
                console.error("Undefined module pos! module: " + xyModuleName + " pos: " + xyPosName)
            }
            if(zModulePosModel === null){
                console.error("Undefined module pos! module: " + zModuleName + " pos: " + zPosName)
            }
        }else{
            xPosViewer.configModel = xyModulePosModel
            yPosViewer.configModel = xyModulePosModel
            zPosViewer.configModel = zModulePosModel
            btns.xyModuleConfigObj = xyModulePosModel
            btns.zModuleConfigObj = zModulePosModel
            xPosViewer.init(ConfigElementType.Double)
            yPosViewer.init(ConfigElementType.Double)
            zPosViewer.init(ConfigElementType.Double)

            zPosViewer.lblName.text = "zPos"
        }
    }
}

