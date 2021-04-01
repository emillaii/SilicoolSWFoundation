import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ConfigElementType 1.0
import "qrc:/qmlComponents/CustomizedUIElement"
import "qrc:/qmlComponents/ConfigManager"
import "qrc:/qmlComponents/AxisModulePos"
import "qrc:/qmlComponents/MotionViewer"

Item {
    Flickable{
        implicitWidth: 1200
        implicitHeight: 800
        clip: true
        MyTabView {
            indent: 2
            trContext: qsTr("FeedingViewer")
            Component
            {
                id: page1
                Item
                {
                    ScrollView
                    {
                        ColumnLayout
                        {
                            width: 1050
                            Button
                            {
                                id:machineOperationSwitch
                                text: qsTr("进载板步长和速度设置")
                                Layout.fillWidth: true
                                onClicked:
                                {
                                    operation.visible = !operation.visible
                                }
                            }
                            GroupBox{
                                id: operation
                                visible: false
                                Layout.fillWidth: true
                                ColumnLayout{
                                    spacing: 10
                                    RowLayout
                                    {
                                        x:parent.left
                                        y:300
                                        spacing: 20
                                        ColumnLayout
                                        {
                                            RowLayout
                                            {
                                                ConfigRow
                                                {
                                                    configName: qsTr("FeedCarrierBoardStep")
                                                    configModel: mFeedingConfig
                                                    Component.onCompleted:
                                                    {
                                                        autoInit()
                                                    }
                                                }
                                                ConfigRow
                                                {
                                                    configName: qsTr("FeedingStep")
                                                    configModel: mFeedingConfig
                                                    Component.onCompleted:
                                                    {
                                                        autoInit()
                                                    }
                                                }
                                            }
                                            RowLayout{
                                                ConfigRow
                                                {
                                                    configName: qsTr("FeedCarrierBoardSpeed")
                                                    configModel: mFeedingConfig
                                                    Component.onCompleted:
                                                    {
                                                        autoInit()
                                                    }
                                                }
                                                ConfigRow
                                                {
                                                    configName: qsTr("DetaperSpeed")
                                                    configModel: mFeedingConfig
                                                    Component.onCompleted:
                                                    {
                                                        autoInit()
                                                    }
                                                }
                                                ConfigRow
                                                {
                                                    configName: qsTr("OutCarrierBoardSpeed")
                                                    configModel: mFeedingConfig
                                                    Component.onCompleted:
                                                    {
                                                        autoInit()
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            Button
                            {
                                id:idFeedCarierBoardPosSet
                                text: qsTr("进载板点位设置")
                                Layout.fillWidth: true
                                onClicked:
                                {
                                    gbFeedCarierBoardPosSet.visible = !gbFeedCarierBoardPosSet.visible
                                }
                            }
                            GroupBox
                            {
                                id: gbFeedCarierBoardPosSet
                                visible: true
                                Layout.fillWidth: true
                                ColumnLayout
                                {
                                    spacing: 20
                                    RowLayout
                                    {
                                        spacing: 20
                                        SAxisModulePos{
                                            moduleName: qsTr("ElevatorX")
                                            posName: qsTr("StandbyPos")
                                            Layout.minimumWidth: 280
                                            Layout.maximumWidth: 280
                                            Layout.minimumHeight: 100
                                            Layout.maximumHeight: 100
                                        }
                                        SAxisModulePos{
                                            moduleName: qsTr("ElevatorX")
                                            posName: qsTr("OutMagPos")
                                            Layout.minimumWidth: 280
                                            Layout.maximumWidth: 280
                                            Layout.minimumHeight: 100
                                            Layout.maximumHeight: 100
                                        }
                                    }
                                    RowLayout
                                    {
                                        spacing: 20
                                        SAxisModulePos{
                                            moduleName: qsTr("ElevatorZ")
                                            posName: qsTr("StandbyPos")
                                            Layout.minimumWidth: 280
                                            Layout.maximumWidth: 280
                                            Layout.minimumHeight: 100
                                            Layout.maximumHeight: 100
                                        }
                                        SAxisModulePos{
                                            moduleName: qsTr("ElevatorZ")
                                            posName: qsTr("FirstCarierBoardPos")
                                            Layout.minimumWidth: 280
                                            Layout.maximumWidth: 280
                                            Layout.minimumHeight: 100
                                            Layout.maximumHeight: 100
                                        }
                                    }
                                    RowLayout
                                    {
                                        spacing: 20
                                        SAxisModulePos{
                                            moduleName: qsTr("ElevatorZ")
                                            posName: qsTr("SecondMagStandbyPos")
                                            Layout.minimumWidth: 280
                                            Layout.maximumWidth: 280
                                            Layout.minimumHeight: 100
                                            Layout.maximumHeight: 100
                                        }
                                        SAxisModulePos{
                                            moduleName: qsTr("ElevatorZ")
                                            posName: qsTr("OutMagPos")
                                            Layout.minimumWidth: 280
                                            Layout.maximumWidth: 280
                                            Layout.minimumHeight: 100
                                            Layout.maximumHeight: 100
                                        }
                                    }
                                }
                            }

                            Button
                            {
                                id:idDetaperPosSet
                                text: qsTr("切膜撕膜相关点位设置")
                                Layout.fillWidth: true
                                onClicked:
                                {
                                    gbDetaperPosSet.visible = !gbDetaperPosSet.visible
                                }
                            }
                            GroupBox
                            {
                                id: gbDetaperPosSet
                                visible: true
                                Layout.fillWidth: true
                                ColumnLayout
                                {
                                    spacing: 20
                                    RowLayout
                                    {
                                        XYModulePos
                                        {
                                            moduleName: qsTr("PAXY")
                                            posName: qsTr("CutCarrierBoardPosXY")
                                            Layout.minimumWidth: 300
                                            Layout.maximumWidth: 300
                                            Layout.minimumHeight: 130
                                            Layout.maximumHeight: 130
                                        }
                                        SAxisModulePos
                                        {
                                            moduleName: qsTr("PACZ")
                                            posName: qsTr("StandbyPos")
                                            Layout.minimumWidth: 300
                                            Layout.maximumWidth: 300
                                            Layout.minimumHeight: 100
                                            Layout.maximumHeight: 100
                                        }
                                        SAxisModulePos
                                        {
                                            moduleName: qsTr("PACZ")
                                            posName: qsTr("CutCarrierBoardPosZ")
                                            Layout.minimumWidth: 300
                                            Layout.maximumWidth: 300
                                            Layout.minimumHeight: 100
                                            Layout.maximumHeight: 100
                                        }
                                    }
                                    RowLayout
                                    {
                                        SAxisModulePos
                                        {
                                            moduleName: qsTr("DetaperZ")
                                            posName: qsTr("DetaperZ_S")
                                            Layout.minimumWidth: 300
                                            Layout.maximumWidth: 300
                                            Layout.minimumHeight: 100
                                            Layout.maximumHeight: 100
                                        }
                                        SAxisModulePos
                                        {
                                            moduleName: qsTr("DetaperZ")
                                            posName: qsTr("DetaperZ_ClampBlueFilmPos")
                                            Layout.minimumWidth: 300
                                            Layout.maximumWidth: 300
                                            Layout.minimumHeight: 100
                                            Layout.maximumHeight: 100
                                        }
                                        SingleCylinder
                                        {
                                            name:qsTr("DtGripper")
                                            Layout.minimumWidth: 300
                                            Layout.maximumWidth: 300
                                            Layout.minimumHeight: 100
                                            Layout.maximumHeight: 100
                                        }
                                    }
                                }
                            }
                            RowLayout{
                                x:parent.top
                                y:50.0
                                spacing: 20
                                Button
                                {
                                    Layout.minimumWidth: 140
                                    Layout.maximumWidth: 140
                                    text: qsTr("一键进载板并切膜")
                                    onClicked:
                                    {
                                        tem.runSingleCmd(mPickArm, qsTr("AutoRunTest"));
                                    }
                                }
                                Button
                                {
                                    Layout.minimumWidth: 140
                                    Layout.maximumWidth: 140
                                    text: qsTr("进载板测试")
                                    onClicked:
                                    {
                                        tem.runSingleCmd(mPickArm, qsTr("AskFeedCarrierBoardTest"));
                                    }
                                }
                                Button
                                {
                                    Layout.minimumWidth: 140
                                    Layout.maximumWidth: 140
                                    text: qsTr("XY到切刀位")
                                    onClicked:
                                    {
                                        tem.runSingleCmd(mPickArm, qsTr("MoveToCutCarrierBoard_XY"));
                                    }
                                }
                                Button
                                {
                                    Layout.minimumWidth: 140
                                    Layout.maximumWidth: 140
                                    text: qsTr("开始切膜")
                                    onClicked:
                                    {
                                        tem.runSingleCmd(mPickArm, qsTr("AskFeedingCutCarrierBoard"));
                                    }
                                }
                                Button
                                {
                                    Layout.minimumWidth: 140
                                    Layout.maximumWidth: 140
                                    text: qsTr("初次剥料测试")
                                    onClicked:
                                    {
                                        tem.runSingleCmd(mPickArm, qsTr("AskFeedingTakeMaterialAwayWithOffsetFirst"));
                                    }
                                }
                                Button
                                {
                                    Layout.minimumWidth: 140
                                    Layout.maximumWidth: 140
                                    text: qsTr("正常剥料测试")
                                    onClicked:
                                    {
                                        tem.runSingleCmd(mPickArm, qsTr("AskFeedingTakeMaterialAwayWithOffset"));
                                    }
                                }
                                Button
                                {
                                    Layout.minimumWidth: 140
                                    Layout.maximumWidth: 140
                                    text: qsTr("出载板")
                                    onClicked:
                                    {
                                        tem.runSingleCmd(mPickArm, qsTr("AskFeedingOutCarrierBoard"));
                                    }
                                }

                            }
                        }
                    }
                }
            }
            Component {
                id: page2
                Item {
                    ScrollView
                    {
                        ColumnLayout
                        {
                            width: 1050
                            Button{
                                id:machineOperationSwitch
                                text: qsTr("Operation")
                                Layout.fillWidth: true
                                onClicked: {
                                    operation.visible = !operation.visible
                                }
                            }
                            GroupBox{
                                id: operation
                                visible: false
                                Layout.fillWidth: true
                                ColumnLayout{
                                    spacing: 10
                                    Button{
                                        id: ltRowColInfo
                                    }
                                    RowLayout{
                                        Button{
                                            Layout.minimumWidth: 180
                                            Layout.maximumWidth: 180
                                            width: 180
                                            text: qsTr("Pick_Lens")
                                            onClicked: {
                                                tem.runSingleCmd(pickArm, qsTr("moveToPickLens"),
                                                                 [ltRowColInfo.unitRow, ltRowColInfo.unitColumn, ltRowColInfo.row, ltRowColInfo.column])
                                            }
                                        }
                                        Button{
                                            Layout.minimumWidth: 180
                                            Layout.maximumWidth: 180
                                            width: 180
                                            text: qsTr("Pick_Sensor")
                                            onClicked: {
                                                tem.runSingleCmd(pickArm, qsTr("moveToPickSensor"),
                                                                 [ltRowColInfo.unitRow, ltRowColInfo.unitColumn, ltRowColInfo.row, ltRowColInfo.column])
                                            }
                                        }
                                    }
                                    RowLayout{
                                        Button{
                                            Layout.minimumWidth: 180
                                            Layout.maximumWidth: 180
                                            width: 180
                                            text: qsTr("AA_Head_Grip_Lens")
                                            onClicked: {
                                                tem.runSingleCmd(aaHead, qsTr("pickLensManual"))
                                            }
                                        }
                                        Button{
                                            Layout.minimumWidth: 180
                                            Layout.maximumWidth: 180
                                            width: 180
                                            text: qsTr("Pr_To_Bond")
                                            onClicked: {
                                                tem.runSingleCmd(aaHead, qsTr("prToBond"))
                                            }
                                        }
                                    }
                                    RowLayout{
                                        Button{
                                            Layout.minimumWidth: 180
                                            Layout.maximumWidth: 180
                                            width: 180
                                            text: qsTr("Excute_Glue")
                                            onClicked: {
                                                tem.runSingleCmd(aaHead, qsTr("shotGlue"), false)
                                            }
                                        }
                                        Button{
                                            Layout.minimumWidth: 180
                                            Layout.maximumWidth: 180
                                            width: 180
                                            text: qsTr("Excute_UV")
                                            onClicked: {
                                                tem.runSingleCmd(aaHead, qsTr("doUV"))
                                            }
                                        }
                                    }
                                    RowLayout{
                                        Button{
                                            Layout.minimumWidth: 180
                                            Layout.maximumWidth: 180
                                            width: 180
                                            text: qsTr("Pick_Good_Camera")
                                            onClicked: {
                                                tem.runSingleCmd(pickArm, qsTr("moveToGetOKProduct"))
                                            }
                                        }
                                        Button{
                                            Layout.minimumWidth: 180
                                            Layout.maximumWidth: 180
                                            width: 180
                                            text: qsTr("Place_Good_Camera")
                                            onClicked: {
                                                tem.runSingleCmd(pickArm, qsTr("moveToPlaceOKProduct"),
                                                                 [ltRowColInfo.row, ltRowColInfo.column, ltRowColInfo.unitRow, ltRowColInfo.unitColumn])
                                            }
                                        }
                                    }
                                }
                            }
                            Button{
                                id: pickSensorFromTraySwitch
                                text: qsTr("Pick Sensor From Tray")
                                Layout.fillWidth: true
                                onClicked: {
                                    pickSensorFromTray.visible = !pickSensorFromTray.visible
                                }
                            }
                            Button{
                                id: pickSensorFromTray
                                visible: false
                            }
                        }
                    }
                }
            }
            Component {
                id: page3
                Item {
                }
            }
            Component.onCompleted: {
                addTab(qsTr("进料部分相关设置"), page1)
                //            addTab(qsTr("Page2"), page2)
                //            addTab(qsTr("Page3"), page3)
            }
        }
    }
}
