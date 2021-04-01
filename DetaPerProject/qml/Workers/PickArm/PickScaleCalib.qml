import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "qrc:/qmlComponents/ConfigManager"
import "qrc:/qmlComponents/AxisModulePos"
import "qrc:/qmlComponents/VisionViewer"
import "qrc:/qmlComponents/CustomizedUIElement"
import "qrc:/qmlComponents/MotionViewer"

ColumnLayout {
    RowLayout{
        ColumnLayout
        {
            VisionLocationViewer{
                visionLocationName: "VisionLocationForScale"
                calibrationName: "CalibrationForScale"
            }
            ConfigRow{
                configName: "PA_L_Offset_X"
                configModel: mPickArmConfig
                Component.onCompleted: {
                    autoInit()
                }
            }
            ConfigRow{
                configName: "PA_L_Offset_Y"
                configModel: mPickArmConfig
                Component.onCompleted: {
                    autoInit()
                }
            }
        }

        GroupBox{
            width: 700
            height: 500
            ColumnLayout{
                spacing: 20
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

                CheckBox{
                    id:idIsLeftNozzle
                    text: qsTr("IsLeftNozzle")
                    checked: true
                }
                Button{
                    text: qsTr("Calibration For CameraToNozzleOffset")
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
                    text: qsTr("Vision测试")
                    onClicked: {
                        tem.runSingleCmd(mPickArm,"VisionTest");
                    }
                }
                RowLayout{
                    spacing: 10
                    ConfigRow{
                        configName: "PA_L_Scale"
                        configModel: mPickArmConfig
                        Component.onCompleted: {
                            autoInit()
                        }
                    }
                    //                    ConfigRow{
                    //                        configName: "PA_L_Offset_Y"
                    //                        configModel: mPickArmConfig
                    //                        Component.onCompleted: {
                    //                            autoInit()
                    //                        }
                    //                    }
                }

                RowLayout{
                    Button{
                        text: qsTr("Pick Glass With Offset")
                        Layout.minimumWidth: 270
                        Layout.maximumWidth: 270
                        width: 270
                        onClicked: {
                            tem.runSingleCmd(mPickArm, "PickGlassWithOffset", [idIsLeftNozzle.checked])
                        }
                    }
                    Button{
                        text: qsTr("Place Glass")
                        Layout.minimumWidth: 270
                        Layout.maximumWidth: 270
                        width: 270
                        onClicked:
                        {
                            tem.runSingleCmd(mPickArm, "PlaceGlassWithOffset", [idIsLeftNozzle.checked])
                        }
                    }
                }
            }
        }
    }

    RowLayout{
        SAxisModulePos{
            moduleName: "PAZL"
            posName: "StandbyPos"
            Layout.minimumWidth: 311
            Layout.maximumWidth: 311
            Layout.minimumHeight: 140
            Layout.maximumHeight: 140
        }
        SAxisModulePos{
            moduleName: "PAZR"
            posName: "StandbyPos"
            Layout.minimumWidth: 311
            Layout.maximumWidth: 311
            Layout.minimumHeight: 140
            Layout.maximumHeight: 140
        }
        XYModulePos{
            moduleName: "PAXY"
            posName: "CalibrateCameraToNozzlePrPos"
            Layout.minimumWidth: 311
            Layout.maximumWidth: 311
            Layout.minimumHeight: 140
            Layout.maximumHeight: 140
        }
    }

    RowLayout{
        //        XYModulePos{
        //            moduleName: "PAXY"
        //            posName: "PaGetMaterial_L"
        //        }
        //        XYModulePos{
        //            moduleName: "PAXY"
        //            posName: "PaGetMaterial_R"
        //        }
        SingleVacuum{
            name: "PickVac_L"
            Layout.minimumWidth: 311
            Layout.maximumWidth: 311
            Layout.minimumHeight: 140
            Layout.maximumHeight: 140
        }
        SingleVacuum{
            name: "PickVac_R"
            Layout.minimumWidth: 311
            Layout.maximumWidth: 311
            Layout.minimumHeight: 140
            Layout.maximumHeight: 140
        }
    }
}
