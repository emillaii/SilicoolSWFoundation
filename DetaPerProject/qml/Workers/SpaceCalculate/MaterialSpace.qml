import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ConfigElementType 1.0
import "qrc:/qmlComponents/CustomizedUIElement"
import "qrc:/qmlComponents/AxisModulePos"
import "qrc:/qmlComponents/ConfigManager"
import "qrc:/qmlComponents/MotionViewer"

RowLayout
{
    spacing: 50
    ColumnLayout
    {
        x:100
        Label
        {
            Layout.minimumHeight: 20
            Layout.maximumHeight: 20
            text: qsTr("第一步:")
            font.pixelSize: 22 //字体大小
            color: "pink"
            background: Rectangle{color: "Transparent"}
        }
        Button
        {
            text: qsTr("拍第一点")
            onClicked:
            {
                mPickArm.testGetSpace1(true);
            }
        }

    }
    Label
    {
        Layout.minimumWidth: 20
        Layout.maximumWidth: 20
        text: "》》》"
    }
    ColumnLayout
    {
        Label
        {
            text: qsTr("第二步:")
            font.pixelSize: 22 //字体大小
            color: "pink"
            background: Rectangle{color: "Transparent"}
        }

        Button
        {
            text: qsTr("拍第二点")
            onClicked:
            {
                mPickArm.testGetSpace2(true, "ForPickArmTrayConfig");
            }
        }

    }
    Label
    {
        Layout.minimumWidth: 20
        Layout.maximumWidth: 20
        text: "》》》"
    }
    ColumnLayout
    {
        Label
        {
            text: qsTr("第三步:")
            font.pixelSize: 22 //字体大小
            color: "pink"
            background: Rectangle{color: "Transparent"}
        }

        Button
        {
            text: qsTr("拍第三点")
            onClicked:
            {
                // mPickArm.testGetSpace1(true);
            }
        }
    }
    Label
    {
        Layout.minimumWidth: 20
        Layout.maximumWidth: 20
        text: "》》》"
    }
    ColumnLayout
    {
        RowLayout
        {
            Label
            {
                Layout.minimumWidth :80
                Layout.maximumWidth :80
                Layout.minimumHeight: 20
                Layout.maximumHeight: 20
                text: qsTr("吸料行间距:")

                background: Rectangle{color: "Transparent"}
            }

            TextField
            {
                text:mPickArmTrayConfig.rowDelta
            }
        }

        RowLayout
        {
            Label
            {
                Layout.minimumWidth :80
                Layout.maximumWidth :80
                Layout.minimumHeight: 20
                Layout.maximumHeight: 20
                text: qsTr("吸料列间距:")

                background: Rectangle{color: "Transparent"}
            }
            TextField
            {
                text:mPickArmTrayConfig.columnDelta
            }
        }
    }

    CheckBox
    {
        id:idIsUseKeyBoard
        checked: false
        text: qsTr("使用键盘控制轴")
        MouseArea
        {
            id: area
            width: parent.width
            height: parent.height
            anchors.fill: parent
            hoverEnabled: true //默认是false
            onEntered:
            {
                idIsUseKeyBoard.checked = true
                idIsUseKeyBoard.forceActiveFocus()
            }
            onExited:
            {
                idIsUseKeyBoard.checked = false
                idIsUseKeyBoard.focus = false
            }
        }
        Keys.onPressed:
        {
            if(idIsUseKeyBoard.checked)
            {
                if(!event.isAutoRepeat)
                {
                    switch(event.key)
                    {
                    case 16777234://left
                        mPickArm.keyBoardMove("PaX",0)
//                        motionManager.executeInstruction(MotionElement.Axis, "PickArm_X",
//                                                                "jogMove", [0])
                        console.log("按下键盘左，X轴向左");
                        break;
                    case 16777236://right
                        mPickArm.keyBoardMove("PaX",1)
//                        motionManager.executeInstruction(MotionElement.Axis, "PickArm_X",
//                                                                "jogMove", [1])
                        console.log("按下键盘右，X轴向右");
                        break;

                    case 16777235://up
                        motionManager.executeInstruction(MotionElement.Type.Axis, "PickArm_Y",
                                                                "jogMove", [0])
                        console.log("按下键盘上，Y轴向前");
                        break;
                    case 16777237://down
                        MotionConfigger
                        motionManager.executeInstruction(MotionElement.Type.Axis, "PickArm_Y",
                                                                "jogMove", [1])
                        console.log("按下键盘下，Y轴向后");
                        break;

                    case Qt.Key_Up:

                        break;

                    case Qt.Key_Down:

                        break;

                    case Qt.Key_Left:

                        break;

                    case Qt.Key_Right:

                        break;
                    }
                }

            }
        }

        Keys.onReleased:
        {
            if(!event.isAutoRepeat)
            {
                //mPickArm.keyBoardStop("PaX")
                switch(event.key)
                {
                case 16777234://left
                    mPickArm.keyBoardStop("PaX")
//                    motionManager.executeInstruction(MotionElement.Axis, "PickArm_X",
//                                                            "stopAndResetState", [])
                    console.log("弹起键盘左，X轴停止");
                    break;
                case 16777236://right
                     mPickArm.keyBoardStop("PaX")
//                    motionManager.executeInstruction(MotionElement.Axis, "PickArm_X",
//                                                            "stopAndResetState", [])
                    console.log("弹起键盘右，X轴停止");
                    break;

                case 16777235://up
                    motionManager.executeInstruction(MotionElement.Axis, "PickArm_Y",
                                                            "stopAndResetState", [])
                    console.log("弹起键盘上，Y轴停止");
                    break;
                case 16777237://down
                    MotionConfigger
                    motionManager.executeInstruction(MotionElement.Axis, "PickArm_Y",
                                                            "stopAndResetState", [])
                    console.log("弹起键盘下，Y轴停止");
                    break;

                case Qt.Key_Up:

                    break;

                case Qt.Key_Down:

                    break;

                case Qt.Key_Left:

                    break;

                case Qt.Key_Right:

                    break;
                }
            }
        }
    }
}

