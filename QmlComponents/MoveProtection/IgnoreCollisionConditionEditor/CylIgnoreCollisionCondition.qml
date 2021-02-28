import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../ConfigManager"
import ConfigElementType 1.0

IgnoreCollisionCondition{
    ConfigRow{
        id: cylName
        configName: "cylName"
    }
    ConfigRow{
        id: targetState
        configName: "targetState"
    }

    function init(){
        cylName.configModel = configModel
        targetState.configModel = configModel
        ignoreCollisionConditionType.configModel = configModel

        cylName.init(ConfigElementType.Other)
        targetState.init(ConfigElementType.Int)
        ignoreCollisionConditionType.init(ConfigElementType.Other)
    }
}
