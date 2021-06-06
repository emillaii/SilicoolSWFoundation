import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import MotionElement 1.0

Frame{
    property string name: "Vacuum"

    property int __vacuumState: 0

    verticalPadding: 5
    horizontalPadding: 5

    ColumnLayout{
        spacing: 3

        Label{
            text: name
        }

        RowLayout{
            Rectangle{
                radius: 18
                height: radius * 2
                width: height
                color: __vacuumState == 1 ? "green" : "gray"
            }

            Button{
                text: qsTr("Open")
                onClicked: {
                    motionManager.executeInstruction(MotionElement.Vacuum, name, "set", [true, false])
                }
            }
            Button{
                text: qsTr("Close")
                onClicked: {
                    motionManager.executeInstruction(MotionElement.Vacuum, name, "set", [false, false])
                }
            }
        }
    }

    Connections{
        target: motionStatePublisher
        onVacuumStatePublished: {
            try{
                __vacuumState = state[name]
            }catch(e){}
        }
    }

    Component.onCompleted: {
        motionStatePublisher.subscribeVacuumState(name)
    }
}
