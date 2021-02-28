import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ColumnLayout{
    readonly property int currentIndex: tabBar.currentIndex
    readonly property int count: pageModel.count
    property int indent: 0
    property string trContext: "MyTabView"

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
                    ToolTip{
                        text: tipText.length > 0 ? qsTranslate(trContext, String(tipText)): qsTranslate(trContext, String(pageTitle))
                        visible: tipText.length > 0 ? parent.hovered : (parent.hovered && pageTitle.length > 4)
                    }
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
                property var title: pageTitle
                sourceComponent: pageComponent
            }
        }
    }

    function addTab(title, component, tipText=""){
        insertTab(pageModel.count, title, component, tipText)
    }

    function insertTab(index, title, component, tipText=""){
        pageModel.insert(index, {"pageTitle": title, "pageComponent": component, "tipText": tipText})
    }

    function removeTab(index){
        pageModel.remove(index)
    }

    function setTitle(index, title){
        pageModel.setProperty(index, "pageTitle", title)
    }
}


