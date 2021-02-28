import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../ConfigManager"

ColumnLayout{
    property alias ignoreCollisionConditionType: ignoreCollisionConditionType
    property var configModel: null

    ConfigRow{
        id: ignoreCollisionConditionType
        configName: "ignoreCollisionConditionType"
    }
}
