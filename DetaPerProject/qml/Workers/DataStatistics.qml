import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ConfigElementType 1.0
import "qrc:/qmlComponents/ConfigManager"
import "qrc:/qmlComponents"
import "qrc:/qmlComponents/CustomizedUIElement"

Item {
    Flickable{
        implicitWidth: 1200
        implicitHeight: 800
        clip: true
        ColumnLayout{
            x:150
            y:200
            spacing: 40
            RowLayout {
                spacing: 40
                RowLayout {
                    x:50
                    spacing: 80
                    TrayMapEditor {
                        Layout.alignment: Qt.AlignTop
                        tableMaxWidth: 600
                        tableMaxHeight: 500
                        trayConfig: mFeedingTrayConfig
                        trayMap: mFeedingMap
                        status2OpacityMap: {
                            "Ok": 0.5
                        }
                        status2ColorMap: {
                            "Init": "gray",
                            "Ok": "limeGreen",
                            "InUse": "limeGreen"
                        }
                        statusEnumeration: ["Init", "Ng", "InUse"]
                    }
                    TrayMapEditor {
                        Layout.alignment: Qt.AlignTop
                        tableMaxWidth: 600
                        tableMaxHeight: 500
                        trayConfig: mPickArmTrayConfig
                        trayMap: mPickArmMap
                        status2OpacityMap: {
                            "Ok": 0.5
                        }
                        status2ColorMap: {
                            "Init": "gray",
                            "Ok": "limeGreen",
                            "InUse": "limeGreen"
                        }
                        statusEnumeration: ["Init", "Ng", "InUse"]
                    }
                    TrayMapEditor {
                        Layout.alignment: Qt.AlignTop
                        tableMaxWidth: 600
                        tableMaxHeight: 500
                        trayConfig: mDischargeTrayConfig
                        trayMap: mDischargeMap
                        status2OpacityMap: {
                            "Ok": 0.5
                        }
                        status2ColorMap: {
                            "Init": "gray",
                            "Ok": "limeGreen",
                            "InUse": "limeGreen"
                        }
                        statusEnumeration: ["Init", "Ng", "InUse"]
                    }
                }
                RowLayout {
                    GroupBox {
                        ColumnLayout {
                            Label {
                                text: qsTr("UPH: ") + uphHelper.uph
                                font.pixelSize: 30
                            }
                            Label {
                                text: qsTr("Total: ") + uphHelper.total
                                font.pixelSize: 30
                            }
                            Label {
                                text: qsTr("CT: ") + uphHelper.lastCircelTime + " s"
                                font.pixelSize: 30
                            }
                            Button {
                                text: qsTr("Reset")
                                onClicked: {
                                    uphHelper.reset()
                                }
                            }
                        }
                    }
                    Label {
                        font.pixelSize: 50
                        text: qsTr("Test")
                    }
                }
            }
            RowLayout
            {
                Button
                {
                    Layout.minimumWidth: 120
                    Layout.maximumWidth: 120
                    text: qsTr("一键进载板并切膜")
                    onClicked:
                    {
                        tem.runSingleCmd(mPickArm, qsTr("AutoRunTest"))
                    }
                }
                Button
                {
                    Layout.minimumWidth: 120
                    Layout.maximumWidth: 120
                    text: qsTr("进载板测试")
                    onClicked:
                    {
                        tem.runSingleCmd(mPickArm, qsTr("AskFeedCarrierBoardTest"))
                    }
                }
                Button
                {
                    Layout.minimumWidth: 120
                    Layout.maximumWidth: 120
                    text: qsTr("XY到切刀位")
                    onClicked:
                    {
                        tem.runSingleCmd(mPickArm, qsTr("MoveToCutCarrierBoard_XY"))
                    }
                }
                Button
                {
                    Layout.minimumWidth: 120
                    Layout.maximumWidth: 120
                    text: qsTr("开始切膜")
                    onClicked:
                    {
                        tem.runSingleCmd(mPickArm, qsTr("AskFeedingCutCarrierBoard"))
                    }
                }
                Button
                {
                    Layout.minimumWidth: 120
                    Layout.maximumWidth: 120
                    text: qsTr("初次剥料测试")
                    onClicked:
                    {
                        tem.runSingleCmd(mPickArm, qsTr("AskFeedingTakeMaterialAwayWithOffsetFirst"))
                    }
                }
                Button
                {
                    Layout.minimumWidth: 120
                    Layout.maximumWidth: 120
                    text: qsTr("正常剥料测试")
                    onClicked:
                    {
                        tem.runSingleCmd(mPickArm, qsTr("AskFeedingTakeMaterialAwayWithOffset"))
                    }
                }
                Button
                {
                    Layout.minimumWidth: 120
                    Layout.maximumWidth: 120
                    text: qsTr("出载板")
                    onClicked:
                    {
                        tem.runSingleCmd(mPickArm, qsTr("AskFeedingOutCarrierBoard"))
                    }
                }
            }
            ColumnLayout{
                CheckBox{
                    id:idIsLeftNozzle
                    checked: true
                    text: qsTr("使用左吸嘴")
                }
                RowLayout{
                    Button
                    {
                        Layout.minimumWidth: 300
                        Layout.maximumWidth: 120
                        text: qsTr("自动运行测试")
                        onClicked:
                        {
                            tem.runSingleCmd(mPickArm, qsTr("StartAutoRun"))
                        }
                    }
                    Button
                    {
                        Layout.minimumWidth: 300
                        Layout.maximumWidth: 120
                        text: qsTr("吸料")
                        onClicked:
                        {
                            tem.runSingleCmd(mPickArm, qsTr("PickMterial"),[idIsLeftNozzle.checked])
                        }
                    }
                    Button{
                        text: qsTr("去到取料测试")
                        Layout.minimumWidth: 270
                        Layout.maximumWidth: 270
                        width: 270
                        onClicked: {
                            tem.runSingleCmd(mPickArm, qsTr("PickMaterialTest"),[idIsLeftNozzle.checked])
                        }
                    }
                    Button{
                        text: qsTr("放料测试")
                        Layout.minimumWidth: 270
                        Layout.maximumWidth: 270
                        width: 270
                        onClicked: {
                            tem.runSingleCmd(mPickArm, qsTr("PlaceMaterialTest"),[idIsLeftNozzle.checked])
                        }
                    }
                }
                RowLayout{
                    Button
                    {
                        Layout.minimumWidth: 120
                        Layout.maximumWidth: 120
                        text: qsTr("测试按钮")
                        onClicked:
                        {
                            tem.runSingleCmd(mPickArm, qsTr("Test"))
                        }
                    }
                    Button
                    {
                        Layout.minimumWidth: 300
                        Layout.maximumWidth: 120
                        text: qsTr("测试服务器")
                        onClicked:
                        {
                            mPickArm.testAdd()
                        }
                    }
                    Button
                    {
                        Layout.minimumWidth: 300
                        Layout.maximumWidth: 120
                        text: qsTr("第一点拍照")
                        onClicked:
                        {
                            mPickArm.testGetSpace1(idIsLeftNozzle.checked);
                        }
                    }
                    Button
                    {
                        Layout.minimumWidth: 300
                        Layout.maximumWidth: 120
                        text: qsTr("第二点拍照")
                        onClicked:
                        {
                            mPickArm.testGetSpace2(idIsLeftNozzle.checked);
                        }
                    }
                    ColumnLayout{
                        Button
                        {
                            Layout.minimumWidth: 120
                            Layout.maximumWidth: 120
                            text: qsTr("进出料盘")
                            onClicked:
                            {
                                tem.runSingleCmd(mDischarge, qsTr("GetTray"),[0])
                            }
                        }
                    }
                }
            }
        }
    }
}
