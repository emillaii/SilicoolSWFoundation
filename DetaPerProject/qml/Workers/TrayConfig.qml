import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ConfigElementType 1.0
import "qrc:/qmlComponents/ConfigManager"
import "qrc:/qmlComponents"
import "qrc:/qmlComponents/CustomizedUIElement"

Item {
    RowLayout {
        spacing: 40
        //Feeding
        ObjectEditor {
            Layout.alignment: Qt.AlignTop
            sectionName: qsTr("FeedingTrayConfig")
            configModel: mFeedingTrayConfig
            Component.onCompleted: {
                init()
            }
        }
        //PickArm
        ObjectEditor {
            Layout.alignment: Qt.AlignTop
            sectionName: qsTr("PickArmTrayConfig")
            configModel: mPickArmTrayConfig
            Component.onCompleted: {
                init()
            }
        }
        //Discharge
        ObjectEditor {
            Layout.alignment: Qt.AlignTop
            sectionName: qsTr("DischargeTrayConfig")
            configModel: mDischargeTrayConfig
            Component.onCompleted: {
                init()
            }
        }
    }
}
