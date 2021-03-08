import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ConfigElementType 1.0
import "qrc:/qmlComponents/CustomizedUIElement"
import "qrc:/qmlComponents/AxisModulePos"

Item {
    MyTabView {
        indent: 2
        trContext: "PickArmViewer"

        Component {
            id: page1
            Item {
                ColumnLayout {
                    XYZModulePos {
                        moduleName: "PAXYZ"
                        posName: "StandbyPos"
                    }
                    XYZModulePos {
                        moduleName: "PAXYZ"
                        posName: "PlacePos"
                    }
                    SAxisModulePos {
                        moduleName: "PAZ"
                        posName: "StandbyPos"
                    }
                    SAxisModulePos {
                        moduleName: "PAZ"
                        posName: "PickPos"
                    }
                }
            }
        }
        Component {
            id: page2
            Item {
            }
        }
        Component {
            id: page3
            Item {
            }
        }
        Component.onCompleted: {
            addTab(qsTr("Page1"), page1)
            addTab(qsTr("Page2"), page2)
            addTab(qsTr("Page3"), page3)
        }
    }
}
