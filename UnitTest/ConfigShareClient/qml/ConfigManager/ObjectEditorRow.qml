import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import ConfigElementType 1.0

BasicArrayRow{
    function init()
    {
        objEditorLoader.source = "ObjectEditor.qml"
        connRowIndexChanged.target = rowModel
    }

    Button{
        text: "Show"
        onClicked: {
            objEditorPopup.open()
        }
    }

    Popup{
        id: objEditorPopup

        anchors.centerIn: parent

        contentItem: Loader{
            id: objEditorLoader
            onLoaded: {
                item.sectionName = "Obj" + index
                item.configModel = configModel
                item.listMaxWidth = 1300
                item.listMaxHeight = 700
                indexBinding.target = item
                item.init()
            }
        }

        Binding{
            id: indexBinding

            target: null
            value: "Obj " + index
            property: "sectionName"
        }
    }
}
