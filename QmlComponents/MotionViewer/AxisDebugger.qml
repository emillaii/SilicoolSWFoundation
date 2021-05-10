import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import MotionElement 1.0

Item {
    property int borderMargin: 10
    property bool __homeMotorsBtnVisible: false
    property string __pageName: ""

    function createItemFunction(name) {
        return {
            "name": name,
            "isEnabled": false,
            "hasHome": false,
            "positiveLimitStatus": false,
            "negativeLimitStatus": false,
            "profilerPos": 0.0,
            "feedbackPos": 0.0,
            "hasAlarm": true,
            "isRunning": false,
            "isInPos": false,
            "velocityRatio": 0.0,
            "hasMasterAxis": false,
            "bindState": false,
            "jogMovable": false
        }
    }

    function init(pageName, axisNames, homeMotorsBtnVisible) {
        itemModel.initItemModel(axisNames, createItemFunction)
        __pageName = pageName
        __homeMotorsBtnVisible = homeMotorsBtnVisible
    }

    function updateMotionState(axisStates) {
        for (var axis in axisStates) {
            if (axis in itemModel.nameIndexMap) {
                var axisState = axisStates[axis]
                var itemModelIndex = itemModel.nameIndexMap[axis]
                itemModel.itemModel.setProperty(itemModelIndex, "isEnabled",
                                                axisState["isEnabled"])
                itemModel.itemModel.setProperty(itemModelIndex, "hasHome",
                                                axisState["hasHome"])
                itemModel.itemModel.setProperty(itemModelIndex, "positiveLimitStatus",
                                                axisState["positiveLimitStatus"])
                itemModel.itemModel.setProperty(itemModelIndex, "negativeLimitStatus",
                                                axisState["negativeLimitStatus"])
                itemModel.itemModel.setProperty(itemModelIndex, "profilerPos",
                                                axisState["profilerPos"])
                itemModel.itemModel.setProperty(itemModelIndex, "feedbackPos",
                                                axisState["feedbackPos"])
                itemModel.itemModel.setProperty(itemModelIndex, "hasAlarm",
                                                axisState["hasAlarm"])
                itemModel.itemModel.setProperty(itemModelIndex, "isRunning",
                                                axisState["isRunning"])
                itemModel.itemModel.setProperty(itemModelIndex, "isInPos",
                                                axisState["isInPos"])
                itemModel.itemModel.setProperty(itemModelIndex,
                                                "velocityRatio",
                                                axisState["velocityRatio"])
                itemModel.itemModel.setProperty(itemModelIndex,
                                                "hasMasterAxis",
                                                axisState["hasMasterAxis"])
                itemModel.itemModel.setProperty(itemModelIndex, "bindState",
                                                axisState["bindState"])
                itemModel.itemModel.setProperty(itemModelIndex, "jogMovable",
                                                axisState["jogMovable"])
            }
        }
    }

    RowLayout {
        id: globalVelocitySetter

        y: -10
        Label {
            text: qsTr("GlobalAxisVelocity")
        }
        Slider {
            id: globalVelocitySlider
            from: 1
            to: 100
            stepSize: 1
            value: masterMotionManager.globalVelocityRatio * 100
            onPressedChanged: {
                if (!pressed) {
                    masterMotionManager.setGlobalVelocityRatio(value / 100)
                }
            }
        }
        Label {
            text: globalVelocitySlider.value
        }
        Button{
            visible: __homeMotorsBtnVisible
            text: qsTr("回零本页面电机")
            onClicked: {
                var homeSeq = masterMotionManager.getHomeSeq(__pageName)
                tem.runSingleCmd(motionManager, "homeMotors", [homeSeq])
            }
        }
    }

    GridView {
        id: viewer
        width: parent.width
        y: globalVelocitySetter.y + globalVelocitySetter.height
        height: parent.height - y

        DebuggerItemModel {
            id: itemModel
        }

        model: itemModel.itemModel
        cellWidth: 20
        cellHeight: 20
        clip: true

        delegate: Frame {
            id: frame

            verticalPadding: 5
            horizontalPadding: 5

            ColumnLayout {
                spacing: 3

                Label {
                    text: name
                    MouseArea {
                        property var isShow: false
                        anchors.fill: parent
                        onClicked: {
                            isShow = !isShow
                            motionManager.executeInstruction(
                                        MotionElement.Axis, name,
                                        "showZeroIoState", [isShow])
                        }
                    }
                }

                GridLayout {
                    rows: 2
                    rowSpacing: 3
                    columnSpacing: 3
                    flow: GridLayout.TopToBottom

                    //                    Rectangle{
                    //                        visible: hasMasterAxis
                    //                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    //                        radius: 15
                    //                        height: radius * 2
                    //                        width: height
                    //                        color: bindState ? "green" : "gray"
                    //                        MouseArea{
                    //                            anchors.fill: parent
                    //                            cursorShape: Qt.ClosedHandCursor
                    //                            onClicked: {
                    //                                if(bindState){
                    //                                    motionManager.executeInstruction(MotionElement.Axis, name, "unBindToMasterAxis", [])
                    //                                }else{
                    //                                    motionManager.executeInstruction(MotionElement.Axis, name, "bindToMasterAxis", [])
                    //                                }
                    //                            }
                    //                        }
                    //                    }
                    //                    Label{
                    //                        visible: hasMasterAxis
                    //                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    //                        text: qsTr("bind")
                    //                    }
                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        radius: 15
                        height: radius * 2
                        width: height
                        color: isRunning ? "green" : "gray"
                    }
                    Label {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        text: qsTr("running")
                    }

                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        radius: 15
                        height: radius * 2
                        width: height
                        color: isInPos ? "green" : "gray"
                    }
                    Label {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        text: qsTr("inPos")
                    }

                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        radius: 15
                        height: radius * 2
                        width: height
                        color: positiveLimitStatus ? "red" : "gray"
                    }
                    Label {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        text: qsTr("L+")
                    }

                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        radius: 15
                        height: radius * 2
                        width: height
                        color: negativeLimitStatus ? "red" : "gray"
                    }
                    Label {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        text: qsTr("L-")
                    }

                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        radius: 15
                        height: radius * 2
                        width: height
                        color: isEnabled ? "green" : "gray"
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.ClosedHandCursor
                            onClicked: {
                                if (isEnabled) {
                                    motionManager.executeInstruction(
                                                MotionElement.Axis, name,
                                                "disable", [])
                                } else {
                                    motionManager.executeInstruction(
                                                MotionElement.Axis, name,
                                                "enable", [])
                                }
                            }
                        }
                    }
                    Label {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        text: qsTr("enable")
                    }

                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        radius: 15
                        height: radius * 2
                        width: height
                        color: hasAlarm ? "red" : "gray"
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.ClosedHandCursor
                            hoverEnabled: true
                            onClicked: {
                                if (hasAlarm) {
                                    motionManager.executeInstruction(
                                                MotionElement.Axis, name,
                                                "clearAlarm", [])
                                }
                            }
                            onDoubleClicked: {
                                motionManager.executeInstruction(
                                            MotionElement.Axis, name,
                                            "clearStatus", [])
                            }
                            ToolTip.text: qsTr("Click to clear alarm. Double click to clear status.")
                            ToolTip.visible: containsMouse
                        }
                    }
                    Label {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        text: qsTr("alarm")
                    }

                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        radius: 15
                        height: radius * 2
                        width: height
                        color: hasHome ? "green" : "gray"
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.ClosedHandCursor
                            onClicked: {
                                motionManager.executeInstruction(
                                            MotionElement.Axis, name,
                                            "home", [])
                            }
                        }
                    }
                    Label {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        text: qsTr("home")
                    }
                    Frame{
                        implicitWidth: 75
                        implicitHeight: 25
                        verticalPadding: 2
                        horizontalPadding: 2
                        Label{
                            anchors.fill: parent
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: Math.round(profilerPos * 100000) / 100000
                        }
                    }
                    Label {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        text: qsTr("P Pos")
                    }
                    Frame{
                        implicitWidth: 75
                        implicitHeight: 25
                        verticalPadding: 2
                        horizontalPadding: 2
                        Label{
                            anchors.fill: parent
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: Math.round(feedbackPos * 100000) / 100000
                        }
                    }
                    Label {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        text: qsTr("F Pos")
                    }
                    TextField {
                        id: txtTargetPos
                        implicitWidth: 130
                        implicitHeight: 40
                        text: "1"
                        selectByMouse: true
                        horizontalAlignment: Qt.AlignHCenter
                        ToolTip.text: qsTr("Target pos")
                        ToolTip.visible: hovered
                        validator: DoubleValidator {
                            notation: DoubleValidator.StandardNotation
                        }
                    }
                    RowLayout {
                        Button {
                            implicitWidth: 40
                            implicitHeight: 30
                            text: "Go"
                            ToolTip.text: qsTr("AbsMove")
                            ToolTip.visible: hovered
                            onClicked: {
                                motionManager.executeInstruction(
                                            MotionElement.Axis, name,
                                            "absMoveWithLimit",
                                            [txtTargetPos.text])
                            }
                        }
                        Button {
                            implicitWidth: 40
                            implicitHeight: 30
                            text: "+"
                            ToolTip.text: qsTr("RelMove")
                            ToolTip.visible: hovered
                            onClicked: {
                                var step = Number(txtTargetPos.text)
                                if (step < 0) {
                                    step = -step
                                }
                                motionManager.executeInstruction(
                                            MotionElement.Axis, name,
                                            "relForceMoveWithLimit", [step])
                            }
                        }
                        Button {
                            implicitWidth: 40
                            implicitHeight: 30
                            text: "-"
                            ToolTip.text: qsTr("RelMove")
                            ToolTip.visible: hovered
                            onClicked: {
                                var step = Number(txtTargetPos.text)
                                if (step > 0) {
                                    step = -step
                                }
                                motionManager.executeInstruction(
                                            MotionElement.Axis, name,
                                            "relForceMoveWithLimit", [step])
                            }
                        }
                    }

                    ToolButton {
                        display: "IconOnly"
                        implicitWidth: 30
                        implicitHeight: 30
                        icon.source: "qrc:/commonicons/left.png"
                        icon.width: implicitWidth
                        icon.height: implicitHeight
                        enabled: jogMovable
                        ToolTip.text: qsTr("JogMoveToNegativeDirection")
                        ToolTip.visible: hovered
                        onPressed: {
                            motionManager.executeInstruction(
                                        MotionElement.Axis, name,
                                        "jogMove", [0])
                        }
                        onReleased: {
                            motionManager.executeInstruction(
                                        MotionElement.Axis, name,
                                        "stopAndResetState", [])
                        }
                    }
                    ToolButton {
                        display: "IconOnly"
                        implicitWidth: 30
                        implicitHeight: 30
                        icon.source: "qrc:/commonicons/right.png"
                        icon.width: implicitWidth
                        icon.height: implicitHeight
                        enabled: jogMovable
                        ToolTip.text: qsTr("JogMoveToPositiveDirection")
                        ToolTip.visible: hovered
                        onPressed: {
                            motionManager.executeInstruction(
                                        MotionElement.Axis, name,
                                        "jogMove", [1])
                        }
                        onReleased: {
                            motionManager.executeInstruction(
                                        MotionElement.Axis, name,
                                        "stopAndResetState", [])
                        }
                    }
                    ToolButton {
                        display: "IconOnly"
                        implicitWidth: 30
                        implicitHeight: 30
                        icon.source: "qrc:/commonicons/up.png"
                        icon.width: implicitWidth
                        icon.height: implicitHeight
                        enabled: jogMovable
                        ToolTip.text: qsTr("JogMoveToPositiveDirection")
                        ToolTip.visible: hovered
                        onPressed: {
                            motionManager.executeInstruction(
                                        MotionElement.Axis, name,
                                        "jogMove", [1])
                        }
                        onReleased: {
                            motionManager.executeInstruction(
                                        MotionElement.Axis, name,
                                        "stopAndResetState", [])
                        }
                    }
                    ToolButton {
                        display: "IconOnly"
                        implicitWidth: 30
                        implicitHeight: 30
                        icon.source: "qrc:/commonicons/down.png"
                        icon.width: implicitWidth
                        icon.height: implicitHeight
                        enabled: jogMovable
                        ToolTip.text: qsTr("JogMoveToNegativeDirection")
                        ToolTip.visible: hovered
                        onPressed: {
                            motionManager.executeInstruction(
                                        MotionElement.Axis, name,
                                        "jogMove", [0])
                        }
                        onReleased: {
                            motionManager.executeInstruction(
                                        MotionElement.Axis, name,
                                        "stopAndResetState", [])
                        }
                    }

                    RowLayout {
                        Slider {
                            id: velSlider
                            ToolTip.text: qsTr("VelocityRatio")
                            ToolTip.visible: hovered
                            implicitWidth: 130
                            implicitHeight: 30
                            from: 1
                            to: 100
                            stepSize: 1
                            value: Math.round(velocityRatio * 100)
                            onPressedChanged: {
                                if (!pressed) {
                                    masterMotionManager.setAxisVelocityRatio(
                                                name, value / 100)
                                }
                            }
                        }
                        Label {
                            width: 30
                            height: 30
                            text: velSlider.value
                            horizontalAlignment: Qt.AlignHCenter
                        }
                    }

                    RowLayout {
                        Button {
                            implicitWidth: 60
                            implicitHeight: 30
                            text: qsTr("Stop")
                            onClicked: {
                                motionManager.executeInstruction(
                                            MotionElement.Axis, name,
                                            "stopAndResetState", [])
                            }
                        }
                        Button {
                            implicitWidth: 95
                            implicitHeight: 30
                            text: qsTr("reciprocate")
                            onClicked: {
                                reciprocatePopup.open()
                            }
                        }
                    }

                    Popup {
                        id: reciprocatePopup

                        anchors.centerIn: Overlay.overlay
                        contentItem: ColumnLayout {
                            GridLayout {
                                columns: 2

                                Label {
                                    text: qsTr("pos1")
                                }
                                TextField {
                                    id: txtPos1
                                    text: "0.0"
                                    selectByMouse: true
                                    implicitWidth: 90
                                    DoubleValidator {
                                        notation: DoubleValidator.StandardNotation
                                    }
                                }

                                Label {
                                    text: qsTr("delay1")
                                }
                                TextField {
                                    id: txtDelay1
                                    text: "100"
                                    selectByMouse: true
                                    implicitWidth: 90
                                    IntValidator {
                                    }
                                }

                                Label {
                                    text: qsTr("pos2")
                                }
                                TextField {
                                    id: txtPos2
                                    text: "0.0"
                                    selectByMouse: true
                                    implicitWidth: 90
                                    DoubleValidator {
                                        notation: DoubleValidator.StandardNotation
                                    }
                                }

                                Label {
                                    text: qsTr("delay2")
                                }
                                TextField {
                                    id: txtDelay2
                                    text: "100"
                                    selectByMouse: true
                                    implicitWidth: 90
                                    IntValidator {
                                    }
                                }

                                Label {
                                    text: qsTr("times")
                                }
                                TextField {
                                    id: txtTimes
                                    text: "100"
                                    selectByMouse: true
                                    implicitWidth: 90
                                    IntValidator {
                                    }
                                }
                            }

                            RowLayout {
                                Button {
                                    text: qsTr("start")
                                    implicitWidth: 70
                                    onClicked: {
                                        motionManager.executeInstruction(
                                                    MotionElement.Axis, name,
                                                    "startReciprocate",
                                                    [txtPos1.text, txtDelay1.text, txtPos2.text, txtDelay2.text, txtTimes.text])
                                    }
                                }
                                Button {
                                    text: qsTr("stop")
                                    implicitWidth: 70
                                    onClicked: {
                                        motionManager.executeInstruction(
                                                    MotionElement.Axis, name,
                                                    "stopReciprocate", [])
                                    }
                                }
                            }
                        }
                    }
                }

                Component.onCompleted: {
                    if (frame.width + borderMargin > viewer.cellWidth) {
                        viewer.cellWidth = frame.width + borderMargin
                    }
                    if (frame.height + borderMargin > viewer.cellHeight) {
                        viewer.cellHeight = frame.height + borderMargin
                    }
                }
            }
        }
    }
}
