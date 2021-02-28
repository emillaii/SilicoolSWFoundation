import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Popup{
    ListModel{
        id: workersModel
    }
    anchors.centerIn: Overlay.overlay
    property int __lblMaxWidth: 50
    property int __rectWidht: 20
    property int __rowSpacing: 10

    Timer{
        id: timer
        interval: 100
        repeat: true
        triggeredOnStart: true
        running: false

        onTriggered: {
            var runStatus = dispatcher.getWorkerRunStatus()
            for(var i in runStatus){
                workersModel.setProperty(i, "isRunning", runStatus[i])
            }
        }
    }

    Component.onCompleted: {
        var workerNames = dispatcher.getWorkerNames()
        for(var i in workerNames){
            workersModel.append({"workerName":workerNames[i], "isRunning":false})
        }
    }

    contentItem: ListView{
        model: workersModel
        spacing: 10
        implicitWidth: __rectWidht + __rowSpacing + __lblMaxWidth + 10
        implicitHeight: (__rectWidht + spacing) * workersModel.count
        delegate: RowLayout{
            spacing: __rowSpacing
            Rectangle{
                width: __rectWidht
                height: width
                radius: width / 2
                color: isRunning ? "yellow" : "gray"
            }
            Label{
                text: workerName
                Component.onCompleted: {
                    if(implicitWidth > __lblMaxWidth){
                        __lblMaxWidth = implicitWidth
                    }
                }
            }
        }
    }

    onOpened: {
        timer.start()
    }
    onClosed: {
        timer.stop()
    }
}
