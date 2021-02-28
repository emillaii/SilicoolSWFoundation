import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import MotionElement 1.0

Frame{
    property string name: "Cylinder"

    property int __cylState: 2

    padding: 5

    ColumnLayout{
        spacing: 3

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
                    if(__cylState == 0){
                        return btnMargin
                    }else if(__cylState == 1){
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
                        if(__cylState == 0){
                            motionManager.executeInstruction(MotionElement.Cylinder, name, "set", [true])
                        }else if(__cylState == 1){
                            motionManager.executeInstruction(MotionElement.Cylinder, name, "set", [false])
                        }
                    }
                }
            }
        }
    }

    Connections{
        target: motionStatePublisher
        onCylStatesPublished: {
            __cylState = state[name]
        }
    }

    Component.onCompleted: {
        motionStatePublisher.subscribeCylinderState(name)
    }
}
