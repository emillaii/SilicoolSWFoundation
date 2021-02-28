import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "qrc:/qmlComponents/ConfigManager"

Window {
    visible: true
    width: 750
    height: 700
    title: qsTr("Hello World")

    RowLayout {
        anchors.fill: parent

        ColumnLayout {
            ArrayEditor {
                sectionName: "SourceFiles"
                configModel: sourceFiles
                listMaxHeight: 300
                Component.onCompleted: {
                    init()
                }
            }
            ArrayEditor {
                sectionName: "DestDirs"
                configModel: destDirs
                listMaxHeight: 300
                Component.onCompleted: {
                    init()
                }
            }
        }
        Button {
            text: "Copy"
            onClicked: {
                fileCopier.copy()
            }
        }
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Label {
                Layout.alignment: Qt.AlignHCenter
                text: "Msg"
            }
            TextEdit {
                id: txtMsg
                Layout.fillHeight: true
                Layout.fillWidth: true
                Connections {
                    target: fileCopier
                    onPubMsg: {
                        txtMsg.text += msg + "\r\n"
                    }
                }
            }
            Button {
                Layout.alignment: Qt.AlignHCenter
                text: "Clear"
                onClicked: {
                    txtMsg.clear()
                }
            }
        }
    }
}
