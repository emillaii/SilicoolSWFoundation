import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import MotionElement 1.0

Frame{
    property string name: "DI"
    property string activatedColor: "green"
    property string unactivatedColor: "gray"

    property bool __ioState: false

    verticalPadding: 5
    horizontalPadding: 5

    RowLayout{
        Label{
            text: name
        }
        Rectangle{
            radius: 18
            height: radius * 2
            width: height

            color: __ioState ? activatedColor : unactivatedColor
        }
    }

    Connections{
        target: motionStatePublisher
        onDiStatePublished: {
            __ioState = state[name]
        }
    }

    Component.onCompleted: {
        motionStatePublisher.subscribeDIState(name)
    }
}
