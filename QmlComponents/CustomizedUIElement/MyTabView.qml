import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ColumnLayout{
    readonly property int currentIndex: tabBar.currentIndex
    readonly property int count: pageModel.count
    property int indent: 0
    property string trContext: "MyTabView"
    property bool deleteUnvisibleTab: true

    property int __circelRadius: 10

    ListModel{
        id: pageModel
    }

    RowLayout{
        Repeater{
            model: indent
            delegate: Rectangle{
                width: __circelRadius * 2
                height: width
                radius: __circelRadius
                color: "pink"
            }
        }

        TabBar{
            id: tabBar
            Layout.fillWidth: true
            clip: true
            Repeater{
                model: pageModel
                delegate: TabButton{
                    width: implicitWidth
                    leftInset: 4
                    leftPadding: 5
                    rightInset: 4
                    rightPadding: 5
                    implicitHeight: 30
                    text: qsTranslate(trContext, String(pageTitle))
                }
            }
        }
    }

    StackLayout{
        Layout.fillWidth: true
        Layout.fillHeight: true

        currentIndex: tabBar.currentIndex

        Repeater{
            model: pageModel
            delegate: Loader{
                id: pageView
                property var title: pageTitle
                sourceComponent: pageComponent
                active: !(deleteUnvisibleTab && deleteWhenUnvisible) || tabBar.currentIndex === index
                Connections{
                    enabled: deleteUnvisibleTab && deleteWhenUnvisible
                    target: tabBar
                    onCurrentIndexChanged: {
                        pageView.active = (tabBar.currentIndex === index)
                    }
                }
            }
        }
    }

    function addTab(title, component, deleteWhenUnvisible=true){
        insertTab(pageModel.count, title, component, deleteWhenUnvisible)
    }

    function insertTab(index, title, component, deleteWhenUnvisible=true){
        pageModel.insert(index, {"pageTitle": title, "pageComponent": component, "deleteWhenUnvisible": deleteWhenUnvisible})
    }

    function removeTab(index){
        pageModel.remove(index)
    }

    function setTitle(index, title){
        pageModel.setProperty(index, "pageTitle", title)
    }
}


