import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import MsgBoxIcon 1.0

Popup {
    property var uiOperationImplementation: null

    property int msgBoxWidth: 500
    property int msgBoxHeight: 200
    property int maxMsgBoxesToShow: 3
    property int msgBoxesMargin: 10
    property int msgBoxCount: 0

    property point clickedPos: "0, 0"

    id: popup
    x: (parent.width - msgBoxWidth) / 2
    y: 200
    closePolicy: Popup.NoAutoClose

    width: msgBoxWidth
    height: {
        var tmp_msgBoxCount = Math.min(msgBoxCount, maxMsgBoxesToShow)
        return msgBoxHeight * tmp_msgBoxCount + msgboxViewer.spacing * (tmp_msgBoxCount - 1) + rectTitle.height + 5
    }

    contentItem: Rectangle{
        anchors.fill: parent

        color: "transparent"

        Rectangle{
            id: rectTitle
            width: msgBoxWidth
            height: 30
            color:  "#26A65B"

            Label {
                anchors.fill: parent
                text: qsTr("NOTIFICATION")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea{
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                onPressed: {
                    clickedPos = Qt.point(mouse.x, mouse.y)
                }
                onPositionChanged: {
                    var delta = Qt.point(mouse.x - clickedPos.x, mouse.y - clickedPos.y)
                    popup.x += delta.x
                    popup.y += delta.y
                }
            }
        }

        Label{
            id: templateLbl
        }

        ListView{
            id: msgboxViewer
            anchors.top: rectTitle.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            clip: true
            model: msgBoxModel

            delegate: Rectangle{
                width: msgBoxWidth
                height: msgBoxHeight
                border.width: 0
                color: "transparent"

                Rectangle{
                    id: rectSpace
                    width: msgBoxWidth
                    height: msgBoxesMargin
                    color: "gray"
                }

                Text {
                    id: txtTitle
                    anchors.top: rectSpace.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: title
                    horizontalAlignment: Text.AlignHCenter
                    color: templateLbl.color
                }
                Rectangle{
                    id: rectLine
                    anchors.top: txtTitle.bottom
                    width: msgBoxWidth
                    height: 2
                    border.width: 1
                }
                Image {
                    id: imgIcon
                    anchors.top: rectLine.bottom
                    anchors.topMargin: 20
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    width: 80
                    height: 80
                    source: {
                        switch(icon)
                        {
                        case MsgBoxIcon.Information:
                            return "qrc:/commonicons/information.png";
                        case MsgBoxIcon.Question:
                            return "qrc:/commonicons/question.png";
                        case MsgBoxIcon.Warning:
                            return "qrc:/commonicons/warning.png";
                        case MsgBoxIcon.Error:
                            return "qrc:/commonicons/error.png";
                        default:
                            return "";
                        }
                    }
                }
                Text {
                    anchors.top: rectLine.bottom
                    anchors.topMargin: 10
                    anchors.left: imgIcon.right
                    anchors.leftMargin: 15
                    anchors.bottom: rowButtons.top
                    anchors.right: parent.right
                    anchors.rightMargin: 10

                    elide: Text.ElideRight
                    wrapMode: Text.WordWrap
                    text: content
                    color: templateLbl.color
                }
                RowLayout{
                    id: rowButtons
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: 80
                    height: 50
                    width: msgBoxWidth - 80
                    Repeater{
                        model: buttons
                        delegate: Button{
                            text: modelData
                            onClicked: {
                                uiOperationImplementation.onUIResponse(uuid, text)
                            }
                        }
                    }
                }
            }

            ScrollBar.vertical: ScrollBar{}
        }
    }
}
