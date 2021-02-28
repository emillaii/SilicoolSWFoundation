import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../ConfigManager"
import ConfigElementType 1.0

ComparisonOperation{
    ConfigRow{
        id: limitPos1
        configName: "limitPos1"
    }

    Button{
        text: qsTr("Read")
        onClicked: {
            collisionCondition.setLimitStateWithCurrentState(1)
        }
    }

    ConfigRow{
        id: limitPos2
        configName: "limitPos2"
    }

    Button{
        text: qsTr("Read")
        onClicked: {
            collisionCondition.setLimitStateWithCurrentState(2)
        }
    }

    function init(){
        limitPos1.configModel = configModel
        limitPos2.configModel = configModel
        cmpOperationType.configModel = configModel
        limitPos1.init(ConfigElementType.Double)
        limitPos2.init(ConfigElementType.Double)
        cmpOperationType.init(ConfigElementType.Other)
    }
}
