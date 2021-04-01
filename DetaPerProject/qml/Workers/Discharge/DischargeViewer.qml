import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ConfigElementType 1.0
import "qrc:/qmlComponents/CustomizedUIElement"
import "qrc:/qmlComponents/ConfigManager"
import "qrc:/qmlComponents/AxisModulePos"

Item {
    MyTabView {
        indent: 2
        trContext: qsTr("DischargeViewer")
        Component {
            id: page1
            Item {
                ColumnLayout
                {
                    width: 1050
                    Button{
                        id:idPosSet
                        text: qsTr("出料部分点位设置")
                        Layout.fillWidth: true
                        onClicked: {
                            gbPosSet.visible = !gbPosSet.visible
                        }
                    }
                    GroupBox{
                        id: gbPosSet
                        visible: true
                        Layout.fillWidth: true
                        ColumnLayout
                        {
                            RowLayout
                            {
                                SAxisModulePos{
                                    moduleName: qsTr("TrackZ")
                                    posName: qsTr("GetTrayPos")
                                    Layout.minimumWidth: 311
                                    Layout.maximumWidth: 311
                                    Layout.minimumHeight: 100
                                    Layout.maximumHeight: 100
                                }
                                SAxisModulePos{
                                    moduleName: qsTr("TrackZ")
                                    posName: qsTr("PlaceMaterialPos")
                                    Layout.minimumWidth: 311
                                    Layout.maximumWidth: 311
                                    Layout.minimumHeight: 100
                                    Layout.maximumHeight: 100
                                }
                            }
                            RowLayout
                            {
                                SAxisModulePos{
                                    moduleName: qsTr("TrackX")
                                    posName: qsTr("GetTrayPos")
                                    Layout.minimumWidth: 311
                                    Layout.maximumWidth: 311
                                    Layout.minimumHeight: 100
                                    Layout.maximumHeight: 100
                                }
                                SAxisModulePos{
                                    moduleName: qsTr("TrackX")
                                    posName: qsTr("TakePicturePosForPlace")
                                    Layout.minimumWidth: 311
                                    Layout.maximumWidth: 311
                                    Layout.minimumHeight: 100
                                    Layout.maximumHeight: 100
                                }
                            }
                        }
                    }
                    Button{
                        id:idJudgeParmeterSet
                        text: qsTr("出料部分判断参数设置")
                        Layout.fillWidth: true
                        onClicked: {
                            gbJudgeParmeterSet.visible = !gbJudgeParmeterSet.visible
                        }
                    }
                    GroupBox{
                        id: gbJudgeParmeterSet
                        visible: true
                        Layout.fillWidth: true
                        RowLayout
                        {
                            ConfigRow{
                                configName: qsTr("JudgeIndex")
                                configModel: mDischargeConfig
                                Component.onCompleted: {
                                    autoInit()
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
            }
        }
        Component {
            id: page3
            Item {
            }
        }
        Component.onCompleted: {
            addTab(qsTr("出料部分设置"), page1)
            //            addTab(qsTr("Page2"), page2)
            //            addTab(qsTr("Page3"), page3)
        }
    }
}
