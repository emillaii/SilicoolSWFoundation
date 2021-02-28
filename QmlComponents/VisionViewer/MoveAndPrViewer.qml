import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../CustomizedUIElement"
import "../ConfigManager"

ColumnLayout {
    ObjectEditor {
        sectionName: qsTr("MoveAndPr")
        configModel: moveAndPrConfig
        Component.onCompleted: {
            init()
        }
    }
    RowLayout {
        Button {
            text: qsTr("Start")
            onClicked: {
                tem.runSingleCmd(visionManager, "startMoveAndPr")
            }
        }
        Button {
            text: qsTr("Stop")
            onClicked: {
                visionManager.stopMoveAndPr()
            }
        }
    }
}
