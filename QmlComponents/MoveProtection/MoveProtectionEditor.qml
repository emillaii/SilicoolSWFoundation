import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../CustomizedUIElement"

Item {
    id: moveProtectionEditor

    property var configModel: null
    property alias tip: tip
     property alias btnUpdateMoveProtection: btnUpdateMoveProtection

    function init(){
        for(var i = 0; i < configModel.count(); i++){
            collisionGroupsViewer.addTab(i, collisionGroupEditorComponent)
        }
        connConfigInserted.target = configModel
        connConfigRemoved.target = configModel
    }

    ColumnLayout{
        anchors.fill: parent

        RowLayout{
            RoundButton{
                icon.source: "qrc:/commonicons/add.png"
                icon.color: "transparent"
                onClicked: {
                    var tabviewIndex = collisionGroupsViewer.currentIndex
                    if(tabviewIndex < 0 || tabviewIndex >= collisionGroupsViewer.count){
                        tabviewIndex = collisionGroupsViewer.count - 1
                    }
                    configModel.add(tabviewIndex + 1)
                }
            }
            RoundButton{
                icon.source: "qrc:/commonicons/remove.png"
                icon.color: "transparent"
                onClicked: {
                    var tabviewIndex = collisionGroupsViewer.currentIndex
                    if(tabviewIndex >= 0 && tabviewIndex < collisionGroupsViewer.count){
                        configModel.remove(tabviewIndex)
                    }
                }
            }
            CheckBox{
                text: qsTr("MoveProtectionEnable")
                enabled: userManagement.currentAuthority >= 2
                checked: masterMotionManager.moveProtectionsEnable
                onCheckedChanged: {
                    masterMotionManager.setMoveProtectionsEnable(checked)
                }
            }
            Button{
                id: btnUpdateMoveProtection
                text: qsTr("Update")
                onClicked: {
                    masterMotionManager.updateMoveProtections()
                }
            }
            Label{
                id: tip
            }
        }

        MyTabView{
            Layout.fillHeight: true
            Layout.fillWidth: true
            id: collisionGroupsViewer
        }
    }

    Component{
        id: collisionGroupEditorComponent
        CollisionGroupEditor{
            Component.onCompleted: {
                var index = parent.title
                configModel = moveProtectionEditor.configModel.getConfig(index)
                init()
            }
        }
    }

    Connections{
        id: connConfigInserted
        target: null
        onConfigInserted:{
            for(var i = index; i < collisionGroupsViewer.count; i++) {
                collisionGroupsViewer.setTitle(i, i + 1)
            }
            collisionGroupsViewer.insertTab(index, index, collisionGroupEditorComponent)
        }
    }
    Connections{
        id: connConfigRemoved
        target: null
        onConfigRemoved:{
            for(var i = index + 1; i < collisionGroupsViewer.count; i++) {
                collisionGroupsViewer.setTitle(i, i - 1)
            }
            collisionGroupsViewer.removeTab(index)
        }
    }
}
