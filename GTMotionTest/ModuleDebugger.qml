import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtCharts 2.3
import "qrc:/qmlComponents/CustomizedUIElement"
import "qrc:/qmlComponents/ConfigManager"

MySwipeTabView {
    indent: 1

    Component{
        id: motionProfiler
        MotionProfiler{
        }
    }

    Component.onCompleted: {
        addTab(qsTr("Profiler"), motionProfiler)

        tabBar.currentIndex = count - 1
    }
}
