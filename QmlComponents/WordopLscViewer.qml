import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import "ConfigManager"

ArrayEditor{
    configModel: wordopLscConfig
    sectionName: qsTr("WordopLightSourceControllerConfig")
    Component.onCompleted: {
        init()
    }
}
