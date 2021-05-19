import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "CustomizedUIElement"
import "ConfigManager"
import ConfigElementType 1.0

Item {
    id: medsEditor

    property var configModel: null
    property alias tip: tip

    function init(){
        for(var i = 0; i < configModel.count(); i++){
            medsViewer.addTab(i, medViewerCom)
        }
        connConfigInserted.target = configModel
        connConfigRemoved.target = configModel
    }

    ColumnLayout{
        anchors.fill: parent

        RowLayout{
            RoundButton{
                icon.source: "qrc:/commonicons/add.png"
                icon.color: "transparent"
                onClicked: {
                    var tabviewIndex = medsViewer.currentIndex
                    if(tabviewIndex < 0 || tabviewIndex >= medsViewer.count){
                        tabviewIndex = medsViewer.count - 1
                    }
                    configModel.add(tabviewIndex + 1)
                }
            }
            RoundButton{
                icon.source: "qrc:/commonicons/remove.png"
                icon.color: "transparent"
                onClicked: {
                    var tabviewIndex = medsViewer.currentIndex
                    if(tabviewIndex >= 0 && tabviewIndex < medsViewer.count){
                        configModel.remove(tabviewIndex)
                    }
                }
            }
            Label{
                id: tip
                visible: text != ""
            }

            Rectangle{
                color: "transparent"
                width: 100
                height: 10
            }
            Button{
                text: qsTr("自动生成Motion调试界面配置文件")
                onClicked: {
                    tem.runSingleCmd(masterMotionManager, "generateMotionElementUILayoutConfigFile")
                }
            }
            Button{
                text: qsTr("自动生成轴页面回零配置文件")
                onClicked: {
                    tem.runSingleCmd(masterMotionManager, "generateAxisPageHomeSeqConfigFile")
                }
            }
        }

        MyTabView{
            id: medsViewer

            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    Component{
        id: medViewerCom

        ColumnLayout{
            id: medViewerRoot
            property var cfgModel: null

            ConfigRow{
                id: processNameCfgRow
                configModel: medViewerRoot.cfgModel
                configName: "processName"
            }
            ConfigRow{
                id: addressCfgRow
                configModel: medViewerRoot.cfgModel
                configName: "address"
            }
            MyTabView{
                id: medViewer

                deleteUnvisibleTab: false
                Layout.fillWidth: true
                Layout.fillHeight: true

                Component{
                    id: axisDefinitionsCom

                    ArrayEditor{
                        titleVisible: false
                        Component.onCompleted: {
                            configModel = medViewerRoot.cfgModel.getConfig("axisDefinitions")
                            init()
                        }
                    }
                }
                Component{
                    id: diNamesCom

                    ArrayEditor{
                        titleVisible: false
                        Component.onCompleted: {
                            configModel = medViewerRoot.cfgModel.getConfig("diNames")
                            init()
                        }
                    }
                }
                Component{
                    id: doNamesCom

                    ArrayEditor{
                        titleVisible: false
                        Component.onCompleted: {
                            configModel = medViewerRoot.cfgModel.getConfig("doNames")
                            init()
                        }
                    }
                }
                Component{
                    id: vacuumNamesCom

                    ArrayEditor{
                        titleVisible: false
                        Component.onCompleted: {
                            configModel = medViewerRoot.cfgModel.getConfig("vacuumNames")
                            init()
                        }
                    }
                }
                Component{
                    id: cylNamesCom

                    ArrayEditor{
                        titleVisible: false
                        Component.onCompleted: {
                            configModel = medViewerRoot.cfgModel.getConfig("cylNames")
                            init()
                        }
                    }
                }
                Component{
                    id: axisModulesCom

                    ArrayEditor{
                        titleVisible: false
                        Component.onCompleted: {
                            configModel = medViewerRoot.cfgModel.getConfig("axisModules")
                            init()
                        }
                    }
                }
                Component{
                    id: softlandingDefinitionsCom

                    ArrayEditor{
                        titleVisible: false
                        Component.onCompleted: {
                            configModel = medViewerRoot.cfgModel.getConfig("softlandingDefinition")
                            init()
                        }
                    }
                }
            }

            Component.onCompleted: {
                var index = parent.title
                medViewerRoot.cfgModel = medsEditor.configModel.getConfig(index)
                processNameCfgRow.init(ConfigElementType.Other)
                addressCfgRow.init(ConfigElementType.Other)
                medViewer.addTab("axisDefinitions", axisDefinitionsCom)
                medViewer.addTab("diNames", diNamesCom)
                medViewer.addTab("doNames", doNamesCom)
                medViewer.addTab("vacuumNames", vacuumNamesCom)
                medViewer.addTab("cylNames", cylNamesCom)
                medViewer.addTab("axisModules", axisModulesCom)
                medViewer.addTab("softlandingDefinitions", softlandingDefinitionsCom)
            }
        }
    }

    Connections{
        id: connConfigInserted
        target: null
        onConfigInserted:{
            for(var i = index; i < medsViewer.count; i++) {
                medsViewer.setTitle(i, i + 1)
            }
            medsViewer.insertTab(index, index, medViewerCom)
        }
    }
    Connections{
        id: connConfigRemoved
        target: null
        onConfigRemoved:{
            for(var i = index + 1; i < medsViewer.count; i++) {
                medsViewer.setTitle(i, i - 1)
            }
            medsViewer.removeTab(index)
        }
    }
}
