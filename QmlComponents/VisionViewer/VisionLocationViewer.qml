import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ConfigElementType 1.0
import "../ConfigManager"

GroupBox {
    property string calibrationName: ""
    property string visionLocationName: ""
    property double resultImageSourceSizeRatio: 2
    property bool performPrResultVisible: true
    property bool calibrateBtnVisible: true
    property bool useCustomCalibrationFunc: false
    property var customCalibrationFunc: null

    property var __visionLocationConfig: null

    bottomPadding: 5
    topPadding: 5

    function performPrResult(){
        tem.runSingleCmd(visionManager, "performPrResult", [calibrationName, visionLocationName, false])
    }

    ColumnLayout{
        ConfigRow{
            id: prFileNameCfg
            configName: "prFileName"
            txtWidth: 300
        }
        RowLayout{
            spacing: 15

            Flickable {
                width: 160
                height: 120
                clip: true
                DragableScalableImage {
                    id: prResultImage
                    width: parent.width
                    height: parent.height
                    sourceSize: Qt.size(width * resultImageSourceSizeRatio, height * resultImageSourceSizeRatio)
                    clip: true
                    cache: false
                    fillMode: Image.PreserveAspectFit
                }
            }
            ColumnLayout{
                Layout.alignment: Qt.AlignBottom
                Layout.bottomMargin: 10

                RowLayout{
                    Slider{
                        id: lightBrightnessSlider
                        implicitWidth: 120
                        implicitHeight: 25
                        stepSize: 1
                        from: 0
                        to: 255
                        onValueChanged: {
                            __visionLocationConfig.setLightBrightness(value)
                        }
                    }
                    Label {
                        text: Math.round(lightBrightnessSlider.value)
                    }
                }
                RowLayout{
                    Slider{
                        id: lightBrightnessSlider2
                        implicitWidth: 120
                        implicitHeight: 25
                        stepSize: 1
                        from: 0
                        to: 255
                        onValueChanged: {
                            __visionLocationConfig.setSecondLightBrightness(value)
                        }
                    }
                    Label {
                        text: Math.round(lightBrightnessSlider2.value)
                    }
                }
            }
        }
        RowLayout{
            Button{
                text: qsTr("更新图片")
                onClicked: {
                    updatePrResultImage()
                }
            }
            Button{
                visible: calibrateBtnVisible
                text: qsTr("校正")
                onClicked: {
                    if(useCustomCalibrationFunc){
                        customCalibrationFunc()
                    }else{
                        tem.runSingleCmd(visionManager, "calibrate", [calibrationName])
                    }
                }
            }
            Button{
                text: qsTr("执行PR")
                onClicked: {
                    tem.runSingleCmd(visionManager, "performPr", [visionLocationName])
                }
            }
            Button{
                visible: performPrResultVisible
                text: qsTr("执行PR结果")
                onClicked: {
                    performPrResult()
                }
            }
            RoundButton{
                icon.source: "qrc:/commonicons/start.png"
                icon.color: "limeGreen"
                ToolTip.text: qsTr("StartContinuallyPerformPr")
                ToolTip.visible: hovered
                onClicked: {
                    tem.runSingleCmd(visionManager, "startContinuallyPerformPr", [visionLocationName])
                }
            }
            RoundButton{
                icon.source: "qrc:/commonicons/stop.png"
                icon.color: "red"
                ToolTip.text: qsTr("StopContinuallyCalculation")
                ToolTip.visible: hovered
                onClicked: {
                    visionManager.stopContinuallyCalculation(visionLocationName)
                }
            }
        }
    }

    Connections{
        id: connVisionLocationConfigChanged
        target: null

        onLightBrightnessChanged: {
            if(!lightBrightnessSlider.pressed){
                lightBrightnessSlider.value = lightBrightness
            }
        }
        onSecondLightBrightnessChanged: {
            if(!lightBrightnessSlider2.pressed){
                lightBrightnessSlider2.value = secondLightBrightness
            }
        }
        onPrResultImageChanged: {
            updatePrResultImage()
        }
    }

    function updatePrResultImage(){
        var resultImage = __visionLocationConfig.prResultImage()
        prResultImage.source = ""
        if(resultImage === ""){
            prResultImage.source = "qrc:/commonicons/no-image.png"
        }else{
            prResultImage.source = resultImage
        }
    }

    Component.onCompleted: {
        if(title == ""){
            title = visionLocationName
        }
        var vlConfig = visionConfigManager.getVisionLocationConfig(visionLocationName)
        if(vlConfig === null){
            console.error("Undefined vision location:" + visionLocationName)
            prFileNameCfg.visible = false
            prResultImage.source = "qrc:/commonicons/no-image.png"
        }else{
            __visionLocationConfig = vlConfig
            prFileNameCfg.configModel = __visionLocationConfig
            prFileNameCfg.init(ConfigElementType.Other)
            updatePrResultImage()
            lightBrightnessSlider.value = __visionLocationConfig.lightBrightness
            lightBrightnessSlider2.value = __visionLocationConfig.secondLightBrightness
            connVisionLocationConfigChanged.target = __visionLocationConfig
        }
    }
}
