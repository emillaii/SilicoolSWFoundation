import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../ConfigManager"

ColumnLayout{
    property alias cmpOperationType: cmpOperationType
    property var configModel: null
    property var collisionCondition: null

    ConfigRow{
        id: cmpOperationType
        configName: "cmpOperationType"
    }
}
