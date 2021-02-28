import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../CustomizedUIElement"
import "../ConfigManager"

Item {
    property alias lblTip: lblTip
    property var maxListHeight: 700

    Component{
        id: diConfigger
        ArrayEditor{
            titleVisible: false
            configModel: diConfigs
            listMaxHeight: maxListHeight
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: doConfigger
        ArrayEditor{
            titleVisible: false
            configModel: doConfigs
            listMaxHeight: maxListHeight
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: cylConfigger
        ArrayEditor{
            titleVisible: false
            configModel: cylConfigs
            listMaxHeight: maxListHeight
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: vacuumConfigger
        ArrayEditor{
            titleVisible: false
            configModel: vacuumConfigs
            listMaxHeight: maxListHeight
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: axisConfigger
        ArrayEditor{
            titleVisible: false
            configModel: axisConfigs
            listMaxHeight: maxListHeight
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: extendedAxisConfigger
        ArrayEditor{
            titleVisible: false
            configModel: extendedAxisConfigs
            listMaxHeight: maxListHeight
            listMaxWidth: parent.parent.width
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: extendedAxis2Configger
        ArrayEditor{
            titleVisible: false
            configModel: extendedAxis2Configs
            listMaxHeight: maxListHeight
            listMaxWidth: parent.parent.width
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: extendedAxis3Configger
        ArrayEditor{
            titleVisible: false
            configModel: extendedAxis3Configs
            listMaxHeight: maxListHeight
            listMaxWidth: parent.parent.width
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: extendedAxis4Configger
        ArrayEditor{
            titleVisible: false
            configModel: extendedAxis4Configs
            listMaxHeight: maxListHeight
            listMaxWidth: parent.parent.width
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: sAxisModuleConfigger
        ArrayEditor{
            titleVisible: false
            configModel: singleAxisModuleConfigs
            listMaxHeight: maxListHeight
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: xyModuleConfigger
        ArrayEditor{
            titleVisible: false
            configModel: xyModuleConfigs
            listMaxHeight: maxListHeight
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: xyzModuleConfigger
        ArrayEditor{
            titleVisible: false
            configModel: xyzModuleConfigs
            listMaxHeight: maxListHeight
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: softLandingConfigger
        ArrayEditor{
            titleVisible: false
            configModel: softLandingConfigs
            listMaxHeight: maxListHeight
            Component.onCompleted: {
                init()
            }
        }
    }

    Component{
        id: xyzrDebuggerConfigger
        ArrayEditor{
            titleVisible: false
            configModel: xyzrDebuggerConfigs
            listMaxHeight: maxListHeight
            Component.onCompleted: {
                init()
            }
        }
    }

    ColumnLayout{
        anchors.fill: parent
        Label{
            id: lblTip
        }
        MyTabView{
            Layout.fillHeight: true
            Layout.fillWidth: true
            trContext: "MotionConfigger"
            id: viewer
        }
    }

    Component.onCompleted: {
        viewer.addTab(qsTr("DI"), diConfigger)
        viewer.addTab(qsTr("DO"), doConfigger)
        viewer.addTab(qsTr("Vacuum"), vacuumConfigger)
        viewer.addTab(qsTr("Cyl"), cylConfigger)
        viewer.addTab(qsTr("Axis"), axisConfigger)
        if(extendedAxisConfigs !== null && extendedAxisConfigs.count() > 0){
            viewer.addTab(qsTr("ExAxis"), extendedAxisConfigger)
        }
        if(extendedAxis2Configs !== null && extendedAxis2Configs.count() > 0){
            viewer.addTab(qsTr("ExAxis2"), extendedAxis2Configger)
        }
        if(extendedAxis3Configs !== null && extendedAxis3Configs.count() > 0){
            viewer.addTab(qsTr("ExAxis3"), extendedAxis3Configger)
        }
        if(extendedAxis4Configs !== null && extendedAxis4Configs.count() > 0){
            viewer.addTab(qsTr("ExAxis4"), extendedAxis4Configger)
        }
        viewer.addTab(qsTr("SingleAxis"), sAxisModuleConfigger)
        viewer.addTab(qsTr("XyModule"), xyModuleConfigger)
        viewer.addTab(qsTr("XyzModule"), xyzModuleConfigger)
        viewer.addTab(qsTr("SoftLanding"), softLandingConfigger)
        viewer.addTab(qsTr("XYZRDbg"), xyzrDebuggerConfigger)
    }
}
