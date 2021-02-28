import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../CustomizedUIElement"

Item {
    id: moveProtectionEditor

    property var configModel: null

    function init(){
        for(var i = 0; i < configModel.count(); i++){
            collisionGroupsTabView.insertTab(i, i, collisionGroupEditorComponent)
        }
        connConfigInserted.target = configModel
        connConfigRemoved.target = configModel
    }

    ColumnLayout{
        anchors.fill: parent

        RowLayout{
            RoundButton{
                icon.source: "/icons/add.png"
                icon.color: "transparent"
                onClicked: {
                    var tabviewIndex = collisionGroupsTabView.currentIndex
                    if(tabviewIndex < 0 || tabviewIndex >= collisionGroupsTabView.count){
                        tabviewIndex = collisionGroupsTabView.count - 1
                    }
                    configModel.add(tabviewIndex + 1)
                }
            }
            RoundButton{
                icon.source: "/icons/remove.png"
                icon.color: "transparent"
                onClicked: {
                    var tabviewIndex = collisionGroupsTabView.currentIndex
                    if(tabviewIndex >= 0 && tabviewIndex < collisionGroupsTabView.count){
                        configModel.remove(tabviewIndex)
                    }
                }
            }
        }

        TabBar{
            id: collisionGroupsTabBar

            Repeater{
                model: 30

                delegate: TabButton{
                    text:  modelData
                    visible: modelData < collisionGroupsTabView.count
                }
            }
        }

        TabView14{
            Layout.fillHeight: true
            Layout.fillWidth: true
            tabsVisible: false
            currentIndex: collisionGroupsTabBar.currentIndex

            id: collisionGroupsTabView
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
            for(var i = index; i < collisionGroupsTabView.count; i++) {
                var tab = collisionGroupsTabView.getTab(i)
                tab.title = Number(tab.title) + 1
            }
            collisionGroupsTabView.insertTab(index, index, collisionGroupEditorComponent)
        }
    }
    Connections{
        id: connConfigRemoved
        target: null
        onConfigRemoved:{
            for(var i = index + 1; i < collisionGroupsTabView.count; i++) {
                var tab = collisionGroupsTabView.getTab(i)
                tab.title = Number(tab.title) - 1
            }
            collisionGroupsTabView.removeTab(index)
        }
    }
}
