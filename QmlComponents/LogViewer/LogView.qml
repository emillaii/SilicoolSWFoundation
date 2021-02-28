import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Rectangle {
    color: "transparent"
    property string category: ""
    property var logHandler: null
    property alias loglevelSelector: loglevelSelector

    border.width: 1

    function setLogModel(model)
    {
        logView.model = model
    }

    function setLogLevel(level){
        if(level === 0){
            rdbLevel0.checked = true
        }else if(level === 1){
            rdbLevel1.checked = true
        }else if(level === 2){
            rdbLevel2.checked = true
        }
    }

    Rectangle{
        id: loglevelSelector
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 3

        border.width: 1
        width: 80
        color: "transparent"

        ColumnLayout{
            anchors.fill: parent

            RadioButton{
                id: rdbLevel0
                text: qsTr("Debug")
                onClicked: {
                    logHandler.setLogLevel(category, 0)
                }
            }
            RadioButton{
                id: rdbLevel1
                text: qsTr("Info")
                onClicked: {
                    logHandler.setLogLevel(category, 1)
                }
            }
            RadioButton{
                id: rdbLevel2
                text: qsTr("Warn")
                onClicked: {
                    logHandler.setLogLevel(category, 2)
                }
            }
        }
    }

    Rectangle{
        anchors.left: loglevelSelector.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 3

        border.width: 1
        color: "transparent"

        Label{
            id: templateLbl
            visible: false
        }

        ListView{
            id: logView
            x: 2
            y: 2
            width: parent.width - 4
            height: parent.height - 4

            clip: true

            delegate: Text {
                width: parent.width - 3
                color: {
                    if(logLevel < 2){
                        return templateLbl.color
                    }else if(logLevel == 2){
                        return  "orangered"
                    }else{
                        return "red"
                    }
                }
                text: logStr
                wrapMode: Text.Wrap
                lineHeight: 1.2
                lineHeightMode: Text.ProportionalHeight
            }

            MouseArea{
                z: 1
                anchors.fill: parent
                onDoubleClicked: {
                    logView.model.onClearLog()
                }
            }

            ScrollBar.vertical: ScrollBar {
                id: scrollBar
                z: 2
                width: 15
            }

            MouseArea{
                id: logViewMouseArea
                z: 3
                anchors.fill: parent
                hoverEnabled: true
                onPressed: {
                    mouse.accepted = false
                }
            }
        }
    }

    Timer {
        id: scrollTimer;
        repeat: true;
        interval: 200;
        onTriggered: {
            if(!logViewMouseArea.containsMouse)
            {
                logView.positionViewAtEnd()
            }
        }
    }

    Component.onCompleted: {
        scrollTimer.start()
    }
}
