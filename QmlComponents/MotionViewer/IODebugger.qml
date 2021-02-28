import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import MotionElement 1.0

Item{
    property bool isDigitalOutput: false

    function createItemFunction(name){
        return {"name": name, "ioState": false }
    }

    function init(ioNames){
        itemModel.initItemModel(ioNames, createItemFunction)
    }

    function updateMotionState(ioStates){
        for(var io in ioStates){
            if(io in itemModel.nameIndexMap){
                itemModel.itemModel.setProperty(itemModel.nameIndexMap[io], "ioState", ioStates[io])
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

        delegate: Rectangle{
            id: rect

            property int borderMargin: 10

            border.width: 1
            color: "transparent"

            RowLayout{
                id: layout

                anchors.centerIn: parent
                spacing: 10

                Label{
                    text: name
                }
                Rectangle{
                    radius: 18
                    height: radius * 2
                    width: height

                    color: ioState ? "green" : "gray"

                    MouseArea{
                        anchors.fill: parent
                        cursorShape: isDigitalOutput ? Qt.ClosedHandCursor : Qt.ArrowCursor
                        onClicked: {
                            if(isDigitalOutput){
                                motionManager.executeInstruction(MotionElement.DO, name, "set", [!ioState])
                            }
                        }
                    }
                }

                Component.onCompleted: {
                    rect.width = layout.width + 10
                    rect.height = layout.height + 10
                    if(rect.width + rect.borderMargin > viewer.cellWidth){
                        viewer.cellWidth = rect.width + rect.borderMargin
                    }
                    if(rect.height + rect.borderMargin > viewer.cellHeight){
                        viewer.cellHeight = rect.height + rect.borderMargin
                    }
                }
            }
        }
    }
}

