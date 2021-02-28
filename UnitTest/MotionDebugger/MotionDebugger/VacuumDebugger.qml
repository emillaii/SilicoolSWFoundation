import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import MotionElement 1.0

GridView{
    DebuggerItemModel{
        id: itemModel
    }

    function createItemFunction(name){
        return {"name": name, "feedbackInputState": false, "vacuumizeOutputState": false, "hasBlowOutput": false,
            "blowOutputState": false}
    }

    function init(cylNames){
        itemModel.initItemModel(cylNames, createItemFunction)
    }

    function updateMotionState(vacuumStates){
        for(var vacuum in vacuumStates){
            if(vacuum in itemModel.nameIndexMap){
                var vacuumState = vacuumStates[vacuum]
                itemModelIndex = itemModel.nameIndexMap[vacuum]
                itemModel.itemModel.setProperty(itemModelIndex, "feedbackInputState", vacuumState["feedbackInputState"])
                itemModel.itemModel.setProperty(itemModelIndex, "vacuumizeOutputState", vacuumState["vacuumizeOutputState"])
                itemModel.itemModel.setProperty(itemModelIndex, "hasBlowOutput", vacuumState["hasBlowOutput"])
                itemModel.itemModel.setProperty(itemModelIndex, "blowOutputState", vacuumState["blowOutputState"])
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

            GridLayout{
                columns : 2
                rowSpacing: 10
                columnSpacing: 10
                Label{
                    text: qsTr("feedback")
                }
                Rectangle{
                    radius: 18
                    height: radius * 2
                    width: height
                    color: feedbackInputState ? "green" : "gray"
                }

                Label{
                    text: qsTr("vacuumize")
                }
                Rectangle{
                    radius: 18
                    height: radius * 2
                    width: height
                    color: vacuumizeOutputState ? "green" : "gray"
                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.ClosedHandCursor
                        onClicked: {
                            motionManager.executeInstruction(MotionElement.Vacuum, name, "set", [!vacuumizeOutputState])
                        }
                    }
                }

                Label{
                    visible: hasBlowOutput
                    text: qsTr("blow")
                }
                Rectangle{
                    visible: hasBlowOutput
                    radius: 18
                    height: radius * 2
                    width: height
                    color: blowOutputState ? "green" : "gray"
                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.ClosedHandCursor
                        onClicked: {
                            motionManager.executeInstruction(MotionElement.Vacuum, name, "setBlow", [!blowOutputState])
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
