import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../ConfigManager"
import ConfigElementType 1.0

ComparisonOperation{
    ConfigRow{
        id: limitPos
        configName: "limitPos"
    }

    Button{
        text: qsTr("Read")
        onClicked: {
            collisionCondition.setLimitStateWithCurrentState()
        }
    }

    function init(){
        limitPos.configModel = configModel
        cmpOperationType.configModel = configModel
        limitPos.init(ConfigElementType.Double)
        cmpOperationType.init(ConfigElementType.Other)
    }
}
