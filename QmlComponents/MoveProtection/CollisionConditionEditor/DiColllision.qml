import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../ConfigManager"
import ConfigElementType 1.0

CollisionCondition{
    ConfigRow{
        id: diName
        configName: "diName"
    }

    function init(){
        diName.configModel = configModel
        collisionConditionType.configModel = configModel
        diName.init(ConfigElementType.Other)
        collisionConditionType.init(ConfigElementType.Other)
    }
}
