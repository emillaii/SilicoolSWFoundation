import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "qrc:/qmlComponents/ConfigManager"

Window {
    visible: true
    width: 1000
    height: 800
    title: qsTr("Hello World")

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            RoundButton {
                icon.source: "qrc:/commonicons/undo.png"
                icon.color: configManager.commandCount > 0 ? "transparent" : "gray"
                onClicked: {
                    configManager.undo()
                }
            }
            RoundButton {
                icon.source: "qrc:/commonicons/redo.png"
                icon.color: configManager.undoCommandCount > 0 ? "transparent" : "gray"
                onClicked: {
                    configManager.redo()
                }
            }
        }

        SwipeView {
            id: view
            Layout.fillWidth: true
            Layout.fillHeight: true

            Page {
                ScrollView {
                    anchors.fill: parent
                    clip: true

                    RowLayout {
                        ObjectEditor {
                            sectionName: "testClassB"
                            configModel: testClassB
                            listMaxWidth: 600
                            Component.onCompleted: {
                                init()
                            }
                        }

                        ObjectEditor {
                            sectionName: "testClassB2"
                            configModel: testClassB2
                            listMaxWidth: 600
                            Component.onCompleted: {
                                init()
                            }
                        }

                        ColumnLayout{
                            CheckBox{
                                id: ckbTwoWay
                                checked: true
                            }
                            Button{
                                text: qsTr("Bind")
                                onClicked: {
                                    t.bind(ckbTwoWay.checked)
                                }
                            }
                            Button{
                                text: qsTr("UnBind")
                                onClicked: {
                                    t.unbind(ckbTwoWay.checked)
                                }
                            }
                        }
                    }
                }
            }
        }

        PageIndicator {
            id: indicator

            count: view.count
            currentIndex: view.currentIndex
            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
        }
    }
}
