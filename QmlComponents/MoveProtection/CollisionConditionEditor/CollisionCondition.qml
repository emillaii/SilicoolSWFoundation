import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../ConfigManager"

ColumnLayout{
    property alias collisionConditionType: collisionConditionType
    property var configModel: null

    ConfigRow{
        id: collisionConditionType
        configName: "collisionConditionType"
        onChildItemInited: {
            childItem.implicitWidth = 220
        }
    }
}
