import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import ConfigElementType 1.0

RowLayout{
    id: configRow
    property var configModel: null
    property string configName: "name"
    property int txtWidth: -1

    property var childItem: null
    property alias lblName: lblName

    Label{
        id: lblName
        text: configName + ":"
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
    }

    signal childItemInited()

    function autoInit()
    {
        var elementType = configModel.getConfigType(configName)
        init(elementType)
    }

    function init(elementType)
    {
        var component = null
        var isTxt = false
        if(elementType === ConfigElementType.Bool){
            component = Qt.createComponent("SilicolCheckBox.qml")
        }else if(elementType === ConfigElementType.Int ||
                 elementType === ConfigElementType.Double ||
                 elementType === ConfigElementType.Other){
            if(configModel.isOptional(configName)){
                component = Qt.createComponent("SilicolComboBox.qml")
            }else{
                component = Qt.createComponent("SilicolTextField.qml")
                isTxt = true
            }
        }
        if(component !== null){
            var obj = component.createObject(configRow)
            if(obj === null){
                console.log("error")
                console.error(component.errorString())
                return
            }
            obj.dataModel = configModel
            obj.identity = configName
            if(isTxt){
                obj.txtWidth = txtWidth
            }
            obj.init()
            childItem = obj
            childItemInited()
            if(languageConfig.language == 0){
                lblName.text = configModel.translate(configName)
            }
        }else{
            console.log("Unknown type: " + elementType)
            return
        }
    } 
}
