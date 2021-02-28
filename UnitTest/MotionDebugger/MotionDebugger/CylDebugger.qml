import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import MotionElement 1.0

GridView{
    DebuggerItemModel{
        id: itemModel
    }

    function createItemFunction(name){
        return {"name": name, "cylState": 0, "falseInputState": false, "trueInputState": false,
            "isSingleOutputControl": true, "falseOutputState": false, "trueOutputState": false}
    }

    function init(cylNames){
        itemModel.initItemModel(cylNames, createItemFunction)
    }

    function updateMotionState(cylStates){
        for(var cyl in cylStates){
            if(cyl in itemModel.nameIndexMap){
                var cylinderState = cylStates[cyl]
                itemModelIndex = itemModel.nameIndexMap[cyl]
                itemModel.itemModel.setProperty(itemModelIndex, "cylState", cylinderState["cylState"])
                itemModel.itemModel.setProperty(itemModelIndex, "falseInputState", cylinderState["falseInputState"])
                itemModel.itemModel.setProperty(itemModelIndex, "trueInputState", cylinderState["trueInputState"])
                itemModel.itemModel.setProperty(itemModelIndex, "isSingleOutputControl", cylinderState["isSingleOutputControl"])
                itemModel.itemModel.setProperty(itemModelIndex, "falseOutputState", cylinderState["falseOutputState"])
                itemModel.itemModel.setProperty(itemModelIndex, "trueOutputState", cylinderState["trueOutputState"])
            }
        }
    }

    model: itemModel.itemModel
    cellWidth: 20
    cellHeight: 20

    delegate: Rectangle{
        id: rect

        property int borderMargin: 10

        border.width: 1

        ColumnLayout{
            id: layout

            anchors.centerIn: parent
            spacing: 10
            Label{
                text: name
            }
            Rectangle{
                border.width: 1
                width: btn.width * 1.8
                height: btn.height + btn.btnMargin * 2
                color: "gray"
                Rectangle{
                    id: btn

                    property int btnMargin: 3

                    x: {
                        if(cylState == 0){
                            return btnMargin
                        }else if(cylState == 1){
                            return parent.width - width - btnMargin
                        }else{
                            return (parent.width - width) / 2
                        }
                    }
                    y: btnMargin
                    width: 90
                    height: 38
                    color: "#FF8040"

                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.ClosedHandCursor
                        onClicked: {
                            if(cylState == 0){
                                motionManager.executeInstruction(MotionElement.Cylinder, name, "set", [true])
                            }else if(cylState == 1){
                                motionManager.executeInstruction(MotionElement.Cylinder, name, "set", [false])
                            }
                        }
                    }
                }
            }
            GridLayout{
                columns : 2
                rowSpacing: 10
                columnSpacing: 10
                Label{
                    text: qsTr("falseInput")
                }
                Rectangle{
                    radius: 18
                    height: radius * 2
                    width: height
                    color: falseInputState ? "green" : "gray"
                }

                Label{
                    text: qsTr("trueInput")
                }
                Rectangle{
                    radius: 18
                    height: radius * 2
                    width: height
                    color: trueInputState ? "green" : "gray"
                }

                Label{
                    visible: !isSingleOutputControl
                    text: qsTr("falseOutput")
                }
                Rectangle{
                    visible: !isSingleOutputControl
                    radius: 18
                    height: radius * 2
                    width: height
                    color: falseOutputState ? "green" : "gray"
                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.ClosedHandCursor
                        onClicked: {
                            motionManager.executeInstruction(MotionElement.Cylinder, name, "setFalseOutput", [!falseOutputState])
                        }
                    }
                }

                Label{
                    text: qsTr("trueOutput")
                }
                Rectangle{
                    radius: 18
                    height: radius * 2
                    width: height
                    color: trueOutputState ? "green" : "gray"
                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.ClosedHandCursor
                        onClicked: {
                            motionManager.executeInstruction(MotionElement.Cylinder, name, "setTrueOutput", [!trueOutputState])
                        }
                    }
                }
            }
        }

        Component.onCompleted: {
            rect.width = layout.width + 10
            rect.height = layout.height + 10
            if(rect.width + rect.borderMargin > cellWidth){
                cellWidth = rect.width + rect.borderMargin
            }
            if(rect.height + rect.borderMargin > cellHeight){
                cellHeight = rect.height + rect.borderMargin
            }
        }
    }
}
