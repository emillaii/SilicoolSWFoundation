import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../ConfigManager"
import ConfigElementType 1.0

CollisionCondition{
    ConfigRow{
        id: axisName
        configName: "axisName"
    }

    function init(){
        axisName.configModel = configModel
        collisionConditionType.configModel = configModel
        axisName.init(ConfigElementType.Other)
        collisionConditionType.init(ConfigElementType.Other)
    }
}
