import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import MotionElement 1.0

Item{
    property bool isDigitalOutput: false
    property int borderMargin: 10

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

        delegate: Frame{
            id: frame

            verticalPadding: 5
            horizontalPadding: 5

            RowLayout{
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
                    if(frame.implicitWidth + borderMargin > viewer.cellWidth){
                        viewer.cellWidth = frame.implicitWidth + borderMargin
                    }
                    if(frame.implicitHeight + borderMargin > viewer.cellHeight){
                        viewer.cellHeight = frame.implicitHeight + borderMargin
                    }
                }
            }
        }
    }
}

