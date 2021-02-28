import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../ConfigManager"
import ConfigElementType 1.0

IgnoreCollisionCondition{
    ConfigRow{
        id: axisName
        configName: "axisName"
    }
    ConfigRow{
        id: moveDirection
        configName: "moveDirection"
    }

    function init(){
        axisName.configModel = configModel
        moveDirection.configModel = configModel
        ignoreCollisionConditionType.configModel = configModel

        axisName.init(ConfigElementType.Other)
        moveDirection.init(ConfigElementType.Other)
        ignoreCollisionConditionType.init(ConfigElementType.Other)
    }
}
