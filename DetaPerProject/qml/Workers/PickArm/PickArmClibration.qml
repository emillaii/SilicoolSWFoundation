import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "qrc:/qmlComponents/ConfigManager"
import "qrc:/qmlComponents/AxisModulePos"
import "qrc:/qmlComponents/VisionViewer"
import "qrc:/qmlComponents/CustomizedUIElement"
import "qrc:/qmlComponents/MotionViewer"

ColumnLayout {

        GroupBox{
        RowLayout{
            GroupBox
            {
                ColumnLayout
                {
                    spacing: 10
                    VisionLocationViewer{
                        visionLocationName: "VisionLocationForScale"
                        calibrationName: "CalibrationForScale"
                    }
                    RowLayout
                    {
                        ConfigRow{
                            configName: "PA_L_Offset_X"
                            configModel: mPickArmConfig
                            Component.onCompleted: {
                                autoInit()
                            }
                        }
                        ConfigRow{
                            configName: "PA_R_Offset_X"
                            configModel: mPickArmConfig
                            Component.onCompleted: {
                                autoInit()
                            }
                        }
                    }

                    RowLayout{
                        ConfigRow{
                            configName: "PA_L_Offset_Y"
                            configModel: mPickArmConfig
                            Component.onCompleted: {
                                autoInit()
                            }
                        }
                        ConfigRow{
                            configName: "PA_R_Offset_Y"
                            configModel: mPickArmConfig
                            Component.onCompleted: {
                                autoInit()
                            }
                        }
                    }
                }
            }
            GroupBox{
                width: 680
                height: 500
                ColumnLayout{
                    spacing: 5
                    RowLayout{
                        spacing:70
                        ConfigRow{
                            configName: "PACalibStepCount"
                            configModel: mPickArmConfig
                            Component.onCompleted: {
                                autoInit()
                            }
                        }
                        ConfigRow{
                            configName: "CalibrateStep"
                            configModel: mPickArmConfig
                            Component.onCompleted: {
                                autoInit()
                            }
                        }
                    }

                    CheckBox
                    {
                        id:idIsLeftNozzle
                        text: qsTr("IsLeftNozzle")
                        checked: true
                    }
                    RowLayout
                    {
                        spacing: 10
                        XYModulePos
                        {
                            moduleName: "PAXY"
                            posName: "CalibrateCameraToNozzlePrPos"
                            Layout.minimumWidth: 325
                            Layout.maximumWidth: 325
                            Layout.minimumHeight: 140
                            Layout.maximumHeight: 140
                        }
                        ColumnLayout
                        {
                            spacing: 5
                            Button{
                                text: qsTr("XY到相机拍照位")
                                Layout.minimumWidth: 160
                                Layout.maximumWidth: 160
                                onClicked: {
                                    tem.runSingleCmd(mPickArm, "XY_MoveToTakePictureForGlassPos")
                                }
                            }
                            Button{
                                text: qsTr("旋转中心标定")
                                Layout.minimumWidth: 160
                                Layout.maximumWidth: 160
                                onClicked: {
                                    if(idIsLeftNozzle.checked)
                                    {
                                        tem.runSingleCmd(mPickArm, "CalibrateCameraToNozzleOffset", [true])
                                    }
                                    else
                                    {
                                        tem.runSingleCmd(mPickArm, "CalibrateCameraToNozzleOffset", [false])
                                    }
                                }
                            }
                            Button{
                                text: qsTr("吸嘴到吸标定片位")
                                Layout.minimumWidth: 160
                                Layout.maximumWidth: 160
                                onClicked: {
                                    if(idIsLeftNozzle.checked)
                                    {
                                        tem.runSingleCmd(mPickArm, "XY_MoveToGetGlassPos", [true])
                                    }
                                    else
                                    {
                                        tem.runSingleCmd(mPickArm, "XY_MoveToGetGlassPos", [false])
                                    }
                                }
                            }
                        }
                    }
                    RowLayout{
                        Button{
                            text: qsTr("拍照并吸取玻璃片")
                            Layout.minimumWidth: 250
                            Layout.maximumWidth: 250
                            onClicked: {
                                tem.runSingleCmd(mPickArm, "PickGlassWithOffset", [idIsLeftNozzle.checked])
                            }
                        }
                        Button{
                            text: qsTr("放玻璃片到视野中心")
                            Layout.minimumWidth: 250
                            Layout.maximumWidth: 250
                            onClicked:
                            {
                                tem.runSingleCmd(mPickArm, "PlaceGlassWithOffset", [idIsLeftNozzle.checked])
                            }
                        }
                    }
                }
            }
        }
    }
    RowLayout
    {
        spacing: 20
        SAxisModulePos{
            moduleName: "PAZL"
            posName: "LPAPickCalibGlassPosUp"
            Layout.minimumWidth: 311
            Layout.maximumWidth: 311
            Layout.minimumHeight: 100
            Layout.maximumHeight: 100
        }
        SAxisModulePos{
            moduleName: "PAZL"
            posName: "LPAPickCalibGlassPosDown"
            Layout.minimumWidth: 311
            Layout.maximumWidth: 311
            Layout.minimumHeight: 100
            Layout.maximumHeight: 100
        }
        SingleVacuum{
            name: "PickVac_L"
            Layout.minimumWidth: 311
            Layout.maximumWidth: 311
            Layout.minimumHeight: 100
            Layout.maximumHeight: 100
        }
    }
    RowLayout
    {
        spacing: 20
        SAxisModulePos{
            moduleName: "PAZR"
            posName: "RPAPickCalibGlassPosUp"
            Layout.minimumWidth: 311
            Layout.maximumWidth: 311
            Layout.minimumHeight: 100
            Layout.maximumHeight: 100
        }
        SAxisModulePos{
            moduleName: "PAZR"
            posName: "RPAPickCalibGlassPosDown"
            Layout.minimumWidth: 311
            Layout.maximumWidth: 311
            Layout.minimumHeight: 100
            Layout.maximumHeight: 100
        }
        SingleVacuum{
            name: "PickVac_R"
            Layout.minimumWidth: 311
            Layout.maximumWidth: 311
            Layout.minimumHeight: 100
            Layout.maximumHeight: 100
        }
    }
}
