import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

RowLayout{
    property var __indicators: []
    visible: basicConfig.showTaskEngineStates
    z: 1000

    Repeater{
        model: tem.taskEngineCount()
        delegate: Rectangle{
            width: 20
            height: width
            radius: width / 2
            color: "gray"
            Component.onCompleted: {
                __indicators.push(this)
            }
        }
    }

    Timer{
        interval: 100
        repeat: true
        triggeredOnStart: true
        running: parent.visible
        onTriggered: {
            var engineStates = tem.getEngineStates()
            for(var i = 0; i < engineStates.length; i++){
                __indicators[i].color = engineStates[i] ? "yellow" : "gray"
            }
        }
    }
}
