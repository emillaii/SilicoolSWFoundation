import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "qrc:/qmlComponents/CustomizedUIElement"
import "qrc:/qmlComponents/ConfigManager"
import "./TrayLoader"
import "./PickArm"

MySwipeTabView {
    indent: 1
    trContext: "ModuleDebugger"

    Component {
        id: dataStatistics
        DataStatistics {
        }
    }

    Component {
        id: trayLoaderViewer
        TrayLoaderViewer {
        }
    }

    Component {
        id: pickArmViewer
        PickArmViewer {
        }
    }

    Component.onCompleted: {
        addTab(qsTr("DataStatistics"), dataStatistics)
        addTab(qsTr("TrayLoader"), trayLoaderViewer)
        addTab(qsTr("PickArm"), pickArmViewer)
    }
}
