import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ConfigElementType 1.0
import "qrc:/qmlComponents/ConfigManager"
import "qrc:/qmlComponents"
import "qrc:/qmlComponents/CustomizedUIElement"

Item {
    ColumnLayout {
        RowLayout {
            GroupBox {
                ColumnLayout {
                    Label {
                        text: "UPH: " + uphHelper.uph
                        font.pixelSize: 30
                    }
                    Label {
                        text: "Total: " + uphHelper.total
                        font.pixelSize: 30
                    }
                    Label {
                        text: qsTr("CT: ") + uphHelper.lastCircelTime + " s"
                        font.pixelSize: 30
                    }
                    Button {
                        text: qsTr("Reset")
                        onClicked: {
                            uphHelper.reset()
                        }
                    }
                }
            }
            Label {
                font.pixelSize: 50
                text: qsTr("Test")
            }
        }
        RowLayout {
            spacing: 40

            ObjectEditor {
                Layout.alignment: Qt.AlignTop

                sectionName: qsTr("TrayConfig")
                configModel: dutTrayConfig
                Component.onCompleted: {
                    init()
                }
            }
            TrayMapEditor {
                Layout.alignment: Qt.AlignTop

                tableMaxWidth: 600
                tableMaxHeight: 500
                trayConfig: dutTrayConfig
                trayMap: dutTrayMap
                status2OpacityMap: {
                    "PrOk": 0.5
                }
                status2ColorMap: {
                    "Init": "gray",
                    "PrOk": "limeGreen",
                    "InUse": "limeGreen"
                }
                statusEnumeration: ["Init", "PrNg", "InUse"]
            }
        }
    }
}
