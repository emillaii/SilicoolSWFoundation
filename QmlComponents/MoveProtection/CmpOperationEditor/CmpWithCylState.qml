import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../ConfigManager"
import ConfigElementType 1.0

ComparisonOperation{
    ConfigRow{
        id: limitState
        configName: "limitState"
    }

    function init(){
        limitState.configModel = configModel
        cmpOperationType.configModel = configModel
        limitState.init(ConfigElementType.Int)
        cmpOperationType.init(ConfigElementType.Other)
    }
}
