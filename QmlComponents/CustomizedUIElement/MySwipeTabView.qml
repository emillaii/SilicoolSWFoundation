import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ColumnLayout{
    readonly property int currentIndex: tabBar.currentIndex
    readonly property int count: pageModel.count
    property alias tabBar: tabBar
    property int indent: 0
    property string trContext: "MySwipeTabView"

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
                    implicitHeight: 30
                    text: qsTranslate(trContext, String(pageTitle))
                }
            }
        }
    }

    SwipeView{
        id: swipeView
        Layout.fillWidth: true
        Layout.fillHeight: true

        currentIndex: tabBar.currentIndex
        orientation: Qt.Vertical
        interactive: false

        Repeater{
            model: pageModel
            delegate: Loader{
                property var title: pageTitle
                sourceComponent: pageComponent
                visible: tabBar.currentIndex === index
            }
        }
    }

    function addTab(title, component){
        insertTab(pageModel.count, title, component)
    }

    function insertTab(index, title, component){
        pageModel.insert(index, {"pageTitle": title, "pageComponent": component})
    }

    function removeTab(index){
        pageModel.remove(index)
    }

    function setTitle(index, title){
        pageModel.setProperty(index, "pageTitle", title)
    }
}


