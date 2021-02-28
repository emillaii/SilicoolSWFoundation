import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../ConfigManager"
import ConfigElementType 1.0

CollisionCondition{
    ConfigRow{
        id: cylName
        configName: "cylName"
    }

    function init(){
        cylName.configModel = configModel
        collisionConditionType.configModel = configModel
        cylName.init(ConfigElementType.Other)
        collisionConditionType.init(ConfigElementType.Other)
    }
}
