import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: root
    width: 560
    height: 250

    Item {
        id: backImage
        width: root.width
        height: 150
        Repeater {
            model: ["icons/D", "icons/U", "icons/M", "icons/M", "icons/Y"]
            delegate: ToolButton {
                id: image
                enabled: false
                display: AbstractButton.IconOnly
                width: 120
                height: 150
                x: index == 0 ? -30 : (width - 10) * index
                y: 0
                icon.source: modelData
                icon.width: width
                icon.height: height
                icon.color: index == 0 ? "transparent" : "#13227a"
                background: Item {
                }

                SequentialAnimation {
                    running: index == 0 ? false : true
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
                        to: "#d4237a"
                        duration: 2000
                    }
                    PropertyAnimation {
                        target: image
                        property: "icon.color"
                        to: "#13227a"
                        duration: 4000
                    }
                    PropertyAnimation {
                        target: image
                        property: "icon.color"
                        to: "#13227a"
                        duration: 2000
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

    Keys.onEscapePressed: {
        btnExit.clicked()
    }
    Keys.onReturnPressed: {
        btnStart.clicked()
    }
}
