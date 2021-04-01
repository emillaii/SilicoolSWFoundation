import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ConfigElementType 1.0
import "qrc:/qmlComponents/CustomizedUIElement"
import "qrc:/qmlComponents/AxisModulePos"
import "qrc:/qmlComponents/ConfigManager"
import "qrc:/qmlComponents/MotionViewer"
import "qrc:/qml/Workers/PickArm"

Item {
    MyTabView {
        indent: 2
        trContext: qsTr("PickArmViewer")
        Component {
            id: page1
            Item
            {
                Flickable{
                    implicitWidth: 1200
                    implicitHeight: 800
                    clip: true
                    ColumnLayout {
                        width: 1100
                        Button{
                            id:idPickArmClibration
                            text: qsTr("吸头比例标定和位置标定")
                            Layout.fillWidth: true
                            onClicked: {
                                idPickArmCalib.visible = !idPickArmCalib.visible
                            }
                        }
                        GroupBox{
                            id:idPickArmCalib
                            visible: false
                            PickArmClibration{}
                        }
                        Button{
                            id:idPickMaterial
                            text: qsTr("取料模板设置")
                            Layout.fillWidth: true
                            onClicked: {
                                idPickMaterialQml.visible = !idPickMaterialQml.visible
                            }
                        }
                        GroupBox{
                            id:idPickMaterialQml
                            visible: true
                            PickMaterialVisionLocationQml{}
                        }
                        Button{
                            id:idPlaceMaterial
                            text: qsTr("放料模板设置")
                            Layout.fillWidth: true
                            onClicked: {
                                idPlaceMaterialQml.visible = !idPlaceMaterialQml.visible
                            }
                        }
                        GroupBox{
                            id:idPlaceMaterialQml
                            visible: true
                            PlaceMaterialVisionLocationQml{}
                        }
                    }
                }
            }
        }
        Component {
            id: page2
            Flickable{
                implicitWidth: 1200
                implicitHeight: 780
                clip: true
                Item {
                    ColumnLayout {
                        width: 1100
                        Button{
                            id:idPickArmPickArmPosSet
                            text: qsTr("PAXY位置和补偿设置")
                            Layout.fillWidth: true
                            onClicked: {
                                gbPickArmPosSet.visible = !gbPickArmPosSet.visible
                            }
                        }

                        GroupBox{
                            id:gbPickArmPosSet
                            Layout.minimumWidth: 1100
                            visible: true
                            ColumnLayout{
                                x:60
                                spacing: 10
                                RowLayout{
                                    XYModulePos{
                                        moduleName: qsTr("PAXY")
                                        posName: qsTr("TakePictureForPickPos")
                                        Layout.minimumWidth: 325
                                        Layout.maximumWidth: 325
                                        Layout.minimumHeight: 140
                                        Layout.maximumHeight: 140
                                    }
                                    ColumnLayout
                                    {
                                        ConfigRow{
                                            configName: qsTr("GetMaterialFixOffset_X")
                                            configModel: mPickArmConfig
                                            Component.onCompleted: {
                                                autoInit()
                                            }
                                        }
                                        ConfigRow{
                                            configName: qsTr("GetMaterialFixOffset_Y")
                                            configModel: mPickArmConfig
                                            Component.onCompleted: {
                                                autoInit()
                                            }
                                        }
                                        ConfigRow{
                                            configName: qsTr("GetMaterialFixOffset_Theta")
                                            configModel: mPickArmConfig
                                            Component.onCompleted: {
                                                autoInit()
                                            }
                                        }
                                    }
                                    ColumnLayout
                                    {
                                        ConfigRow{
                                            configName: qsTr("PlaceMaterialFixOffset_X")
                                            configModel: mPickArmConfig
                                            Component.onCompleted: {
                                                autoInit()
                                            }
                                        }
                                        ConfigRow{
                                            configName: qsTr("PlaceMaterialFixOffset_Y")
                                            configModel: mPickArmConfig
                                            Component.onCompleted: {
                                                autoInit()
                                            }
                                        }
                                        ConfigRow{
                                            configName: qsTr("PlaceMaterialFixOffset_Theta")
                                            configModel: mPickArmConfig
                                            Component.onCompleted: {
                                                autoInit()
                                            }
                                        }
                                    }
                                }

                            }
                        }

                        Button{
                            text: qsTr("PAZ位置设置")
                            Layout.fillWidth: true
                            onClicked: {
                                gbPaPosSet.visible = !gbPaPosSet.visible
                            }
                        }
                        GroupBox
                        {
                            id: gbPaPosSet
                            Layout.minimumWidth: 1100
                            visible: false
                            ColumnLayout{
                                spacing: 20
                                RowLayout
                            {
                                spacing: 20
                                SAxisModulePos{
                                    moduleName: "PAZL"
                                    posName: "LPaPickMaterialPosUp"
                                    Layout.minimumWidth: 311
                                    Layout.maximumWidth: 311
                                    Layout.minimumHeight: 100
                                    Layout.maximumHeight: 100
                                }
                                SAxisModulePos{
                                    moduleName: "PAZL"
                                    posName: "LPaPickMaterialPosDown"
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
                                    posName: "RPaPickMaterialPosUp"
                                    Layout.minimumWidth: 311
                                    Layout.maximumWidth: 311
                                    Layout.minimumHeight: 100
                                    Layout.maximumHeight: 100
                                }
                                SAxisModulePos{
                                    moduleName: "PAZR"
                                    posName: "RPaPickMaterialPosDown"
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
                            RowLayout
                            {
                                spacing: 20
                                SAxisModulePos{
                                    moduleName: "PAZL"
                                    posName: "LPaPlaceMaterialPosUp"
                                    Layout.minimumWidth: 311
                                    Layout.maximumWidth: 311
                                    Layout.minimumHeight: 100
                                    Layout.maximumHeight: 100
                                }
                                SAxisModulePos{
                                    moduleName: "PAZL"
                                    posName: "LPaPlaceMaterialPosDown"
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
                                    posName: "RPaPlaceMaterialPosUp"
                                    Layout.minimumWidth: 311
                                    Layout.maximumWidth: 311
                                    Layout.minimumHeight: 100
                                    Layout.maximumHeight: 100
                                }
                                SAxisModulePos{
                                    moduleName: "PAZR"
                                    posName: "RPaPlaceMaterialPosDown"
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
                           }
                        Button{
                            text: qsTr("PAθ位置设置")
                            Layout.fillWidth: true
                            onClicked: {
                                gbPaPosSet1.visible = !gbPaPosSet1.visible
                            }
                        }
                       GroupBox
                       {
                           id:gbPaPosSet1
                           Layout.minimumWidth: 1100
                           visible: false
                           ColumnLayout
                           {
                               spacing: 20
                               RowLayout
                               {
                                   spacing: 20
                                   SAxisModulePos{
                                       moduleName: "PAθL"
                                       posName: "StandbyPos"
                                       Layout.minimumWidth: 311
                                       Layout.maximumWidth: 311
                                       Layout.minimumHeight: 100
                                       Layout.maximumHeight: 100
                                   }
                                   SAxisModulePos{
                                       moduleName: "PAθR"
                                       posName: "StandbyPos"
                                       Layout.minimumWidth: 311
                                       Layout.maximumWidth: 311
                                       Layout.minimumHeight: 100
                                       Layout.maximumHeight: 100
                                   }
                               }
                               }
                           }
                        ColumnLayout{
                            RowLayout{
                                CheckBox{
                                    id:idIsLeftNozzle
                                    checked: true
                                    text: qsTr("使用左吸嘴")
                                }
                                Button{
                                    text: qsTr("去到拍盘子的位置")
                                    Layout.minimumWidth: 270
                                    Layout.maximumWidth: 270
                                    width: 270
                                    onClicked: {
                                        tem.runSingleCmd(mPickArm, qsTr("MoveToTakeOutTrayPicturePos"))
                                    }
                                }
                                Button{
                                    text: qsTr("拍照并取料")
                                    Layout.minimumWidth: 270
                                    Layout.maximumWidth: 270
                                    width: 270
                                    onClicked: {
                                        tem.runSingleCmd(mPickArm, qsTr("PickMaterialTest"),[idIsLeftNozzle.checked])
                                    }
                                }
                                Button{
                                    text: qsTr("拍照并放料")
                                    Layout.minimumWidth: 270
                                    Layout.maximumWidth: 270
                                    width: 270
                                    onClicked: {
                                        tem.runSingleCmd(mPickArm, qsTr("PlaceMaterialTest"),[idIsLeftNozzle.checked])
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        Component {
            id: page3
            Item
            {
                ColumnLayout
                {
                    x:30
                    y:80
                    spacing: 30
                    RowLayout
                    {
                        y:50.0
                        spacing: 20
                        Button
                        {
                            Layout.minimumWidth: 120
                            Layout.maximumWidth: 120
                            text: qsTr("进载板测试")
                            onClicked:
                            {
                                tem.runSingleCmd(mPickArm, qsTr("AskFeedCarrierBoardTest"));
                            }
                        }
                        Button
                        {
                            Layout.minimumWidth: 120
                            Layout.maximumWidth: 120
                            text: qsTr("XY到切刀位")
                            onClicked:
                            {
                                tem.runSingleCmd(mPickArm, qsTr("MoveToCutCarrierBoard_XY"));
                            }
                        }
                        Button
                        {
                            Layout.minimumWidth: 120
                            Layout.maximumWidth: 120
                            text: qsTr("开始切膜")
                            onClicked:
                            {
                                tem.runSingleCmd(mPickArm, qsTr("AskFeedingCutCarrierBoard"));
                            }
                        }
                        Button
                        {
                            Layout.minimumWidth: 120
                            Layout.maximumWidth: 120
                            text: qsTr("初次剥料测试")
                            onClicked:
                            {
                                tem.runSingleCmd(mPickArm, qsTr("AskFeedingTakeMaterialAwayWithOffsetFirst"));
                            }
                        }
                        Button
                        {
                            Layout.minimumWidth: 120
                            Layout.maximumWidth: 120
                            text: qsTr("正常剥料测试")
                            onClicked:
                            {
                                tem.runSingleCmd(mPickArm, qsTr("AskFeedingTakeMaterialAwayWithOffset"));
                            }
                        }
                        Button
                        {
                            Layout.minimumWidth: 120
                            Layout.maximumWidth: 120
                            text: qsTr("出载板")
                            onClicked:
                            {
                                tem.runSingleCmd(mPickArm, qsTr("AskFeedingOutCarrierBoard"));
                            }
                        }
                    }
                    RowLayout
                    {
                        x:parent.top
                        spacing: 20
                        Button
                        {
                            Layout.minimumWidth: 120
                            Layout.maximumWidth: 120
                            text: qsTr("PA去初始位");
                            onClicked:
                            {
                                tem.runSingleCmd(mPickArm, qsTr("MoveToStandbyPos"), [false]);
                            }
                        }
                        Button
                        {
                            Layout.minimumWidth: 120
                            Layout.maximumWidth: 120
                            text: qsTr("PA去拍物料位")
                            onClicked:
                            {
                                tem.runSingleCmd(mPickArm, qsTr("MoveToTakeMaterialPicturePos"));
                            }
                        }
                        Button
                        {
                            Layout.minimumWidth: 120
                            Layout.maximumWidth: 120
                            text: qsTr("PA去放料拍照位")
                            onClicked:
                            {
                                tem.runSingleCmd(mPickArm, "MoveToTakeOutTrayPicturePos");
                            }
                        }
                    }
                }
            }
        }

        Component {
            id: page4
            Item
            {
                TestForKeyboard
                {

                }

            }
        }

        Component.onCompleted: {
            addTab(qsTr("模板和标定"), page1)
            addTab(qsTr("吸放料调试"), page2)
            addTab(qsTr("手动动作调试"), page3)
            addTab(qsTr("TestKeyBoard"), page4)
        }
    }
}
