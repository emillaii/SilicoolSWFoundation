import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import ConfigElementType 1.0

BasicConfigEditor{
    function init(_isExpand=true)
    {
        isExpand = _isExpand
        configRepeter.model = configModel.getConfigNamesToShow()
    }

    ColumnLayout{
        visible: isExpand
        spacing: 3

        Repeater{
            id: configRepeter
            delegate: Frame{
                id: frame
                background: Rectangle{
                    radius: 5
                    border.width: 1
                    color: "transparent"
                }
                topInset: 0
                bottomInset: 0
                padding: 5

                ColumnLayout{
                    Loader{
                        id: objectEditorLoader
                        source: "ObjectEditor.qml"
                        active: false
                        visible: active
                        onLoaded: {
                            item.configModel = configModel.getConfig(modelData)
                            item.sectionName = configModel.translate(modelData)
                            item.listMaxWidth = listMaxWidth
                            item.listMaxHeight = listMaxHeight
                            item.init(false)
                        }
                    }

                    Loader{
                        id: arrayEditorLoader
                        source: "ArrayEditor.qml"
                        active: false
                        visible: active
                        onLoaded: {
                            item.configModel = configModel.getConfig(modelData)
                            item.sectionName = configModel.translate(modelData)
                            item.listMaxWidth = listMaxWidth
                            item.listMaxHeight = listMaxHeight
                            item.init(false)
                        }
                    }

                    Loader{
                        id: configRowLoader

                        property var elementType: null
                        source: "ConfigRow.qml"
                        active: false
                        visible: active
                        onLoaded: {
                            item.configModel = configModel
                            item.configName = modelData
                            item.init(elementType)

                            if(isMarerialStyle){ // Material风格有多余的空白
                                frame.implicitHeight = item.implicitHeight + frame.padding * 2
                                item.y = -frame.padding
                            }
                        }
                    }
                }

                Component.onCompleted: {
                    var elementType = configModel.getConfigType(modelData)
                    if (elementType === ConfigElementType.ConfigObj){
                        objectEditorLoader.active = true
                    }else if(elementType === ConfigElementType.ConfigArr ||
                             elementType === ConfigElementType.ConfigObjArr){
                        arrayEditorLoader.active = true
                    }else {
                        configRowLoader.elementType = elementType
                        configRowLoader.active = true
                    }
                }
            }
        }
    }
}
