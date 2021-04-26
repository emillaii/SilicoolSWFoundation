import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import MotionElement 1.0

Item{
    property int borderMargin: 10

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
                var itemModelIndex = itemModel.nameIndexMap[vacuum]
                itemModel.itemModel.setProperty(itemModelIndex, "feedbackInputState", vacuumState["feedbackInputState"])
                itemModel.itemModel.setProperty(itemModelIndex, "vacuumizeOutputState", vacuumState["vacuumizeOutputState"])
                itemModel.itemModel.setProperty(itemModelIndex, "hasBlowOutput", vacuumState["hasBlowOutput"])
                itemModel.itemModel.setProperty(itemModelIndex, "blowOutputState", vacuumState["blowOutputState"])
            }
        }
    }

    GridView{
        id: viewer

        DebuggerItemModel{
            id: itemModel
        }

        anchors.fill: parent
        model: itemModel.itemModel
        cellWidth: 20
        cellHeight: 20
        clip: true

        delegate: Frame{
            id: frame

            verticalPadding: 5
            horizontalPadding: 5

            ColumnLayout{
                id: layout

                anchors.centerIn: parent
                spacing: 5
                Label{
                    text: name
                }

                GridLayout{
                    columns : 2
                    rowSpacing: 10
                    columnSpacing: 5
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
                                motionManager.executeInstruction(MotionElement.Vacuum, name, "set", [!vacuumizeOutputState, false])
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
                        onVisibleChanged: {
                            if(visible){
                                frame.setCellWidthHeight()
                            }
                        }
                    }
                }
            }

            Component.onCompleted: {
                if(frame.width + borderMargin > viewer.cellWidth){
                    viewer.cellWidth = frame.width + borderMargin
                }
                if(frame.height + borderMargin > viewer.cellHeight){
                    viewer.cellHeight = frame.height + borderMargin
                }
            }
        }
    }
}
