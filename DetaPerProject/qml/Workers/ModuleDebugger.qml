import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtCharts 2.3
import "qrc:/qmlComponents/CustomizedUIElement"
import "qrc:/qmlComponents/ConfigManager"
import "qrc:/qml/Workers/Feeding"
import "qrc:/qml/Workers/Discharge"
import "qrc:/qml/Workers/PickArm"
import "qrc:/qml/Workers/SpaceCalculate"

MySwipeTabView {
    indent: 1
    trContext: qsTr("ModuleDebugger")
    Component {
        id: dataStatistics
        DataStatistics {}
    }
    Component {
        id: trayConfigCom
        TrayConfig {}
    }
    Component {
        id: idFeedingViewer
        FeedingViewer {}
    }
    Component {
        id: idPickArmViewer
        PickArmViewer {}
    }
    Component {
        id: idDischargeViewer
        DischargeViewer {}
    }
    Component {
        id: idSpaceCalculateViewer
        SpaceCalculateView{}
    }
    Component.onCompleted: {
        addTab(qsTr("数据状态"), dataStatistics)
        addTab(qsTr("地图设置"), trayConfigCom)
        addTab(qsTr("进料部分"), idFeedingViewer)
        addTab(qsTr("机头部分"), idPickArmViewer)
        addTab(qsTr("出料部分"), idDischargeViewer)
        addTab(qsTr("间距计算部分"), idSpaceCalculateViewer)
    }
}
