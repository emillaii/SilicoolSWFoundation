import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../CustomizedUIElement"

Item {
    id: itemXYZR

    property var xyzrDebugger: null
    property var xyzrDebuggerConfig: null

    property var __needResize: false

    implicitWidth: popupXYZR.implicitWidth
    implicitHeight: popupXYZR.implicitHeight

    function open(){
        popupXYZR.open()
        if(__needResize){
            popupXYZR.resize()
            __needResize = false
        }
    }

    Popup{
        id: popupXYZR

        function resize(){
            implicitHeight = xyzrContent.implicitHeight + 15
            implicitWidth = xyzrContent.implicitWidth + 15
        }

        contentItem: ColumnLayout{
            id: xyzrContent
            RowLayout{
                Layout.alignment: Qt.AlignHCenter

                ToolButton{
                    implicitWidth: 40
                    implicitHeight: 40
                    display: AbstractButton.IconOnly
                    icon.source: "qrc:/commonicons/calculator.png"
                    onClicked: {
                        calculatorView.visible = !calculatorView.visible
                        popupXYZR.resize()
                    }
                }
                Label{
                    text: xyzrDebuggerConfig.debuggerName
                }
                Rectangle{
                    width: 30
                    height: 30
                    color: "transparent"
                    Image {
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/commonicons/move.png"
                    }
                    MouseArea{
                        property var clickedPos: "0,0"
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton
                        onPressed: {
                            clickedPos = Qt.point(mouse.x, mouse.y)
                        }
                        onPositionChanged: {
                            if(containsMouse){
                                var delta = Qt.point(mouse.x - clickedPos.x, mouse.y - clickedPos.y)
                                itemXYZR.x += delta.x
                                itemXYZR.y += delta.y
                            }
                        }
                    }
                }
            }

            RowLayout{
                Label{
                    text: "StepSize"
                }
                MyDoubleInput{
                    id: txtStepSize
                    implicitWidth: 60
                    text: "1"
                }
//                Label{
//                    text: "mm"
//                }
                RowLayout{
                    spacing: 0
                    Repeater{
                        model: ["5", "1", "0.1", "0.01", "0.001"]
                        delegate: RadioButton{
                            text: modelData
                            onClicked: {
                                txtStepSize.text = modelData
                            }
                            implicitHeight: 25
                            Component.onCompleted: {
                                if(modelData == "1"){
                                    checked = true
                                }
                            }
                        }
                    }
                }
            }

            RowLayout{
                Label{
                    text: "JogVelocity"
                }
                MyDoubleInput{
                    id: txtJogVelocity
                    implicitWidth: 60
                    text: "5"
                }
//                Label{
//                    text: "mm/s"
//                }
                RowLayout{
                    spacing: 0
                    Repeater{
                        model: ["50", "20", "10", "5", "1"]
                        delegate: RadioButton{
                            text: modelData
                            onClicked: {
                                txtJogVelocity.text = modelData
                            }
                            implicitHeight: 25
                            Component.onCompleted: {
                                if(modelData == "5"){
                                    checked = true
                                }
                            }
                        }
                    }
                }
            }

            RowLayout{
                UDLRMoveView{
                    leftIcon: "qrc:/commonicons/small_left.png"
                    leftLeftIcon: "qrc:/commonicons/left.png"
                    upIcon: "qrc:/commonicons/small_up.png"
                    upUpIcon: "qrc:/commonicons/up.png"
                    downIcon: "qrc:/commonicons/small_down.png"
                    downDownIcon: "qrc:/commonicons/down.png"
                    rightIcon: "qrc:/commonicons/small_right.png"
                    rightRightIcon: "qrc:/commonicons/right.png"
                    horizontalAxisName: "x"
                    verticalAxisName: "y"
                    horizontalVisible: xyzrDebuggerConfig.xAxisName !== ""
                    verticalVisible: xyzrDebuggerConfig.yAxisName !== ""

                    onJogMove: {
                        xyzrDebugger.jogMove(axisType, dir, txtJogVelocity.text)
                    }
                    onStopJogMove: {
                        xyzrDebugger.stopJogMove(axisType)
                    }
                    onStepMove: {
                        var step = Math.abs(Number(txtStepSize.text)) * dir
                        xyzrDebugger.stepMove(axisType, step)
                    }
                    onHorizontalVisibleChanged: {
                        __needResize = true
                    }
                    onVerticalVisibleChanged: {
                        __needResize = true
                    }
                }
                Rectangle{
                    width: 10
                    height: 10
                    color: "transparent"
                }
                UDLRMoveView{
                    leftIcon: "qrc:/commonicons/small_ccw.png"
                    leftLeftIcon: "qrc:/commonicons/ccw.png"
                    upIcon: "qrc:/commonicons/small_up.png"
                    upUpIcon: "qrc:/commonicons/up.png"
                    downIcon: "qrc:/commonicons/small_down.png"
                    downDownIcon: "qrc:/commonicons/down.png"
                    rightIcon: "qrc:/commonicons/small_cw.png"
                    rightRightIcon: "qrc:/commonicons/cw.png"
                    horizontalAxisName: "r"
                    verticalAxisName: "z"
                    horizontalVisible: xyzrDebuggerConfig.rAxisName !== ""
                    verticalVisible: xyzrDebuggerConfig.zAxisName !== ""

                    onJogMove: {
                        xyzrDebugger.jogMove(axisType, dir, txtJogVelocity.text)
                    }
                    onStopJogMove: {
                        xyzrDebugger.stopJogMove(axisType)
                    }
                    onStepMove: {
                        var step = Math.abs(Number(txtStepSize.text)) * dir
                        xyzrDebugger.stepMove(axisType, step)
                    }
                    onHorizontalVisibleChanged: {
                        __needResize = true
                    }
                    onVerticalVisibleChanged: {
                        __needResize = true
                    }
                }
            }

            RowLayout{
                id: rowAxisPos
                property int __txtWidth: 70
                RowLayout{
                    visible: xyzrDebuggerConfig.xAxisName !== ""
                    Label{
                        text: "XPos:"
                    }
                    TextField{
                        id: txtXPos
                        text: "0"
                        readOnly: true
                        selectByMouse: true
                        implicitWidth: rowAxisPos.__txtWidth
                    }
                }
                RowLayout{
                    visible: xyzrDebuggerConfig.yAxisName !== ""
                    Label{
                        text: "YPos:"
                    }
                    TextField{
                        id: txtYPos
                        text: "0"
                        readOnly: true
                        selectByMouse: true
                        implicitWidth: rowAxisPos.__txtWidth
                    }
                }
                RowLayout{
                    visible: xyzrDebuggerConfig.rAxisName !== ""
                    Label{
                        text: "RPos:"
                    }
                    TextField{
                        id: txtRPos
                        text: "0"
                        readOnly: true
                        selectByMouse: true
                        implicitWidth: rowAxisPos.__txtWidth
                    }
                }
                RowLayout{
                    visible: xyzrDebuggerConfig.zAxisName !== ""
                    Label{
                        text: "ZPos:"
                    }
                    TextField{
                        id: txtZPos
                        text: "0"
                        readOnly: true
                        selectByMouse: true
                        implicitWidth: rowAxisPos.__txtWidth
                    }
                }
            }

            ColumnLayout{
                id: calculatorView
                visible: false
                spacing: 0
                XYPosView{
                    id: pos1
                    title: "Pos1"
                    relatedDebugger: xyzrDebugger
                }
                XYPosView{
                    id: pos2
                    title: "Pos2"
                    relatedDebugger: xyzrDebugger
                }
                RowLayout{
                    Label{
                        text: qsTr("XStep:")
                    }
                    MyIntInput{
                        id: txtXStep
                        text: "1"
                    }
                    Label{
                        text: qsTr("YStep:")
                    }
                    MyIntInput{
                        id: txtYStep
                        text: "1"
                    }
                    Button{
                        text: qsTr("Calculate")
                        onClicked: {
                            txtXInterval.text = ((pos2.xPos() - pos1.xPos()) / Number(txtXStep.text)).toFixed(5)
                            txtYInterval.text = ((pos2.yPos() - pos1.yPos()) / Number(txtYStep.text)).toFixed(5)
                        }
                    }
                }
                RowLayout{
                    Label{
                        text: qsTr("XInterval:")
                    }
                    MyDoubleInput{
                        id: txtXInterval
                    }
                    Label{
                        text: qsTr("YInterval:")
                    }
                    MyDoubleInput{
                        id: txtYInterval
                    }
                }
            }
        }

        onOpened: {
            xyzrDebugger.startUpdateAxesPos()
        }
        onClosed: {
            xyzrDebugger.stopUpdateAxesPos()
        }

        Connections{
            target: xyzrDebugger
            onAxesPosPublished:{
                txtXPos.text = x
                txtYPos.text = y
                txtRPos.text = r
                txtZPos.text = z
            }
        }

        Component.onCompleted: {
            resize()
        }
    }
}
