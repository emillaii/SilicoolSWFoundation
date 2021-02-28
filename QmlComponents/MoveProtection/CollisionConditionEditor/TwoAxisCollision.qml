import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../ConfigManager"
import ConfigElementType 1.0

CollisionCondition{
    ConfigRow{
        id: axis1Name
        configName: "axis1Name"
    }
    ConfigRow{
        id: posComposition
        configName: "posComposition"
    }
    ConfigRow{
        id: axis2Name
        configName: "axis2Name"
    }

    function init(){
        axis1Name.configModel = configModel
        posComposition.configModel = configModel
        axis2Name.configModel = configModel
        collisionConditionType.configModel = configModel
        axis1Name.init(ConfigElementType.Other)
        posComposition.init(ConfigElementType.Other)
        axis2Name.init(ConfigElementType.Other)
        collisionConditionType.init(ConfigElementType.Other)
    }
}
