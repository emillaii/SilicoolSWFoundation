import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: root
    width: 560
    height: 250
    focus: true

    Item {
        id: backImage
        x: 0
        y: -43
        width: 560
        height: 193
        anchors.topMargin: -22
        anchors.top: parent.top
        clip: false
        Repeater {
            model: ["icons/d", "icons/t", "icons/p"]
            delegate: ToolButton {
                id: image
                enabled: false
                display: AbstractButton.IconOnly
                width: 120
                height: 180
                x: index == 0 ? 100 : (index == 1)? 220 : 340
                y: 0
                icon.source: modelData
                icon.width: width
                icon.height: height
                icon.color: "#13227a"
                background: Item {
                }

                SequentialAnimation {
                    running: index == 0 ? true : true
                    loops: Animation.Infinite
                    PropertyAnimation {
                        target: image
                        property: "icon.color"
                        to: "#d4237a"
                        duration: 4000
                    }
                    PropertyAnimation {
                        target: image
                        property: "icon.color"
                        to: "#244314"
                        duration: 4000
                    }
                    PropertyAnimation {
                        target: image
                        property: "icon.color"
                        to: "#DBBCEB"
                        duration: 4000
                    }
                }
            }
        }
    }

    ColumnLayout {
        y: backImage.height + 10
        anchors.horizontalCenter: root.horizontalCenter

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            RowLayout {
                Rectangle {
                    width: 65
                    height: Math.min(cmbDutType.implicitHeight, 35)
                    color: "#707070"
                    Label {
                        anchors.centerIn: parent
                        text: qsTr("DutType:")
                    }
                }

                ComboBox {
                    id: cmbDutType
                    implicitWidth: 200
                    property bool isInit: false
                    model: dutTypeManager.dutTypes
                    onCurrentTextChanged: {
                        if (cmbDutType.isInit) {
                            basicConfig.setDutType(currentText)
                        }
                    }
                    Component.onCompleted: {
                        cmbDutType.isInit = true
                        var i = 0
                        for (; i < dutTypeManager.dutTypes.length; i++) {
                            if (dutTypeManager.dutTypes[i] === basicConfig.dutType) {
                                currentIndex = i
                                break
                            }
                        }
                    }
                }
            }
            Button {
                id: btnStart
                text: qsTr("Start")
                onClicked: {
                    if (cmbDutType.currentText != "") {
                        dutTypeManager.setLaunchResult(1)
                    }
                }
            }
            Button {
                id: btnExit
                text: qsTr("Exit")
                onClicked: {
                    dutTypeManager.setLaunchResult(0)
                }
            }
        }
    }

    Keys.enabled: true
    Keys.onPressed: {
        switch (event.key)
        {
        case Qt.Key_Return:
            btnStart.clicked();
            break;
        case Qt.Key_Escape:
            btnExit.clicked();
            break;

        default:
            return;
        }
    }

}



/*##^## Designer {
    D{i:1;anchors_height:150;anchors_width:560;anchors_x:136;anchors_y:"-43"}
}
 ##^##*/
