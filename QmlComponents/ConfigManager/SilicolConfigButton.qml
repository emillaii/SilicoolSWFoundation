import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import ConfigElementType 1.0

Button{
    property var dataModel: null
    property var identity: null

    text: identity
    implicitWidth: 120

    onClicked: {
        popupConfigEditor.open()
    }

    Popup{
        id: popupConfigEditor

        anchors.centerIn: Overlay.overlay
        contentItem: Frame{
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
                        __initEditor(item)
                    }
                }

                Loader{
                    id: arrayEditorLoader
                    source: "ArrayEditor.qml"
                    active: false
                    visible: active
                    onLoaded: {
                        __initEditor(item)
                    }
                }
            }
        }
    }

    Connections{
        id: connAuthority
        target: null
        onCurrentAuthorityChanged: {
            updateAuthority()
        }
    }

    function init(){
        var elementType = dataModel.getConfigType(identity)
        if (elementType === ConfigElementType.ConfigObj){
            objectEditorLoader.active = true
        }else if(elementType === ConfigElementType.ConfigArr ||
                 elementType === ConfigElementType.ConfigObjArr){
            arrayEditorLoader.active = true
        }
        if(languageConfig.language == 0){
            text = dataModel.translate(identity)
        }
        if(dataModel.needEngineerAuthority(identity)){
            updateAuthority()
            connAuthority.target = userManagement
        }
    }

    function updateAuthority(){
        enabled = (userManagement.currentAuthority >= 2)
    }

    function __initEditor(item){
        item.configModel = dataModel.getConfig(identity)
        item.titleVisible = false
        item.init()
    }
}
